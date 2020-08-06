# Makefile

main: main.o login.o console.o bank_functions.o
	gcc main.o login.o console.o bank_functions.o -o main
main_debug: main.o login.o console.o bank_functions.o
	gcc -g main.o login.o console.o bank_functions.o -o main_debug
main.o: main.c
	gcc -g -c main.c -o main.o
login.o: login.c
	gcc -g -c login.c -o login.o
console.o: console.c
	gcc -g -c console.c -o console.o
bank_functions.o: bank_functions.c
	gcc -g -c bank_functions.c -o bank_functions.o
clean:
	rm *.o
