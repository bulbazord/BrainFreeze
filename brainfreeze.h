#ifndef _BRAINFREEZE_H
#define _BRAINFREEZE_H

#define MAX_CELLS 30000
#define DEFAULT_CODE_SIZE 128

struct machine_state {
	unsigned char cells[MAX_CELLS];
	unsigned char *code;
	unsigned int code_size;
	unsigned int max_code_size;
	unsigned int instr_ptr;
	unsigned int data_ptr;
};

void execute_program(void);
int validate_program(void);
void load_program(FILE *program);
void print_program(void);

#endif
