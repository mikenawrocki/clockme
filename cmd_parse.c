#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cmd_parse.h"
#include "cmd_handlers.h"

struct cmd_map commands[] = {
	{ "ls",   list },
	{ "l",    list },
	{ "quit", quit },
	{ "q",    quit },
	{ "a",    add },
	{ "add",  add },
	{ "p",  pause },
	{ "pause",  pause },
	{ "r",  resume },
	{ "resume",  resume },
};

static void make_args(int *argc, char ***argv, char *args)
{
	*argc = 0;
	argv[*argc] = malloc(sizeof(char *) * 5); //max 5 arguments
	char *str_arg = strtok(args, " ");
	while(str_arg && *argc < 5) {
		(*argv)[(*argc)++] = str_arg;
		str_arg = strtok(NULL, " ");
	}
}

void cmd_parse(char *cmd)
{
	int i,argc;
	char **argv;
	if(!cmd) {
		printf("quit\n");
		quit(0, NULL);
	}
	make_args(&argc,&argv, cmd); 
	for(i=0; i < LEN(commands); i++) {
		if(!strcmp(commands[i].cmd_str, argv[0])) {
			commands[i].func(argc,argv);
		}
	}
	free(argv);
}
