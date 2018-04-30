all: OpenMP.c
	gcc -Wall -Wextra -fopenmp OpenMP.c -o OpenMP

clean:
	rm *.out

run: a.out
	srun -n1 -T20 ./a.out wiki_dump.txt
	
	
