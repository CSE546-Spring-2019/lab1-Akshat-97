/*
CSE-5462 Network Programming
Lab-1 Assignment
Author: Akshat Devrani (devrani.1)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*To handle the constraint of maximum permitted buffer size. The data from the file will be read in chunks of 100 bytes*/
#define max_buffer 100

int *compute_prefix_function(char *pattern, int psize);
void kmp(char *target, int tsize, char *pattern, int psize, int *count);
void compute_file_size(FILE *input_file, FILE *output_file);


int main(int argc, char *argv[])
{
	/* Check if the input arguments are entered correctly */
	if(argc!=4)
	{
		printf("Inavlid number of input arguments, input should be of the form: <input-filename> <search-string> <output-filename> \n");
	}
	
	FILE *input_file;
	
	/*Check if the input file was correctly opened */
	input_file= fopen(argv[1],"rb");
	if(input_file == NULL)
	{
		printf("Input file could not be accessed, please try again. \n");
	}
			
	FILE *output_file;

	/*Check if the output file was correctly opened */
	output_file= fopen(argv[3], "wb");
	if(output_file == NULL)
	{
		printf("Output file could not be accessed, please try again. \n");
	}
	
	/*Calculate the length of the string to be searched*/
	int pattern_length=0;
	int count= 0;
	while(argv[2][pattern_length] != '\0')
	{
		pattern_length++;
	}
	printf("The size of the search string is: %d \n", pattern_length);
	
	/*Read the data from the input file*/
	char* target=  malloc(max_buffer* sizeof(char));
	int chunk_size = fread(target, sizeof(char), max_buffer, input_file);
	
	/*Search for the string. The case where the string rolls over from one buffer to the next is handled by moving the pointr back to overlap the buffers appropriately*/	 
	while(chunk_size>0)
	{
		kmp(target, chunk_size, argv[2], pattern_length, &count);
		if(feof(input_file))
			break;
		fseek(input_file, -1L*(pattern_length-1), SEEK_CUR); 
		chunk_size = fread(target, sizeof(char), max_buffer, input_file);
	}
	
	compute_file_size(input_file, output_file);
	printf("The number of occurences of the searched string in the input file are: %d \n", count);
	fprintf(output_file, "The number of occurences of the searched string in the input file are: %d \n", count);
	free(target);
	fclose(input_file);
	fclose(output_file);
}


/*Function to compute and print the size of the input file. */
void compute_file_size(FILE *input_file, FILE *output_file)
{
	fseek(input_file,0L, SEEK_END);
	long int file_size= ftell(input_file);
	rewind(input_file);
	printf("The size of the input file is: %ld \n", file_size);
	fprintf(output_file, "The size of the input file is: %ld \n", file_size);
}


/*Function to compute the prefix function for employing the KMP algorithm */	
int *compute_prefix_function(char *pattern, int psize)
{
	int k = -1;
	int i = 1;
	int *pi = malloc(sizeof(int)*psize);
	if (!pi)
		return NULL;

	pi[0] = k;
	for (i = 1; i < psize; i++) 
	{
		while (k > -1 && pattern[k+1] != pattern[i])
			k = pi[k];
		if (pattern[i] == pattern[k+1])
			k++;
		pi[i] = k;
	}
	return pi;
}


/*Function to search for the number of occurences of the string provided by the user from the command line*/
void kmp(char *target, int tsize, char *pattern, int psize, int *count)
{
	int i;
	int *pi = compute_prefix_function(pattern, psize);
	int k = -1;
	if (!pi)
		return;
	for (i = 0; i < tsize; i++) 
	{
		while (k > -1 && pattern[k+1] != target[i])
			k = pi[k];
		if (target[i] == (pattern[k+1] & 0xff))
			k++;
		if (k == psize - 1) 
		{
			*count= *count + 1;
		}
	}
	free(pi);
}
