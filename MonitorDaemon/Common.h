#ifndef COMMON_H_
#define COMMON_H_


#ifndef TRUE
#  define TRUE 1
#endif

#ifndef FALSE
#  define FALSE 0
#endif

#define Boolean int

#define OBJECT_NAME_SIZE 126  /* Maximum size of debugging file names */
#define PROBE_INTERVAL    60  /* Time between probes, in minutes */
#define MONITOR_SOCKET  4000  /* Socket for the monitor daemon to listen on */
#define PING_SOCKET     5000  /* Socket for ping daemon to listen on */
#define PING_WAIT       1000  /* Wait time for ping in milliseconds */
#define PING_RETRY         5  /* Number of retries for ping */
#define TERMINATE_CODE     0  /* To kill ping and monitor daemons */
#define TRANSMIT_DATA      1  /* Get monitor daemon to send back collected data */
#define OK                 2

/*
 * After this size of output file we back off on
 * the timings.
 */

#define FILESIZE 1024000

/*
 * Locations of debugging files and configuration
 * data file.
 */

#define defaultFile "/var/tmp/monitor.out"
#define errorFile "/var/tmp/monitor.err"
#define configurationFile "/var/tmp/monitor.conf"

/*
 * Some signatures for routines not provided in certain
 * header files.
 */

#if defined(sun) || defined(__ultrix)
extern "C"
{
    int gethostname (char *, int);
    int gettimeofday(struct timeval*, struct timezone*);    
}
#endif

/*
 * Signature of the ping routine.
 */

extern Boolean ping (const char*, long&, int = PING_SOCKET);

#endif
