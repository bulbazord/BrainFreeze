#include <stdio.h>
#include <stdlib.h>
#include "brainfreeze.h"

struct machine_state state;

/*
 * Main interpreter loop
 */
void execute_program(void)
{
}

/*
 * A simple function to load the entire program into the program's state.
 */
void load_program(FILE *program)
{
	char instr;
	while (fscanf(program, "%c", &instr) != EOF) {
		switch(instr) {
			case '>':
			case '<':
			case '+':
			case '-':
			case '.':
			case ',':
			case '[':
			case ']':
				state.code[state.code_size++] = instr;
				break;
			default:
				break;
		}
	}
}

/*
 * Print the program for debugging purposes.
 */
void print_program(void)
{
	for (int i = 0; i < state.code_size; i++) {
		printf("%c", state.code[i]);
	}
	printf("\n");
}

/*
 * Simple function to print the program's usage.
 */
void usage()
{
	printf("Usage: brainfreeze <prog>\n");
	printf("<prog> is a program written in Brainfuck\n");
}

/*
 * Main function of the interpreter
 *
 * Parsing command line arguments:
 * Here I opted to just roll my own system.
 * I might switch to getopts if I do anything more complicated
 * than just taking in a program. Probably will, honestly.
 */
int main(int argc, char** argv)
{
	FILE *program;
	char *filename;

	if (argc < 2) {
		printf("Error: No filename given\n");
		usage();
		exit(1);
	} else if (3 < argc) {
		printf("Error: Only expecting program filename\n");
		usage();
		exit(1);
	}
	filename = argv[1];

	program = fopen(filename, "r");
	if (!program) {
		fprintf(stderr, "Error: %s doesn't exist or can't be read\n",
				filename);
		exit(1);
	}

	state.code = malloc(sizeof(char) * DEFAULT_CODE_SIZE);
	state.code_size = 0;

	load_program(program);
	print_program();

	fclose(program);
	free(state.code);
	return 0;
}

