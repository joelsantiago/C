//
//  main.c
//  TwoFloats
//
//  Created by Joel Santiago on 6/16/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    float num1, num2;
    double num3;
    
    printf("Please enter the value for num1: ");
    scanf("%f",&num1);
    printf("Please enter the value for num2: ");
    scanf("%f",&num2);
    
    num3 = num1 + num2;
    
    printf("The sum of num1 and num2 comes to %.1f", num3);
}