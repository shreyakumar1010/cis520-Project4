all: Pthreads.c
      gcc -Wall -Wextra -lpthread Pthreads.c -o Pthreads
 
run: Pthread
      ./Pthreads
       
schedule:
      srun -N4 ./Pthreads
