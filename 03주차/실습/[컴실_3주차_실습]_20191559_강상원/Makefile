OBJECTS = main.o Array.o RangeArray.o
SRC = main.cpp Array.cpp RangeArray.cpp

CC = g++
CPPFLAGS = -g -c

TARGET = main

$(TARGET): $(OBJECTS) Array.h RangeArray.h
	$(CC) -o $(TARGET) $(OBJECTS)

clean: 
	rm $(OBJECTS) $(TARGET)
	
