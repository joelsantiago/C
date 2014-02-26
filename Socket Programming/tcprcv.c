// TCPRCV.C
// Joel Santiago

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <strings.h>

#define TRUE 1
#define FILESIZE 500000 // Hardcoded file size - 500KB * 1000 = 500,000 Bytes

/* This is the receiver program. It opens a socket, sets the receive
window as requested, and then begins an infinite loop. Each time through
the loop it accepts a connection and prints out messages from it. When
the connection breaks or a termination message comes through it, the
program accepts a new connection.

The form of the command line is: 'tcprcv buffer_length'  */

int main(int argc, char *argv[])
{
  int sd, length; // socked descriptor and length variables
  struct sockaddr_in rcvr; // struct variable for socket values
  int sda;
  int buflen = atoi(argv[1]); // buffer length entered by user through CLI
  char buf[buflen]; // assigns buffer length as length of buf array
  int rcvwin; // packet send window variable
  int rval; // recvfrom function assigns value to variable
  int seqno; // sequence number counter
  struct timeval sndtime1, sndtime2; // struct variables used to get before and after time values
  struct timezone zone; // struct variables used to get current time zone
  int packet_count = FILESIZE / buflen; // calculates the total amount of packets need based on filesize / buffer length
  int i = 0;

  rcvwin = buflen;

  /* create socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd < 0) { // displays error if socket creation returns an error
    perror("opening stream socket");
    exit(1);
  }

  /* name socket using wildcards */
  rcvr.sin_family = AF_INET;
  rcvr.sin_addr.s_addr = INADDR_ANY;
  rcvr.sin_port = 0;
  if(bind(sd, (struct sockaddr *)&rcvr, sizeof(rcvr))) { // bind takes entered name and binds it to the socket
    perror("binding socket name");
    exit(1);
  }

  /* find out assigned port number and print out */
  length = sizeof(rcvr);
  if(getsockname(sd, (struct sockaddr *)&rcvr, &length)) { // returns an error message if getsockname() function call returns an error
    perror("getting socket name");
    exit(1);
  }

  printf("Socket has port #%d\n",ntohs(rcvr.sin_port));

  /* Can get same effect by doing reads of the correct size. */
  rcvwin = buflen;

  if (gettimeofday(&sndtime1, &zone) < 0) { // gets transfer start time and returns error if function returns < 0
    perror("Getting time");
  }

  printf("Start time: %ld %ld\n", sndtime1.tv_sec, sndtime1.tv_usec); // displays start time in seconds and microseconds

  /* Accept connections from the transmitter */
  listen(sd, 5); // listens to socked sd for 5 intervals
  do {
    sda = accept(sd,0,0); // accepts the first connectino on the socket and assigns to sda variable

    if(sda == -1) // error message returned if accept function call returns -1
      perror("accept");
    else
      do {
        bzero(buf,sizeof(buf)); // places zero-valued bits to the amount of sizeof(buf) into buf

        if((rval = recv(sda,buf,rcvwin,MSG_WAITALL))<0) // receives packet from server and returns error if function call fails
          perror("reading message stream");

        if(rval == 0) { // ends connection when rval is returned 0
          printf("Ending connection\n");
        }
        else {
      	  sscanf(buf, "%d", &seqno); // assigns sequence number transmitted to server to buffer for packet
          printf("Received packet: seqno = %d length = %d\n", seqno, rval);
        }
      }while (rval != 0);

    close(sda);
    i++;

  }while (i < packet_count);

  if (gettimeofday(&sndtime2, &zone) < 0) { // gets transfer start time and returns error if function returns < 0
    perror("Getting time");
  }
  printf("End time: %ld %ld\n", sndtime2.tv_sec, sndtime2.tv_usec);
}
