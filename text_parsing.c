#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_parsing.h"
#include "main.h"

/*int main (int argc, char *argv) {
	Parse_Input *input = malloc(sizeof(Parse_Input));
	input->file_line = malloc(sizeof(char) * LINE_BUFF_SIZE);
	
	strcpy(input->file_line, "\tHello how is it going?\n");

	ParseText(input);

	return 1;
}*/

Parse_Output * ParseText(Parse_Input * input) {
	Parse_Output *output = malloc(sizeof(Parse_Output));
	output->file_line_array = malloc(sizeof(char*) * LINE_BUFF_SIZE);
	for (int i = 0; i < LINE_BUFF_SIZE; i++) {
		output->file_line_array[i] = malloc(sizeof(char) * LINE_BUFF_SIZE);
	}

	int output_word = 0;
	int output_word_size = 0;
	char file_char;

	if (input->file_line[0] == '\40') {
		return NULL;
	}

	for (int j = 0; j < LINE_BUFF_SIZE; j++) {
		file_char = input->file_line[j];
		if (file_char == '\40' || file_char == '\n') {
			while (input->file_line[j + 1] == '\40') {
				j++;
			}
			if (input->file_line[j + 1] == '\72') {
				output->file_line_array[output_word][output_word_size] = input->file_line[j + 1];
				output_word++;
				output_word_size = 0;
				j++;
				while (input->file_line[j + 1] == '\40') {
					j++;
				}
			} else {
				output_word++;
				output_word_size = 0;
			}
		} else if (file_char == '\72') {
			output->file_line_array[output_word][output_word_size] = file_char;
			while (input->file_line[j + 1] == '\40') {
				j++;
			}
			output_word++;
			output_word_size = 0;
		} else {
			output->file_line_array[output_word][output_word_size] = file_char;
			output_word_size++;
		}
	}

	if (output->file_line_array[0][0] == '\t') {
		for (int i = 0; i < LINE_BUFF_SIZE - 1; i++) {
			output->file_line_array[0][i] = output->file_line_array[0][i + 1];
		}
		output->line_type = 'c';
	} else if (output->file_line_array[0][0] == 0 || output->file_line_array[0][0] == '\0') {
		output->line_type = 'e';
	} else if (output->file_line_array[0][0] != '\40') {
		for (int k = 0; k < LINE_BUFF_SIZE; k++) {
			if (output->file_line_array[0][k] == 0) {
				if (output->file_line_array[0][k - 1] == '\72') {
					output->line_type = 't';
					output->file_line_array[0][k - 1] = 0;
					break;
				} else {
					return NULL;
				}
			}
		}
	} else {
		return NULL;
	}

	return output;
}
