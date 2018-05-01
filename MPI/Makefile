all: MPI.c
	gcc -Wall -Wextra -lpthread MPI.c -o mpi

run: mpi
	./mpi
	
	#srun -N4 ./mpi
