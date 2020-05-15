all: compile

marin: compile-macos

compile-macos:
	@gcc-9 -o bin/main sources/*.c includes/*.c -I includes/. -Wall -ansi

compile:
	@gcc -o bin/main sources/*.c includes/*.c -I includes/. -Wall -ansi

run:
	@bin/main