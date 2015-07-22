# Author - Jeremiah Simonsen

CXXFLAGS =	-O2 -Wall -g
LDFLAGS = -lstdc++ -L. -llabvolt -ludev

# Add additional objects for the build here!
OBJS =		Matrix.o Point.o Jointset.o main.o

# Include the labvolt and usb libraries
INC= -I/home/bcrem/work/robotics/lab2/
LIBSDIR=-L./
LIBS=${LIBSDIR} -lstdc++ -llabvolt -ludev


armDriver:	$(OBJS)
	${CXX} ${CFLAGS} -o armDriver ${OBJS} ${INC} ${LIBS}

Matrix.o: Matrix.h Point.h Jointset.h

Jointset.o: Jointset.h
	$(CXX) -c -o Jointset.o Jointset.cpp $(LIBS)

all:	armDriver
	
clean:
	rm -rf *~ $(OBJS) armDriver
