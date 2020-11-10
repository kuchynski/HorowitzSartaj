all: main.c Horowitz_Sartaj.c service_function.c bmemory.c
	g++ -std=c++11 -o Horowitz_Sartaj $^
clean: rm -f *.o
