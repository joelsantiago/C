/* Joel Santiago */
/* CS 4590 Program 3 */

#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#define TRUE 1

/**
 * [main  description]
 * @param  argc
 * @param  argv
 * @return
 */
int main (int argc, char *argv[]) {

    /* File identifier to open up a file for caching page contents after load */
    FILE *file;

    int sd;
    unsigned long rval;
    struct sockaddr_in rcvr;
    struct hostent *hp, *gethostbyname();

    char url[256];
    char proto[8];
    char domain[128];
    char page_loc[120];
    char httpget[256];
    char http_resp[20000];
    char *http_body;

    /* Prompt user for URL to load */
    printf("\nEnter a URL: ");
    scanf("%s", url);

    printf("\nLoading: %s\n\n", url);

    /* Scans URL for Prototype, Domain and Page Location */
    sscanf(url, "%7[^:]://%127[^/]%120[^\n]", proto, domain, page_loc);
    printf("Proto\t\t= \"%s\"\n", proto);
    printf("Domain\t\t= \"%s\"\n", domain);
    printf("Page Loc.\t= \"%s\"\n\n", page_loc);

    char file_header[30];
    int count = 0;
    int a;
    /**
     * Parses page location and copies everything but the file name
     * to another placeholder variable to be used later
     */
    for (a = 0; a < strlen(page_loc); ++a)
    {
        if (count != 3) {
            file_header[a] = page_loc[a];
            if (page_loc[a] == '/') {
                count++;
            }
        }
    }

    /**
     * Variable reassignment to ensure contents can be used initially
     * as well as after the loop completes and link is appended onto
     * the page header string
     */
    char *page_header;
    page_header = file_header;

    /**
     * Infinite loop waits for new pages to load
     */
    while (TRUE) {

        /* create socket */
        sd = socket(AF_INET, SOCK_STREAM, 0);
        if(sd < 0) {
            perror("opening stream socket");
            exit(1);
        }

        /* Create rcvr socket address using command line arguments */
        rcvr.sin_family = AF_INET;
        hp = gethostbyname(domain);
        if(hp == 0) {
            printf("%s: unknown host\n", domain);
            exit(2);
        }

        bcopy(hp->h_addr, &rcvr.sin_addr, hp->h_length);
        rcvr.sin_port = htons(80);

        /* connect socket using name specified at command line */
        if(connect(sd, (struct sockaddr *)&rcvr, sizeof(rcvr)) < 0) {
            perror("connecting stream socket");
            exit(1);
        }

        /**
         * Variable reassignment from variable page_loc created at the end of loop
         * when link is chosen
         */
        char *pagereq = page_loc;

        /* Creates packet to be sent to server requesting page */
        sprintf(httpget, "GET %s HTTP/1.0\r\n\r\n", pagereq);

        /* Sends packet to server containing URL */
        printf("Sending %zd bytes\n", send(sd, httpget, strlen(httpget),0));

        /* Waits to receive packet from server containing HTML code */
        if ((rval = recv(sd, http_resp, sizeof(http_resp), MSG_WAITALL)) == -1)
            perror("reading message stream");

        /* Close socket */
        close(sd);
        printf("Receiving %lu bytes\n", rval);

        /**
         * Finds substring for CR/LF pair between header and paylod
         * and moves start of array to beginning of HTML payload
         */
        http_body = strstr(http_resp, "\r\n\r\n");
        if(http_body)
            http_body += 4;

        /**
         * If cache.txt exists, delete old file
         */
        if ((file = fopen("cache.txt", "r"))) {
            remove("cache.txt");
        }
        /* Open cache.txt file to be written to */
        file = fopen("cache.txt", "w");
        /* Write body contents to cache.txt file */
        fprintf(file, "%s", http_body);
        /* Close cache file */
        fclose(file);

        /**
         * Finds substring matching the html and body headers of the webpage
         * and moves starting array point to the beginning of body contents
         */
        http_body = strstr(http_body, "<html>\n<body>\n");
        if(http_body)
            http_body += 13;

        /* Assign strtok to variable with necessary delimiters */
        char *strptr = strtok(http_body, "\n<>\"");
        char tokens[50][100];
        char links[3][15];
        char link_url[3][20];
        char temp[20];
        int i = 0;
        int j = 0;
        int k = 0;
        int link_count = 0;
        int tempcount = 0;

        /* Tokens char array and assign to tokens array */
        while (strptr) {
            strcpy(tokens[i], strptr);
            strptr = strtok(NULL, "\n<>\"");
            i++;
        }

        i = 0;
        char new_body[50][100];

        /**
         * Checks for '/body' string and copies values
         * to new array until string is found
         */
        while ((strcmp(tokens[i], "/body")) != 0) {
            if ((strcmp(tokens[i], "/body")) == 0) {
                break;
            }
            strcpy(new_body[i], tokens[i]);
            i++;
        }

        printf("\n");

        j = 0;
        link_count = 0;

        /**
         * Parse string array to find specific values and assign correct
         * replacement characters in their place or -1 or -2 in places that need
         * to be altered
         */
        for (i = 0; i < (sizeof(new_body) / sizeof(new_body[0])); i++) {
            /* CLOSING PAGE TAGS */
            if (((strcmp(new_body[i], "/html")) == 0) ||
                ((strcmp(new_body[i], "/HTML")) == 0) ||
                ((strcmp(new_body[i], "/BODY")) == 0) ||
                ((strcmp(new_body[i], "/body")) == 0)) {
                strcpy(new_body[i], "");
            }
            else if (((strcmp(new_body[i], "p")) == 0) ||
                     ((strcmp(new_body[i], "br")) == 0) ||
                     ((strcmp(new_body[i], "ol")) == 0)) {

                if (((strcmp(new_body[i+1], "p")) == 0) ||
                    ((strcmp(new_body[i+1], "br")) == 0)) {
                    strcpy(new_body[i+1], "-1");
                    if (((strcmp(new_body[i+2], "p")) == 0) ||
                        ((strcmp(new_body[i+2], "br")) == 0)) {
                        strcpy(new_body[i+2], "-1");
                    }
                }
                strcpy(new_body[i], "");
            }
            /* IMAGES */
            else if ((strcmp(new_body[i], "img SRC=")) == 0) {
                strcpy(new_body[i], "-1");
                strcpy(new_body[i+1], "-1");
                strcpy(new_body[i+2], "-1");
                if ((strcmp(new_body[i+4], "")) == 0) {
                    strcpy(new_body[i+4], "-1");
                }
            }
            /* UNORDERED LISTS */
            else if ((strcmp(new_body[i], "ul")) == 0) {
                strcpy(new_body[i], "-1");
                if ((strcmp(new_body[i+1], "li")) == 0) {
                    strcpy(new_body[i+1], "-1");

                    i += 3;
                    for (k = 0; k < 3; ++k)
                    {
                        if ((strcmp(new_body[i], "a href=")) == 0) {
                            sscanf(new_body[i+2], "%s", temp);
                            sprintf(links[link_count], "  - %s[%d]", temp, link_count+1);
                            sprintf(link_url[link_count], "%s", new_body[i+1]);
                            link_count++;

                            strcpy(new_body[i], "-2");
                            strcpy(new_body[i+1], "-1");
                            strcpy(new_body[i+2], "-1");
                            strcpy(new_body[i+3], "-1");
                            strcpy(new_body[i+4], "-1");
                            strcpy(new_body[i+5], "-1");

                            if (k == 2)
                                i += 4;
                            else
                                i += 6;
                        }
                    }
                }
            }
            /* LISTS */
            else if ((strcmp(new_body[i], "li")) == 0) {
                if ((strcmp(new_body[i+1], "a href=")) == 0) {
                    sscanf(new_body[i+3], "%s", temp);
                    sprintf(links[link_count], "\t%d. %s[%d]", link_count+1, temp, link_count+1);
                    sprintf(link_url[link_count], "%s", new_body[i+2]);
                    link_count++;
                    tempcount++;

                    strcpy(new_body[i], "-1");
                    strcpy(new_body[i+1], "-2");
                    strcpy(new_body[i+2], "-1");
                    strcpy(new_body[i+3], "-1");

                }
            }
            /* LINKS */
            else if ((strcmp(new_body[i], "a href=")) == 0) {
                sscanf(new_body[i+2], "%s", temp);
                sprintf(links[link_count], "%s[%d]", temp, link_count+1);
                sprintf(link_url[link_count], "%s", new_body[i+1]);

                if ((strcmp(new_body[i-1], "")) == 0) {
                    strncpy(new_body[i-1], "-2", 2);
                    strcpy(new_body[i], "-1");
                }
                else {
                    strcpy(new_body[i], "-2");
                }
                strcpy(new_body[i+1], "-1");
                strcpy(new_body[i+2], "-1");
                link_count++;
            }
            /* CLOSING TAGS */
            else if (((strcmp(new_body[i], "/a")) == 0) ||
                     ((strcmp(new_body[i], "/ul")) == 0) ||
                     ((strcmp(new_body[i], "/li")) == 0)) {
                strcpy(new_body[i], "-1");
            }
        }

        char newtemp[50][100];
        link_count = 0;
        j = 0;

        /**
         * Iterate through array and replace -1's or -2's with new content or new links
         */
        for (i = 0; (i < (sizeof(newtemp) / sizeof(newtemp[0]))); ++i) {
            if ((strcmp(new_body[i], "-1")) != 0) {
                strcpy(newtemp[j], new_body[i]);
                j++;
            }
            if ((strcmp(new_body[i], "-2")) == 0) {
                strcpy(newtemp[j-1], links[link_count]);
                link_count++;
            }
        }

        printf("--------------------PAGE CONTENTS--------------------\n\n");

        /**
         * Display contents of page while checking for two null lines, if found, stop output
         */
        for (i = 0; i < 50; ++i) {
            printf("%s\n", newtemp[i]);
            if ((strcmp(newtemp[i+1], "\0")) == 0) {
                if ((strcmp(newtemp[i+2], "\0")) == 0) {
                    i = 50;
                }
            }
        }

        printf("-----------------------------------------------------\n\n");

        char fullpageloc[30];
        char *templink = NULL;

        /* assign page header to new variable, waiting for file name to appended */
        strcpy(fullpageloc, page_header);

        printf("Enter choice for 1, 2, or 3 OR enter q to quit: ");

        char choice;
        int quit = 0;

        /* loop until entered link value is correct */
        while (!quit) {

            scanf("%s", &choice);
            printf("\n");

            switch (choice) {
                case '1':
                    /* assigns link from array to templink variable */
                    templink = link_url[0];
                    /* appends templink variable to page header */
                    strcat(fullpageloc, templink);
                    printf("Loading page: %s://%s%s\n\n", proto, domain, fullpageloc);
                    /* quit loop */
                    quit = 1;
                    break;
                case '2':
                    templink = link_url[1];
                    strcat(fullpageloc, templink);
                    printf("Loading page: %s://%s%s\n\n", proto, domain, fullpageloc);
                    quit = 1;
                    break;
                case '3':
                    templink = link_url[2];
                    strcat(fullpageloc, templink);
                    printf("Loading page: %s://%s%s\n\n", proto, domain, fullpageloc);
                    quit = 1;
                    break;
                case 'q':
                case 'Q':
                    /* quits program */
                    printf("Quitting program\n\n");
                    exit(1);
                    break;
                default:
                    /* user entered incorrect value */
                    printf("Enter a valid choice\n\n");
                    break;
            }
        }
        /* copies new page location to old page_loc variable to be reused when looping around for a new page */
        strcpy(page_loc, fullpageloc);
    }
}