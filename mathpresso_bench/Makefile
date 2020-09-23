CC = g++
LD = g++

TARGET = test

INCLUDES += -I./ -I /usr/local/benchmark/include/

LIB = -L /usr/local/benchmark/lib64/

CFLAGS = -O3 -c -std=c++17 -Wall

LDFLAGS = -lbenchmark -lpthread -lm -lz -lrt ../libmathpresso.a

SRC_C :=

SRC_CPP := benchmark_test.cpp

OBJS_C=$(SRC_C:.c=.o)
OBJS_CPP=$(SRC_CPP:.cpp=.o)

#$(warning $(OBJS_C))

.PHONY:all clean

all: $(TARGET)
$(TARGET): $(OBJS_C) $(OBJS_CPP)
	$(LD) $^ -o $@ $(LDFLAGS) $(LIB) 
	./$(TARGET)

%.o:%.c
	$(CC) $(CFLAGS) $< $(INCLUDES) -o $@

%.o:%.cpp
	$(CC) $(CFLAGS) $< $(INCLUDES) -o $@

clean:
	rm -rf $(TARGET) $(OBJS_C) $(OBJS_CPP)

