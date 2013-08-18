//
//  main.c
//  File Transfer
//
//  Created by Joel Santiago on 2/19/13.
//  Copyright (c) 2013 Joel Santiago. All rights reserved.
//

#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char * argv[])
{
    int infile, outfile;
    char buffer;
    long int i = 0, fileSize = 0;
    
    infile = open(argv[1], O_RDONLY);
    outfile = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0755);
    
    while (read(infile,&buffer,1)>0)
        fileSize++;
    while (++i <= fileSize)
    {
        lseek(infile, -i, SEEK_END);
        read(infile, &buffer, 1);
        write(outfile, &buffer, 1);
    }
    close(infile);
    close(outfile);
}

