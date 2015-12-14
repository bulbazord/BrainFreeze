#include <stdio.h>
#include <stdlib.h>
#include "brainfreeze.h"

/*
 * A simple function to print out the entire program.
 */
void print_program(FILE *program)
{
	char instruction;
	while (fscanf(program, "%c", &instruction) != EOF) {
		switch(instruction) {
			case '>':
			case '<':
			case '+':
			case '-':
			case '.':
			case ',':
			case '[':
			case ']':
				printf("%c", instruction);
				break;
			default:
				break;
		}
	}
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

	printf("BrainFreeze v0.1\n");
	printf("Program loaded: %s\n\n", filename);
	print_program(program);
	printf("\n");

	fclose(program);
	return 0;
}

