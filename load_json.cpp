#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "haversine.cpp"

// The data are in name/value pairs
// Data objects are separated by commas
// Curly braces {} hold objects
// Square brackets [] hold arrays
// Each data element is enclosed with quotes "" if it is a character, or without quotes if it is a numeric value
// ir kad paskutinis elementas neturi ",".

// geresne macro function
// #define MIN(A, B) ({ \
//						typeof(A) _a = (A); \
//						typeof(A) _a = (A); \
//						_a < _b ? _a : _b; \				
//						})

// arr[x] is the same as *(arr + x)

typedef uint8_t u8;
typedef double f64;

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define false 0
#define true 1

int main (void)
// int main (int argc, char **argv)
{
	int if_start = false; 
	int if_end = false;

	FILE *jsonf = fopen("data_1000_flex.json", "r");
	if (jsonf == NULL)
	{
		printf("Unable to open file, good luck\n");
	}
	
	int end_seek_idx = -1;
	
	int start_count_nl = 1;
	int start_ignore_s_t_nl = 0;
	int end_ignore_s_t_nl = 0;
	int bad_chars = 0;
	
	int end_step = 2;

	char start_char = (char)fgetc(jsonf);

	int first_bracket = false;
	while(start_char != '{')
	{
		if (start_char == ' ' || start_char == '\n' || start_char == '\t')
		{
			start_ignore_s_t_nl++;
			if (start_char == '\n')
			{
				start_count_nl++;
			}
			start_char = (char)fgetc(jsonf);
		}
		else
		{
			bad_chars++;
			break;
		}
	}

	if (bad_chars != 0)
	{
		printf("Incorrect json file at line %d\n", start_count_nl);
		return false;
	}

	if(start_char == '{')
	{
		if_start = true;
		first_bracket = true;
		printf("START {\n");
	}

	fseek(jsonf, end_seek_idx, SEEK_END);
	char end_char = (char)fgetc(jsonf);

	long data_length = ftell(jsonf);
	printf("data_length %ld\n", data_length);

	while(end_char != '}')
	{	
		fseek(jsonf, end_seek_idx, SEEK_END);
		end_char = (char)fgetc(jsonf);
		
		if (end_char == ' ' || end_char == '\n' || end_char == '\t')
		{
			end_ignore_s_t_nl++;
		}
		else
		{
			break;
		}
		end_seek_idx--;
	}

	if (end_char == '}')
	{
		if_end = true;
		printf("END {\n");
	}
	else
	{
		printf("There should be no any charater in end of json file\n");
		return false;
	}
	fseek(jsonf, 0, SEEK_SET);

	int angle_brack_idx = -1;
	f64 sum = 0;
	int count_elem = 0;

	if (if_start && if_end)
	{
		// +1 because of \0 for c string arrays
		long full_length = data_length + 1;
		printf("full_length %ld\n", full_length);
		char *data_buffer = (char *) malloc((full_length) * sizeof(char));	
		data_buffer[data_length] = '\0';

		size_t bytes_read = fread(data_buffer, 1, data_length, jsonf);
		printf("bytes_read %zd\n", bytes_read);
		int count_quotes = 0;
		int bad_quotes = false;
		int count_nl = 1;
		for (int i = (start_ignore_s_t_nl+1); data_buffer[i] != '\0'; i++)
		{
			if (data_buffer[i] == '\n') count_nl++;
			if (data_buffer[i] == '"')
			{
				count_quotes++;
				continue;
			}
			if (data_buffer[i] == ':')
			{
				if ((count_quotes%2) != 0)
				{
					bad_quotes = true;
					break;
				}
				else
				{
					count_quotes = 0;
				}
			}	
			int count_s_t_nl = 0;
			if (data_buffer[i] == '[')
			{
				char char_to_test = data_buffer[data_length-end_step-end_ignore_s_t_nl-count_s_t_nl];
				while(char_to_test != ']')
				{
					if (char_to_test == ' ' || char_to_test == '\n' || char_to_test == '\t')
					{
						count_s_t_nl++;
						char_to_test = data_buffer[data_length-end_step-end_ignore_s_t_nl-count_s_t_nl];
					}
					else
					{
						break;
					}
				}
				
				if (data_buffer[data_length-end_step-end_ignore_s_t_nl-count_s_t_nl] == ']')
				{
					angle_brack_idx = data_length-end_step-end_ignore_s_t_nl-count_s_t_nl;
				}
				else
				{
					printf("There can't be any other characters between ']' and '}'\n");
				}
			}

			int n_of_chars = 0;
			int count_coma = 0;
			int count_colon = 0;

			if (data_buffer[i] == '{')
			{
				f64 x0 = 0;
				f64 y0 = 0;
				f64 x1 = 0;
				f64 y1 = 0;
				
				char tempx0[25] = {'\0'};
				char tempy0[25] = {'\0'};
				char tempx1[25] = {'\0'};
				char tempy1[25] = {'\0'};

				if (first_bracket)
				{
					first_bracket = false;
				}
				int closed_idx = NULL;
				int coma_idx = NULL;

				int k = 1;
				int n_of_vals = 0;

				while(data_buffer[i+k] != '{')
				{				
					n_of_chars++;
					if ((i+k) >= angle_brack_idx) break;
					if (data_buffer[i+k] == ':')
					{
						count_colon++;
						for (int j = 1; data_buffer[i+k+j+1] != '}'; j++)
						{
							if (data_buffer[i+k+j] == ',')
							{
								count_coma++;
								continue;
							}
							else
							{
								if ((count_colon == 1))
								{
									tempx0[j-1] = data_buffer[i+k+j];
								}
								else if (count_colon == 2)
								{
									tempy0[j-1] = data_buffer[i+k+j];
								}
								else if (count_colon == 3)
								{
									tempx1[j-1] = data_buffer[i+k+j];
								}
								else
								{
									tempy1[j-1] = data_buffer[i+k+j];
								}
							}
						}
					}
					if (data_buffer[i+k] == '}')
					{
						closed_idx = i+k;
						count_elem++;
					}
					if (data_buffer[i+k] == ',')
					{
						coma_idx = i+k;
					}
					k++;
				}
				char *eptr;
				x0 = strtod(tempx0, &eptr);
				y0 = strtod(tempy0, &eptr);
				x1 = strtod(tempx1, &eptr);
				y1 = strtod(tempy1, &eptr);
				printf("x0 %.16f\n", x0);
				printf("y0 %.16f\n", y0);
				printf("x1 %.16f\n", x1);
				printf("y1 %.16f\n", y1);
				f64 earth_radius = 6372.8;
				f64 haversine = ReferenceHaversine(x0, y0, x1, y1, earth_radius);
				sum += haversine;
				printf("haversine %.16f\n", haversine);

				if (!closed_idx)
				{
					printf("It seems that '}' missing at line %d\n", count_nl+1);
					return false;
				}

				if ((coma_idx < closed_idx) && (data_buffer[i+n_of_chars] != ']'))
				{
					printf("It seems that ',' missing at line %d\n", count_nl+1);
					return false;
				}
			}
			i += n_of_chars;
		}		
		printf("\n");
		if (bad_quotes)
		{
			printf("Bad file structure, missing \"\n");
		}
		printf("count_elem %d", count_elem);
	}
	else
	{
		printf("Incorrect json file, it seems that '}' missing\n");
	}
	printf("AVR %.16f\n", sum/count_elem);

	fclose(jsonf);
	return 0;
}