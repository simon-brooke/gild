/**************************************************************************\
*                                                                          *
*            Project: Gild		                                   *
*            gild.h                                                        *
*                                                                          *
*            Purpose: public header file for gild server.		   *
*                                                                          *
*            Author   : Simon Brooke                                       *
*            Copyright: (c) Simon Brooke 1997                              *
*            Version  : 0.1                                                *
*            Created  : 6th October 1997				   *
*                                                                          *
\**************************************************************************/

/* $Header$ */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <syslog.h>

#include <linux/in.h>

#define GILD_NAME 	"gild"
#define GILD_VERSION 	"$Revision$"
#define GILD_ID 	"gild $Revision$"

#define CONFIG_PATH "/usr/local/etc/gild/gild.conf"
#define DEFAULT_PORT_NO 8421
#define MAX_PENDING_REQUESTS 5

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define DEBUG 1

#define ever (;;)
#define null (((void *) 0))


typedef struct handler
{
     struct handler * next;	/* next one down the chain */
     char * protocol;		/* a name for the protocol handled  */
     regex_t * pattern;		/* the pattern to match to select this
				   handler */
     char * command;		/* the command to invoke this handler */
} handler;

void error( int severity);
/* log the current contents of errorBuff and then if severity is bad die */

char * get_handler_command( char * match);
/* find a handler whose pattern matches match, and return it's command */

int log( int level, char *message);
/* hand this message over to the syslog daemon for recording */

int parse_config( char * path);
/* parse the config file and identify the handlers I handle */

void wrapper( int conversation);
/* conversation is the handle on an open socket communicating with a
   client. What I want to do is quite simple, and there must be a
   straightforward way of doing it: attach the conversation to both
   the standard input and the standard output of a process, and then
   exec the handler within that process... I can't (at present) find
   an easy way of doing that, however */
