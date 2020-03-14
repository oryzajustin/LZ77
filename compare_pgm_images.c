#include "mean_absolute_error.h"

int main(int argc, char * argv[])
{	
	char * pgm_image_name1;
    char * pgm_image_name2;

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
    			pgm_image_name1 = malloc(sizeof(char) * 50);
    			strcpy(pgm_image_name1, argv[input]);
    		}
    		else
    		{
    			printf("%s\n", "Invalid input file name.");
    			return 0;
    		}
    	}
    	else if(input == 2) // search buffer size
    	{
    		if(strcmp(argv[input], "") != 0) // check if the input file name isn't blank
            {
                pgm_image_name2 = malloc(sizeof(char) * 50);
                strcpy(pgm_image_name2, argv[input]);
            }
            else
            {
                printf("%s\n", "Invalid input file name.");
                return 0;
            }
    	}
    }
    printf("%f\n", mean_absolute_error(pgm_image_name1, pgm_image_name2));
}