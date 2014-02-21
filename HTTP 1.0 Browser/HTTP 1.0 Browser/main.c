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
    
    printf("\nEnter a URL: ");
    scanf("%s", url);
    
    printf("\nLoading: %s\n\n", url);
    
    sscanf(url, "%7[^:]://%127[^/]%120[^\n]", proto, domain, page_loc);
    printf("Proto\t\t= \"%s\"\n", proto);
    printf("Domain\t\t= \"%s\"\n", domain);
    printf("Page Loc.\t= \"%s\"\n\n", page_loc);
    
    char file_header[30];
    int count = 0;
    int a;
    for (a = 0; a < strlen(page_loc); ++a)
    {
        if (count != 3) {
            file_header[a] = page_loc[a];
            if (page_loc[a] == '/') {
                count++;
            }
        }
    }
    
    char *page_header;
    page_header = file_header;

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
        
        char *pagereq = page_loc;
        
        sprintf(httpget, "GET %s HTTP/1.0\r\n\r\n", pagereq);
        
        printf("Sending %zd bytes\n", send(sd, httpget, strlen(httpget),0));
        
        strcpy(http_resp, "");
        
        if ((rval = recv(sd, http_resp, sizeof(http_resp), MSG_WAITALL)) == -1)
            perror("reading message stream");
        
        close(sd);
        printf("Receiving %lu bytes\n", rval);
        
        http_body = strstr(http_resp, "\r\n\r\n");
        if(http_body)
            http_body += 4;
        
        http_body = strstr(http_body, "<html>\n<body>\n");
        if(http_body)
            http_body += 13;
        
        if ((file = fopen("cache.txt", "r"))) {
            remove("cache.txt");
        }
        file = fopen("cache.txt", "w");
        fprintf(file, "%s", http_body);
        fclose(file);
        
        char *newstring = http_body;
        
        char *strptr = strtok(newstring, "\n<>\"");
        char tokens[50][100];
        char links[3][15];
        char link_url[3][20];
        char temp[20];
        int i = 0;
        int j = 0;
        int k = 0;
        int link_count = 0;
        int tempcount = 0;
        
        while (strptr) {
            strcpy(tokens[i], strptr);
            strptr = strtok(NULL, "\n<>\"");
            i++;
        }
        
        i = 0;
        char new_body[50][100];
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
        for (i = 0; i < (sizeof(new_body) / sizeof(new_body[0])); i++) {
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
            else if ((strcmp(new_body[i], "img SRC=")) == 0) {
                strcpy(new_body[i], "-1");
                strcpy(new_body[i+1], "-1");
                strcpy(new_body[i+2], "-1");
                if ((strcmp(new_body[i+4], "")) == 0) {
                    strcpy(new_body[i+4], "-1");
                }
            }
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
            else if (((strcmp(new_body[i], "/a")) == 0) ||
                     ((strcmp(new_body[i], "/ul")) == 0) ||
                     ((strcmp(new_body[i], "/li")) == 0)) {
                strcpy(new_body[i], "-1");
            }
        }
        
        char newtemp[50][100];
        link_count = 0;
        j = 0;
        
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
        strcpy(fullpageloc, page_header);
        
        printf("Enter choice for 1, 2, or 3 OR enter q to quit: ");
    
        char choice;
        int quit = 0;
        
        while (!quit) {
            
            scanf("%s", &choice);
            printf("\n");
            
            switch (choice) {
                case '1':
                    templink = link_url[0];
                    strcat(fullpageloc, templink);
                    printf("Loading page: %s://%s%s\n\n", proto, domain, fullpageloc);
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
                    printf("Quitting program\n\n");
                    exit(1);
                    break;
                default:
                    printf("Enter a valid choice\n\n");
                    break;
            }
        }
        strcpy(page_loc, fullpageloc);
    }
}