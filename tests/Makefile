all: compile

marin: compile-macos

compile-macos:
	@gcc-9 -o main *.c -I. -Wall -ansi

compile:
	@gcc -o main *.c -I. -Wall -ansi

run:
	@./main
