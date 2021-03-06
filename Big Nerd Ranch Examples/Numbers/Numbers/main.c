//
//  main.c
//  Numbers
//
//  Created by Joel Santiago on 6/17/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, const char * argv[])
{
    printf("3 * 3 + 5 * 2 = %d\n", 3 * 3 + 5 * 2);
    printf("11 / 3 = %d remainder of %d \n", 11 / 3, 11 % 3);
    printf("11 / 3 = %f\n", (float) 11 / 3);
    printf("The absolute value of -5 is %d\n", abs(-5));
    
    double y = 12345.6789;
    printf("y is %.2f\n", y);
    printf("y is %.2e\n", y);
    
    printf("The sine of 1 radian is %.3f.\n",sin(1.0));
    return 0;
}

