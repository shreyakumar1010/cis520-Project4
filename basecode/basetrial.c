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
struct substring 
{ int length;
  char * location;
}

substring longestCommonSubstring(char* X, char* Y)
{   
    int lenX = 0;
    int lenY = 0;
    int z = 0;
    char * ret;
    char * retend;
    substring returnSubstring;
    char* looper = X;
    while(looper != '/n')
    {
        lenX++;
        looper++;
    }
    looper = Y;
    while(looper != '/n')
    {
        lenY++;
        looper++;
    }
    int L [ lenX + 1] [lenY + 1];
    //initialize the array to 0s
    int p, k;
    for (p = 0; p < lenX + 1; p++)
    {
        for( k = 0; k < lenY + 1; k++)
        { L[p, k] = 0;}
    }
   //made arrays of the two strings 
    char AX[lenX + 1];
    strcpy(AX, X);
    char AY[lenY + 1];
    strcpy((L + lenX +1), Y);
    
    //strcpy(dest, src);
    subArray[lenX+1];
    
  
        
    //algo
    int j, i;
    for( i = 0; i < lenX; i++)
    {
        for(j = 0; j < lenY; j++)
        {
            if(AX[i] == AY[j])
               {
                   if (i == 1 || j == 1)
                   {
                       L[i,j] = 1;
                   }
                   else
                   {
                       L[i,j] = L[i-1, j-1] + 1;
                   }
                   if( L[i,j] > z)
                   {
                       z = L[i,j];
                       ret = (void*) AX[i - z + 1];
                       retend = (void*) AX[i]; 
                   }
                else
                {
                    if(L[i,j] == z)
                    {
                        //position of substring 
                        int v = i - z + 1;
                        
                        //total string length - position of substring
                        int u = i - v;
                        
                        //looping through their difference to copy the contents onto ret 
                        for (u; u < i; u++)
                        {
                            
                        }
                    }
                }
                
                else
                {
                    L[i, j] += 0;
                
               }
        }
    }
    
    // required longest common substring
    return ret;
    
}
