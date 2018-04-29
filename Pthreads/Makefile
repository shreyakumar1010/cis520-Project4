all: Pthreads.c
	gcc -pthread -o Pthreads Pthreads.c

run: Pthreads
	./Pthreads
	
	srun -N4 ./Pthreads
