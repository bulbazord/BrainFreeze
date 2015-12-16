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
	int data_ptr;
	struct list *jump_list;
};

struct list {
	struct list *next;
	unsigned int start;
	unsigned int stop;
};

void execute_program(void);
void jump_fwd(void);
void jump_bkwd(void);
int validate_program(void);
void cleanup_jump_list(void);
void load_program(FILE *program);
void print_program(void);

#endif
