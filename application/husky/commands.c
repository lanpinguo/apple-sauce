/******************************************************************************

  Copyright (C), 2001-2011, Pure Co., Ltd.

 ******************************************************************************
  File Name     : commands.c
  Version       : Initial Draft
  Author        : lanpinguo
  Created       : 2018/5/17
  Last Modified :
  Description   : command for local user
  Function List :
              addargs
              clear_arglist
              init_arglist
  History       :
  1.Date        : 2018/5/17
    Author      : lanpinguo
    Modification: Created file

******************************************************************************/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/
#define _GNU_SOURCE
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>

#ifndef DISABLE_NOTIFICATIONS
#include <pthread.h>
#endif


#include "commands.h"
#include "readinput.h"



/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/
void print_version();



/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/
volatile int verb_level = 0;
volatile int time_commands = 0;

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/
	COMMAND commands[] = {
		{"help", 	cmd_help, "Display this text"},
	/* synonyms for previous commands */
		{"debug", cmd_debug, NULL},
		{"?", 		cmd_help, NULL},
		{"exit", 	cmd_quit, NULL},
		{NULL, NULL, NULL}
	};
	
	
	
	struct arglist {
		char** list;
		int count;
		int size;
	};

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/
/*----------------------------------------------*
 * macros 																			*
 *----------------------------------------------*/
#define BUFFER_SIZE 1024


/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/


/**
 * \brief Initiate arglist to defined values
 *
 * \param args          pointer to the arglist structure
 * \return              0 if success, non-zero otherwise
 */
void init_arglist(struct arglist* args) {
	if (args != NULL) {
		args->list = NULL;
		args->count = 0;
		args->size = 0;
	}
}

/**
 * \brief Clear arglist including free up allocated memory
 *
 * \param args          pointer to the arglist structure
 * \return              none
 */
void clear_arglist(struct arglist* args) {
	int i = 0;

	if (args && args->list) {
		for (i = 0; i < args->count; i++) {
			if (args->list[i]) {
				free(args->list[i]);
			}
		}
		free(args->list);
	}

	init_arglist(args);
}

/**
 * \brief add arguments to arglist
 *
 * Adds erguments to arglist's structure. Arglist's list variable
 * is used to building execv() arguments.
 *
 * \param args          arglist to store arguments
 * \param format        arguments to add to the arglist
 */
void addargs(struct arglist* args, char* format, ...) {
	va_list arguments;
	char* aux = NULL, *aux1 = NULL;
	int len;

	if (args == NULL) {
		return;
	}

	/* store arguments to aux string */
	va_start(arguments, format);
	if ((len = vasprintf(&aux, format, arguments)) == -1)
	perror("addargs - vasprintf");
	va_end(arguments);

	/* parse aux string and store it to the arglist */
	/* find \n and \t characters and replace them by space */
	while ((aux1 = strpbrk(aux, "\n\t")) != NULL) {
		*aux1 = ' ';
	}
	/* remember the begining of the aux string to free it after operations */
	aux1 = aux;

	/*
	 * get word by word from given string and store words separately into
	 * the arglist
	 */
	for (aux = strtok(aux, " "); aux != NULL; aux = strtok(NULL, " ")) {
		if (!strcmp(aux, ""))
		continue;

		if (args->list == NULL) { /* initial memory allocation */
			if ((args->list = (char**)malloc(8 * sizeof(char*))) == NULL) {
				perror("Fatal error while allocating memory");
			}
			args->size = 8;
			args->count = 0;
		} else if (args->count + 2 >= args->size) {
			/*
			 * list is too short to add next to word so we have to
			 * extend it
			 */
			args->size += 8;
			args->list = realloc(args->list, args->size * sizeof(char *));
		}
		/* add word in the end of the list */
		if ((args->list[args->count] = (char*)malloc((strlen(aux) + 1) * sizeof(char))) == NULL)
		perror("Fatal error while allocating memory");
		strcpy(args->list[args->count], aux);
		args->list[++args->count] = NULL; /* last argument */
	}
	/* clean up */
	free(aux1);
}



int cmd_quit(const char* UNUSED(arg), const char* UNUSED(old_input_file), FILE* output, FILE* input) {
	return EXIT_SUCCESS;
}

int cmd_verbose(const char *UNUSED(arg), const char* UNUSED(old_input_file), FILE* output, FILE* UNUSED(input)) {
	if (verb_level != 1) {
		verb_level = 1;
		fprintf(output, "Verbose level set to VERBOSE\n");
	} else {
		verb_level = 0;
		fprintf(output, "Verbose messages switched off\n");
	}

	return EXIT_SUCCESS;
}

int cmd_debug(const char *UNUSED(arg), const char* UNUSED(old_input_file), FILE* output, FILE* UNUSED(input)) {
	if (verb_level != 2) {
		verb_level = 2;
		fprintf(output, "Verbose level set to DEBUG\n");
	} else {
		verb_level = 0;
		fprintf(output, "Verbose messages switched off\n");
	}

	return EXIT_SUCCESS;
}

int cmd_help(const char* arg, const char* UNUSED(old_input_file), FILE* output, FILE* input) {
	int i;
	char *args = strdupa(arg);
	char *cmd = NULL;
	char cmdline[BUFFER_SIZE];

	strtok(args, " ");
	if ((cmd = strtok(NULL, " ")) == NULL) {
		/* generic help for the application */
		print_version();

generic_help:
		INSTRUCTION(output, "Available commands:\n");
		for (i = 0; commands[i].name; i++) {
			if (commands[i].helpstring != NULL) {
				fprintf(output, "  %-15s %s\n", commands[i].name, commands[i].helpstring);
			}
		}

		INSTRUCTION(output, "To delete a command history entry, use CTRL+X.\n\n");
	} else {
		/* print specific help for the selected command */

		/* get the command of the specified name */
		for (i = 0; commands[i].name; i++) {
			if (strcmp(cmd, commands[i].name) == 0) {
				break;
			}
		}

		/* execute the command's help if any valid command specified */
		if (commands[i].name) {
			snprintf(cmdline, BUFFER_SIZE, "%s --help", commands[i].name);
			commands[i].func(cmdline, NULL, output, input);
		} else {
			/* if unknown command specified, print the list of commands */
			fprintf(output, "Unknown command \'%s\'\n", cmd);
			goto generic_help;
		}
	}

	return EXIT_SUCCESS;
}






