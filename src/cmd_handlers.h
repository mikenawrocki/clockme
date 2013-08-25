#ifndef CMD_HANDLERS_H
#define CMD_HANDLERS_H

typedef void (*cmd_func)(int, char**);

extern struct node *charge_num_list_head;
extern FILE *logfile;

struct cmd_map {
	const char *cmd_str;
	const cmd_func func;
};

void pause(int argc, char **argv);
void resume(int argc, char **argv);
void add(int argc, char **argv);
void add_number(char *num);
void list(int argc, char **argv);
void quit(int argc, char **argv);

#endif
