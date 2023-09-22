all: clean pingpong test

test:
	mkfifo test
	./pingpong test

pingpong: 
	gcc -o pingpong pingpong.c

clean:
	rm -f pingpong
	rm -f test
