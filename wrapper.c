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

extern char * errorBuff;

void wrapper( int conversation)
/* conversation is the handle on an open socket communicating with a
   client. What I want to do is quite simple, and there must be a
   straightforward way of doing it: attach the conversation to both
   the standard input and the standard output of a process, and then
   exec the handler within that process... I can't (at present) find
   an easy way of doing that, however */
{
     char firstln[ 1024];
     char * command;
     int i, hear, say = conversation;
     FILE * c_rx, * c_tx;

     printf( "wrapper started with fdes [%d]\n",
		  conversation); 

     hear = dup( conversation);	/* creat two handles on conversation */
     say = dup( conversation);	/* one for reading and one for writing */

#ifdef globba
     for( i = 0; ( i < 1024 ) && ( ( i == 0) || ( firstln[ i - 1] > ' ')); i++)
     {
	  read( hear, ( char *)firstln + i, 1);
				/* read a control-terminated line from the
                                   conversation, one byte at a time
                                   (prolly a better way of doing this) */
	  puts( firstln);
     }
     firstln[ i] = '\0';	/* terminate the string */
#endif

     recv( hear, firstln, 80, MSG_PEEK);

     printf( "looking for command to match [%s]\n", firstln);

     command = get_handler_command( firstln);
				/* and find the appropriate handler */

     printf( "got command [%s]\n", command);

     if ( ! command)	/* didn't find one */
     {
	  sprintf( errorBuff, "no handler registered for %s", firstln);
	  error( FATAL_ERROR);
     }
     else			/* did find one */
     {
	  sprintf( errorBuff, "using handler %s for protocol %s", 
		  command, firstln);
	  error( FATAL_ERROR);
     }

     recv( hear, firstln, 1024, 0);
     exit( 0);

     if ( dup2( hear, 0) == -1)
     {
	  sprintf( errorBuff, 
		  "failed to duplicate conversation [%d] onto stdin: %s",
		  hear, strerror( errno));
	  error( FATAL_ERROR);
     }
     dup2( say, 1);
     {
	  sprintf( errorBuff, 
		  "failed to duplicate conversation [%d] onto stdout: %s",
		  say, strerror( errno));
	  error( FATAL_ERROR);
     }

/*     gets( firstln); */
     puts( "hello!\n");
/*     puts( firstln); */

     puts("\tClosing...");
     exit( 0);
}





