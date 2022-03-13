.POSIX:

# CC = gcc -std=c99 -fno-stack-protector
# CC = gcc -std=c99 -O3 -fno-stack-protector -static
CC = clang -std=c99 -O3 -fno-stack-protector -static

main: main.c pos.o node.o
	$(CC) $^

pos.o: pos.c pos.h
	$(CC) -c $<

node.o: node.c node.h pos.o
	$(CC) -c $<


clean:
	rm -rf *.o *.gch a.out

.PHONY: clean
