/***********************************************************************\
*                                                                       *
*            Project: Gild		                                *
*            log.c                                                      *
*                                                                       *
*            Purpose: Interface to syslogd. Why do our own logging, 	*
*		which only adds to sysadmin hassle and confusion, 	*
*		when we can use the system log?				*
*                                                                       *
*            Author   : Simon Brooke                                    *
*            Copyright: (c) Simon Brooke 1997                           *
*            Version  : 0.1                                             *
*            Created  : 15th October 1997				*
*                                                                       *
\***********************************************************************/

/* $Header$ */

#include "gild.h"

int log( int level, char *message)
/* hand this message over to the syslog daemon for recording */
{
     openlog( GILD_ID, 0, LOG_DAEMON);
     syslog( level, message);
     closelog);
}
     
