//
//  main.c
//  Process ID
//
//  Created by Joel on 2/26/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    uid_t uid, euid;
    
    uid = getuid();
    euid = geteuid();
    
    seteuid(100);
    
    pid_t pid;
    pid = fork();
    
    if (pid != 0) {
        printf("This is the parent process. My pid is %d and my parent's id is %d. My UID is %d and my EUID is %d.\n\n", getpid(), getppid(), getuid(), geteuid());        
    }
    else {        
        printf("This is the child process. My pid is %d and my parent's id is %d. My UID is %d and my EUID is %d.\n\n", getpid(), getppid(), getuid(), geteuid());
    }

    if (uid == euid) {
        printf("Effective UID and Real UID are the same\n\n");
    }
}

