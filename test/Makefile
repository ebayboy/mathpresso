
CC := g++

SOURCE := $(wildcard *.c) 
SOURCE += $(wildcard *.cc) 
SOURCE += $(wildcard *.cpp)

OBJS   := $(patsubst %.c,%.o, $(patsubst %.cpp,%.o, $(patsubst %cc,%o, $(SOURCE)) ))

DEFINES :=
LIBS	:= -L.
INCLUDE := -I. -I../src/
LDFLAGS := -lrt -lpthread  ../src/mathpresso/libmathpresso.a
CFLAGS	:= -c -std=c++17 -Wall

############## add self define  ######################

#DEFINES +=
#LIBS	+= 
#INCLUDE +=
#LDFLAGS +=
#CFLAGS	+= 

############## add self define  ######################

DEBUG=n
CONV=n

ifeq ($(DEBUG),y)
	CFLAGS += -g -O0
else
	CFLAGS += -O3 -g2
endif

ifeq ($(CONV),y)
	INCLUDE+= -I /usr/local/libiconv/include
	LIBS+= -L /usr/local/libiconv/lib

	CFLAGS += -fprofile-arcs -ftest-coverage
	LDFLAGS += -lgcov
endif

CXXFLAGS:= -DHAVE_CONFIG_H $(CFLAGS) $(DEFINES) $(INCLUDE)

.PHONY :  clean all

all : mpeval mptest mptutorial

clean:
	rm -fr  mpeval mptest mptutorial

%.o:%.c
	$(CC) $(CFLAGS) $< $(INCLUDE) -o $@

%.o:%.cc
	$(CC) $(CFLAGS) $< $(INCLUDE) -o $@

%.o:%.cpp
	$(CC) $(CFLAGS) $< $(INCLUDE) -o $@

mpeval : mpeval.cpp
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

mptest : mptest.cpp
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

mptutorial : mptutorial.cpp
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

