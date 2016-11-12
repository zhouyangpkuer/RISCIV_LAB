CPPFLAGS = -Wall -O1 -std=c++11 -w -lm
PROGRAMS = main 

all: $(PROGRAMS)

main:main.cpp load.h decode.h exec.h params.h
	g++ -o main main.cpp $(CPPFLAGS)

clean:
	rm -f *.o $(PROGRAMS)
