//
//  main.c
//  FloatValues
//
//  Created by Joel Santiago on 6/19/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    int one = 1;
    int two = 2;
    int three = 3;
    
    int *a = &one;
    int *b = &two;
    int *c = &three;
    
    printf("On this computer, in X-Code running in 64-bit mode, a float consumes %lu bytes of RAM\n", sizeof(float));
    
    printf("\n");
    
    printf("float pointer a = %p\n", &a);
    printf("a = %d\n", *a);
    printf("float pointer b = %p\n", &b);
    printf("b = %d\n", *b);
    printf("float pointer c = %p\n", &c);
    printf("c = %d\n", *c);
    return 0;
}

