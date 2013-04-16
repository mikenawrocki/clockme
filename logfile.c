#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAXDATELEN 12
#define LOGDIR "/.clockme/"
#define LOGEX ".log"
#define LOGDIRMOD 0755

FILE *logfile;

static char *iso8601_date(time_t convtime)
{
	char *datestr = malloc(MAXDATELEN);
	struct tm *loctime = localtime(&convtime);
	if(loctime)
		strftime(datestr, MAXDATELEN, "%F", loctime);
	else {
		perror("Time");
		exit(-2);
	}

	return datestr;
}

void open_log()
{
	char *isodate;
	char logname[PATH_MAX];
	time_t curtime = time(NULL);
	isodate = iso8601_date(curtime);

	strncpy(logname, getenv("HOME"), PATH_MAX);
	strcat(logname, LOGDIR);
	if(mkdir(logname, LOGDIRMOD) < 0) {
		if(errno != EEXIST) {
			perror("logging directory");
			exit(-errno);
		}
	}
	strncat(logname, isodate, MAXDATELEN);
	strcat(logname, LOGEX);

	logfile = fopen(logname, "w+");

	if(!logfile) {
		perror("Logging");
		exit(-3);
	}

}
