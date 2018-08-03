INC_PATH=-I./source/
CC=gcc
CFLAGS=-std=c99
LIBS=-ljpeg -lpng -lexif -lm
INCLUDES=$(wildcard source/*.h)
SOURCES=$(wildcard source/*.c)
OBJECTS=$(patsubst %.c, %.o, $(notdir $(SOURCES)))
TARGET=bin/as_ntu_find_cloud

all: build $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %o: source/%c
	$(CC) $(CFLAGS) $(INC_PATH) -c $^ -o $@

build:
	@mkdir -p bin
	
clean:
	rm -rf $(TARGET)
	rm -rf $(OBJECTS)
	