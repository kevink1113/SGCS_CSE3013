OBJECTS = main.o
SRC = main.cpp
CC = g++
CPPFLAGS = -g -c
TARGET = main
$(TARGET): $(OBJECTS) Array.h GrowableArray.h
	$(CC) -o $(TARGET) $(OBJECTS)
rm:
	rm $(OBJECTS) $(TARGET)
