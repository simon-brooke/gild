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

#define _POSIX_SOURCE
/* this is a fudge. The whole thing depends on fdopen, and I'm having
   great difficulty making it work */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <regexp.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <linux/in.h>


#define CONFIG_PATH "/usr/local/etc/gild/gild.conf"
#define DEFAULT_PORT_NO 1984
#define MAX_PENDING_REQUESTS 5

#define null (((void *) 0))

#define FATAL_ERROR 1
#define NOTICE 0

#define DEBUG 1

#define ever (;;)

typedef struct handler
{
     struct handler * next;	/* next one down the chain */
     int    port;		/* the port on which I listen */
     char * protocol;		/* a name for the protocol handled  */
     regexp * pattern;		/* the pattern to match to select this
				   handler */
     char * command;		/* the command to invoke this handler */
} handler;

void error( int severity);
/* log the current contents of errorBuff and then if severity is bad die */

char * get_handler_command( char * match);
/* find a handler whose pattern matches match, and return it's command */

void parse_config( char * path);
/* parse the config file and identify the handlers I handle */

void wrapper( int conversation);
/* conversation is the handle on an open socket communicating with a
   client. What I want to do is quite simple, and there must be a
   straightforward way of doing it: attach the conversation to both
   the standard input and the standard output of a process, and then
   exec the handler within that process... I can't (at present) find
   an easy way of doing that, however */
