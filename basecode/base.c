#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>

#define WIKI_ARRAY_SIZE 10
#define WIKI_LINE_SIZE 10

//load the lines into an array
 char  **wiki_array;
 char **longestSub;


 //int p = 0;

 //char longestSubstring[WIKI_ARRAY_SIZE - 1] [10] ; 
 int lengthOfSubstring[WIKI_ARRAY_SIZE] ;
 

bool readToMemory();
int LCS(char *s1, char *s2, char **longest_common_substring);
void printResults();


int main()
{
    struct timeval time1, time2, time3, time4;
    double elapsed;
    int numSlots, line_num, Version = 1; //what
    //printf("in main after timing vars");
    //read file into mem
    bool success = readToMemory();
    //printf("in main after Read");
    gettimeofday(&time1, NULL); 
    //probably some sort of loop checking lone 0 to 1, 1 to 2, .... 999999 to million
    int i;
    for(i = 0; i < WIKI_ARRAY_SIZE - 1 ; i++)  
    { 
	  
          char* temp;
	  
          lengthOfSubstring[i]= LCS((void*)wiki_array[i], (void*)wiki_array[i+1], longestSub);
         
	  longestSub++;
          //strcpy(longestSubstring[i] , temp);
    }   
      //printResults();	 
     
    //gettimeofday(&time2, NULL);
    //elapsed = (time2.tv_sec - time1.tv_sec);
    //printf("DATA, %d, %s, %f\n", Version, getenv("NSLOTS"), elapsed); //this needs work
}

bool readToMemory()
{ 
	int nlines, maxlines = 10;
	//int nwords, maxwords = 10;
	int i, k, n, err, *count, nthreads = 24;
	double nchars = 0;
	//double tstart, ttotal;
	FILE *fd;
	
	 //Adding malloc for space
	wiki_array = (char **) malloc( WIKI_ARRAY_SIZE * sizeof(char *));

	for (i; i < WIKI_ARRAY_SIZE; i++)
	{
	  wiki_array[i] = malloc(2001);
	}
	//saved results
	longestSub = (char **) malloc( WIKI_ARRAY_SIZE * sizeof(char *));

	for (i; i < WIKI_ARRAY_SIZE -1; i++)
	{
	  longestSub[i] = malloc(2001);
	}


       
	fd = fopen("/homes/coreyvessar/cis520/cis520-Project4/basecode/sampletext.txt", "r");
	nlines = -1;
	do {
	err = fscanf(fd, "%[^\n]\n", wiki_array[++nlines]);
	if(wiki_array[nlines] != NULL) nchars += (double) strlen(wiki_array[nlines]);
	}
	while (err != EOF && nlines < WIKI_ARRAY_SIZE);
	fclose(fd);

	printf("Read in %d lines averaging %.01f chars/line\n", nlines, nchars / nlines);

}

void printResults()
{ int i;
 printf("do we even get to print results");
 longestSub = longestSub - WIKI_ARRAY_SIZE; 
  for(i=0; i <= WIKI_ARRAY_SIZE - 1; i++)
  { 
      printf("%d-%d : %s", i,i+1,longestSub[i]); 
      printf("\n");
  }
}

 static int **_matrix;
 static int _matrix_row_size = 0;
 static int _matrix_collumn_size = 0;


static void init(int s1_length, int s2_length){
    if (s1_length+1 > _matrix_row_size || s2_length+1 > _matrix_collumn_size){
	/* free matrix */
	int i;
	for (i = 0; i < _matrix_row_size; i++)
	    free(_matrix[i]);
	free(_matrix);
	
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
}


int LCS(char *s1, char *s2, char **longest_common_substring){
    int s1_length = strlen(s1);
    int s2_length = strlen(s2);

    init(s1_length, s2_length);

    int max_len = 0, max_index_i = -1;
    int i,j;
    for (i = s1_length-1; i >= 0; i--){
    	for (j = s2_length-1; j >= 0; j--){
    	    if (s1[i] != s2[j]){
    		_matrix[i][j] = 0;
    		continue;
    	    }

    	    _matrix[i][j] = _matrix[i+1][j+1] + 1;
    	    if (_matrix[i][j] > max_len){
    		max_len = _matrix[i][j];
    		max_index_i = i;
    	    }
    	}
    }

    if (longest_common_substring != NULL){
	*longest_common_substring = malloc(sizeof(char) * (max_len+1));
	strncpy(*longest_common_substring, s1+max_index_i, max_len);
	(*longest_common_substring)[max_len] = '\0';
	
	//strncpy(longestSub[p], *longest_common_substring, 800);
	    
        //p++;
	//printf("%s\n", *longest_common_substring);
    }

    return max_len;
}
