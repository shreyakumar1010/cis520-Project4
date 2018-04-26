#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include <mpi.h>

#define WIKI_ARRAY_SIZE 500
#define WIKI_LINE_SIZE 2001
#define MASTER 0

int lengthOfSubstring [WIKI_ARRAY_SIZE];
int LCS (char * s1, char * s2, char ** longest_common_substring);
int NumThreads;

char  ** wiki_array;
char ** longestSub;

void readToMemory();
void printResults();
void printToFile();
void * findem(void * rank);

int main(int argc, char* argv[])
{
	struct timeval time1;
    	struct timeval time2;
    	struct timeval time3;
    	struct timeval time4;
    	double e1, e2, e3;    
    	int numSlots, Version = 1; //base = 1, pthread = 2, openmp = 3, mpi = 4
	longestSub = (char **) malloc( WIKI_ARRAY_SIZE * sizeof(char *)); //saved results
	int i;
	for (i = 0; i < WIKI_ARRAY_SIZE -1; i++)//initializing saved results
	  	longestSub[i] = malloc(2001);
	
	//===================INIT MPI=======================
	int rc, NumTasks, rank;
	MPI_Status status;
	rc = MPI_Init(&argc, &argv);
	if(rc != MPI_SUCCESS)
  	{
    		printf("MPI isn't working");
    		MPI_Abort(MPI_COMM_WORLD, rc);
  	}
	MPI_Comm_size(MPI_COMM_WORLD, &NumTasks);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	NumberofThreads = NumTasks;
	
    	//==================READ TO MEMORY==================
    	gettimeofday(&time1, NULL);
    	readToMemory(); //reading
    	gettimeofday(&time2, NULL);//time to read to memory	
    	e1 = (time2.tv_sec - time1.tv_sec) * 1000.0; //sec to ms 
    	e1 += (time2.tv_usec - time1.tv_usec) / 1000.0; // us to ms
    	printf("Time to read full file to Memory: %f\n", e1);
	
	//====FINDING LONGEST CMN SUBSTR. & PARALLELIZING====
    	gettimeofday(&time3, NULL);	
	MPI_Bcast(wiki_array, WIKI_ARRAY_SIZE * WIKI_LINE_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
	findem(&rank);  
	if(rank == MASTER)
	{
	//=================FINAL TIMING===================
   		gettimeofday(&time4, NULL);
		MPI_Reduce(longestSub, longestSub, WIKI_ARRAY_SIZE * WIKI_LINE_SIZE, MPI_CHAR, MPI_SUM, 0, MPI_COMM_WORLD);
   		printToFile();
		//time to find all longest substrings	
   		e2 = (time4.tv_sec - time3.tv_sec) * 1000.0; //sec to ms
   		e2 += (time4.tv_usec - time3.tv_usec) / 1000.0; // us to ms
   		printf("Time find all Substrings: %f\n", e2);
   		//total elapsed time between reading and finding all longest substrings	
   		e3 = (time4.tv_sec - time1.tv_sec) * 1000.0; //sec to ms
   		e3 += (time4.tv_usec - time1.tv_usec) / 1000.0; // us to ms
   		printf("DATA, %d, %s, %f\n", Version, getenv("NSLOTS"), e3); 
	}
	MPI_Finalize();
}

void * findem(void * rank)
{
	int myID = *((int *) rank);
	int startPos = ((long) myID) * (WIKI_ARRAY_SIZE / NumThreads);
  	int endPos = startPos + (WIKI_ARRAY_SIZE / NumThreads);
	if(myID == NumThreads - 1)
		endPos = WIKI_ARRAY_SIZE;
    	int i, j;
    	for(i = 0; i < WIKI_ARRAY_SIZE - 1 ; i++)  
    	{ 
		for(j = startPos; j < endPos; j++)
		{
       			lengthOfSubstring[i]= LCS((void*)wiki_array[i], (void*)wiki_array[i+1], longestSub);
       			longestSub++; 
		}
    	} 
}

void readToMemory()
{ 
	int nlines, maxlines = 10;
	int k, n, err, *count, nthreads = 24;
	int i;
	double nchars = 0;
	FILE *fd;
	wiki_array = (char **) malloc( WIKI_ARRAY_SIZE * sizeof(char *));
	for (i = 0; i < WIKI_ARRAY_SIZE; i++)//initializing the array
	{
	  	wiki_array[i] = malloc(2001);
	}
	//==================READING FILE=================
	fd = fopen("/homes/dan/625/wiki_dump.txt", "r");
	nlines = -1;
	do 
	{
		err = fscanf(fd, "%[^\n]\n", wiki_array[++nlines]);
		if(wiki_array[nlines] != NULL) 
			nchars += (double) strlen(wiki_array[nlines]);
	}
	while (err != EOF && nlines < WIKI_ARRAY_SIZE);
	fclose(fd);
	printf("Read in %d lines averaging %.01f chars/line\n", nlines, nchars / nlines);
}

void printToFile()//self-explanitory. Prints the substrings found to a file for processing
{
	FILE *f = fopen("LargestCommonSubstrings.txt", "w");
	if (f == NULL)//if there's a problem with the file error handling
	{
    		printf("Error opening LargestCommonSubstrings.txt!\n");
    		exit(1);
	}
	longestSub = longestSub - (WIKI_ARRAY_SIZE - 1);
	int i; 
	for(i = 0; i < WIKI_ARRAY_SIZE - 2; i++)
	{
		fprintf(f, "%d-%d: %s", i, i + 1,longestSub[i]);
		fprintf(f, "\n");
	}
	fclose(f);
}

void printResults()//printing to console for debugging
{ 
  	int i;
	longestSub = longestSub - (WIKI_ARRAY_SIZE - 1);
  	for(i = 0; i <= WIKI_ARRAY_SIZE - 2; i++)
  	{ 
      		printf("%d-%d: %s", i , i + 1 ,longestSub[i]); 
      		printf("\n");
  	}
}

int LCS(char *s1, char *s2, char **longest_common_substring)
{
    	int s1_length = strlen(s1);
    	int s2_length = strlen(s2);
	int ** _matrix;
	int _matrix_row_size = 0;
	int _matrix_collumn_size = 0;
    	if (s1_length+1 > _matrix_row_size || s2_length+1 > _matrix_collumn_size)
    	{
		int i;
		/* malloc matrix */
		_matrix = (int **)malloc((s1_length+1) * sizeof(int*));
		for (i = 0; i < s1_length+1; i++)
	    		_matrix[i] = (int *)malloc((s2_length+1) * sizeof(int));
		_matrix_row_size = s1_length+1;
		_matrix_collumn_size = s2_length+1;
    	}
	int i;
    	for (i = 0; i <= s1_length; i++)
		_matrix[i][s2_length] = 0;
    	int j;
    	for (j = 0; j <= s2_length; j++)
		_matrix[s1_length][j] = 0;
    	int max_len = 0, max_index_i = -1;
        for (i = s1_length-1; i >= 0; i--)
        {
    		for (j = s2_length-1; j >= 0; j--)
		{
    	    		if (s1[i] != s2[j])
	    		{
    				_matrix[i][j] = 0;
    				continue;
    	    		}
    	    		_matrix[i][j] = _matrix[i+1][j+1] + 1;
    	    		if (_matrix[i][j] > max_len)
	    		{
    				max_len = _matrix[i][j];
    				max_index_i = i;
    	    		}
    		}
	 }
    	if (longest_common_substring != NULL)
    	{
		*longest_common_substring = malloc(sizeof(char) * (max_len+1));
		strncpy(*longest_common_substring, s1+max_index_i, max_len);
		(*longest_common_substring)[max_len] = '\0';
    	}		/* free matrix */
	for (i = 0; i < _matrix_row_size; i++)
    		free(_matrix[i]);
	free(_matrix);
    	return max_len;
}
