# Author - Jeremiah Simonsen

CXXFLAGS =	-O2 -Wall -g
LDFLAGS = -lstdc++ -L. -llabvolt -ludev

# Add additional objects for the build here!
OBJS =		Matrix.o Point.o Jointset.o main.o

TARGET =	armServer

# Include the labvolt and usb libraries
INC= -I/home/bcrem/work/robotics/lab2/
LIBSDIR=-L./
LIBS=${LIBSDIR} -lstdc++ -llabvolt -ludev

$(TARGET):	$(OBJS)
	${CXX} ${CFLAGS} -o ${TARGET} ${OBJS} ${INC} ${LIBS}

Matrix.o: Matrix.h Point.h Jointset.h

Jointset.o: Jointset.h
	$(CXX) -c -o Jointset.o Jointset.cpp $(LIBS)

all:	$(TARGET)
	
clean:
	rm -rf *~ $(OBJS) $(TARGET)
