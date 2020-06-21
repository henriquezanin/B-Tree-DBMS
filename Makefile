# Henrique Gomes Zanin NUSP: 10441321
# Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521
all: compile

marin: compile-macos

compile-macos:
	@gcc-9 -o bin/main sources/*.c -I includes/. -Wall -ansi

compile:
	@gcc -o bin/main sources/*.c -I includes/. -Wall -ansi

run:
	@bin/main

clean:
	@rm bin/*.idx
	@rm bin/*.reg
