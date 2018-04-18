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

int main()
{
    struct timeval time1, time2, time3, time4;
    double elapsed;
    int numSlots, line_num, Version = 1; //what
    //read file into mem
    bool success = readToMemory();
    gettimeofday(&time1, NULL); 
    //probably some sort of loop checking lone 0 to 1, 1 to 2, .... 999999 to million
    for(int i = 0; i < WIKI_ARRAY_SIZE; i++)         
        longestSubString[i] = longestCommonSubstring(wiki_array[i], wiki_array[i+1]);          
    gettimeofday(&time2, NULL);
    elapsed = (time2.tv_sec - time1.tv_sec);
    printf("DATA, %d, %s, %f\n", Version, getenv("NSLOTS"), elapsed); //this needs work
}

bool readToMemory()
{
    char * filename = "~dan/625/wiki_dump.txt";    
    FILE * file = fopen(fileName, "r"); /* should check the result */
    if(file == NULL) 
    {
        printf("failed to open");
        return false;
    }
    /* Read each wiki line into memory. */
    int line_num = 0;
    char * line = malloc(WIKI_STRING_SIZE);
    while(fgets(line, WIKI_STRING_SIZE, file) != NULL) 
    {
          strcpy(wiki_array[line_num], line);
          line_num++;
    }
    fclose(file);
    free(line);
    return true;
}

void printResults()
{
  for(int i=0; i <= 1000000, i++)
  {
      printf("%d-%d : %s", i,i+1,longestSubstring[i]); 
      printf("\n");
  }
}

string longestCommonSubstring(const string &str1, const string &str2)
{
  if(str1.empty() || str2.empty())
        return 0;
  int *curr = new int [str2.size()];
  int *prev = new int [str2.size()];
  int *swap = NULL;
  int maxSubstr = 0;
  string longest;
  for(unsigned int i = 0; i < str1.size(); ++i)
  {
    for(unsigned int j = 0; j < str2.size(); ++j)
    {
      if(str1[i] != str2[j])
            curr[j] = 0;
      else
      {
        if(i == 0 || j == 0)
            curr[j] = 1;
        else
            curr[j] = 1 + prev[j-1];
        if(maxSubstr < curr[j])
        {
            maxSubstr = curr[j];
            longest.clear();
        }
        if (maxSubstr == curr[j])
            longest += str1.substr(i - maxSubstr + 1, i + 1);
      }
    }
    swap=curr;
    curr=prev;
    prev=swap;
  }
  delete [] curr;
  delete [] prev;
  return (longest.substr(0, maxSubstr));
}
    



