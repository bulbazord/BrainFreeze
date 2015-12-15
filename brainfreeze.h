#ifndef _BRAINFREEZE_H
#define _BRAINFREEZE_H

#define MAX_CELLS 30000
#define DEFAULT_CODE_SIZE 200

struct machine_state {
	char cells[MAX_CELLS];
	char *code;
	unsigned int code_size;
};

void load_program(FILE *program);
void print_program(void);

#endif
