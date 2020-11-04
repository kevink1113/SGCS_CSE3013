CC = g++
CFLAGS = -g -c

OBJECTS = main.o Str.o
SRC = main.cpp Str.cpp

TARGET = main

$(TARGET): $(OBJECTS) Str.h
	$(CC) -o $(TARGET) $(OBJECTS)

clean: 
	rm $(OBJECTS) $(TARGET)
