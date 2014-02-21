//
//  main.c
//  Heap
//
//  Created by Joel Santiago on 6/19/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float heightInMeters;
    int weightInKilos;
} Person;

float bodyMassIndex(Person *p) {
    return p->weightInKilos / (p->heightInMeters * p->heightInMeters);
}

int main(int argc, const char * argv[])
{
    float *startOfBuffer;
    
    startOfBuffer = malloc(1000 * sizeof(float));
    
    printf("%p\n", &startOfBuffer);
    printf("%ld\n", sizeof(startOfBuffer));
    
    free(startOfBuffer);
    
    startOfBuffer = NULL;
    
    printf("\n");
    
    // Allocate memory for one Person structure
    Person *x = (Person *)malloc(sizeof(Person));
    
    // Fill in two members of the structure
    x->weightInKilos = 81;
    x->heightInMeters = 2.0;
    
    // Print out the BMI of the original Person
    float xBMI = bodyMassIndex(x);
    printf("x has a BMI of %f\n", xBMI);
    
    // Let the memory be recycled
    free(x);
    
    // Forget where it was
    x = NULL;
    
    return 0;
}

