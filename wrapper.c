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

#include "gild.h"

char errorBuff[ 1024];

void die( void)			/* inherited from cgi-lib; a way of
				   getting rid of errant programs */
{
     sprintf( errorBuff, "potentially serious: handler over-ran alloted time");
     error( LOG_ERR);
}


void wrapper( int conversation, char * client_id)
/* conversation is the handle on an open socket communicating with a
   client. What I want to do is quite simple, and there must be a
   straightforward way of doing it: attach the conversation to both
   the standard input and the standard output of a process, and then
   exec the handler within that process... I can't (at present) find
   an easy way of doing that, however */
{
     char firstln[ 1024];
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
	  sprintf( errorBuff, 
		  "using handler '%s' to handle %s request from %s", 
		  command->command, command->protocol, client_id);
	  error( LOG_NOTICE);
				/* log the request */

	  if ( command->timeout != 0)
				/* prevent runaway processes; if a
				 timeout has been specified for this
				 handler, enforce it */
	  {
	       signal( SIGALRM,(void (*)())die);
	       alarm( command->timeout);
	  } 

	  setenv( "REMOTE_HOST", client_id, 1);
				/* set up the handler environment */

	  system( command->command);
     }

     exit( 0);
}





