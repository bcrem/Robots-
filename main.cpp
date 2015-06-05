#include <iostream>
#include "Matrix.h"
#include "Jointset.h"
#include "labvolt.h"
#include <cmath>
#include <unistd.h>

#include <cstdlib>
#include <cstring>

#include <stdio.h>
#include <strings.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>

using namespace std;

void armMove(char*, int);
void armOpen();
void armClose();
void armNest();
void armShutdown();

void moveToJointAngles(Jointset desired);
void moveToPointXYZ(Point desired);

Jointset current;
int piece = 0;


#define MY_PORT		9998
#define MAXBUF		1024

#define TRUE		1
#define FALSE		0 


bool armFlag = TRUE;  //  Assume we're plugged into an arm.
bool initFlag = FALSE;  //  Call init() unless told otherwise

int main(int argc, char** argv)
{
	int sockfd;
	struct sockaddr_in self;
	char buffer[MAXBUF];

    if ( argc > 1 )
    {
        if (*argv[1] == 'd')
        {
            armFlag = FALSE;

            cout << "********************" << endl
                 << "***  DUMMY MODE  ***" << endl
                 << "********************" << endl;
        }

        if (*argv[1] == 'q')
        {
            initFlag = TRUE;

            cout << "*************************" << endl
                 << "***  QUICK-INIT MODE  ***" << endl
                 << "*************************" << endl;
        }
    }


    if (initFlag)
        quickInit1();
    else if (armFlag)
        init();

    if (armFlag) zero();
	
	// Create streaming socket
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket");
		return errno;
	}
	
	/*---Initialize address/port structure---*/
	bzero(&self, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(MY_PORT);
	self.sin_addr.s_addr = INADDR_ANY;

	/*---Assign a port number to the socket---*/
    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 )
	{
		perror("socket--bind");
		return errno;
	}

	/*---Make it a "listening socket"---*/
	if ( listen(sockfd, 20) != 0 )
	{
		perror("socket--listen");
		return errno;
	}

	Jointset tmp(0, 130, 135, 0, 0);
	current = tmp;
	
	while (1)
    {
		int clientfd;
		struct sockaddr_in client;
		int addrlen = sizeof(client);
		
		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client, (socklen_t*)&addrlen);
		printf("%s:%d connected\n",
		        inet_ntoa(client.sin_addr),
		        ntohs(client.sin_port));
		
		int bytesReceived = recv(clientfd, buffer, MAXBUF, 0);
	
		// have something
		if (bytesReceived)
        {
            cout << "Read " << bytesReceived << " bytes." << endl;
            buffer[bytesReceived] = '\0';
			
            cout << "buffer: " << buffer << endl;

            //  Extract command from buffer
            switch (buffer[0])
            {
                case 'M':
                    armMove(buffer, bytesReceived);
                    break;

                case 'O':
                    armOpen();
                    break;

                case 'C':
                    armClose();
                    break;

                case 'I':
                    cout << "Initializing..." << endl;
                    if (armFlag) init();
                    break;

                case 'N':
                    armNest();
                    break;

                case 'S':
                    armShutdown();
                    break;
            }
		}

		const char *msg = "success";
		send(clientfd,msg,strlen(msg),0);

  		close(clientfd);
	}
	

	return 0;
}


void armMove(char *buf, int msgSize)
{
    float x,y,z;

    x = atof(strtok ((buf+2),","));
    y = atof(strtok (NULL,","));
    z = atof(strtok (NULL,","));

    printf("armMove(): Moving to Point(%f, %f, %f)\n", x,y,z);

    //  Extract x-y-z coords from msg.
    if (armFlag)
    {

        moveToPointXYZ(Point(x,y,z));
    }
}


void armOpen()
{
    cout << "Opening gripper..." << endl;
    if (armFlag) gripperOpen();
}


void armClose()
{
    cout << "Closing gripper..." << endl;
    if (armFlag) gripperClose();
}


void armNest()
{
    cout << "Returning arm to current 'home'..." << endl;
    if (armFlag) nest();
}


void armShutdown()
{
    cout << "Shuttting down..." << endl;
    if (armFlag) shut_down();
}



void moveToJointAngles(Jointset desired)
{
	Jointset move = desired - current;

	// converts theta 1 to steps, + = CCW
	double base = move.angles[0] * 6500. / 360.;
	// converts theta 2 to steps, + = up/back
	double shoulder = move.angles[1] * 8600. / 360.;
	// converts theta 3 to steps, + = down, dependent on shoulder
	double elbow = move.angles[2] * 8600. / 360 - shoulder;
	// +M4 and -M5 pitches wrist down
	double M4 = move.angles[3] * 6500. / 360. +
                move.angles[4] * 6500. / 360. +
                elbow*6500/8600 - base;
	// +M4 and +M5 rolls wrist right
	double M5 = -move.angles[3] * 6500. / 360. +
                move.angles[4] * 6500. / 360. -
                elbow*6500/8600 - base;
	
  	moverel(base,shoulder,elbow,M4,M5);
	
	current = desired;
}

void moveToPointXYZ(Point desired)
{
    // matrix representing position and orientation (gripper down)
	double tmp[4][4] = {{-1., 0., 0., desired.x},
			    {0., 1., 0.,  desired.y},
			    {0., 0., -1., desired.z},
			    {0., 0., 0., 1}};

	Matrix x(tmp);
    
    // calculate angles to move based on that matrix
	Jointset anglesMove = inverseKin(x);
	moveToJointAngles(anglesMove);	// move to new position
}
