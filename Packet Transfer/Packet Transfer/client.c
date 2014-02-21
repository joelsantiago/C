//
//  client.c
//  Packet Transfer
//
//  Created by Joel Santiago on 11/11/13.
//  Copyright (c) 2013 Joel Santiago. All rights reserved.
//

// TCPXMIT.C

#include <stdio.h>
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

/* This is the transmitter program. It creates a socket and initiates a
 connection with the socket given in the command line. The send buffer
 is set as requested.  One message is sent over the connection and then the
 socket is closed.
 
 The form of the command line is: `tcpxmit rcvr-hostname rcvr-port buflen'. */

int main(int argc,char *argv[])
{
    int sd;
    struct sockaddr_in rcvr;
    struct hostent *hp, *gethostbyname();
    int buflen = atoi(argv[3]);
    char buf[1500];
    int sndwin, optlen;
    struct timeval sndtime;
    struct timezone zone;
    int rval, seqno;
    
    int count;
    
    sndwin = buflen;
    
    /* create socket */
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd < 0) {
        perror("Opening stream socket");
        exit(1);
    }
    
    /* Create rcvr socket address using command line arguments */
    rcvr.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if(hp == 0) {
        printf("%s: unknown host\n",argv[1]);
        exit(2);
    }
    bcopy(hp->h_addr, &rcvr.sin_addr, hp->h_length);
    rcvr.sin_port = htons(atoi(argv[2]));
    
    /* connect socket using name specified at command line */
    if(connect(sd, (struct sockaddr *)&rcvr, sizeof(rcvr)) < 0) {
        perror("connecting stream socket");
        exit(1);
    }
    
    if (gettimeofday(&sndtime, &zone) < 0) {
        perror("Getting time");
    }
    printf("\nStart time: %ld %ld\n", sndtime.tv_sec, sndtime.tv_usec);
    
    seqno = 1;
    
    /* create a packet */
    sprintf(buf, "%d\0", seqno);
    
    /* send the packet */
    if((rval=send(sd, buf, sndwin,0)) < 0) {
        perror("writing on stream socket");
    } else {
        printf("Sent packet of length %d\n", rval);
    }
    
    printf("End time: %ld %ld\n\n", sndtime.tv_sec, sndtime.tv_usec);
    
    close(sd);
}
