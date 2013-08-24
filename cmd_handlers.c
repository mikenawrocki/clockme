#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <error.h>
#include <errno.h>

#include "cmd_handlers.h"
#include "list.h"
#include "numfile.h"

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
			tmp->data.seconds_elapsed += (long)difftime(time(NULL),
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
	int order = atoi(num);
	if(!tmp || !num) {
		fprintf(stderr, "Error! No charge numbers entered.");
		return;
	}

	do {
		data = &(tmp->data);
		if(!order--) {
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
	if(argc < 2 || argc > 3) {
		printf("Invalid Arguments for charge number add.\n");
		return;
	}
	add_number(argv[1]);
	write_number(argv[1]);
}

void add_number(char *num)
{
	struct node *new_node = malloc(sizeof(struct node));

	strncpy(new_node->data.num, num, 72);
	new_node->data.num[49] = '\0';
	new_node->data.seconds_elapsed = 0;
	new_node->data.start_time = 0;
	new_node->data.status = status_inactive;
	new_node->next = charge_num_list_head;

	charge_num_list_head = new_node;
}

static void print_charge_status(struct charge_num *num)
{
	switch(num->status) {
	case status_active:
		printf("[a]");
		break;
	case status_inactive:
		printf("[ ]");
		break;
	case status_paused:
		printf("[p]");
		break;
	}
}

static void write_times_formatted(FILE *out, int logging)
{
	double elapsed, total_elapsed = 0.0;
	struct node *tmp = charge_num_list_head;
	int cur = 0;

	while(tmp) {
		elapsed = (double)tmp->data.seconds_elapsed;
		if(tmp->data.status == status_active)
			elapsed += difftime(time(NULL), tmp->data.start_time);
		elapsed /= 60*60;
		total_elapsed += elapsed;

		if(!logging) {
			print_charge_status(&(tmp->data));
			fprintf(out, "(%d) %03.3f hrs : Charge %s\n", 
					cur++, elapsed, tmp->data.num);
		}
		else {
			fprintf(out, "Charge %s : %03.3f hrs\n", 
					tmp->data.num, elapsed);
		}

		tmp = tmp->next;
	}

	fprintf(out, "Total: %03.3f hrs\n", total_elapsed);
}

void list(int argc, char **argv)
{
	write_times_formatted(stdout, 0);
}

static void write_log(void)
{
	write_times_formatted(logfile, 1);
}

void quit(int argc, char **argv)
{
	list(0, NULL);
	write_log();
	fclose(logfile);
	exit(EXIT_SUCCESS);
}
