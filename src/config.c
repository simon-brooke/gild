/**************************************************************************\
*                                                                          *
*            Project: Gild		                                   *
*            config.c                                                      *
*                                                                          *
*            Purpose: Handle configuration file				   *
*                                                                          *
*            Author   : Simon Brooke                                       *
*            Copyright: (c) Simon Brooke 1997                              *
*            Version  : 0.1                                                *
*            Created  : 8th October 1997				   *
*                                                                          *
\**************************************************************************/

/* $Header$ */

#include "gild.h"

extern char errorBuff[];	/* where I assemble logging messages */
handler * handlers = ( handler *) null;
				/* the list of handlers I handle */

int parse_config( char * path)
/* parse the config file and identify the handlers I handle */
{
     FILE * configFile;		/* the file handle from which to load config */
     char * line[ 1024];	/* a buffer to read lines into */
     int n = 0;			/* number of handlers we find */

     sprintf( errorBuff, "Loading configuration from %s", path);
     error( LOG_NOTICE);

     configFile = fopen( path, "r");

     if ( configFile == ( FILE *)null)
     {
	  sprintf( errorBuff, "failed to open configuration file %s: %s",
		  path, strerror( errno));
	  error( LOG_ERR);
     }

     while( ! feof( configFile))
     {
	  char buff[ 1024], protocol[ 1024], pattern[ 1024], command[ 1024];
	  int timeout;		/* how long to wait for the handler to
	  			   complete */
	  
	  fgets( buff, 1024, configFile);
				/* get a line from the config file */
	  if ( buff[ 0] == '#');
				/* it's a comment, and can be ignored */
	  else if ( fscanf( configFile, "%s %s %s %d", protocol,
		      pattern, command, &timeout) == 4)
				/* otherwise look for four fields. If
                                   we find them... */
	  {
	       handler * newhandler = 
		    ( struct handler *) malloc( sizeof( struct handler));
				/* create a handler */
	       struct re_pattern_buffer * patternBuff = 
		    ( struct re_pattern_buffer *) 
			 malloc( sizeof( struct re_pattern_buffer));
				/* and reserve space for a compiled regex */

	       if ( newhandler == ( handler *) null)
	       {		/* unlikely, but... best check */
		    sprintf( errorBuff, 
			    "out of memory whilst allocating handler?");
		    error( LOG_ERR);
	       }

	       regcomp( patternBuff, pattern, 
		       REG_ICASE | REG_NEWLINE);
				/* and load it with the values we found */
	       newhandler->protocol = strdup( protocol);
	       newhandler->pattern = patternBuff;
	       newhandler->command = strdup( command);
	       newhandler->timeout = timeout;

				/* then splice it into the handler chain */
	       newhandler->next = handlers;
	       handlers = newhandler;

	       n ++;		/* increment the counter */
	       
				/* and log it. */
	       sprintf( errorBuff, 
		       "registering handler [%s] for protocol %s", 
		       newhandler->command, newhandler->protocol);
	       error( LOG_NOTICE);
	  }
     }
     return ( n);		/* say how many we found */
}

handler * get_handler( char * match)
/* find a handler whose pattern matches match, and return it's command */
{
     handler * h = null, * i;
     regmatch_t pmatch[ 2];

     for ( i = handlers; ( h == null) && ( i != null); i = i->next)
				/* scan down the list of handlers looking
				   for a match... */
     {
	  if ( regexec( i->pattern, match, 2, pmatch, 0) == 0)
	  {
	       h = i;
	  }
     }

     return( h);
}
