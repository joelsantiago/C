//
//  main.c
//  Execute
//
//  Created by Joel Santiago on 3/8/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Search function that takes line and searches for commands entered, then returns commands back to main function to be executed
void search(char *line, char **argv)
{
    // While loop that exists while char pointer line does not equal '\0' (end of line)
    while (*line != '\0')
    {
        // While loop that exists while char pointer line is either blank, contains a tab or contains a new line
        while (*line == ' ' || *line == '\t' || *line == '\n')
        {
            // Assigns a terminator 0 after each command and then increments line to next element
            *line++ ='\0';
        }
        // Assignes current contents of line to char array pointer argv then increments argv to next element
        *argv++ = line;
        
        // While loop that exists while char array pointer line does not equal '\0', ' ', '\t' and '\n'; line must contain character values
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
        {
            // Moves to next element in line array
            line++;
        }
    }
    // Once completed, adds 0 terminator to char array pointer argv and returns argv back to main for execution
    *argv = '\0';
}

// Execute function that takes commands in char array pointer to a pointer argv as an argument
void execute(char **argv)
{
    // Signed integer pid representing a process id
    pid_t pid;
    
    // Status value for child process
    int status;
    
    if ((strcmp(argv[0], "cd")) == 0)
    {
        chdir(argv[1]);
        system("pwd");
    }
    
    // pid creates a child process, enters if as long as pid < 0
    if ((pid = fork()) < 0)
    {
        // Error prompt
        printf("** ERROR: forking child process failed **\n");
        // Exits program
        exit(1);
    }
    // After forking, enters only if pid == 0
    else if (pid == 0)
    {
        // Function executes commands entered
        // Takes *argv as name of file to be executed
        // Takes argv as commands entered that will be executed
        // If execute fails, it returns a negative value, and enters if as long as execvp < 0
        if (execvp(*argv, argv) < 0)
        {
            // Error prompt
            printf("** ERROR: execution failed **\n");
            // Exits program
            exit(1);
        }
    }
    else
        // Tells parent process to wait for completion of child process
        while (wait(&status) != pid);
}

// Main Function
int main()
{
    
    // Character Array for the command the user inputs
    char line[256];
    
    // Character pointer argv that is the return variable of the search function
    // Holds an array of char pointers pointing to separate character strings, these are the commands entered
    char *argv[16];
    
    // While loop that exists while true
    while (1)
    {
        // Prompt output
        printf("Computer:~ user$ ");
        
        // Takes in command line entered by user as char array line
        //gets(line);			// Only way i could get this work, can cause buffer overflow
        fgets(line, 256, stdin);	// Could not get to work
        
        // Adds next line to end of command string so code knows where end of line is
        printf("\n");
        
        // Function call to search through char array line and determine what commands were entered
        // Takes char array line and argv as arguments
        // Returns argv
        search(line,argv);
        
        
        // If argv is returned with no values in array and is equal to string "exit"
        if (strcmp(argv[0], "exit") == 0)
        {
            // exits program
            exit(0);
        }
        // Function call to execute commands entered with argv as argument
        execute(argv);
    }
}
