// TCPXMIT.C
// Joel Santiago

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>		/* for gettimeofday() */
#include <stdlib.h>
#include <strings.h>

#define TRUE 1
#define FILESIZE 500000 // Hardcoded file size - 500KB * 1000 = 500,000 Bytes

/* This is the transmitter program. It creates a socket and initiates a
connection with the socket given in the command line. The send buffer
is set as requested.  One message is sent over the connection and then the
socket is closed.

The form of the command line is: `tcpxmit rcvr-hostname rcvr-port buffer_length'. */

int main(int argc,char *argv[])
{
  int sd; // socket descriptor variable
  struct sockaddr_in rcvr; // struct variable for socket values
  struct hostent *hp, *gethostbyname(); // host name struct variables
  int buflen = atoi(argv[3]); // buffer length entered by user through CLI
  char buf[buflen]; // assigns buffer length as length of buf array
  int sndwin; // packet send window variable
  struct timeval sndtime1, sndtime2; // struct variables used to get before and after time values
  struct timezone zone; // struct variables used to get current time zone
  int elapsed_sec, elapsed_usec; // second and microsecond elapsed values
  int rval, seqno, i; // recv value assigned to rval, sequence number counter and i counter
  int packet_count = FILESIZE / buflen; // calculates the total amount of packets need based on filesize / buffer length

  sndwin = buflen;

  /* create socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd < 0) { // displays error if socket creation returns an error
    perror("opening stream socket");
    exit(1);
  }

  /* Create rcvr socket address using command line arguments */
  rcvr.sin_family = AF_INET;
  hp = gethostbyname(argv[1]); // function to get the host name from the CLI arguments
  if(hp == 0) { // displays error message if hostname returns false
    printf("%s: unknown host\n",argv[1]);
    exit(2);
  }

  bcopy(hp->h_addr, &rcvr.sin_addr, hp->h_length); // bcopy copies host address to the sin_addr variable
  rcvr.sin_port = htons(atoi(argv[2])); // assigns user entered port number from CLI to sin_port value

  /* connect socket using name specified at command line */
  if(connect(sd, (struct sockaddr *)&rcvr, sizeof(rcvr)) < 0) { // connect function connects to socket returns error message if failed
    perror("connecting stream socket");
    exit(1);
  }

  /* get current relative time */
  if (gettimeofday(&sndtime1, &zone) < 0) { // gets transfer start time and returns error if function returns < 0
    perror("Getting time");
  }

  printf("Start time: %ld %ld\n", sndtime1.tv_sec, sndtime1.tv_usec); // displays start time in seconds and microseconds

  seqno = 1;

  for (i = 0; i < packet_count; ++i)
  {
    /* create a packet */
    sprintf(buf, "%d\0", seqno);

    /* send the packet */
    if((rval=send(sd, buf, sndwin,0)) < 0) { // sends packet to client and returns error if function call fails
       perror("writing on stream socket");
    } else {
       printf("Sent packet %d of length %d\n", seqno, rval);
       usleep(400000); // sleeps loop for 1/25 of a second
    }

    seqno++; // sequence number incrementer
  }

  if (gettimeofday(&sndtime2, &zone) < 0) { // gets transfer start time and returns error if function returns < 0
    perror("Getting time");
  }

  printf("End time: %ld %ld\n", sndtime2.tv_sec, sndtime2.tv_usec);

  elapsed_sec = (sndtime2.tv_sec - sndtime1.tv_sec); // finds out elapsed seconds for transfer
  elapsed_usec = (sndtime2.tv_usec - sndtime1.tv_usec) / 1000000.0; // finds out elapsed microseconds for transfer

  if (elapsed_usec < 0) // accounts for negative values from microsecond calculation
    elapsed_usec *= -1;

  printf("Elapsed Time: %d sec\n", elapsed_sec + elapsed_usec); // displays total elapsed time
}
