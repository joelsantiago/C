//
//  main.c
//  StructChallenge
//
//  Created by Joel Santiago on 6/19/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>
#include <time.h>

int main(int argc, const char * argv[])
{
    long secondsSince1970 = time(NULL);
    printf("secondsSince1970 = %ld\n", secondsSince1970);
    secondsSince1970 += 4000000;
    printf("secondsSince1970 + 4 million seconds = %ld\n", secondsSince1970);
    
    struct tm now;
    localtime_r(&secondsSince1970, &now);
    printf("The current date is %d-%d-%d\n", now.tm_mon + 1, now.tm_mday, now.tm_year + 1900);
    
    return 0;
}

