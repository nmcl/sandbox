/* Program to configure and receive connections from the ethernet.
 * The connection is auth'd and then the server program is started.
 * CRS -  4-Jan-90  9:58:17 
 * Modified SMR - 11-1-1995 11:19:33 */

#include <System/string.h>
#include <System/stdio.h>
#include <System/signal.h>
#include <System/sys/types.h>
#include <System/netinet/in.h>
#include <System/sys/socket.h>
#include <System/netdb.h>
#include <System/sys/time.h>
#include <System/errno.h>
#include <System/unistd.h>
#include <System/stdlib.h>
#include <System/unistd.h>
#include <System/sys/wait.h>
#include <System/iostream.h>

#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern FILE *outstr;
extern Boolean verbose;
int tcpl, fromlen;
struct sockaddr_in from;
char* logFile = (char*) 0;

char *Username;

#ifdef HAVE_INTSIG
static int sig_child (int dummy DOTDOTDOT)
#else     
static void sig_child (int dummy DOTDOTDOT)
#endif     
{
    pid_t childpid;
#ifdef HAVE_UNION_WAIT
    union wait i;
#else
    int i;
#endif

    reportf( "Signal child received and processed.\n" );
    childpid = wait( &i );
    reportf( "Child %s%i has finished.\n", "", (int) childpid );
}

Boolean Stop;

#ifdef HAVE_INTSIG
static int sig_stop (int dummy DOTDOTDOT)
#else     
static void sig_stop (int dummy DOTDOTDOT)
#endif
{
    reportf( "Signal stop received.\n" );
    Stop = TRUE;
}

Boolean isPaused;

#ifdef HAVE_INTSIG
static int sig_pause (int dummy DOTDOTDOT)
#else
static void sig_pause (int dummy DOTDOTDOT)
#endif     
{
    isPaused = !isPaused;
  
    if (isPaused == TRUE)
	reportf("Driver paused\n");
    else
	reportf("Driver restarted\n");

    signal(SIGUSR2, sig_pause);    
}

void SetSignal ()
{
#ifdef VERY_VERBOSE    
    reportf( "Setting the child catch signal\n" );
#endif    
    if( signal( SIGCLD, sig_child) == SIG_ERR )
	perror( "Failed to init signal" );
}

int GetConnection()
{
	int s;

	SetSignal();
	
#ifdef VERY_VERBOSE
	reportf( "Into GetConnection.\n" );
#endif	

	fromlen = sizeof( struct sockaddr_in );
	memset( &from, 0, fromlen );
	from.sin_family = AF_INET;

	do {
	  s =  accept(tcpl, (struct sockaddr*) &from, &fromlen);

	  if( s == -1 && errno != EINTR )
	  {
	    reportf( "Error number is %s%i and handle is %i.\n", "", errno, s );
	    perror ("Fail in accept");
	    close (tcpl);
	    exit (3); 
	  }

	  hostent* hp = gethostbyaddr((char*) &from.sin_addr, sizeof(from.sin_addr), AF_INET);

	  if (hp)
	      reportf("Accepting connection from machine %s\n", hp->h_name);
	  else
	      reportf("Accepting connection from unknown machine.\n");

	  SetSignal();
	  
	} while (s == -1);

#ifdef VERY_VERBOSE	
	reportf( "Out of GetConnection.\n" );
#endif	
	
	return s; 
}

void StartServer( char* Username, int fd )
{
  char fdText1[ 5 ], fdText2[ 5 ];

  memset(fdText1, '\0', 5);
  memset(fdText2, '\0', 5);

  close( tcpl );
  
#ifdef VERY_VERBOSE  
  reportf ("Exec-ing server.\n");
#endif
  
  sprintf( fdText1, "%i", fd);
  sprintf( fdText2, "%i", fd );

  const char* isVerbose = "0";
  
  if (verbose)
      isVerbose = "1";
  
  execl(SERVER, SERVER, Username, fdText1, fdText2, isVerbose, NULL );
  
  perror( "Exec failed" );
  reportf ( "Exec failed for %s!\n", SERVER );
  close( fd );
}

void usage (char* name)
{
    cout << "Usage: " << name << " [-port <tcp port>] [-v] [-log <log file>] [-help]" << endl;
}

int main (int argc, char** argv)
{
  int s, tcpport;
  pid_t pid;
  char UserID[9], VersionLine[ MAXBUFF ], Reply[ MAXBUFF ];
  
  memset( UserID, 0, 9 );
  memset( Reply, 0, MAXBUFF );
  Username = UserID;
  Stop = FALSE;
  isPaused = FALSE;
		
  if ( signal( SIGUSR1, sig_stop) == SIG_ERR )
  {
    perror( "Failed to init stop signal" );
  }

  signal(SIGUSR2, sig_pause);

  tcpport = REGTCPPORT;
  
  /*
   * Some defaults.
   */
  
  outstr = stdout;
  verbose = FALSE;
  
  for (int i = 1; i < argc; i++)
  {
      if (strcmp(argv[i], "-port") == 0)
      {
	  tcpport = atoi (argv[i+1]);
	  i = i+1;
      }
      else
      {
	  if (strcmp(argv[i], "-help") == 0)
	  {
	      usage(argv[0]);
	      return 0;
	  }
	  else
	  {
	      if (strcmp(argv[i], "-log") == 0)
	      {
		  logFile = argv[i+1];
		  outstr = fopen (logFile, "w+");
		  if (outstr == NULL)
		  {
		      fprintf (stderr, "Unable to open logfile\n");
		      return -1;
		  }

		  i = i+1;
	      }
	      else
	      {
		  if (strcmp(argv[i], "-v") == 0)
		      verbose = TRUE;
		  else
		  {
		      cout << "Unknown option " << argv[i] << endl;
		      usage(argv[0]);
		      return -2;
		  }
	      }
	  }
      }
  }

  ConfigNet( tcpport );

  reportf( "\nListening on port %s%d ...\n", "", tcpport);

  for (;!Stop;)
  {
      /*
       * GetConnection returns the socket to use when talking to
       * the front-end. tcpl is the listening socket.
       */
      
      s = GetConnection();

      reportf ("Call accepted ...\n");
      reportf ("Starting new process with parameter %s%d.\n", "", s);
		
      if ((pid = fork ()) == 0)
      {
#ifdef VERY_VERBOSE	  
	  reportf ("Fork successful - child spawned.\n");
#endif	  
	  
	  int res = -1;

	  /*
	   * We must do the entire protocol stack before we can report that the
	   * driver is paused!
	   */
	  
#ifndef DEBUG
	  res = seclogin( s, UserID );
#else
	  (void) seclogin( s, UserID );
#endif
	  switch (res)
	  {
	  case -1:
	      reportf( "Auth'd OK.\n");
		  
	      memset( Reply, 0, MAXBUFF );
	      memset( VersionLine, 0, MAXBUFF );
	      Receive( s, VersionLine );

	      res = CheckVersion(VersionLine, Reply);

	      /*
	       * This is where we check to see if we are paused. If we are
	       * send back a message to the front-end to say so, and then
	       * issue a login failure, and quit the connection.
	       */
	      
	      if (isPaused)
	      {
		  /*
		   * Send message to front-end indicating that the driver is
		   * currently paused.
		   */

		  ::memset(Reply, '\0', MAXBUFF);
		  CreateActionFault(Reply, DRIVERSOFFLINE, TRUE);
		  Send(s, Reply);
		  ::memset(Reply, '\0', MAXBUFF);
		  CreateActionFault(Reply, LOGINFAIL, TRUE);
		  Send(s, Reply);		  
	      }
	      else
	      {
		  switch (res)
		  {
		  case -1:
		      Send( s, Reply );
		      StartServer( Username, s );
		      break;
		  case 0:
		      Send( s, Reply );
		      memset( Reply, 0, MAXBUFF );
		      CreateActionFault( Reply, VERSIONFAIL, TRUE );
		      Send( s, Reply );
		      break;
		  default:
		      reportf( "Connection dies.\n" );
		      break;
		  }
	      }
	      
	      break;
	  case 1:
	      printf( "Auth failed - too old.\n" );
	      break;
	  case 2:
	      printf("Auth failed - ident not known\n" );
	      break;
	  case 3:
	      printf("Auth failed - cancel\n");
	      break;
	  case 4:
	      printf("Auth failed - Password incorrect\n");
	      break;
	  default:
	      break;
	  }
	      
	  close( s );
	  close( tcpl );
	  exit( 0 );
      }
      
      close (s);  /* parent also closes socket */
  }
}

