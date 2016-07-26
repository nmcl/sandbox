/* ******************************************* */
/* University of Newcastle Upon Tyne           */
/* Code written by Christian Leroux             */
/* Date of creation 13/07/94                   */
/* Last modification 29/07/97 by C. L.         */
/* Copyrighted by the University of Newcastle  */

/* File name : test1.cc  */

#include <System/sys/param.h>
#include <System/sys/types.h>
#include <System/sys/socket.h>
#include <System/sys/file.h>
#include <System/fcntl.h>
#include <System/sys/stat.h>
#include <System/netinet/in.h>
#include <System/arpa/inet.h>
#include <System/netdb.h>
#include <System/stdio.h>
#include <System/string.h>
#include <System/stdlib.h>
#include <System/unistd.h>
#include <System/sys/time.h>
#include <System/time.h>
#include <System/ndbm.h>
#include <System/math.h>
#include <System/unistd.h>
#include <System/protos.h>
#include <System/iostream.h>

#include <Common/Boolean.h>

#include <arjuna.h>
#include <local.h>
#include <Defs.h>

/*
 * The file to use for getting student numbers.
 */

#define DBM_FILE staticKeyMap

extern void Login( char* Input, char* Output );
extern void VersionMessage( char* Output );

extern "C"
{
    double drand48 ();
    void srand48 (long);
}

#define RECORD_SKIP 2000
#define SIZEOFBUF  MAXBUFF
        /* length of message that can be received from the A. client*/
#define NUM_TRY       20
        /* Number of re-try transaction in case of failure when save*/
#define NUM_REQUEST  5
        /* Number of requests the front-end asks to the Arjuna client*/
#define STAT_FILE "result/stat.num"
        /* file in which are stored the execution time of all the clients */
#define FAIL_FILE "result/fail.txt"
        /* File which holds failure information */
#define MIN_WAITTIME 30
        /* Time waited is uniformly distributed between */ 
#define MAX_WAITTIME  (60)
        /* MIN_WAITTIME and MAX_WAITTIME (secondes) */
#define MODE (S_IRUSR||S_IRGRP||S_IROTH)
        /* Mode for reading dbm database */
        /* Actually, read by user  (see chmod(2)) */
#define PERC_SAVE 50
        /* Percentage to save the student record rather than closing it */


static char login[]="login";      /* login & logout message for the driver */
static char logout[]="logout";    /* Real version of the driver should not need it */

static char OpenRequest[]="OR";        /* Header of messages */
static char OpenSuccess[]="OS";
static char SaveRequest[]="SR";
static char SaveSuccess[]="SS";
static char CloseRequest[]="CR";
static char CloseSuccess[]="CS";

static char buf[SIZEOFBUF];
static char path[SIZEOFBUF];
static int sock;

static int failure_o=0,           /* Number of open requests failed */
           failure_c=0,           /* Number of close requests failed */
           failure_s=0,           /* Number of save requests failed */
          time_slept=0;           /* Time spent sleeping */

int iter = 0;
const char* Username = DEBUG_USER;
static Boolean verbose = FALSE;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Functions concerned with the recording on information to files     */
/* on the local system.                                               */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* copy filename to path */
void make_name( char* file_name )
{
  strcpy( path, file_name ); 
}  


/* Record in file STAT_FILE the time needed for this client */
void record_time( struct timeval first )
{
  FILE *stat;
  struct timeval second, lapsed;
  int cpt;

  /* What time is it ? and compute the elapsed time */
  gettimeofday( &second, NULL );
  if ( first.tv_sec > second.tv_usec ) {
    second.tv_usec += 1000000;
    second.tv_sec--;
  }
  lapsed.tv_usec = second.tv_usec - first.tv_usec ;
  lapsed.tv_sec  = second.tv_sec  - first.tv_sec - time_slept ;

  /* record time data into STAT_FILE */
  make_name( STAT_FILE );
  for(cpt=0 ;
      (( ( stat = fopen ( path, "a" )) == NULL ) && (cpt <= NUM_TRY) ) ;
      cpt++, sleep(1) );
  if ( stat == NULL ) {
    fprintf( stderr, "Unable to open %s file.\n", STAT_FILE );
    stat = stderr;
  }
  fprintf( stat, "%u %u\n", lapsed.tv_sec, lapsed.tv_usec );
  if ( stat != stderr ) fclose( stat );
}

/* Record in the file FAIL_FILE the number of messages failures that happened */
void record_failure( )
{
  int cpt;
  FILE *fail; 

  make_name( FAIL_FILE );   /* record number of failure into FAIL_FILE */

  for( cpt=0 ;
      (( ( fail = fopen ( path, "a" )) == NULL ) && (cpt <= NUM_TRY) ) ;
      cpt++, sleep(1) );
  if ( fail == NULL ) {
    fprintf( stderr, "Unable to open %s file.\n", STAT_FILE );
    fail = stderr;
  }

  fprintf( fail, "%d %d %d\n", failure_o, failure_c, failure_s ); 
  if (fail != stderr) fclose( fail );
}

/* ******************************************************************* */
/* Extract randomly a valid student number from the database           */
/* Skip a random number of record                                      */
void give_std_num ( DBM *db, datum &k, char *s )
{
  int skip= (int) (( RECORD_SKIP ) * drand48() );
/*  fprintf( stdout, "Choosing another student record with skip=%d\n", skip );*/
  if (k.dptr == NULL) 
    k = dbm_firstkey( db );  

  for( ; skip != 0 ; skip-- ) {
    k = dbm_nextkey( db );
    if (k.dptr == NULL) 
     k = dbm_firstkey( db );
  }

  strncpy( s, k.dptr, k.dsize );
  s[k.dsize] = '\0';
}

/* ********************************************************************** */
/* This function opens the socket to be able to talk to the driver        */
/* ********************************************************************** */
int open_socket( char* RemoteHostName, int RemotePortNumber ) 
{
  struct sockaddr_in from, to;
  struct hostent *hp1, *hp3;
  char name_host[MAXHOSTNAMELEN+1];
  int sock;

  if ( gethostname(name_host, MAXHOSTNAMELEN+1) != 0 ) {
    perror("Can not find name of current host");
    exit(4);
  }
  fprintf(stdout, "The current host is %s.\n", name_host);
  fflush(stdout);

  hp3 = gethostbyname(name_host);
  if ( hp3 == NULL ) {
    fprintf(stderr, "%s: Unknown host", name_host);
    exit(1);
  }

  memset( (char*)&from, 0, sizeof(from) );
  memcpy( &from.sin_addr, hp3->h_addr, hp3->h_length );
  from.sin_family = AF_INET;
  from.sin_port = htons( INADDR_ANY );
  fprintf ( stdout, "Local address is: %s\n", inet_ntoa (from.sin_addr) );
  fflush(stdout);
  
  /* create the socket: Internet+Stream */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0 ) {
    perror( "client:Opening stream socket failure" );
    exit( 2 );
  }
  
  /* bind socket with local address */
  if ( bind( sock, (struct sockaddr*)&from, sizeof(from) ) <0 ) {
    perror( "binding socket to local address");
    exit(3);
  }
  
  /* Connect socket to <machine> */
  hp1 = gethostbyname(RemoteHostName);
  if (hp1 == NULL ) {
    fprintf(stderr,"client:%s: unknown host!",RemoteHostName);
    exit(1);
  }
  
  /* Connect to remote host */
  memset( (char*)&to, 0, sizeof(to) );
  memcpy( (char*)&to.sin_addr, hp1->h_addr, hp1->h_length);
  to.sin_family = hp1->h_addrtype;
  to.sin_port = htons( RemotePortNumber );

  if ( connect( sock, (struct sockaddr*)&to, sizeof(to)) <0 ) {
    perror ( "client:Connecting socket to driver error" );
    exit( 3 );
  }
  return sock;
}



/* ******************************************************************* */
/* This function makes a full request to the Arjuna client, i.e. open  */
/* a student record, waits a while and saves/closes it.                */

void ProcessRequest( char* StudentNumber, int Handle )
{
	char OutgoingMessage[SIZEOFBUF];
	char IncomingMessage[SIZEOFBUF];
	int hasard;
	struct timeval stime, ftime;
	Boolean save = FALSE;

	if (verbose)
	    cout << "\nOpen request for student:  " << StudentNumber << endl;

	memset( OutgoingMessage, 0, SIZEOFBUF );
	memset( IncomingMessage, 0, SIZEOFBUF );

	/* create the OR message */
	memcpy( &OutgoingMessage[16], "OR", 2);
	memcpy( &OutgoingMessage[18], "12345", 5);
	memcpy( &OutgoingMessage[23], StudentNumber, 9);

	if (verbose)
		gettimeofday(&stime, NULL);

	/* Send the message to the server */
	Send( Handle, OutgoingMessage);

	/* get the reply */
	Receive( Handle, IncomingMessage);

        if (verbose)
        {
            gettimeofday(&ftime, NULL);

             cout << "Total time taken: " << (((ftime.tv_sec*1000000)+ftime.tv_usec) -
                                              ((stime.tv_sec*1000000)+stime.tv_usec)) / 1000
                                          << " milliseconds. Status: ";
        }

	/* is it an open success ? */
	if (strncmp( &IncomingMessage[16], "OS", 2) == 0 )
	{
	        if (verbose)
                    cout << "OS" << endl;

        	/* wait for a random length of time */
	        hasard = (int) (MIN_WAITTIME + (MAX_WAITTIME-MIN_WAITTIME) * drand48());
		time_slept += hasard;

		cout << iter+1 << ": Time waited " << hasard << endl;
		
    		sleep( hasard );

		/* clear the output message buffer */
		memset( OutgoingMessage, 0, SIZEOFBUF);

		/* should the record be saved or closed ? */
    		if ( (drand48() * 100) > PERC_SAVE)
		{
		    if (verbose)
			cout << "Close request for student: " << StudentNumber << endl;

		    memcpy( &OutgoingMessage[16], "CR", 2);
    		}
 		else
		{
		    if (verbose)
			cout << "Save request for student:  " << StudentNumber << endl;
		    
		    memcpy( &OutgoingMessage[16], "SR", 2);
		    save = TRUE;
      		}
		memcpy( &OutgoingMessage[18], "12345", 5);
		memcpy( &OutgoingMessage[23], StudentNumber, 9);
		memcpy(&OutgoingMessage[32], &IncomingMessage[32], 1015);

                if (verbose)
                    gettimeofday(&stime, NULL);

		/* send the reply */
		Send( Handle, OutgoingMessage );

		/* clear the incoming buffer */
		memset( IncomingMessage, 0, SIZEOFBUF);

		/* get the reply */
		Receive( Handle, IncomingMessage);

		if (verbose)
		{
			gettimeofday(&ftime, NULL);

			cout << "Total time taken: " << (((ftime.tv_sec*1000000)+ftime.tv_usec) -
							((stime.tv_sec*1000000)+stime.tv_usec)) / 1000
				<< " milliseconds. Status: ";
		}

		if (strncmp( &IncomingMessage[16], "SF", 2)==0)
		{
		    failure_s += 1;

		    if (verbose)
			    cout << "SF" << endl;

		    fprintf( stdout, "Failure: Save record request impossible for student %s.\n", StudentNumber);
		    fflush(stdout);
		}
		else
		{
		    if (strncmp( &IncomingMessage[16], "CF", 2) == 0)
		    {
			failure_c +=1;

			if (verbose)
				cout << "CF" << endl;

			fprintf( stdout, "Failure: Close record request impossible for student %s.\n", StudentNumber);
		        fflush(stdout);
		    }
		    else
		    {
			if (verbose)
			{
			    if (save)
				    cout << "SS" << endl;
			    else
				    cout << "CS" << endl;

			    cout << "Request successful." << endl;
			}
		    }
		}
  	}
	else 
        {
                if (verbose)
                    cout << "OF" << endl;

		fprintf(stderr, "Did not get OS message for student %s.\n", StudentNumber);
		failure_o +=1;
		fflush(stderr);
  	}
}


void usage (char* name)
{
    cout << "Usage: " << name << " [-host <host name>] [-port <port number>] [-user <user id>] [-number <number of requests>] [-v] [-help]" << endl;
}

/* *** main program *** */

int main (int argc, char** argv)
{
  char ReplyLine[SIZEOFBUF]; 
  char InputLine[SIZEOFBUF];
  char* HostName = 0;
  int PortNumber = REGTCPPORT;
  char CurrentStudentNumber[STUDENTNUMBERSIZE +1];
  int numberOfRequests = NUM_REQUEST;
  struct timeval first;
  DBM* db = NULL;
  datum k;

  for (int i = 1; i < argc; i++)
  {
      if (::strcmp(argv[i], "-help") == 0)
      {
	  usage(argv[0]);
	  return 0;
      }
      else
      {
	  if (::strcmp(argv[i], "-port") == 0)
	  {
	      PortNumber = ::atoi(argv[i+1]);
	      i = i+1;
	  }
	  else
	  {
	      if (::strcmp(argv[i], "-user") == 0)
	      {
		  Username = argv[i+1];
		  i = i+1;
	      }
	      else
	      {
		  if (::strcmp(argv[i], "-host") == 0)
		  {
		      HostName = ::new char[::strlen(argv[i+1])+1];
		      ::strcpy(HostName, argv[i+1]);
		      i = i+1;
		  }
		  else
		  {
		      if (::strcmp(argv[i], "-number") == 0)
		      {
			  numberOfRequests = ::atoi(argv[i+1]);
			  i = i+1;
		      }
		      else
		      {
			  if (::strcmp(argv[i], "-v") == 0)
			  {
			      verbose = TRUE;
			  }
			  else
			  {
			      cerr << "Unknown option " << argv[i] << endl;
			      usage(argv[0]);
			  
			      return -1;
			  }
		      }
		  }
	      }
	  }
      }
  }

  if (!HostName)
  {
      HostName = ::new char[MAXHOSTNAMELEN+1];
      ::memset(HostName, '\0', MAXHOSTNAMELEN+1);
      ::gethostname(HostName, MAXHOSTNAMELEN);
  }

  if ((db = dbm_open(DBM_FILE, O_RDONLY, MODE)) == NULL)
  {
      cerr << "Error - unable to open student database " << DBM_FILE << endl;
      ::delete [] HostName;
      
      return -3;
  }
  else
      k = dbm_firstkey(db);
  
  memset( ReplyLine, 0, SIZEOFBUF );
  memset( InputLine, 0, SIZEOFBUF );

  /* descriptor needed for the socket implementation */
  
  sock = open_socket( HostName, PortNumber );

  GetLine(sock, InputLine, SIZEOFBUF );
  if( strncmp( &InputLine[ 16 ], "LN", 2 ) == 0 )
  {
    Login( InputLine, ReplyLine );
    write(sock, ReplyLine, strlen(ReplyLine));
  }
  else
  {
    fprintf(stderr, "Did not receive LN request\n");
  }

  memset( ReplyLine, 0, SIZEOFBUF );
  memset( InputLine, 0, SIZEOFBUF );
  GetLine(sock, InputLine, SIZEOFBUF );

  if ( strncmp( &InputLine[ 16 ], "LS", 2 ) == 0 )
  {
      VersionMessage( ReplyLine );
      Send(sock, ReplyLine);
  }
  else
      fprintf(stderr, "Expecting LS, but received %c%c\n",InputLine[16],InputLine[17]);

  memset( ReplyLine, 0, SIZEOFBUF );
  memset( InputLine, 0, SIZEOFBUF );
  Receive(sock, InputLine);
  if ( strncmp( &InputLine[ 16 ], "VS", 2 ) != 0 )
  {
    fprintf(stderr, "Expecting VS, but received %c%c\n",InputLine[16],InputLine[17]);
  }

  gettimeofday( &first, NULL );

  /* Installation of the random generator */
  srand48( getpid() );

  /* *************************** */
  /* ***   SIMULATION PART   *** */
  /* *************************** */

  /* Send message to the driver to initiate an Arjuna Client*/
  /* NB: The first message initiates the creation of the client... */
  /*  communication(sock, login, buf );*/

  for( iter = 0 ; iter < numberOfRequests; iter++ )
  {
      give_std_num( db, k, CurrentStudentNumber );
      ProcessRequest(CurrentStudentNumber, sock);
  }

  /* send end message to finish the Arjuna client */
  /*  communication( sock, logout, buf );*/
 
  /* Record the running time */
  record_time( first );
  record_failure();

  /* Close the database file */
  dbm_close( db );

  /* close socket */
  close(sock);

  ::delete [] HostName;
  
  return 0;
}
