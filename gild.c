/***********************************************************************\
*                                                                       *
*            Project: Gild		                                *
*            gild.c                                                     *
*                                                                       *
*            Purpose: Generalised Internet Listener Daemon	   	*
*		-- a daemon which listens for requests on a specified	*
*		IP port, and despatches them to specified handlers	*
*		depending on the content of the first line.		*
*                                                                       *
*            Author   : Simon Brooke                                    *
*            Copyright: (c) Simon Brooke 1997                           *
*            Version  : 0.1                                             *
*            Created  : 5th October 1997				*
*                                                                       *
\***********************************************************************/

/* $Header$ */

#include "gild.h"

int port = DEFAULT_PORT_NO;	/* the port I shall listen on */
char errorBuff[ 1024];		/* somewhere to assemble error messages */
extern handler * handlers;
				/* the list of handlers I support */
FILE * logstream = stderr;	/* where I stick log messages */


void error( int severity)
/* log the current contents of errorBuff and then if severity is bad die */
{
     log( severity, errorBuff);

     switch ( severity)
     {
     case LOG_ERR:
	  exit( 1);
	  break;
     }
}


int main( int argc, char * argv[])
{
     char * configPath = CONFIG_PATH; /* the path to my default config file */
     int arg;			/* the argument being handled */
     int keyhole;		/* where I listen, of course */
     struct sockaddr_in * address = 
	  ( struct sockaddr_in *)malloc( sizeof( struct sockaddr_in));
				/* the address I bind the keyhole to */

     sprintf( errorBuff, "GILD starting...");
     error( LOG_NOTICE);

     for ( arg = 1; argc > arg; arg ++)
     {				/* process arguments */
	  switch ( argv[ arg][ 0])
	  {
	  case '-':
	       switch( argv[ arg][ 1])
	       {
	       case 'f':	/* specify a config file to load */
		    arg ++;
		    configPath = argv[ arg];
		    break;
	       case 'p':	/* specify a port to listen on */
		    arg ++;
		    port = atoi( argv[ arg]);
		    break;
	       default:
		    sprintf( errorBuff, 
			    "unrecognised command line switch [-%c]",
			    argv[ arg][ 1]);
		    error( LOG_ERR);
		    break;
	       }
	  default:
	       sprintf( errorBuff, "unrecognised command line token [%s]",
		       argv[ arg]);
	  }
     }

     if ( parse_config( configPath) == 0)
     {
	  sprintf( errorBuff, "failed to load any handlers");
	  error( LOG_ERR);
     }

     keyhole = socket( AF_INET, SOCK_STREAM, 0);
     if ( keyhole == -1)
     {
	  sprintf( errorBuff, "failed to intialise socket");
	  error( LOG_ERR);
     }

     memset( address, 0, sizeof( address));
				/* sanitation */

				/* now set up the address preparatory
                                   to binding it */
     address->sin_family = AF_INET;
				/* it's an internet address */
     address->sin_addr.s_addr = htonl( INADDR_ANY);
				/* and we don't mind where it comes from */
     address->sin_port = htons( port);	
				/* that's the port we listen on, remember? */

     if ( bind( keyhole, ( struct sockaddr *)address, 
	       sizeof( struct sockaddr_in)) == -1)
     {				/* attempt to bind keyhole to address */
	  sprintf( errorBuff, "failed to bind socket");
	  error( LOG_ERR);
     }

#ifndef DEBUG
				/* we become a real class one daemon */
     if ( fork() != 0) exit(0);             
				/* the parent exits */
     setsid();			/* release the controlling tty */
#endif

     if ( listen( keyhole, MAX_PENDING_REQUESTS) == -1)
     {
	  sprintf( errorBuff, "failed in listen()?");
	  error( LOG_ERR);
     }
     sprintf( errorBuff, "GILD: awaiting requests on port %d", port);
     error( LOG_NOTICE);

     for ever
     {
	  struct sockaddr_in * client = 
	       ( struct sockaddr_in *) malloc( sizeof( struct sockaddr_in));
				/* the address of the client calling in */
	  int conversation;	/* a handle on the conversation we're having */
	  int clientsize = sizeof( struct sockaddr_in);
				/* the size of the client object */

	  if ( client == 0)
	  {			/* unlikely, but might as well check... */
	       sprintf( errorBuff, "Out of memory?");
	       error( LOG_ERR);
	  }
	  memset( client, 0, sizeof( client));
				/* sanitation */

	  conversation = accept( keyhole, ( struct sockaddr *)client, 
				&clientsize);

	  if ( conversation == -1)
	  {			/* again, check we set it up OK */
	       sprintf( errorBuff, "Could not establish conversation [%d]",
		       errno);
	       error( LOG_ERR);
	  }

	  sprintf( errorBuff, "Connection received");
	  error( LOG_NOTICE);

	  switch( fork())
	  {
	  case -1:		/* Blew it: whinge and die */
	       sprintf( errorBuff, "failed to fork?");
	       error( LOG_ERR);
	       break;
	  case 0:		/* I'm the child */
	       close( keyhole);

	       wrapper( conversation);
				/* wrapper (q.v.) handles the conversation */
	       break;
	  default:		/* I'm the parent */
	       close( conversation); 
	  }
     }
}
