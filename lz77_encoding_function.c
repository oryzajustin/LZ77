#include "lz77_encoding_function.h"
#include <unistd.h>
#include <time.h>

void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size,
float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr) 
{
	clock_t begin = clock();

	struct PGM_Image pgm_image;
	load_PGM_Image(&pgm_image, in_PGM_filename_Ptr);
	int img_height = pgm_image.height;
	int img_width = pgm_image.width;

	struct token * tokens = malloc(pgm_image.height * pgm_image.width * sizeof(struct token));

	int token_total = 0;

	unsigned char * one_d_image_array = malloc(img_height * img_width * sizeof(int));

	// output file name // 
	// char *test = (char*) malloc(12*sizeof(char));
	char new_name[126] = {0};
	// memset ( new_name, 0, 126 );
	strcat(new_name, in_PGM_filename_Ptr);
	char buffer_string[126] = {0};
	// memset ( buffer_string, 0, 126 );
	snprintf(buffer_string, 10, ".%d", searching_buffer_size);
	strcat(new_name, buffer_string);
	strcat(new_name, ".lz");

	// offset histogram file name

	char histogram_offset_file_name[126] = {0};
	// memset ( histogram_offset_file_name, 0, 126 );
	strcat(histogram_offset_file_name, in_PGM_filename_Ptr);
	strcat(histogram_offset_file_name, buffer_string);
	strcat(histogram_offset_file_name, ".offsets.csv");

	// match_length histogram file name

	char histogram_match_length_file_name[126] = {0};
	// memset ( histogram_match_length_file_name, 0, 126 );
	strcat(histogram_match_length_file_name, in_PGM_filename_Ptr);
	strcat(histogram_match_length_file_name, buffer_string);
	strcat(histogram_match_length_file_name, ".lengths.csv");

	// -------------- //

	int one_d = 0;
	for(int row = 0; row < pgm_image.height; row++) // convert image to 1D array
	{
		for(int col = 0; col < pgm_image.width; col++)
		{
			one_d_image_array[one_d] = pgm_image.image[row][col];
			one_d++;
		}
	}

	// for(int i = 0; i < (pgm_image.height * pgm_image.width)/50; i++) // debug
	// {
	// 	printf("%d ", one_d_image_array[i]);
	// }

	FILE * fp;
	fp = fopen(new_name, "wb");

	fprintf(fp, "%d %d\n", img_width, img_height);
	fprintf(fp, "%d\n", searching_buffer_size);

	for(int i = 0; i < img_height * img_width; i++) // going through entire 1D image
	{
		if(i >= img_height * img_width)
		{
			break;
		}
		// Create temporary token array
		struct token * tokens_temp = malloc(searching_buffer_size * sizeof(struct token));

		int token_count = 0;

		int buffer_index = i - searching_buffer_size; // buffer start (reset to 0 for first buffer_size elements)

		int curr_i = i;

		if(buffer_index < 0) // make sure the buffer index doesn't go out of bounds
		{
			buffer_index = 0;
		}

		// assert(buffer_index >= 0);

		for(; buffer_index < i; buffer_index++) // go through the buffer
		{	
			int offset = 0, match_length = 0; // variables for token creation

			unsigned char next_value;

			if(one_d_image_array[buffer_index] == one_d_image_array[curr_i]) // if we find a match in the buffer to our current i
			{
				offset = i - buffer_index; // store the offset

				for(int match_index = buffer_index; one_d_image_array[match_index] == one_d_image_array[curr_i]; match_index++) // as long as we keep finding matches
				{
					match_length++; // increase the length
					if(match_index + 1 == i) // reset buffer position
					{
						match_index = buffer_index - 1;
					}

					if(curr_i <= (img_height * img_width) - 1) // within the bounds, increase curr_i
					{
						curr_i++;
					}
					else
					{
						break;
					}
				}
			}
			next_value = one_d_image_array[curr_i]; // get next value

			tokens_temp[token_count].offset = offset;
			tokens_temp[token_count].match_length = match_length;
			tokens_temp[token_count].next_value = next_value;

			token_count++;
		}

		// find the best token to use
		struct token best_token;
		if(token_count != 0) // if we generated tokens, find the best one
		{
			int longest_length = -1;
			for(int j = 0; j < searching_buffer_size; j++)
			{
				if(tokens_temp[j].match_length > longest_length)
				{
					best_token = tokens_temp[j];
					longest_length = best_token.match_length;
				}
			}
		}
		else // generate a default token
		{
			best_token.offset = 0;
			best_token.match_length = 0;
			best_token.next_value = one_d_image_array[i];
		}

		tokens[token_total] = best_token;
		token_total++;

		// move i over by match length
		i += best_token.match_length;

		// free(tokens_temp);
	}

	fprintf(fp, "%d\n", token_total);

	for(int i = 0; i < token_total; i++) // write the offset, match_length, next_val of each token to output file
	{
		fprintf(fp, "%d ", tokens[i].offset);
		fprintf(fp, "%d ", tokens[i].match_length);
		fprintf(fp, "%u ", tokens[i].next_value);
	}

	free(one_d_image_array); // free memory
	fclose(fp); 

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("Compression time: %lf\n", time_spent);

	// ----------------------------------------- ordered offset csv ----------------------------------------- //

	FILE * fp2 = fopen(histogram_offset_file_name, "w");
	
	if(fp2 == NULL)
	{
		printf("FAIL");
	}

	int * offset_frequencies = calloc(searching_buffer_size, sizeof(int));

	for(int i = 0; i < token_total; i++) // generate offset frequencies
	{
		if(tokens[i].offset > 0)
		{
			offset_frequencies[tokens[i].offset]++;
		}
	}

	for(int i = 0; i < searching_buffer_size; i++)
	{
		if(offset_frequencies[i] > 0)
		{
			fprintf(fp2, "%d, %d\n", i, offset_frequencies[i]);
		}
	}
	// printf("%s", histogram_match_length_file_name);

	fclose(fp2);
	// ----------------------------------------- ordered match length csv ----------------------------------------- //
	
	FILE * fp3 = fopen(histogram_match_length_file_name, "w");

	if(fp3 == NULL)
	{
		printf("FAIL");
	}

	int * match_length_frequencies = calloc(img_width * img_height, sizeof(int));

	for(int i = 0; i < token_total; i++) // generate offset frequencies
	{
		if(tokens[i].match_length > 0)
		{
			match_length_frequencies[tokens[i].match_length]++;
		}
	}

	for(int i = 0; i < img_width  * img_height; i++)
	{
		if(match_length_frequencies[i] > 0)
		{
			fprintf(fp3, "%d, %d\n", i, match_length_frequencies[i]);
		}
	}

	fclose(fp3);

	// ----------------------------------------- calculate averages ----------------------------------------- //

	float offset_average = 0, match_length_average = 0;

	for(int i = 0; i < token_total; i++) // offset and match_length sum
	{
		offset_average += tokens[i].offset;
		match_length_average += tokens[i].match_length;
	}

	offset_average /= token_total;
	match_length_average /= token_total;

	// ----------------------------------------- calculate standard deviations ----------------------------------------- //

	float offset_std = 0, match_length_std = 0;
	float component_offset = 0, component_match_length = 0;

	for(int i = 0; i < token_total; i++) // offset standard deviation
	{
		component_offset += (tokens[i].offset - offset_average) * (tokens[i].offset - offset_average);
		component_match_length += (tokens[i].match_length - match_length_average) * (tokens[i].match_length - match_length_average);
	}

	offset_std = sqrt((component_offset / token_total));
	match_length_std = sqrt((component_match_length / token_total));

	// save them in pointers

	* avg_offset_Ptr = offset_average;

	* std_offset_Ptr = offset_std;

	* avg_length_Ptr = match_length_average;

	* std_length_Ptr = match_length_std;

	free_PGM_Image(&pgm_image);
}
