#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_parsing.h"
#include "main.h"

int main (int argc, char *argv) {
	Parse_Input *input = malloc(sizeof(Parse_Input));
	input->file_line = malloc(sizeof(char) * LINE_BUFF_SIZE);
	
	strcpy(input->file_line, "Hello: how is it going?\n");

	ParseText(input);

	return 1;
}

Parse_Output * ParseText(Parse_Input * input) {
	Parse_Output *output = malloc(sizeof(Parse_Output));
	output->file_line_array = malloc(sizeof(char*) * LINE_BUFF_SIZE);
	for (int i = 0; i < LINE_BUFF_SIZE; i++) {
		output->file_line_array[i] = malloc(sizeof(char) * LINE_BUFF_SIZE);
	}

	int output_word = 0;
	int output_word_size = 0;
	char file_char;

	for (int j = 0; j < LINE_BUFF_SIZE; j++) {
		file_char = input->file_line[j];
		if (file_char == '\40' || file_char == '\n') {
			output_word++;
			output_word_size = 0;
		} else {
			output->file_line_array[output_word][output_word_size] = file_char;
			output_word_size++;
		}
	}

	char last_char;

	if (output->file_line_array[0][0] == '\t') {
		output->line_type = 't';
	} else {
		for (int k = 0; k < LINE_BUFF_SIZE; k++) {
			if (output->file_line_array[0][k] == 0) {
				if (output->file_line_array[0][k - 1] == '\72') {
					output->line_type = 'c';
				} else {
					output->line_type = 'e';
				}
				break;
			}
		}
	}

	printf("%c\n", output->line_type);

	return output;
}
