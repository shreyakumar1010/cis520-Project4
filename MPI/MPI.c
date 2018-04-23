#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ARRAY_SIZE 1000000

#define MAX_LINE_SIZE 2001 //after running the longestline function once we know this value.
			   //we are hard coding it so we don't waste time counting again.

char wiki_array [ARRAY_SIZE][MAX_LINE_SIZE];

int longestline()//after using this, we know that the value is 2001. 
{
	FILE *f = fopen("/homes/dan/625/wiki_dump.txt", "r"); //open file
	if (f == NULL)
	{
		printf("failed to open \n");
		return -1;
	}
	fseek(f, 0, SEEK_END); //run to the end of the file to find its length
	long size = ftell(f); //size is the total number of characters in the file
	rewind(f);
	char * wiki = malloc(size +1); //allocating the string
	fread(wiki, sizeof(char), size, f); //reading the file into wiki string
	fclose(f);//closing the file
	int i, temp, count = 0;
	while(i <= size) //while we aren't at the end of the string
	{
		temp = temp +1; //increment the temp for each character
		if((char) wiki[i] == '\n') //until you get a new line.
		{
			if(temp > count) //if the temp is higher than current count
				count = temp;//set count to the temp
			temp = 0; //since we've received a new line we need to reset temp
		}
		i++; //recursion
	}
	return count;		
}

bool read_wiki()
{
	int c;
	FILE *f = fopen("/homes/dan/625/wiki_dump.txt", "r");
	if(f == NULL)
	{
		printf("failed to open file \n");
		return (false); 
	}
	rewind(f);
	int i, j = 0;
	while(!feof(f))
	{
		c = fgetc(f);
		if(c == '\n' || c == '\r')
		{
			i++;
			j = 0;
		}
		else 
		{
			wiki_array[i][j] = (char) c;
		}
		j++;
	}
	fclose(f);
	return (true);
}

char * compare_lines(int start)
{
	printf("Comparing lines ");
	printf("%d", start);
	printf(" and ");
	printf("%d", start+1);
	printf("\n");
	
	char * lineA = malloc( sizeof(char) * ( MAX_LINE_SIZE + 1 ) )
	lineA = &wiki_array [start][0];
	char * lineB = malloc( sizeof(char) * ( MAX_LINE_SIZE + 1 ) )
	lineB =	&wiki_array [start+1][0];
	bool matches = false;
	int size1, size2 = 0;
	
	char * common = malloc(sizeof(char)*(MAX_LINE_SIZE +1);
	char * longest= malloc(sizeof(char)*(MAX_LINE_SIZE +1);
	int i, j, k, l;
	int lasti = 0;
	k = 0;
	for(i = 0; i < MAX_LINE_SIZE; i++)
	{
		for(j = 0; j < MAX_LINE_SIZE; j++)//look at each value in lineA compared to each value in lineB
		{
			if(lineA[i] == lineB[j])//if there's a match
				matches = true;
			else //if the char isn't a match
			{ 
				matches = false; //set flag that it doesn't match
				k = 0; //reset k to 0 so we can overwrite the stored value
				if(size1 > 0)//we've received a full matching substring
				{ 
					printf("We have found a matching substring : ");
					for(l = 0; l < size1; l++)
					    printf("%c", (char) common[l]);
					printf("\n"); 
					    
					i = i - lasti; //reset i  
					lasti = 0; //reset lasti    
		
					if(size1 > size2)// its longer than our previous
					{
						size2 = size1; //store the length of our longest string
						printf("\n");
						printf("WE HAVE A NEW LONGEST SUBSTRING : \n");
						for(l = 0; l < size2; l++);
						{
							longest[l] = common[l];
							printf("%c", (char) longest[l]);
						}
						printf("\n");printf("\n");
					}
					size1 = 0; //reset size 1
				}
			}
			if(matches == true)
			{
				lasti ++; //how many times we have incremented i in a row
				common[k] = lineB[j]; //store the common char
				size1 ++; //increment the size of the current string
				i++; //increment i so we can look at next value
				k++; //increment k so we can store next value
			}
		}
	}
	printf("Finished \n");
	//for(i = 0; i < size2; i++)
	//	printf("%c", (char) longest[i]); 
	return (longest);
}

main() 
{
	bool success = read_wiki();
	printf("finished reading \n");
	
	compare_lines(0);

	
}
