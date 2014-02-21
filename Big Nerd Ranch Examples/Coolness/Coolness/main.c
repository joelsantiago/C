//
//  main.c
//  Coolness
//
//  Created by Joel Santiago on 6/17/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    int c = 0;
    while (c < 12) {
        printf("Aaron is Cool\n");
        c++;
    }
    
    printf("\n");
    
    for (int i = 0; i < 12; i++) {
        printf("Aaron is Cool\n");
    }
    
    printf("\n");
    
    int h;
    for (h = 0; h < 12; h++) {
        printf("Checking h = %d\n", h);
        if (h + 90 == h * h) {
            break;
        }
    }
    printf("The answer is %d.\n", h);
    
    printf("\n");
    
    int j;
    for (j = 0; j < 12; j++) {
        if (j % 3 == 0) {
            continue;
        }
        printf("Checking j = %d\n", j);
        if (j + 90 == j * j) {
            break;
        }
    }
    printf("The answer is %d.\n", j);
    
    printf("\n");
    
    int k = 0;
    do {
        printf("%d. Aaron is Cool\n", k);
        k++;
    } while (k < 13);
    
    return 0;
}

