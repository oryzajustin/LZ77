#include "lz77_encoding_function.h"

int main(int argc, char * argv[])
{	
	char * pgm_image_name;

	unsigned int search_buffer_size;

	float avg_offset, std_offset, avg_length, std_length;

	// -------------------------------------------------------- Preliminary Check -------------------------------------------------------- //

  	//check if the user entered 2 arguments (mandatory)
  	if ((argc - 1) != 2) // argv[0] = program execution call, so omit the first index when checking correct fields
    {
    	printf("Please try again with the correct inputs.\n"); // there must be 2 inputs, otherwise exit program
    	return 0;
    }

    // store relevant information from command line arguments
    for(int input = 1; input < argc; input++)
    {
    	if(input == 1)// input image name
    	{
    		if(strcmp(argv[input], "") != 0) // check if the input file name isn't blank
    		{
    			pgm_image_name = malloc(sizeof(char) * (strlen(argv[input]) + 1));
    			strcpy(pgm_image_name, argv[input]);
    		}
    		else
    		{
    			printf("%s\n", "Invalid input file name.");
    			return 0;
    		}
    	}
    	else if(input == 2) // search buffer size
    	{
    		int convert = atoi(argv[input]); // convert the command line argument into an integer value

    		if(convert > 0) // check if the buffer size is a positive value
    		{
    			search_buffer_size = convert;
    		}
    		else // error and terminate
    		{
    			printf("%s\n", "Invalid buffer size.");
    			return 0;
    		}
    	}
    }
    Encode_Using_LZ77(pgm_image_name, search_buffer_size, &avg_offset, &std_offset, &avg_length, &std_length);

    printf("Offset average: %f\n", avg_offset);
	printf("Offset standard deviation: %f\n", std_offset);

	printf("Match length average: %f\n", avg_length);
	printf("Match length standard deviation: %f\n", std_length);

}