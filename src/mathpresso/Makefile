
CC		:= g++
AR		:= ar
RANLIB	:= ranlib

SOURCE	:= $(wildcard *.c) $(wildcard *.cpp)
SOURCE	+= $(wildcard asmjit/core/*.c) $(wildcard asmjit/core/*.cpp)
SOURCE	+= $(wildcard asmjit/x86/*.c) $(wildcard asmjit/x86/*.cpp)

$(warning $(SOURCE) )

OBJS	:= $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

 #$(warning SOURCE=$(SOURCE))

TARGET := libmathpresso.a

DEFINES :=

LIBS	:= -L.
INCLUDE := -I.

LDFLAGS := 

CFLAGS	:= -std=c++17	-Wall $(DEFINES) $(INCLUDE)

DEBUG=n
CONV=n

ifeq ($(DEBUG),y)
	CFLAGS += -g2 -O0
else
	CFLAGS += -O3
endif

ifeq ($(CONV),y)
	INCLUDE+= -I /usr/local/libiconv/include
	LIBS+= -L /usr/local/libiconv/lib

	CFLAGS += -fprofile-arcs -ftest-coverage
	CFLAGS += -lgcov
endif

CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H

.PHONY : everything objs clean veryclean rebuild

everything : $(TARGET)

all : $(TARGET)

objs : $(OBJS)

rebuild: veryclean everything

clean : 
	rm -fr $(OBJS) $(TARGET)

veryclean : clean
	rm -rf *.gcno *.gcda

$(TARGET) : $(OBJS)
	$(AR) cru $(TARGET) $(OBJS)
	$(RANLIB) $(TARGET)

