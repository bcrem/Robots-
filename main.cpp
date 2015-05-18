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
void armShutdown();

void moveToJointAngles(Jointset desired);
void moveToPointXYZ(Point desired);

Jointset current;
int piece = 0;

/*
void PieceToNode(int a, int b);

Point Board[9][9] = {{Point(9.00,-10.0,5.0), Point(8.50,-7.5,5.0), Point(8.50,-5.0,5.0), Point(8.00,-2.5,5.0), Point(8.00,0.0,5.0), Point(8.00,2.5,5.0), Point(8.00,5.0,5.0), Point(8.00,7.5,5.0), Point(8.50,10.5,5.0)},
   					 {Point(12.0,-11.0,4.5), Point(12.0,-8.0,4.5), Point(11.5,-5.0,4.5), Point(11.5,-2.5,4.5), Point(11.5,0.0,4.5), Point(11.5,2.5,4.5), Point(11.5,5.5,4.5), Point(11.5,8.5,4.5), Point(11.5,11.0,4.0)},
					 {Point(15.0,-11.5,4.0), Point(15.0,-8.5,4.0), Point(15.0,-5.0,4.0), Point(14.5,-2.5,4.5), Point(14.5,0.0,4.0), Point(14.5,3.0,4.0), Point(14.5,5.5,4.0), Point(14.5,8.5,4.0), Point(15.0,11.5,4.0)},
					 {Point(18.0,-11.5,4.0), Point(18.0,-8.5,4.0), Point(18.0,-5.5,4.0), Point(18.0,-2.5,4.0), Point(18.0,0.0,4.0), Point(18.0,3.0,4.0), Point(18.0,5.5,4.0), Point(18.0,8.5,4.0), Point(18.0,11.5,4.0)},
					 {Point(21.0,-11.5,4.0), Point(21.0,-8.5,4.0), Point(21.0,-5.5,4.0), Point(21.0,-2.5,4.0), Point(21.0,0.0,4.0), Point(21.0,3.0,4.0), Point(21.0,6.0,4.0), Point(21.0,8.5,4.0), Point(21.0,12.0,4.0)},
					 {Point(25.0,-12.0,3.5), Point(25.0,-9.0,3.5), Point(25.0,-5.5,3.5), Point(25.0,-2.5,3.5), Point(24.5,0.0,3.5), Point(25.0,3.5,3.5), Point(25.0,6.0,3.5), Point(25.0,9.0,3.5), Point(24.5,12.5,3.5)},
					 {Point(28.0,-12.0,3.5), Point(28.0,-9.0,3.5), Point(28.0,-5.5,3.5), Point(28.0,-2.5,3.5), Point(28.0,0.0,3.5), Point(28.0,3.0,3.5), Point(28.0,6.0,3.5), Point(28.0,9.0,3.5), Point(28.0,12.0,3.5)},
					 {Point(31.0,-12.0,3.5), Point(31.0,-9.0,3.5), Point(31.0,-6.0,3.5), Point(31.0,-3.0,3.5), Point(31.0,0.0,3.5), Point(31.0,3.0,3.5), Point(31.0,6.0,3.5), Point(31.0,9.0,3.5), Point(31.0,12.0,3.5)},
					 {Point(35.0,-12.0,3.5), Point(34.0,-9.0,3.5), Point(34.0,-6.0,3.5), Point(34.0,-3.0,3.5), Point(34.0,0.0,3.5), Point(34.0,3.0,3.5), Point(34.0,6.0,3.5), Point(34.0,9.0,3.5), Point(34.0,12.0,3.5)}};

Point Pieces[23] = {Point(22.0,-23.0,2.0), Point(22.0,-18.0,2.0),
					Point(18.0,-28.0,2.0), Point(17.0,-23.0,2.0), Point(17.0,-17.5,2.0),
					Point(13.0,-28.0,2.0), Point(12.5,-22.5,2.5), Point(12.0,-17.5,3.0), 
					Point(8.25,-27.5,2.0), Point(8.0,-22.5,2.5),  Point(7.5,-17.0,3.0), 
					Point(3.5,-27.5,2.0),  Point(3.25,-22.5,2.5), Point(3.25,-17.0,3.0),
					Point(-1.0,-27.5,2.0), Point(-1.0,-22.5,2.5), Point(-1.0,-17.0,3.0),
					Point(-5.5,-27.5,2.0), Point(-5.5,-22.5,3.0), Point(-5.5,-17.0,3.0),
					Point(-10.5,-27.5,2.0), Point(-10.5,-22.5,2.5), Point(-10.5,-17.0,3.0)
					};
*/



#define MY_PORT		9998
#define MAXBUF		1024

#define TRUE		1
#define FALSE		0 


bool armFlag = TRUE;  //  Assume we're plugged into an arm.

int main(int argc, char** argv)
{
	int sockfd;
	struct sockaddr_in self;
	char buffer[MAXBUF];

    if ( argc > 1 )
    {
        armFlag = FALSE;
    }


    if (armFlag) init();
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


void armMove(char *msg, int msgSize)
{
    cout << "armMove(): Received \"" << msg << "\"" << endl;

    //  Extract x-y-z coords from msg.
    if (armFlag)
    {
        ;
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
