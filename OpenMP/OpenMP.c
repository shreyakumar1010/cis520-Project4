#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include <omp.h>

#define WIKI_ARRAY_SIZE 500 //How many lines are in the array (For testing we use 500, for final, we use 1M)
#define WIKI_LINE_SIZE 2001 //How many maximum characters there are per line
#define num_threads 1			    //We found this value via a function written in a previous commit

int lengthOfSubstring [WIKI_ARRAY_SIZE];

int myID;

int LCS (char * s1, char * s2, char ** longest_common_substring);

char  ** wiki_array; //array that the wiki_dump file is stored into once read in
//char ** longestSub;
char ** longestSubChunk[num_threads];

//omp_lock_t theLock;//locking to avoid race conditions



void readToMemory();
void printResults();
void printToFile();

int main()
{
	//omp_init_lock(&theLock);//init lock
	struct timeval time1;
    	struct timeval time2;
    	struct timeval time3;
    	struct timeval time4;
    	double e1, e2, e3;    
    	int numSlots, Version = 3; //base = 1, pthread = 2, openmp = 3, mpi = 4
    
    	//======================Reading in=============================
	gettimeofday(&time1, NULL);
    	readToMemory();
    	gettimeofday(&time2, NULL);
    	//time to read to memory	
    	e1 = (time2.tv_sec - time1.tv_sec) * 1000.0; //sec to ms 
    	e1 += (time2.tv_usec - time1.tv_usec) / 1000.0; // us to ms
    	printf("Time to read full file to Memory: %f\n", e1);
	
	//==========Finding Longest Substrings & Parallelizing==========
    	gettimeofday(&time3, NULL);	
    	int i,j, startPos, endPos;
	
	omp_set_num_threads(num_threads);
	#pragma omp parallel private(myID, startPos, endPos, j)
	{     //allocate the chunk of answer array 
		//saved results
		
		myID = omp_get_thread_num();
                startPos = (myID) * (WIKI_ARRAY_SIZE / num_threads);
                endPos = startPos + (WIKI_ARRAY_SIZE / num_threads);
		if(myID == num_threads-1)
                {
                    endPos = WIKI_ARRAY_SIZE - 1 ;
                }
		
		longestSubChunk[myID] = (char **) malloc( (endPos - startPos)  * sizeof(char *));

		for (i = 0; i < (endPos - startPos) -1; i++)
		{
			longestSubChunk[myID][i] = malloc(2001);
		}
		
		//longestSub = longestSub + startPos;
                
			for (j = startPos; j< endPos; j++)
			{
				lengthOfSubstring[j]= LCS((void*)wiki_array[j], (void*)wiki_array[j+1], longestSubChunk[myID]);
				longestSubChunk[myID]++;   
			} 
	}
    	printResults();
	//printToFile();
   	gettimeofday(&time4, NULL);
   	//time to find all longest substrings	
   	e2 = (time4.tv_sec - time3.tv_sec) * 1000.0; //sec to ms
   	e2 += (time4.tv_usec - time3.tv_usec) / 1000.0; // us to ms
   	printf("Time find all Substrings: %f\n", e2);
   
   	//total elapsed time between reading and finding all longest substrings	
   	e3 = (time4.tv_sec - time1.tv_sec) * 1000.0; //sec to ms
   	e3 += (time4.tv_usec - time1.tv_usec) / 1000.0; // us to ms
   	printf("DATA, %d, %s, %f\n", Version, getenv("NSLOTS"), e3); 
}

void readToMemory()
{ 
	int nlines, maxlines = 10;
	int k, n, err, *count;
	int i;
	double nchars = 0;
	FILE *fd;
	//Adding malloc for space
	wiki_array = (char **) malloc( WIKI_ARRAY_SIZE * sizeof(char *));

	for (i = 0; i < WIKI_ARRAY_SIZE; i++)
	{
	  	wiki_array[i] = malloc(2001);
	}
	//saved results
	//longestSub = (char **) malloc( WIKI_ARRAY_SIZE * sizeof(char *));

	/*for (i = 0; i < WIKI_ARRAY_SIZE -1; i++)
	{
	  	longestSub[i] = malloc(2001);
	}*/
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

/*
void printToFile()
{
	FILE *f = fopen("LargestCommonSubstrings.txt", "w");
	if (f == NULL)
	{
    		printf("Error opening LargestCommonSubstrings.txt!\n");
    		exit(1);
	}
	//longestSub = longestSub - (WIKI_ARRAY_SIZE - 1);
	int i; 
	for(i = 0; i < WIKI_ARRAY_SIZE - 2; i++)
	{
		fprintf(f, "%d-%d: %s", i, i + 1,longestSub[i]);
		fprintf(f, "\n");
	}
	fclose(f);
}
*/

void printResults()
{ 
  	int i, j;
	//longestSub = longestSub - (WIKI_ARRAY_SIZE - 1);
	int chunksize = WIKI_ARRAY_SIZE / num_threads;
	int lineNumber = 0;
	int lineNumberPlusOne = 1;
	for ( i = 0; i < num_threads; i++)
	{
		for  (j = 0; j < chunksize; j++)
		{
			printf("%d-%d: %s",lineNumber  ,lineNumberPlusOne  ,longestSubChunk[i]); 
			printf("\n");
			longestSubChunk[i]++;
			lineNumber++;
			lineNumberPlusOne++;
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
		//omp_set_lock(&theLock);
		*longest_common_substring = malloc(sizeof(char) * (max_len+1));
		strncpy(*longest_common_substring, s1+max_index_i, max_len);
		(*longest_common_substring)[max_len] = '\0';
		//omp_unset_lock(&theLock);
		//printf("%s\n", *longest_common_substring);
    	}		/* free matrix */
	for (i = 0; i < _matrix_row_size; i++)
    		free(_matrix[i]);
	free(_matrix);
    	return max_len;

}
