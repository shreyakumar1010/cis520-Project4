all: MPI.c
	mpicc -Wall MPI.c -o mpi

run: mpi
	./mpi
	
	#srun -N4 ./mpi
