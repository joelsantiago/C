// USERVER.C
// Joel Santiago

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>           /* for gettimeofday() */
#include <stdlib.h>
#include <strings.h>

#define TRUE 1
#define FILESIZE 500000 // Hardcoded file size - 500KB * 1000 = 500,000 Bytes

/* This is the server program. It opens a socket with the given port
 * and then begins an infinite loop. Each time through the loop it
 * accepts a pkt and prints out the contents and the current time.
 *
 * Command line is `userver portnumber buffer_length'.
 */

int main(int argc,char *argv[]) {

  int sock, length;
  struct sockaddr_in server, client;
  int msgsock;
  int buflen = atoi(argv[2]); // buffer length entered by user through CLI
  unsigned char buf[buflen]; // assigns buffer length as length of buf array
  int rval, clilen, i; // values for socket function call recvfrom, length of the client address and i counter
  int seqno = 1; // packet sequence number assigned to 1
  struct timeval rcvtime; // struct variable used to get current time
  struct timezone zone; // struct variable used to get current time zone
  int count = 1; // count variable for labeling acknowledges sent back to client

  /* create socket */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0) { // displays error if socket creation returns an error
    perror("opening datagram socket");
    exit(1);
  }

  /* name socket using wildcards */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(atoi(argv[1])); // assigns user entered port number from CLI to sin_port value
  if(bind(sock, (struct sockaddr *)&server, sizeof(server))) { // bind takes entered name and binds it to the socket
    perror("binding socket name");
    exit(1);
  }

  /* find out assigned port number and print out */
  length = sizeof(server);
  if(getsockname(sock, (struct sockaddr *)&server, &length)) { // returns an error message if getsockname() function call returns an error
    perror("getting socket name");
    exit(1);
  }
  printf("Socket has port #%d\n\n",ntohs(server.sin_port));

  /* for loop that iterates up to the total amount of packets needed based on file size / buffer length */
  for (i = 0; i < (FILESIZE / buflen); i++) {

      clilen = sizeof(client);
      rval = recvfrom(sock,buf,buflen,0,(struct sockaddr *)&client,&clilen); // checks socket for any transmitted packets
      if (rval > 0) {
        buf[rval] = 0;
        printf("received message: \"%s\" (%d bytes)\n", buf, rval);
      }
      else // display error message if recvfrom function returns <= 0
        perror("receiver recvfrom");

      sprintf(buf, "ack %d", count++); // creates acknowledgement packet with count value
      printf("sending response \"%s\"\n\n", buf);

      if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&client, clilen) < 0) // sends packet acknowledgement to client
        perror("sendto");
  }

  /* get current relative time */
  if (gettimeofday(&rcvtime, &zone) < 0) { // gets current time and returns error if function returns < 0
    perror("getting time");
    exit(1);
  }

  printf("End Time: %ld %ld\n", rcvtime.tv_sec, rcvtime.tv_usec); // displays end time in seconds and microseconds
  // close(sock);
}






