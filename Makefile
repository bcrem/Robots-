# Author - Jeremiah Simonsen

CXXFLAGS =	-O2 -Wall -g
LDFLAGS = -lstdc++ -L. -llabvolt -ludev

# Add additional objects for the build here!
OBJS =		Matrix.o Point.o Jointset.o main.o

DUMMY_OBJS =		dummyMain.o

# Include the labvolt and usb libraries
INC= -I/home/bcrem/work/robotics/lab2/
LIBSDIR=-L./
LIBS=${LIBSDIR} -lstdc++ -llabvolt -ludev


dummyArmServer:	$(DUMMY_OBJS)
	${CXX} ${CFLAGS} -o dummyArmServer ${DUMMY_OBJS}


armServer:	$(OBJS)
	${CXX} ${CFLAGS} -o armServer ${OBJS} ${INC} ${LIBS}

Matrix.o: Matrix.h Point.h Jointset.h

Jointset.o: Jointset.h
	$(CXX) -c -o Jointset.o Jointset.cpp $(LIBS)

all:	armServer dummyArmServer
	
clean:
	rm -rf *~ $(OBJS) $(DUMMY_OBJS) armServer dummyArmServer
