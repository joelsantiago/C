/* Joel Santiago */

/* Urecv.C */
/* Server */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define TRUE 1
#define FALSE 0

/* This is the receiver program. It opens a socket with the given port
 * and then begins an infinite loop. Each time through the loop it
 * accepts a pkt and prints out the contents and the current time.
 * Command line is `urecv4 portnumber'.
 * The port number is a number between 1000 and 64000.
 */

int main(int argc,char *argv[]) {

  FILE *file;
  int sock, length;
  struct sockaddr_in source, dest;
  int msgsock;
  char buf[1024];
  char filename[256];
  char line[12];
  char frame[60];
  int rval, srclen, sval;
  int i, frame_no, count;
  struct timeval timeout;
  struct timezone zone;
  bool var;
  var = FALSE;

  /* check arguments */
  if (argc != 2) {
    printf("Usage: urecv localport\n");
    exit(0);
  }

  /* create socket */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0) {
    perror("opening datagram socket");
    exit(1);
  }

  /* name socket using wildcard for IP address and given port number */
  dest.sin_family = AF_INET;
  dest.sin_addr.s_addr = INADDR_ANY;
  dest.sin_port = htons(atoi(argv[1]));
  if(bind(sock, (struct sockaddr *)&dest, sizeof(dest))) {
    perror("binding socket name");
    exit(1);
  }

  /* find out assigned port number and print out */
  length = sizeof(dest);
  if(getsockname(sock, (struct sockaddr *)&dest, &length)) {
    perror("getting socket name");
    exit(1);
  }
  printf("Socket has port #%d\n\n",ntohs(dest.sin_port));

  /* do-while loop sends and recieves frame from client chosen file */
  do {
    bzero(buf,sizeof(buf));
    srclen = sizeof(source);

    /* wait for a frame from the client to receive */
    while ((rval = recvfrom(sock,buf,1024,0,(struct sockaddr *)&source,&srclen)) < 0){
      perror("receiver recvfrom");
    }

    /* scan initial recieved frame for sequence number and filename */
    sscanf(buf, "%d %s", &frame_no, &filename);

    /* time values */
    time_t timer;
    time(&timer);
    struct tm* timeinfo;
    timeinfo = localtime(&timer);

    /* print time and sequence number of initial frame */
    printf("Server received frame: Sequence Number = %d\tTime = %.2d:%.2d:%.2d\t\tPackage Contents = %s\n\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, filename);

    /* print client requested file that will be opened */
    printf("File to be opened: \'%s\'\n", filename);

    /* use filename to open local file */
    file = fopen(filename, "r");

    /* if file returns false and file cannot be opened */
    if (!file) {

      /* display local error message */
      printf("File \'%s\' could not be opened\n\n", filename);

      time(&timer);
      timeinfo = localtime(&timer);
      /* create frame containing file open error message */
      sprintf(buf, "%d File \'%s\' could not be opened", frame_no, filename);
      printf("Server sent frame: Sequence Number = %d\t\tTime = %.2d:%.2d:%.2d\t\tPackage Contents = %s\n\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, buf);

      /* send error acknowledgment frame to client */
      if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&source, srclen) < 0) // sends frame acknowledgement to client
        perror("sendto");

      /* exit program */
      exit(1);
    }
    /* otherwise if file open returns true and file was opened */
    else {

      /* display local confirmation message of file open */
      printf("File \'%s\' has been opened\n\n", filename);

      time(&timer);
      timeinfo = localtime(&timer);
      /* create frame containing file open confirmation message */
      sprintf(buf, "%d File \'%s\' has been opened", frame_no, filename);
      printf("Server sent frame: Sequence Number = %d\t\tTime = %.2d:%.2d:%.2d\t\tPackage Contents = %s\n\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, buf);

      /* send confirmation acknowledgment frame to client */
      if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&source, srclen) < 0) // sends frame acknowledgement to client
        perror("sendto");
    }

    /* file row counter */
    count = 1;
    /* frame counter */
    frame_no++;

    /* open file using sent filename */
    while (fgets(line, sizeof line, file) != NULL) {

      /* concatenates line from file into frame placeholder */
      strcat(frame, line);
      count++;

      /* if 60 bytes (characters) have been met, create frame with payload and send to client */
      if (count == 7) {

        time(&timer);
        timeinfo = localtime(&timer);

        /* create frame containing header value and payload */
        sprintf(buf, "%d %s", frame_no, frame); // creates acknowledgement frame with count value

        /* send frame to client */
        if ((sval = sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&source, srclen)) < 0) // sends frame acknowledgement to client
          perror("sendto");

        printf("Server sent frame: Sequence Number = %d\t\tTime = %.2d:%.2d:%.2d\t\tPackage Contents = %.5s\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, frame);

        /* sleep loop for 1 second */
        sleep(1);

        /* set timeout check to TRUE */
        bool tocheck = TRUE;

        /* do-while loop to wait for receieved frames or verify if a timeout occurred and resend the current frame */
        do {

          /* timeout variables */
          fd_set input;
          int retval;

          /* itnitialize the input set */
          FD_ZERO(&input);
          FD_SET(sock, &input);

          /* initialize the timeout structure with seconds value */
          timeout.tv_sec = 1;

          /* check if timeout select function failed altogether */
          if ((retval = (select(sizeof(input)*8, &input, NULL, NULL, &timeout))) == -1)
            perror("select failed");
          /* check if timeout select function passed with no activity on selected socket */
          else if ((retval = (select(sizeof(input)*8, &input, NULL, NULL, &timeout))) == 0) {
            time(&timer);
            timeinfo = localtime(&timer);
            /* display timeout error message to server */
            printf("Server timeout: Sequence number = %d\t\tTime = %.2d:%.2d:%.2d\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

            /* resend current frame to client */
            if ((sval = sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&source, srclen)) < 0) {// sends frame acknowledgement to client
              perror("sendto");
            }

            /* assign timeout check to TRUE to stay in loop */
            tocheck = TRUE;

            /* sleep loop to allow 2 seconds to pass between frame sends */
            sleep(1);
          }
          /* if select function passed and activity was encountered on selected socket */
          else {
            /* receive frame from client */
            if ((rval = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&source, &srclen)) < 0) {
              perror("receiver recvfrom");
            }

            /* assign timeout check to FALSE to exit loop */
            tocheck = FALSE;
            /* time variables */
            time(&timer);
            timeinfo = localtime(&timer);

            /* scan frame for ACK number and display message to server */
            sscanf(buf, "%d", frame_no);
            printf("Server received ACK: Sequence Number = %d\tTime = %.2d:%.2d:%.2d\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
          }
        } while (tocheck);

        /* reset line count value to 1 and start again */
        count = 1;
        /* increase frame number by 1 */
        frame_no++;
        /* erase current contents of frame so that frame is empty for next 6 lines (60 bytes) */
        strcpy(frame, "");
      }
    }
    /* final check if end of file is met and count is less than 7 (frame is not full) */
    if ((fgets(line, sizeof line, file) == NULL) && count < 7) {

      time(&timer);
      timeinfo = localtime(&timer);

      /* create frame containing sequence number and remaining lines of text (sub-60 bytes in the payload) */
      sprintf(buf, "%d %s", frame_no, frame);
      printf("Server sent frame: Sequence Number = %d\t\tTime = %.2d:%.2d:%.2d\t\tPackage Contents = %.5s\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, frame);

      /* send confirmation acknowledgment frame to client */
      if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&source, srclen) < 0) // sends frame acknowledgement to client
        perror("sendto");

      /* sleep loop for 1 second */
      sleep(1);

      /* receive frame from client */
      if ((rval = recvfrom(sock,buf,1024,0,(struct sockaddr *)&source,&srclen)) < 0){
        perror("receiver recvfrom");
      }
      /* scan frame for ACK number */
      sscanf(buf, "%d", frame_no);
      printf("Server received ACK: Sequence Number = %d\tTime = %.2d:%.2d:%.2d\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

      /* close socket and exit program */
      printf("Socket closed\n");
      close(sock);
      exit(1);
    }
  }
  /* unless rval does not equal 0 */
  while (rval != 0);
}
