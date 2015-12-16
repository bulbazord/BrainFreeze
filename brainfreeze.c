#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brainfreeze.h"

#define OOB_ERR "Out of bounds exception: Cannot exceed 30000 cell limit"
#define OOB_ERR2 "Out of bounds exception: Cannot go below the 0 cell limit."

struct machine_state state;

/*
 * Main interpreter loop
 */
void execute_program(void)
{
	char instr = state.code[state.instr_ptr];
	while (state.instr_ptr < state.code_size) {
		switch (instr) {
			case '>':
				state.data_ptr++;
				if (state.data_ptr >= MAX_CELLS) {
					printf(OOB_ERR);
					return;
				}
				break;

			case '<':
				state.data_ptr--;
				if (state.data_ptr < 0) {
					printf(OOB_ERR2);
					return;
				}
				break;

			case '+':
				state.cells[state.data_ptr]++;
				break;

			case '-':
				state.cells[state.data_ptr]--;
				break;

			case '.':
				putchar(state.cells[state.data_ptr]);
				break;

			case ',':
				state.cells[state.data_ptr] = getchar();
				break;

			case '[':
				if (state.cells[state.data_ptr] == 0) {
					jump_fwd();
				}
				break;

			case ']':
				if (state.cells[state.data_ptr] != 0) {
					jump_bkwd();
				}
				break;

			default:
				break;
		}
		instr = state.code[++state.instr_ptr];
	}
	printf("\n");
}

void jump_fwd(void)
{
	struct list *curr = state.jump_list;
	while (curr->start != state.instr_ptr) {
		curr = curr->next;
	}
	if (curr->start != state.instr_ptr) {
		printf("jump_fwd(): ");
		printf("You should not be here, mortal.\n");
	} else {
		state.instr_ptr = curr->stop;
	}
}

void jump_bkwd(void)
{
	struct list *curr = state.jump_list;
	while (curr->stop != state.instr_ptr) {
		curr = curr->next;
	}
	if (curr->stop != state.instr_ptr) {
		printf("jump_bkwd(): ");
		printf("You should not be here, mortal.\n");
	} else {
		state.instr_ptr = curr->start;
	}
}

/*
 * A pass over the code to ensure square brackets are balanced.
 * Creates the jump-list for the code as well.
 * If code is invalid, return 0. If code is valid, return a non-zero value.
 */
int validate_program(void)
{
	struct list *head = NULL;
	for (unsigned int i = 0; i < state.code_size; i++) {
		if (state.code[i] == '[') {
			struct list *temp = malloc(sizeof(struct list));
			temp->start= i;
			temp->next = head;
			head = temp;
		} else if (state.code[i] == ']') {
			if (head == NULL) {
				return 0;
			} else {
				struct list *temp = head;
				head = head->next;

				temp->stop = i;
				temp->next = state.jump_list;
				state.jump_list = temp;
			}
		}
	}

	int ret = (head == NULL);
	if (ret == 0) {
		while (head != NULL) {
			struct list *temp = head;
			head = head->next;
			free(temp);
		}
		cleanup_jump_list();
	}

	return ret;
}

void cleanup_jump_list(void)
{
	while (state.jump_list != NULL) {
		struct list *temp = state.jump_list;
		state.jump_list = state.jump_list->next;
		free(temp);
	}
}

/*
 * A simple function to load the entire program into the program's state.
 */
void load_program(FILE *program)
{
	char instr;
	while (fscanf(program, "%c", &instr) != EOF) {
		// Check to see if need to resize code buffer
		if (state.code_size == state.max_code_size) {
			state.max_code_size *= 2;
			unsigned char *temp = malloc(
					sizeof(char *) * state.max_code_size);
			memcpy(temp, state.code, state.code_size);
			free(state.code);
			state.code = temp;
		}

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
	for (unsigned int i = 0; i < state.code_size; i++) {
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
	state.max_code_size = DEFAULT_CODE_SIZE;
	state.instr_ptr = 0;
	state.data_ptr = 0;
	state.jump_list = NULL;

	load_program(program);

	int valid = validate_program();
	if (valid == 0) {
		printf("Unbalanced brackets. Invalid Brainfuck program.\n");
	} else {
		execute_program();
	}

	cleanup_jump_list();
	fclose(program);
	free(state.code);
	return 0;
}

