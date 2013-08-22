#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "cmd_handlers.h"

#define LOGDIR "/.clockme/"
#define NUMNAME "numbers"
#define LOGDIRMOD 0750

static FILE *numfile;

void open_numfile()
{
	char numfile_name[PATH_MAX];

	strncpy(numfile_name, getenv("HOME"), PATH_MAX);
	strcat(numfile_name, LOGDIR);
	if(mkdir(numfile_name, LOGDIRMOD) < 0) {
		if(errno != EEXIST) {
			perror("logging directory");
			exit(-errno);
		}
	}
	strcat(numfile_name, NUMNAME);

	numfile = fopen(numfile_name, "a+");

	if(!numfile) {
		perror("numfile");
		exit(-4);
	}
}

void read_numbers()
{
	char chargenum[72];

	open_numfile();

	while(fgets(chargenum, 72, numfile)) {
		chargenum[strnlen(chargenum,72)-1] = '\0';

		add_number(chargenum);
	}

	fclose(numfile);
}

void write_number(char *num)
{
	open_numfile();

	fseek(numfile, 0, SEEK_END);
	fputs(num, numfile);
	fputs("\n", numfile);

	fclose(numfile);
}
