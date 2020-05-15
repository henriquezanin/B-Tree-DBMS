all: compile

compile:
	@gcc -o bin/main sources/*.c includes/*.c -I includes/. -Wall

run:
	@bin/main