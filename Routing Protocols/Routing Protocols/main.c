//
//  main.c
//  Routing Protocols
//
//  Created by Joel Santiago on 10/23/13.
//  Copyright (c) 2013 SofDev Inc. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    char new_dest_IP[16];               // Variable to hold converted and shortened destination address
    char new_addr_IP[16];               // Variable to hold converted and shortened interface address
    
    char next_hop[16];                  // Next hop value placeholder
    int router_count;                   // Counter for router location
    int dest_router;                    // Counter for destination router location
    
    char source_IP[16];                 // User entered source IP Address
    char dest_IP[16];                   // User entered destination IP Address
    char placeholder_IP[16];            // Placeholder variable for IP address conversion
    
    char temp_dest_IP[16];              // Temporary variable for destination IP Address conversion
    char *temp_dest;                    // Temporary placeholder for destination IP Address during conversion using subnet mask value
    char *temp_addr;                    // Temporary placeholder variable
    int count = 1;                      // Conversion loop counter
    int loop_count = 0;                 // Routing loop counter
    
    // Router interface array
    char *addresses[8][3] = {
        {"192.0.0.1", "192.0.1.1", ""},             // Router 1
        {"192.0.3.1", "192.0.4.1", ""},             // Router 2
        {"192.0.6.1", "192.0.3.2", ""},             // Router 3
        {"192.0.8.1", "192.0.6.2", ""},             // Router 4
        {"192.0.4.2", "192.0.1.2", ""},             // Router 5
        {"192.0.11.1", "192.0.0.2", "192.0.8.2"},   // Router 6
        {"192.0.11.2", "192.0.13.1", ""},           // Router 7
        {"192.0.13.2", "192.0.15.1", ""}            // Router 8
    };
    
    // Routing table array
    char *routing_table[8][13][2] = {
        {
            // Router 1
            { "192.0.4.0", "192.0.1.2" },
            { "192.0.10.0", "192.0.1.2" },
            { "192.0.3.0", "192.0.1.2" },
            { "192.0.5.0", "192.0.1.2" },
            { "192.0.6.0", "192.0.1.2" },
            { "192.0.7.0", "192.0.1.2" },
            { "192.0.8.0", "192.0.1.2" },
            { "192.0.9.0", "192.0.1.2" },
            { "192.0.11.0", "192.0.1.2" },
            { "192.0.12.0", "192.0.1.2" },
            { "192.0.13.0", "192.0.1.2" },
            { "192.0.14.0", "192.0.1.2" },
            { "192.0.15.0", "192.0.1.2" },
        },
        {
            // Router 2
            { "192.0.1.0", "192.0.4.2" },
            { "192.0.10.0", "192.0.4.2" },
            { "192.0.6.0", "192.0.3.2" },
            { "192.0.7.0", "192.0.3.2" },
            { "192.0.8.0", "192.0.3.2" },
            { "192.0.9.0", "192.0.3.2" },
            { "192.0.11.0", "192.0.3.2" },
            { "192.0.12.0", "192.0.3.2" },
            { "192.0.13.0", "192.0.3.2" },
            { "192.0.14.0", "192.0.3.2" },
            { "192.0.15.0", "192.0.3.2" },
            { "192.0.2.0", "192.0.4.2" },
            { "", "" }
        },
        {
            // Router 3
            { "192.0.4.0", "192.0.3.1" },
            { "192.0.5.0", "192.0.3.1" },
            { "192.0.8.0", "192.0.6.2" },
            { "192.0.9.0", "192.0.6.2" },
            { "192.0.11.0", "192.0.6.2" },
            { "192.0.12.0", "192.0.6.2" },
            { "192.0.1.0", "192.0.3.1" },
            { "192.0.10.0", "192.0.3.1" },
            { "192.0.13.0", "192.0.6.2" },
            { "192.0.14.0", "192.0.6.2" },
            { "192.0.15.0", "192.0.6.2" },
            { "192.0.2.0", "192.0.3.1" },
            { "", "" }
        },
        {
            // Router 4
            { "192.0.3.0", "192.0.6.1" },
            { "192.0.7.0", "192.0.6.1" },
            { "192.0.11.0", "192.0.8.2" },
            { "192.0.12.0", "192.0.8.2" },
            { "192.0.13.0", "192.0.8.2" },
            { "192.0.14.0", "192.0.8.2" },
            { "192.0.15.0", "192.0.8.2" },
            { "192.0.4.0", "192.0.6.1" },
            { "192.0.5.0", "192.0.6.1" },
            { "192.0.10.0", "192.0.6.1" },
            { "192.0.1.0", "192.0.6.1" },
            { "192.0.2.0", "192.0.6.1" },
            { "", "" }
        },
        {
            // Router 5
            { "192.0.2.0", "192.0.1.1" },
            { "192.0.3.0", "192.0.4.1" },
            { "192.0.5.0", "192.0.4.1" },
            { "192.0.6.0", "192.0.4.1" },
            { "192.0.7.0", "192.0.4.1" },
            { "192.0.8.0", "192.0.4.1" },
            { "192.0.9.0", "192.0.4.1" },
            { "192.0.11.0", "192.0.4.1" },
            { "192.0.12.0.", "192.0.4.1" },
            { "192.0.13.0", "192.0.4.1" },
            { "192.0.14.0", "192.0.4.1" },
            { "192.0.15.0", "192.0.4.1" },
            { "", "" }
        },
        {
            // Router 6
            { "192.0.13.0", "192.0.11.2" },
            { "192.0.14.0", "192.0.11.2" },
            { "192.0.6.0", "192.0.8.1" },
            { "192.0.9.0", "192.0.8.1" },
            { "192.0.3.0", "192.0.8.1" },
            { "192.0.7.0", "192.0.8.1" },
            { "192.0.15.0", "192.0.11.2" },
            { "192.0.5.0", "192.0.8.1" },
            { "192.0.4.0", "192.0.8.1" },
            { "192.0.10.0", "192.0.8.1" },
            { "192.0.1.0", "192.0.8.1" },
            { "192.0.2.0", "192.0.8.1" },
            { "", "" }
        },
        {
            // Router 7
            { "192.0.15.0", "192.0.13.2" },
            { "192.0.8.0", "192.0.11.1" },
            { "192.0.12.0", "192.0.11.1" },
            { "192.0.3.0", "192.0.11.1" },
            { "192.0.6.0", "192.0.11.1" },
            { "192.0.7.0", "192.0.11.1" },
            { "192.0.9.0", "192.0.11.1" },
            { "192.0.5.0", "192.0.11.1" },
            { "192.0.4.0", "192.0.11.1" },
            { "192.0.10.0", "192.0.11.1" },
            { "192.0.1.0", "192.0.11.1" },
            { "192.0.2.0", "192.0.11.1" },
            { "", "" }
        },
        {
            // Router 8
            { "192.0.11.0", "192.0.13.1" },
            { "192.0.14.0", "192.0.13.1" },
            { "192.0.8.0", "192.0.13.1" },
            { "192.0.12.0", "192.0.13.1" },
            { "192.0.3.0", "192.0.13.1" },
            { "192.0.6.0", "192.0.13.1" },
            { "192.0.7.0", "192.0.13.1" },
            { "192.0.9.0", "192.0.13.1" },
            { "192.0.5.0", "192.0.13.1" },
            { "192.0.4.0", "192.0.13.1" },
            { "192.0.10.0", "192.0.13.1" },
            { "192.0.1.0", "192.0.13.1" },
            { "192.0.2.0", "192.0.13.1" }
        }
    };
    
    // Information retrieval
    printf("Enter a Source IP Address\n");
    scanf("%s", source_IP);
    printf("\n");
    
    printf("Enter a Destination IP Address\n");
    scanf("%s", dest_IP);
    printf("\n");
    
    // Reassign destination IP address value to another expendable value before tokenization
    strcpy(temp_dest_IP, dest_IP);
    temp_dest = strtok(temp_dest_IP, ".");
    strcpy(new_dest_IP, temp_dest);
    
    // Converts destination address into a new shortened IP address using the subnet mask of 255.255.255.0
    while (temp_dest != NULL) {
        
        // Concatenate ".0" to end of destination IP Address based on if subnet mask value has been reached
        if (count == 3) {
            strcat(new_dest_IP, ".0");
            break;
        }
        count++;
        
        // Concatenate a period delimiter between values pulled from original destination address
        strcat(new_dest_IP, ".");
        
        // Move up one token in the destination IP Address
        temp_dest = strtok(NULL, ".");
        
        if (temp_dest == NULL) {
            break;
        }
        
        // Concatenate newly pulled token from original destination IP Address to new destination IP variable
        strcat(new_dest_IP, temp_dest);
    }
    
    // Loop through addresses to get source and destination router numbers
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
            
            // Source router
            if ((strcmp(source_IP, addresses[i][j])) == 0) {
                
                // If equal, remember router number for next pass
                printf("Router %d ", i + 1);
                printf("(%s) => ", addresses[i][j]);
                router_count = i;
            }
            
            // Destination router
            if ((strcmp(dest_IP, addresses[i][j])) == 0) {
                dest_router = i;
            }
        }
    }
    
    // Loop through all routers - if applicable based on the network setup
    while (loop_count < 8) {
        
        bool not_found = true;
        
        // Loop through routing table array
        for (int j = 0; j < 13; j++) {
            
            // Compare new destination IP address to the values within routing table
            if ((strcmp(new_dest_IP, routing_table[router_count][j][0])) == 0) {
                
                // If equal, copy next hop value stored in routing table to next hop variable
                strcpy(next_hop, routing_table[router_count][j][1]);
                not_found = false;
            }
        }
        
        // Loop through router addresses
        for (int i = 0; i < 8; i++) {
            for (int k = 0; k < 3; k++) {
                
                // Compare next hop address to router addresses looking for a connected network
                if ((strcmp(next_hop, addresses[i][k])) == 0) {
                    
                    // If equal, remember router number for next pass
                    printf("Router %d ", i + 1);
                    printf("(%s) => ", addresses[i][k]);
                    router_count = i;
                }
                
                // IP Address conversion using subnet mask on the final routers direct connections
                if (not_found == true && k < 2) {
                    
                    // Transfer router address to an expendable variable
                    strcpy(placeholder_IP, addresses[router_count][k]);
                    
                    // Tokenize address
                    temp_addr = strtok(placeholder_IP, ".");
                    strcpy(new_addr_IP, temp_addr);
                    
                    // Address counter to know when 4th value in IP address is reached
                    int addr_count = 1;
                    
                    // Tokenizer loop
                    while (temp_addr != NULL) {
                        
                        // Concatenates a ".0" on the end of the IP address based on subnet mask
                        if (addr_count == 3) {
                            strcat(new_addr_IP, ".0");
                            break;
                        }
                        addr_count++;
                        
                        // Concatenate a period between values
                        strcat(new_addr_IP, ".");
                        
                        // Move forward one token
                        temp_addr = strtok(NULL, ".");
                        
                        if (temp_addr == NULL) {
                            break;
                        }
                        
                        // Concatenate temp address token onto new address variable
                        strcat(new_addr_IP, temp_addr);
                    }
                    
                    // Compare new destination address to router addresses looking for a connection on the same network to find final router
                    if ((strcmp(new_dest_IP, new_addr_IP)) == 0) {
                        printf("Router %d (%s) Success!\n", dest_router + 1, dest_IP);
                        not_found = false;
                        return 0;
                    }
                }
            }
        }
        loop_count++;
    }
}