all: Pthreads.c
	gcc -Wall -Wextra -lpthread Pthreads.c -o Pthreads

run: Pthreads
	./Pthreads

:wq
:q


	srun -N4 ./Pthreads
