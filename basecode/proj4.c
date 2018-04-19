#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>

#define WIKI_ARRAY_SIZE 1000000
#define WIKI_LINE_SIZE 2001

//load the lines into an array
char wiki_array[WIKI_ARRAY_SIZE] [WIKI_LINE_SIZE];
char longestSubstring[WIKI_ARRAY_SIZE + 1] [500]; 

bool readToMemory();
char* longestCommonSubstring(char* X, char* Y);

int main()
{
    struct timeval time1, time2, time3, time4;
    double elapsed;
    int numSlots, line_num, Version = 1; //what
    //read file into mem
    bool success = readToMemory();
    gettimeofday(&time1, NULL); 
    //probably some sort of loop checking lone 0 to 1, 1 to 2, .... 999999 to million
    int i;
    for(i = 0; i < WIKI_ARRAY_SIZE; i++)  
    {
         char * temp = longestCommonSubstring(wiki_array[i], wiki_array[i+1]);
         strcpy(longestSubstring[i] , temp);
    }   
     
    gettimeofday(&time2, NULL);
    elapsed = (time2.tv_sec - time1.tv_sec);
    printf("DATA, %d, %s, %f\n", Version, getenv("NSLOTS"), elapsed); //this needs work
}

bool readToMemory()
{
    char * filename = "~dan/625/wiki_dump.txt";    
    FILE * file = fopen(filename, "r"); /* should check the result */
    if(file == NULL) 
    {
        printf("failed to open");
        return false;
    }
    /* Read each wiki line into memory. */
    int line_num = 0;
    char * line = malloc(WIKI_LINE_SIZE);
    while(fgets(line, WIKI_LINE_SIZE, file) != NULL) 
    {
          strcpy(wiki_array[line_num], line);
          line_num++;
    }
    fclose(file);
    free(line);
    return true;
}

void printResults()
{ int i;
  for(i=0; i <= 1000000; i++)
  {
      printf("%d-%d : %s", i,i+1,longestSubstring[i]); 
      printf("\n");
  }
}

char* longestCommonSubstring(char* X, char* Y)
{
    int lenA = 0;
    int lenB = 0;
    char* looper = X;
    while(looper != '/n')
    {
        lenA++;
        looper++;
    }
    looper = Y;
    while(looper != '/n')
    {
        lenB++;
        looper++;
    }
    
    
    int j, i;
    
    // required longest common substring
    return resultStr;
    
}
    



