//
//  main.c
//  Token 2
//
//  Created by Joel Santiago on 7/7/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void printTokens();

int main(int argc, char *argv[]) {
    
    // File identifier that holds the passed file
	FILE *file;
    
    // Char array of the line receiving the input read from the newly opened file
    char line[1024];
	
    int i = 0;
	int tokenCount = 0;
    
    // Counter variables for each token type
    int countK = 0,
    countI = 0,
    countStr = 0,
    countC = 0,
    countS = 0,
    countO = 0;
    
    // Char arrays holding the tokens for each token type
    char tokens[100][50];
    
    char keywords[100][50];
    char identifiers[100][50];
    char constants[100][50];
    char strings[100][50];
    char specials[100][50];
    char operators[100][50];
    
    // Array holding strings for each token type
    char *type[] = {"Keywords", "Identifiers", "Constants", "Strings", "Specials", "Operators"};
    
    // Print error if no command line argument was passed
	if (argc < 2) {
		printf("Error: file name argument not given\n");
		exit(1);
	}
    
    // Open file passed through commmand line argument argv
	file = fopen(argv[1], "r");
    
    // If file returned false, print error message, else print success message
	if (!file) {
		printf("File %s could not be opened\n", argv[1]);
		exit(1);
	} else {
		printf("File %s has been opened\n", argv[1]);
	}
    
    // Iterate through file line by line, tokenizes the line, sorts the tokens into category arrays and moves to
    // the next line until null pointer is reached
    while (fgets(line, sizeof line, file)) {
        
        // Searches for newline character at the end of each line
        char *nlptr = strchr(line, '\n');
        
        // If nlptr is true and newline character exists, it sets the value to a 0 bit
        if (nlptr) {
            *nlptr = '\0';
        }
        
        // Tokenize string with a blank space as a delimiter
        char *tokenPtr = strtok(line, " ");
        
        // While tokenPtr is true and not null, perform contents
        while (tokenPtr) {
            
            // Copy token from tokenPtr to tokens array at element tokenCount
            strcpy(tokens[tokenCount], tokenPtr);
            
            // Move to the next token
            tokenPtr = strtok(NULL, " ");
            
            // Increment token count
            tokenCount++;
            
            //  Searches token array for various types of tokens and separates them into their specified array accordingly
            //  and increments counter of total number of tokens within each category
            
            // Keyword check
            if (((strcmp(tokens[i], "if")) == 0) ||
                ((strcmp(tokens[i], "else")) == 0) ||
                ((strcmp(tokens[i], "end")) == 0) ||
                ((strcmp(tokens[i], "int")) == 0)) {
                strcpy(keywords[countK], tokens[i]);
                countK++;
            }
            
            // Identifier check
            else if (((strcmp(tokens[i], "#include")) == 0) ||
                     ((strcmp(tokens[i], "<stdio.h>")) == 0) ||
                     ((strcmp(tokens[i], "main")) == 0) ||
                     ((strcmp(tokens[i], "printf")) == 0) ||
                     ((strcmp(tokens[i], "i")) == 0)) {
                strcpy(identifiers[countI], tokens[i]);
                countI++;
            }
            
            // Constants check
            else if (((strcmp(tokens[i], "0")) == 0) ||
                     ((strcmp(tokens[i], "1")) == 0) ||
                     ((strcmp(tokens[i], "2")) == 0) ||
                     ((strcmp(tokens[i], "3")) == 0) ||
                     ((strcmp(tokens[i], "3")) == 0) ||
                     ((strcmp(tokens[i], "4")) == 0) ||
                     ((strcmp(tokens[i], "5")) == 0) ||
                     ((strcmp(tokens[i], "6")) == 0) ||
                     ((strcmp(tokens[i], "7")) == 0) ||
                     ((strcmp(tokens[i], "8")) == 0) ||
                     ((strcmp(tokens[i], "9")) == 0)) {
                strcpy(constants[countC], tokens[i]);
                countC++;
            }
            
            // Specials check
            else if (((strcmp(tokens[i], "(")) == 0) ||
                     ((strcmp(tokens[i], ")")) == 0) ||
                     ((strcmp(tokens[i], "{")) == 0) ||
                     ((strcmp(tokens[i], "}")) == 0) ||
                     ((strcmp(tokens[i], ";")) == 0) ||
                     ((strcmp(tokens[i], "\"")) == 0)) {
                strcpy(specials[countS], tokens[i]);
                countS++;
            }
            
            // Operators check
            else if (((strcmp(tokens[i], "+")) == 0) ||
                     ((strcmp(tokens[i], "-")) == 0) ||
                     ((strcmp(tokens[i], "*")) == 0) ||
                     ((strcmp(tokens[i], "/")) == 0) ||
                     ((strcmp(tokens[i], "=")) == 0) ||
                     ((strcmp(tokens[i], "<")) == 0) ||
                     ((strcmp(tokens[i], ">")) == 0)) {
                strcpy(operators[countO], tokens[i]);
                countO++;
            }
            
            // String check
            else {
                strcpy(strings[countStr], tokens[i]);
                countStr++;
            }
            i++;
        }
    }
    
    // Close the file opened by program
    fclose(file);
    
	printf("\n");
    
	printf("There are a total of %d tokens in the file %s\n", tokenCount, argv[1]);
    
    printf("\n");
    
    // printTokens function call to print file tokens
    printTokens(countK, keywords, type[0]);
    printTokens(countI, identifiers, type[1]);
    printTokens(countC, constants, type[2]);
    printTokens(countStr, strings, type[3]);
    printTokens(countS, specials, type[4]);
    printTokens(countO, operators, type[5]);
    
}

/*  void printTokens()
 *   function:   takes the various arrays containing now separated tokens, checks for
 *               duplicates, counts how many times each token appears, and prints the results
 *
 *   parameters: int counter - variable for the total tokens in each token category
 *               char tokArray[100][50] - array of tokens in each token category
 *               char *name - token category string
 */
void printTokens(int counter, char tokArray[100][50], char *name) {
    
    int i,j;
    
    // Placeholder character that replaces duplicate tokens, signifies unused array element
    char *blank = "`";
    
    // Token counter array that holds total number of tokens for each individual token within category
    int array[counter];
    
    for (i = 0; i < counter; i++) {
        
        // Token counter variables increments when a duplicate is found
        int tokenCtr = 0;
        
        for (j = 0; j < counter; j++) {
            
            // Checks for duplicate tokens in passed array
            if ((strcmp(tokArray[i], tokArray[j])) == 0) {
                
                // Increments token counter variable and assigns it to array holding all token amounts in specified category
                array[i] = ++tokenCtr;
            }
        }
    }
    
    for (i = 0; i < counter; i++) {
        
        // j = i + 1 so that when replacing duplicates, the initial match is left untouched for printing later
        for (j = i + 1; j < counter; j++) {
            
            // Checks for duplicate tokens in passed array
            if ((strcmp(tokArray[i], tokArray[j])) == 0) {
                
                // Replaces duplicate token with placeholder variable that signifies unused array element
                strcpy(tokArray[j], blank);
            }
        }
    }
    
    // Checks to see if the passed name is equal to string to determine what line to print (This is really just a formatting issue that bothered me)
    if ((strcmp(name, "Strings")) != 0) {
        printf("%s [%d]\t-> ", name, counter);
    } else {
        printf("%s [%d]\t\t-> ", name, counter);
    }
    
    // Prints through tokens within each category
    for (i = 0; i < counter; i++) {
        
        // Only prints line if token array element is not equal to placeholder blank variables
        if ((strcmp(tokArray[i], blank)) != 0) {
            printf("%s [%d] | ", tokArray[i], array[i]);
        }
    }
    
    printf("\n");
}
