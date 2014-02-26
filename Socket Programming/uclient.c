// UCLIENT.C
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

/* This is the client program. It creates a socket and initiates a
 * connection with the socket given in the command line.
 *
 * The form of the command line is `uclient hostname portnumber buffer_length'.
 */

int main(int argc,char *argv[]) {

  int sock;
  struct sockaddr_in server, client;
  struct hostent *hp, *gethostbyname();
  int buflen = atoi(argv[3]); // buffer length entered by user through CLI
  char buf[buflen]; // assigns buffer length as length of buf array
  int recvlen; // variable for value from recvfrom function call
  int addrlen = sizeof(server); // variable for the length of ther server struct
  struct timeval rcvtime1, rcvtime2; // struct variables used to get current time for before and after
  struct timezone zone; // struct variable used to get current time zone
  int elapsed_sec, elapsed_usec; // variables to hold seconds and microseconds values

  /* create socket */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0) { // displays error if socket creation returns an error
    perror("Opening datagram socket");
    exit(1);
  }

  /* now bind client address. Port is wildcard, doesn't matter. */
  client.sin_family = AF_INET;
  client.sin_addr.s_addr = INADDR_ANY;
  client.sin_port = 0;
  if (bind(sock, (struct sockaddr *)&client, sizeof(client)) < 0) { // bind takes entered name and binds it to the socket
    perror("binding socket name");
    exit(1);
  }

  server.sin_family = AF_INET;
  hp = gethostbyname(argv[1]); // function to get the host name from the CLI arguments
  if(hp == 0) { // displays error message if hostname returns false
    printf("%s: unknown host\n",argv[1]);
    exit(2);
  }

  bcopy(hp->h_addr, &server.sin_addr, hp->h_length); // bcopy copies host address to the sin_addr variable
  server.sin_port = htons(atoi(argv[2])); // assigns user entered port number from CLI to sin_port value

  /* create a packet */
  sprintf(buf, "%d\0", 2500);

  /* get current relative time */
  if (gettimeofday(&rcvtime1, &zone) < 0) { // gets transfer start time and returns error if function returns < 0
    perror("getting time");
    exit(1);
  }

  printf("Start Time: %ld %ld\n\n", rcvtime1.tv_sec, rcvtime1.tv_usec); // displays start time in seconds and microseconds

  /* for loop that iterates up to the total amount of packets needed based on file size / buffer length */
  for (int i = 0; i < (FILESIZE / buflen); i++) {

    printf("Sending packet %d to %s port %s\n", i, argv[1], argv[2]);
    sprintf(buf, "This is packet %d", i+1); // packet creation message for outgoing packet
    if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&server, addrlen) == -1) { // sends packet to socket, displays error if function call returns an error
      perror("sendto");
      exit(1);
    }

    // printf("waiting on port %d\n", atoi(argv[2]));
    recvlen = recvfrom(sock, buf, buflen, 0, (struct sockaddr *)&server, &addrlen); // checks socket for any transmitted packets
    printf("received %d bytes\n", recvlen);
    if (recvlen > 0) {
      buf[recvlen] = 0;
      printf("received message: \"%s\"\n\n", buf);
    }
  }

  /* get current relative time */
  if (gettimeofday(&rcvtime2, &zone) < 0) { // gets transfer end time and returns error if function returns < 0
    perror("getting time");
    exit(1);
  }
  printf("End Time: %ld %ld\n", rcvtime2.tv_sec, rcvtime2.tv_usec);

  elapsed_sec = (rcvtime2.tv_sec - rcvtime1.tv_sec); // finds out elapsed seconds for transfer
  elapsed_usec = (rcvtime2.tv_usec - rcvtime2.tv_usec) / 1000000; // finds out elapsed microseconds for transfer

  if (elapsed_usec < 0) // accounts for negative values from microsecond calculation
    elapsed_usec *= -1;

  printf("Elapsed Time: %d sec\n", elapsed_sec + elapsed_usec); // displays total elapsed time
}
