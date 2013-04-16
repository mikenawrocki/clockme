#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <error.h>
#include <errno.h>

#include "cmd_handlers.h"
#include "list.h"

void pause(int argc, char **argv)
{
	struct node *tmp = charge_num_list_head;
	if(!tmp) {
		fprintf(stderr, "Error! No charge numbers entered.");
		return;
	}
	do {
		if(tmp->data.status == status_active) {
			tmp->data.status = status_paused;
			tmp->data.seconds_elapsed = (long)difftime(time(NULL),
				tmp->data.start_time);
			printf("Pausing charge number %s\n", tmp->data.num);
			return;
		}
	} while(tmp->next && (tmp = tmp->next));

	printf("No Active Charge Number!\n");
}

static void activate_num(char *num)
{
	struct node *tmp = charge_num_list_head;
	struct charge_num *data;
	char found = 0;
	if(!tmp || !num) {
		fprintf(stderr, "Error! No charge numbers entered.");
		return;
	}

	do {
		data = &(tmp->data);
		if(!strcmp(data->num, num)) {
			data->status = status_active;
			data->start_time = time(NULL);
			printf("Resuming charge number: %s\n",data->num);
			found = 1;
		}
		else if(data->status == status_active) {
			data->status = status_inactive;
			data->seconds_elapsed += (long)difftime(time(NULL),
					data->start_time);
		}
		else {
			data->status = status_inactive;
		}
	} while(tmp->next && (tmp = tmp->next));
	if(!found)
		fprintf(stderr, "ERROR! Invalid charge number.\n");
}

static void activate_paused()
{
	struct node *tmp = charge_num_list_head;
	struct charge_num *data;

	if(!tmp) {
		fprintf(stderr, "Error! No charge numbers entered.");
		return;
	}

	do {
		data = &(tmp->data);
		if(data->status == status_paused) {
			data->status = status_active;
			data->start_time = time(NULL);
			printf("Resuming charge number: %s\n",data->num);
			return;
		}
	 } while(tmp->next && (tmp = tmp->next));
	fprintf(stderr, "ERROR! No paused charge number.\n");
}

void resume(int argc, char **argv)
{
	if(argc < 2) {
		activate_paused();
	}
	else {
		activate_num(argv[1]);
	}
}

void add(int argc, char **argv)
{
	struct node *new_node = malloc(sizeof(struct node));

	strncpy(new_node->data.num, argv[1], 50);
	new_node->data.num[49] = '\0';
	new_node->data.seconds_elapsed = 0;
	new_node->data.start_time = time(NULL);
	new_node->data.status = status_active;
	new_node->next = NULL;

	struct node *tmp = charge_num_list_head;
	struct charge_num *data;
	if(tmp) {
		do {
			data = &(tmp->data);
			if(data->status == status_active) {
				data->status = status_inactive;
				data->seconds_elapsed += (long)difftime(time(NULL),
						data->start_time);
			}
		 } while(tmp->next && (tmp = tmp->next));
		tmp->next = new_node;
	} else {
		charge_num_list_head = new_node;
	}
}

void list(int argc, char **argv)
{
	double elapsed;
	struct node *tmp = charge_num_list_head;
	while(tmp) {
		elapsed = (double)tmp->data.seconds_elapsed;
		if(tmp->data.status == status_active)
			elapsed += difftime(time(NULL), tmp->data.start_time);
		elapsed /= 60*60;
		printf("Charge %s : %03.3f hrs\n", 
				tmp->data.num, elapsed);
		tmp = tmp->next;
	}
}

static void write_log(void)
{
	double elapsed;
	struct node *tmp = charge_num_list_head;
	while(tmp) {
		elapsed = (double)tmp->data.seconds_elapsed;
		if(tmp->data.status == status_active)
			elapsed += difftime(time(NULL), tmp->data.start_time);
		elapsed /= 60*60;
		fprintf(logfile, "Charge %s : %03.3f hrs\n", 
				tmp->data.num, elapsed);
		tmp = tmp->next;
	}
}

void quit(int argc, char **argv)
{
	list(0, NULL);
	write_log();
	fclose(logfile);
	exit(EXIT_SUCCESS);
}
