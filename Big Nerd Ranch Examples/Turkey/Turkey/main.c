//
//  main.c
//  Turkey
//
//  Created by Joel Santiago on 6/16/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[])
{
    // Declare the variable called 'weight' of type float
    float weight;
    
    // Put a number in that variable
    printf("Please enter the weight of the turkey: ");
    scanf("%f",&weight);
    
    //weight = 14.2;
    
    // Log it to the user
    printf("The turkey weighs %.1f.\n", weight);
    
    // Declare another variable of type float
    float cookingTime;
    
    // Calculate the cooking time and store it in the variable
    // In this case, '*' means 'multiplied by'
    cookingTime = 15.0 + 15.0 * weight;
    
    // Log that to the user
    printf("Cook it for %.1f minutes.\n", cookingTime);
    
    // End this function and indicate success
    
    return 0;
}

