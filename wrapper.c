/**************************************************************************\
*                                                                          *
*            Project: Gild		                                   *
*            wrapper.c                                                     *
*                                                                          *
*            Purpose: Given a open socket connected to a client, handle    *
*		the conversation with the client by detecting the protocol *
*		in use and invoking the appropriate handler		   *
*                                                                          *
*            Author   : Simon Brooke                                       *
*            Copyright: (c) Simon Brooke 1997                              *
*            Version  : 0.1                                                *
*            Created  : 7th October 1997				   *
*                                                                          *
\**************************************************************************/

/* $Header */

#include "gild.h"

extern char errorBuff[ 1024];
extern char **environ;

void die( void)			
/* inherited from cgi-lib; a way of getting rid of errant programs */
{
     sprintf( errorBuff, "potentially serious: handler over-ran alloted time");
     error( LOG_ERR);
     exit( 1);			/* belt and braces; should never be called */
}


void wrapper( int conversation, char * client_id)
/* conversation is the handle on an open socket communicating with a
   client. */
{
     char firstln[ 1024];
     char * exec_args[1];	/* 'arguments' to pass to execve */
     handler * command = null;

     recv( conversation, firstln, 80, MSG_PEEK);
				/* get the first thing the client
				 says, but leave it on the input
				 stream for the handler. */

     if ( dup2( conversation, STDIN_FILENO) == -1)
     {
	  sprintf( errorBuff, 
		  "failed to duplicate conversation [%d] onto stdin: %s",
		  conversation, strerror( errno));
	  error( LOG_ERR);
     }

     if ( dup2( conversation, STDOUT_FILENO) == -1)
     {
	  sprintf( errorBuff, 
		  "failed to duplicate conversation [%d] onto stdout: %s",
		  conversation, strerror( errno));
	  error( LOG_ERR);
     }

     command = get_handler( firstln);
				/* and find the appropriate handler */
     if ( command == null)	/* didn't find one */
     {
	  sprintf( errorBuff, "no handler registered for %s", firstln);
	  error( LOG_ERR);
     }
     else			/* did find one */
     {
	  setenv( "REMOTE_HOST", client_id, 1);
				/* set up the handler environment */

	  sprintf( errorBuff, 
		  "using handler '%s' [%d] to handle %s request from %s", 
		  command->command, ( int)getpid(), command->protocol, 
		  client_id);
	  error( LOG_NOTICE);
				/* log the request, and... */

	  exec_args[ 0] = command->command;
	  exec_args[ 1] = null;
				/* set up the dummy arguments */

	  if ( execve( command->command, exec_args, environ) == -1)
				/* ...execute the command (shouldn't return) */
	  {			/* if it did we've got an error */
	       sprintf( errorBuff, 
		       "error [errno %d] whislt execing handler '%s' [%d]\n",
		       errno, command->command, ( int)getpid()),
	       error( LOG_ERR);
	  }		    
     }

     exit( 0);
}




