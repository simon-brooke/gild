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

#include "gild.h"

extern char errorBuff[];	/* where I assemble logging messages */
handler * handlers = ( handler *) null;
				/* the list of handlers I handle */

void parse_config( char * path)
/* parse the config file and identify the handlers I handle */
{
     FILE * configFile;		/* the file handle from which to load config */
     char * line[ 1024];	/* a buffer to read lines into */
     

     sprintf( errorBuff, "Loading configuration from %s", path);
     error( NOTICE);

     configFile = fopen( path, "r");

     if ( configFile == ( FILE *)null)
     {
	  sprintf( errorBuff, "failed to open configuration file %s: %s",
		  path, strerror( errno));
	  error( FATAL_ERROR);
     }

     while( ! feof( configFile))
     {
	  char buff[ 1024], protocol[ 1024], pattern[ 1024], command[ 1024];
	  int port;
	  
	  fgets( buff, 1024, configFile);
				/* get a line from the config file */
	  if ( buff[ 0] == '#');
				/* it's a comment, and can be ignored */
	  else if ( fscanf( configFile, "%s %d %s %s", protocol, &port,
		      pattern, command) == 4)
				/* otherwise look for four fields. If
                                   we find them... */
	  {
	       handler * newhandler = 
		    ( struct handler *) malloc( sizeof( struct handler));
				/* create a handler */

	       if ( newhandler == ( handler *) null)
	       {		/* unlikely, but... best check */
		    sprintf( errorBuff, 
			    "out of memory whilst allocating handler?");
		    error( FATAL_ERROR);
	       }

				/* and load it with the values we found */
	       newhandler->port = port;
	       newhandler->protocol = strdup( protocol);
	       newhandler->pattern = regcomp( pattern);
	       newhandler->command = strdup( command);
				/* then splice it into the handler chain */
	       newhandler->next = handlers;
	       handlers = newhandler;

				/* and log it. */
	       sprintf( errorBuff, 
		       "registering handler [%s] for protocol %s", 
		       newhandler->command, newhandler->protocol);
	       error( NOTICE);

				/* ultimately we will deal with
                                   listening on multiple ports, but
                                   not yet! */
	       if ( newhandler->next != null)
	       {
		    if ( newhandler->next->port != port)
		    {
			 sprintf( errorBuff,
				 "port for %s [%d] differs from %s [%d] - this version of GILD only handles one port!", 
				 protocol, port, newhandler->next->protocol,
				 newhandler->next->port);
			 error( FATAL_ERROR);
		    }
	       }		 
	  }
     }
}

char * get_handler_command( char * match)
/* find a handler whose pattern matches match, and return it's command */
{
     handler * h;
     char * command = null;

     for ( h = handlers; ( command == null) && ( h != null); h = h->next)
				/* scan down the list of handlers looking
				   for a match... */
     {
	  if ( 0) /* ( regexec( h->pattern, match)) */
	  {
	       command = h->command;
	  }
     }

     return( command);
}
