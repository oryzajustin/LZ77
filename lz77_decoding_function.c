#include "lz77_decoding_function.h"
#include "lz77_encoding_function.h"
#include <time.h>

void Decode_Using_LZ77(char *in_compressed_filename_Ptr) 
{
	clock_t begin = clock();

	FILE *fp;

	fp = fopen(in_compressed_filename_Ptr, "r"); // read mode

	if (fp == NULL) // if failed to open the file
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	int value;
	int img_width, img_height;
	int buffer_size;
	int num_tokens;

	// output file name // 

	char out_file_name[126] = {0};
	strcat(out_file_name, in_compressed_filename_Ptr);
	strcat(out_file_name, ".pgm");

	// --------------- //

	fscanf (fp, "%d", &value); // read in first value initially (width)
	img_width = value;
	fscanf (fp, "%d", &value); // height
	img_height = value;
	fscanf (fp, "%d", &value); // buffer size
	buffer_size = value;
	fscanf (fp, "%d", &value); // num tokens
	num_tokens = value;
	fscanf (fp, "%d", &value); // offset first val

	// --------------------- //

    int img_size = img_height * img_width;

    struct PGM_Image output_image; // hold the pgm image
	create_PGM_Image(&output_image, img_width, img_height, MAX_GRAY_VALUE);

    unsigned char * one_D_image_array = malloc(img_size * sizeof(unsigned char)); // image to be converted to PGM

    struct token * tokens = malloc(img_size * sizeof(struct token)); // read in all tokens

    // CREATE TOKENS //
    int token_count = 0;
    while (!feof (fp)) // reading in 3s
    {  
    	tokens[token_count].offset = value;
    	fscanf (fp, "%d", &value); // next value (match length)
    	tokens[token_count].match_length = value;
    	fscanf(fp, "%d", &value); // next value (next_value)
    	tokens[token_count].next_value = value;

    	fscanf(fp,"%d", &value);
    	token_count++;
    }

    fclose(fp); // close file pointer to the read file 

    int curr_token = 0;
    int pixel = 0;
    for(int j = 0; j < img_size; j++) // for the entire image going for each pixel
    {
    	if(j > img_size || pixel > img_size)
    	{
    		break;
    	}
    	int buffer_start = (int)(pixel - buffer_size); // buffer start set to 0 if below image start index
    	int curr_pixel = pixel; // temp int for placing successive values
    	if(buffer_start < 0)
    	{
    		buffer_start = 0;
    	}
		
		if(tokens[curr_token].match_length == 0) // just place the next value (default)
		{
			one_D_image_array[pixel] = tokens[curr_token].next_value;
		}
		if(tokens[curr_token].match_length > 0) // loop through matching, to place values 
		{
			int match_index = (int)(pixel - tokens[curr_token].offset);

			// assert(match_index >= 0);

			int curr_match_index = match_index;

			for(int i = 0; i < tokens[curr_token].match_length; i++)
			{
				if(curr_match_index == curr_pixel)
				{
					curr_match_index = match_index;
				}
				one_D_image_array[pixel] = one_D_image_array[curr_match_index];
				pixel++;
				curr_match_index++;
			}
			one_D_image_array[pixel] = tokens[curr_token].next_value;
		}

		pixel++;
		curr_token++;
    }

    FILE * fp2 = fopen(out_file_name, "wb"); // write mode to output file

	if (fp == NULL) // if failed to open the file
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	for(int row = 0; row < img_height; row++) // convert the 1D image to 2D
	{
		for(int col = 0; col < img_width; col++)
		{
			output_image.image[row][col] = one_D_image_array[(row * img_width) + col];
		}
	}

	free(one_D_image_array);

	save_PGM_Image(&output_image, out_file_name, false); // save the image SEG FAULT ON GAUL?
	fclose(fp2);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("Decompression time: %f\n", time_spent);
}