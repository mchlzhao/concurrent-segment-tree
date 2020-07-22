all: main

main:
	gcc -o main main.c seg-tree-common.c -Wall -pthread -O
