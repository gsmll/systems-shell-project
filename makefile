compile: main.o parse.o
	@ gcc -o shell.out main.o parse.o
main.o: main.c
	@ gcc -c main.c main.h
parse.o: parse.c
	@ gcc -c parse.c parse.h
run:
	@ ./shell.out
run-c:
	@ make compile -s
	@ make run  -s
