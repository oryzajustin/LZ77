#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "libpnm.h"

struct token 
{
	int offset, match_length;
	unsigned char next_value;
};

void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size,
float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr);


