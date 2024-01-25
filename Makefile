CC = gcc
name = iou
args = -Wall -fsanitize=address -g -O0

######

all: build

build:
	$(CC) $(args) src/*.c -o $(name)

clean:
	rm -rf $(name)