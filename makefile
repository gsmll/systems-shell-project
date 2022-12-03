compile: main.o parse.o
	@ gcc -o program main.o parse.o
main.o: main.c
	@ gcc -c main.c
parse.o: parse.c
	@ gcc -c parse.c parse.h
run:
	@ ./program $(ARGS)
run-c:
	@ make compile -s
	@ make run $(ARGS) -s
