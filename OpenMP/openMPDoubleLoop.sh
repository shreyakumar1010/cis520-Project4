#!/bin/bash -l




for i in 1000 10000 100000 500000 1000000
do
  for j in 1 2 4 8 16 32
  do
    corestring="_$j"
    job_name="omp$corestring" 
    sbatch --mem-per-cpu=4G  --time=03:00:00  --partition=killable.q --nodes=1 --ntasks-per-node=$j --constraint=dwarves --job-name=$job_name --mail-type=coreyvessar@ksu.edu openmp.sh $j $i 
    done
done
