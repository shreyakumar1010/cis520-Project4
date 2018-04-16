#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 1000000

const char * wiki;

int longestline()
{
	FILE *f = fopen("wiki_dump.txt", "r"); //open file
	fseek(f, 0, SEEK_END); //run to the end of the file to find its length
	long size = ftell(f); //size is the total number of characters in the file
	wiki = malloc(size +1); //allocating the string
	fread(wiki, size, 1, f); //reading the file into wiki string
	fclose(f);//closing the file
	
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
