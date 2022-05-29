#ifndef __SHELL_H
#define	__SHELL_H
#include "stdint.h"
#include "fs.h"
void my_shell(void);
void print_prompt(void);
static void readline(char * buf, int32_t count);
static int32_t cmd_parse(char * cmd_str, char ** argv, char token);
extern char final_path[MAX_PATH_LEN];
static void cmd_execute(uint32_t argc, char ** argv);
#endif
