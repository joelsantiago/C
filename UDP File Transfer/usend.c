/* Joel Santiago*/

/* Usend.c */
/* Client */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

/* This is the sender program. It creates a socket and initiates a
 * connection with the socket given in the command line.
 * 1 frame is sent and then the program exits.
 * The form of the command line is `usend4 hostname portnumber'.
 * The hostname might be algebra.sci.csueastbay.edu.  The port number
 * must be between 1000 and 64000 and must be the same as the
 * port number the receiver is running on.
 */

int main(int argc,char *argv[]) {

  FILE *file;
  int sock;
  struct sockaddr_in source, extrasa, dest;
  struct hostent *hp, *gethostbyname();
  char buf[1024];
  char ack[10];
  char frame[60];
  int seqno, frame_no;
  int rnum, rval;
  int recvlen;
  int destlen = sizeof(dest);
  srand(time(NULL));

  /* Check arguments */
  if (argc != 3) {
    printf("Usage: usend hostname portnumber\n");
    exit(0);
  }

  /* create socket */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0) {
    perror("Opening stream socket");
    exit(1);
  }

  /* now bind source address. Port is wildcard, doesn't matter. */
  source.sin_family = AF_INET;
  source.sin_addr.s_addr = INADDR_ANY;
  source.sin_port = 0;
  if (bind(sock, (struct sockaddr *)&source, sizeof(source)) < 0) {
    perror("binding socket name");
    exit(1);
  }

  /* get and create host using hostname from command line argument */
  dest.sin_family = AF_INET;
  hp = gethostbyname(argv[1]);
  if(hp == 0) {
    printf("%s: unknown host\n",argv[1]);
    exit(2);
  }

  bcopy(hp->h_addr, &dest.sin_addr, hp->h_length);
  dest.sin_port = htons(atoi(argv[2]));

  /* get user entered filename to be retrieved from server */
  printf("Enter the file to be retrieved: ");
  char filename[256];
  scanf("%s", &filename);
  printf("\n");

  /* copy user entered filename to a temporary filename variable that will be used for fopen function later */
  char newfile[256];
  strcpy(newfile,filename);

  /* check if file already exists */
  if (fopen(newfile, "rb")) {
    printf("File: %s already exists\n", newfile);
    printf("Do you want to:\n");
    printf("Keep\t= 1\n");
    printf("Replace\t= 2\n");
    printf("Stop\t= 3\n\n");
    printf("Enter choice: ");

    int choice;
    int quit = 0;
    int let;
    char tempname[256];
    char dot = '.';

    /* while not true */
    while (!quit) {
      /* get user choice */
      scanf("%d", &choice);

      /* switch statement for file options */
      switch(choice) {
        /* keep file choice - append copy to filename */
        case 1:
          /* iterate through newfile char array to find file format specifier */
          for (let = 0; let < strlen(newfile); ++let) {
            /* if char in newfile == . insert "copy" */
            if (newfile[let] == dot) {
              strncpy(tempname, newfile, let);
              tempname[let] = '\0';
              strcat(tempname, " copy");
              strcat(tempname, newfile + let);
            }
          }
          /* move filename from tempname back to newfile */
          strcpy(newfile, tempname);
          printf("New filename: %s\n\n", newfile);
          /* last check - removes duplicate copy file if it exists */
          remove(newfile);
          /* sets quit to 1 to exit while loop */
          quit = 1;
          break;

        /* remove original file instead of renaming */
        case 2:
          printf("File: %s has been deleted\n\n", newfile);
          /* remove original file */
          remove(newfile);
          /* sets quit to 1 to exit while loop */
          quit = 1;
          break;

        /* exits program */
        case 3:
          printf("Stopping file transfer\n\n");
          exit(1);

        /* incorrect entry - prompts user for re-entry */
        default:
          printf("Please enter a valid choice (1 or 2 or 3): ");
          break;
      }
    }
  }

  /* create initial frame with initial seqno and filename */
  frame_no = 0;
  sprintf(buf, "%d %s", frame_no, filename);

  /* time variables, will be repeated throughout to update time on display */
  time_t timer;
  time(&timer);
  struct tm* timeinfo;
  timeinfo = localtime(&timer);

  printf("Client sent frame: Sequence Number = %d\t\tTime = %.2d:%.2d:%.2d\t\tPackage Contents = %s\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, filename);

  /* send frame with filename to server */
  if ((rval=sendto(sock,buf,sizeof(buf),0,(struct sockaddr *)&dest,sizeof(dest))) < 0) {
    perror("writing on datagram socket");
  }

  /* wait to receive a frame to be sent from the server, display error message if recvfrom fails */
  while ((recvlen = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&dest, &destlen)) < 0) {
    perror("receiver recvfrom");
  }

  /* if recvfrom returns true */
  if (recvlen > 0) {
    buf[recvlen] = 0;
    /* scan frame for seqno and file open confirmation */
    sscanf(buf, "%d %[^\n]s", &frame_no, &filename);

    time(&timer);
    timeinfo = localtime(&timer);
    /* display confirmation message */
    printf("Client received ACK: Sequence Number = %d\tTime = %.2d:%.2d:%.2d\t\tPackage Contents = %s\n\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, filename);
  }

  /* do-while loop until recvlen == 0 */
  do {
    /* wait to receive frame from server, display error message if recvfrom fails */
    if ((recvlen = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&dest, &destlen)) < 0) {
      perror("receiver recvfrom");
    }

    /* if recvlen returns true */
    if (recvlen > 0) {
      buf[recvlen] = 0;
      /* scan received frame for seqno and payload */
      sscanf(buf,"%d %[^ ]s", &frame_no, &frame);

      time(&timer);
      timeinfo = localtime(&timer);

      /* random number generator */
      rnum = random() % 4;

      /* if rnum does not equal 0 */
      if (rnum != 0) {
        /* display confirmation message of received frame */
        printf("Client received frame: Sequence Number = %d\tTime = %.2d:%.2d:%.2d\t\tPackage Contents = %.5s\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, frame);

        time(&timer);
        timeinfo = localtime(&timer);

        /* open or create a file with stored filename */
        file = fopen(newfile, "ab");
        /* if file doesn't exist */
        if (!file)
          return 1;

        /* printf frame payload to file */
        fprintf(file, frame);
        /* close file completely or until next loop */
        fclose(file);

        /* display ACK received confirmation message */
        printf("Client sent ACK: Sequence Number = %d\t\tTime = %.2d:%.2d:%.2d\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

        /* print ACK seqno to buf to be sent to server */
        sprintf(buf, "ACK %d", frame_no);
        /* send ACK frame back to server or display message if sendto fails */
        if ((rval=sendto(sock,buf,sizeof(buf),0,(struct sockaddr *)&dest,sizeof(dest))) < 0) {
          perror("sendto");
        }
      }
      /* if rnum == 0 and frame is discarded */
      else {
        time(&timer);
        timeinfo = localtime(&timer);
        /* display received message to client */
        printf("Client received frame: Sequence Number = %d\tTime = %.2d:%.2d:%.2d\t\tPackage Contents = %.5s\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, frame);

        time(&timer);
        timeinfo = localtime(&timer);
        /* display frame discarding message to client */
        printf("Client discarding frame: Sequence Number = %d\tTime = %.2d:%.2d:%.2d\n", frame_no, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
      }
    }
  } while (recvlen != 0);
  /* close socket */
  close(sock);
}
