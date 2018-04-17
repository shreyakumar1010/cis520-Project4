#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 1000000

const char * wiki_file;

int longestline()
{
	printf("finding the longest line length \n");
	FILE *f = fopen("~dan/625/wiki_dump.txt", "rb"); //open file
	
	if (f == NULL)
	{
		printf("failed to open \n");
		return -1;
	}
	
	printf("file opened \n");
	fseek(f, 0, SEEK_END); //run to the end of the file to find its length
	long size = ftell(f); //size is the total number of characters in the file
	printf("total chars in file is : ");
	printf("%d", size);
	printf("\n");
	
	void * wiki = malloc(size +1); //allocating the string
	fread(wiki, size, 1, f); //reading the file into wiki string
	printf("file read");
	fclose(f);//closing the file
	
	wiki_file = wiki;
	
	int i = 0;
	int temp = 0;
	int count = 0;
	while(i <= size) //while we aren't at the end of the string
	{
		temp ++; //increment the temp for each character
		if(wiki[i] == '\n') //until you get a new line.
		{
			if(temp > count) //if the temp is higher than current count
			{
				count = temp;//set count to the temp
			}
			temp = 0; //since we've received a new line we need to reset temp
		}
		i++; //recursion
	}
	return count;		
}


main() 
{
	int num = longestline();
	printf("%d", num);
}
