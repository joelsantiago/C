//
//  main.c
//  Triangle
//
//  Created by Joel Santiago on 6/17/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>

float remainingAngle(angleA, angleB) {
    return 180 - angleA - angleB;
}

int main(int argc, const char * argv[])
{
    float angleA = 30.0;
    float angleB = 60.0;
    float angleC = remainingAngle(angleA, angleB);
    printf("The third angle is %.2f\n", angleC);
    return 0;
}

