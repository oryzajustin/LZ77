#include "lz77_decoding_function.h"

int main(int argc, char * argv[])
{	
	char * file_to_decode;

	// -------------------------------------------------------- Preliminary Check -------------------------------------------------------- //

  	//check if the user entered 2 arguments (mandatory)
  	if ((argc - 1) != 1) // argv[0] = program execution call, so omit the first index when checking correct fields
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
    			file_to_decode = malloc(sizeof(char) * (strlen(argv[input]) + 1));
    			strcpy(file_to_decode, argv[input]);
    		}
    		else
    		{
    			printf("%s\n", "Invalid input file name.");
    			return 0;
    		}
    	}
    }
    Decode_Using_LZ77(file_to_decode);
}