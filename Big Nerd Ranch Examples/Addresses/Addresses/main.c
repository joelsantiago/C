//
//  main.c
//  Addresses
//
//  Created by Joel Santiago on 6/19/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    int i = 17;
    int *addressOfI = &i;
    printf("this function starts at %p\n", main);
    
    printf("\n");
    
    printf("i stores its value at %p\n", addressOfI);
    printf("the int stored at addressOfI is %d\n", *addressOfI);
    
    *addressOfI = 89;
    printf("Now i is %d\n", *addressOfI);
    
    printf("\n");
    
    printf("An int is %zu bytes\n", sizeof(int));
    printf("A pointer is %zu bytes\n", sizeof(int *));
    printf("The pointer addressOfI is %zu bytes\n", sizeof(addressOfI));
    
    printf("\n");
    
    float *myPointer;
    myPointer = NULL;
    if (myPointer) {
        // if myPointer is not NULL
        printf("Test one\n");
    } else
        printf("Test two\n");
    return 0;
}

