#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include <pthread.h>

#define WIKI_ARRAY_SIZE 50000
#define WIKI_LINE_SIZE 2001
#define num_threads 32

pthread_mutex_t lock;

int lengthOfSubstring [WIKI_ARRAY_SIZE];
int LCS (char * s1, char * s2, char ** longest_common_substring);
void * loopingFunc(void * tid);

//load the lines into an array
char  **wiki_array;
char **longestSub;

void readToMemory();
void printResults();
void printToFile();

int main()
{
	struct timeval time1;
    	struct timeval time2;
    	struct timeval time3;
    	struct timeval time4;
    	double e1, e2, e3;    
    	int Version = 2; //base = 1, pthread = 2, openmp = 3, mpi = 4    
	
	int rc, i, tids[num_threads];
	pthread_t threads[num_threads];
	pthread_attr_t attr;
	void *status;
	
	/* Initialize and set thread detached attribute */
	pthread_mutex_init(&lock, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
    	gettimeofday(&time1, NULL);
    	readToMemory();
    	gettimeofday(&time2, NULL);
	
    	//time to read to memory	
    	e1 = (time2.tv_sec - time1.tv_sec) * 1000.0; //sec to ms 
    	e1 += (time2.tv_usec - time1.tv_usec) / 1000.0; // us to ms
    	printf("Time to read full file to Memory: %f\n", e1);
	
    	gettimeofday(&time3, NULL);	
       
	for ( i = 0; i < num_threads; i++)
	{
		rc = pthread_create(&threads[i], &attr, loopingFunc, (void *) &tids[i]);
              	if(rc)
	      	{
		    printf("ERROR; return code from pthread_create() is %d\n", rc);
	            exit(-1);	
	     	}		
	}
	
        for(i=0; i<num_threads; i++)
        {
             rc = pthread_join(threads[i], &status);
             if (rc)
             {
	        printf("ERROR; return code from pthread_join() is %d\n", rc);
	        exit(-1);
              }
       }
    	printResults();
   	gettimeofday(&time4, NULL);
	
   	//time to find all longest substrings	
   	e2 = (time4.tv_sec - time3.tv_sec) * 1000.0; //sec to ms
   	e2 += (time4.tv_usec - time3.tv_usec) / 1000.0; // us to ms
   	printf("Time find all Substrings: %f\n", e2);
   
   	//total elapsed time between reading and finding all longest substrings	
   	e3 = (time4.tv_sec - time1.tv_sec) * 1000.0; //sec to ms
   	e3 += (time4.tv_usec - time1.tv_usec) / 1000.0; // us to ms
   	printf("DATA, %d, %s, %f\n", Version, getenv("NSLOTS"), e3); 
	
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&lock);
	pthread_exit(NULL);
	
}

void * loopingFunc(void * tid)
{
	int * myID = (int *) tid;
	//start position of the array
	int startPos = ((int) myID) * (WIKI_ARRAY_SIZE / num_threads);
	char **threadLongestSub = longestSub + startPos;
	//end position of the array
	int endPos = startPos + (WIKI_ARRAY_SIZE / num_threads);
	
	if((int)myID == num_threads -1)
	{
           endPos = WIKI_ARRAY_SIZE - 1 ;
	}
	
    	int j;
	
	   for(j = startPos; j < endPos; j++)
	   {
       		LCS((void*)wiki_array[j], (void*)wiki_array[j+1], threadLongestSub);
       		threadLongestSub++;    
	   } 
	pthread_exit(NULL);
} 

void readToMemory()
{ 
	int nlines = 10;
	int err;
	int i;
	double nchars = 0;
	FILE *fd;
	
	 //Adding malloc for space
	wiki_array = (char **) malloc( WIKI_ARRAY_SIZE * sizeof(char *));

	for (i = 0; i < WIKI_ARRAY_SIZE; i++)
	{
	  	wiki_array[i] = malloc(2001 * sizeof(char));
	}
	//saved results
	longestSub = (char **) malloc( WIKI_ARRAY_SIZE * sizeof(char *));

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

void printToFile()
{
	FILE *f = fopen("LargestCommonSubstrings.txt", "w");
	if (f == NULL)
	{
    		printf("Error opening LargestCommonSubstrings.txt!\n");
    		exit(1);
	}
	
	int i; 
	for(i = 0; i < WIKI_ARRAY_SIZE - 2; i++)
	{
		fprintf(f, "%d-%d: %s", i, i + 1,longestSub[i]);
		fprintf(f, "\n");
	}
	
	fclose(f);
}

void printResults()
{ 
  	int i;
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
		pthread_mutex_lock(&lock);
		//int theInt = (int) sizeof(char) * (int) (max_len + 1);
		longest_common_substring = malloc(sizeof(char) * (max_len + 1));
		strncpy(*longest_common_substring, s1+max_index_i, max_len);
		(*longest_common_substring)[max_len] = '\0';
		pthread_mutex_unlock(&lock);
    	}		/* free matrix */
	for (i = 0; i < _matrix_row_size; i++)
    		free(_matrix[i]);
	free(_matrix);
    	return max_len;

}
