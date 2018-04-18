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
char * longestCommonSubstring(const char * str1, const char * str2);

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
{
  for(int i=0; i <= 1000000; i++)
  {
      printf("%d-%d : %s", i,i+1,longestSubstring[i]); 
      printf("\n");
  }
}

char* longestCommonSubstring(char* X, char* Y)
{
    // Create a table to store lengths of longest common
    // suffixes of substrings.   Note that LCSuff[i][j]
    // contains length of longest common suffix of X[0..i-1]
    // and Y[0..j-1]. The first row and first column entries
    // have no logical meaning, they are used only for
    // simplicity of program
    int m = strlen(X);
    int n = strlen(Y);
    int LCSuff[m + 1][n + 1];
 
    // To store length of the longest common substring
    int len = 0;
 
    // To store the index of the cell which contains the 
    // maximum value. This cell's index helps in building 
    // up the longest common substring from right to left.
    int row, col;
 
    /* Following steps build LCSuff[m+1][n+1] in bottom
       up fashion. */
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                LCSuff[i][j] = 0;
 
            else if (X[i - 1] == Y[j - 1]) {
                LCSuff[i][j] = LCSuff[i - 1][j - 1] + 1;
                if (len < LCSuff[i][j]) {
                    len = LCSuff[i][j];
                    row = i;
                    col = j;
                }
            } else
                LCSuff[i][j] = 0;
        }
    }
 
    // if true, then no common substring exists
    if (len == 0) {
        char* t = "No Common Substring";
        return t ;
    }
 
    // allocate space for the longest common substring
    char* resultStr = (char*)malloc((len + 1) * sizeof(char));
    resultStr[len] = '&#092;&#048;';
 
    // traverse up diagonally form the (row, col) cell
    // until LCSuff[row][col] != 0
    while (LCSuff[row][col] != 0) {
        resultStr[--len] = X[row - 1]; // or Y[col-1]
 
        // move diagonally up to previous cell
        row--;
        col--;
    }
 
    // required longest common substring
    return resultStr;
    
}
    



