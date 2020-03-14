#include "mean_absolute_error.h"

float mean_absolute_error(char *file_name_1_ptr, char *file_name_2_ptr) 
{
	struct PGM_Image pgm_image1, pgm_image2;

	float image_scale;

	load_PGM_Image(&pgm_image1, file_name_1_ptr);
	load_PGM_Image(&pgm_image2, file_name_2_ptr);

	if(pgm_image1.height != pgm_image2.height || pgm_image1.width != pgm_image2.width)
	{
		return -1;
	}

	if(pgm_image1.maxGrayValue > pgm_image2.maxGrayValue)
	{
		image_scale = pgm_image1.maxGrayValue / pgm_image2.maxGrayValue; 
		pgm_image2.maxGrayValue *= image_scale;
	}
	else if(pgm_image2.maxGrayValue > pgm_image1.maxGrayValue)
	{
		image_scale = pgm_image2.maxGrayValue / pgm_image1.maxGrayValue; 
		pgm_image1.maxGrayValue *= image_scale;
	}

	int constant = 1/(pgm_image1.width * pgm_image1.height);

	int sum = 0;

	for(int row = 0; row < pgm_image1.height; row++)
	{
		for(int col = 0; col < pgm_image1.width; col++)
		{
			sum += abs(pgm_image1.image[row][col] - pgm_image2.image[row][col]);
		}
	}

	int mean_absolute_error = constant * sum;

	return mean_absolute_error;
}