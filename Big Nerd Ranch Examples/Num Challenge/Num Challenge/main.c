//
//  main.c
//  Num Challenge
//
//  Created by Joel Santiago on 6/17/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    int i = 99;
    while (i >= 0) {
        printf("%d\n", i);
        if (i % 5 == 0) {
            printf("Found one!\n");
        }
        i -= 3;
    }
    return 0;
}

