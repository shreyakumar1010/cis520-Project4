all: Pthreads.c
      gcc -Wall -Wextra -lpthread Pthreads.c -o Pthreads
 
run: Pthreads
      ./Pthreads
       
schedule:
      srun -N4 ./Pthreads
