#ifndef LIST_H
#define LIST_H

enum {status_inactive, status_active, status_paused};

struct charge_num {
	char num[72];
	long seconds_elapsed;
	time_t start_time;
	int status;
};

struct node {
	struct charge_num data;
	struct node *next;
};

#endif
