#ifndef CMD_HANDLERS_H
#define CMD_HANDLERS_H

extern struct node *charge_num_list_head;
extern FILE *logfile;

struct cmd_map {
	const char *cmd_str;
	void (*func)(int argc, char **argv);
};

void pause(int argc, char **argv);
void resume(int argc, char **argv);
void add(int argc, char **argv);
void list(int argc, char **argv);
void quit(int argc, char **argv);

#endif
