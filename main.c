#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "cmd_handlers.h"
#include "cmd_parse.h"
#include "list.h"
#include "logfile.h"
#include "numfile.h"

struct node *charge_num_list_head;

int main(void)
{
	char *input;
	charge_num_list_head = NULL;
	read_numbers();
	open_log();
	atexit(clear_history);
	for(;;) {
		input = readline("(clockme) ");
		cmd_parse(input);
		add_history(input);
		free(input);
	}

	fclose(logfile);
	return 0;
}
