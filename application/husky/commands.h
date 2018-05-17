
#include <stdlib.h>

#ifndef COMMANDS_H_
#define COMMANDS_H_

char some_msg[4096];
#define INSTRUCTION(output,format,args...) {snprintf(some_msg,4095,format,##args);fprintf(output,"\n  %s",some_msg);}
#define ERROR(function,format,args...) {snprintf(some_msg,4095,format,##args);fprintf(stderr,"%s: %s\n",function,some_msg);}

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

typedef struct
{
	char *name; /* User printable name of the function. */
	int (*func)(const char*, const char*, FILE*, FILE*); /* Function to call to do the command. */
	char *helpstring; /* Documentation for this function.  */
} COMMAND;


int cmd_debug(const char *UNUSED(arg), const char* UNUSED(old_input_file), FILE* output, FILE* UNUSED(input));
int cmd_help(const char* arg, const char* UNUSED(old_input_file), FILE* output, FILE* input);
int cmd_quit(const char* UNUSED(arg), const char* UNUSED(old_input_file), FILE* output, FILE* input);
int cmd_verbose(const char *UNUSED(arg), const char* UNUSED(old_input_file), FILE* output, FILE* UNUSED(input));



#endif /* COMMANDS_H_ */
