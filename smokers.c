#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

void agent ();
void smoker (int * id);
void putdown (int type);
void pickup (int smoker_type);

pthread_t smoker_t[3];		
pthread_t agent_t;			
pthread_mutex_t table;	
pthread_cond_t self;

int a=1;		// agent counter
int t=0;		// tobacco counter for what's on the table
int p=0;		// paper counter for what's on the table
int m=0;		// matches counter for what's on the table
 
int main () {
	int i;
	int pid[3];
	int * o;

  // Synchronizes the threads and also performs an error check
	if (pthread_mutex_init(&table, NULL)) {	
  	perror ("pthread_mutex_init");
  	exit (1);
  }

	pthread_cond_init(&self, NULL); 						

  // Creates the agent thread
	if (pthread_create(&(agent_t), NULL, (void *)agent, NULL ) != 0) {						
  	perror ("pthread_create");
   	exit (1);
  }

  // Allocate memory for the 3 smokers
	for (i = 0; i < 3; i++) {																				
  	if ((o = (int *)malloc(sizeof(int))) == NULL) {	
    	perror ("malloc");
    	exit (1);
    }

  	*o = i;

    // Creates the smoker threads
  	if (pthread_create(&(smoker_t[i]), NULL, (void *)smoker, o) != 0) {					
    	perror ("pthread_create");
    	exit (1);
    }
	}

  // Destruction of the agent thread
	pthread_join (agent_t, NULL);														

  printf ("Agent thread has been terminated\n\n");

  for (i = 0; i < 3; i++) {		

    // Destruction of smoker threads																		
   	pthread_kill (smoker_t[i], 0);																	
   	printf ("Smoker %d has been terminated\n", i);
	}

  exit (0);																									
}

// Smoker picks up ingredients from the table
void smoker (int * id) {																					
	printf ("Smoker %d has been created\n", *id);
  	while (1) {																	
  		pickup (*id);																							
  		printf ("Smoker %d has stopped smoking\n", *id);

      // Sleep function call the put current smoker thread to sleep temporarily
  		sleep (1);																								
  	}
  	exit(0);
}

// Agent creates ingredients on table
void agent() {																									
  	srand(time(NULL));																					
  	int random;																									

    // Counts through the amount of times the agent puts ingredients
  	int x = 0;																									
  	printf ("Agent has been created\n\n");
  
    // Puts down ingredients on the table
  	while (x < 30) {				

      // Random value determines what is put on table
     	random = (rand() % 3);																				
    	putdown (random);																					

      // Agent thread sleeps while smokers to pick up ingredient	
    	sleep (1); 																								
    	x++;																										
  	}
  	printf ("Agent has finished\n");
  	return;
}

// Agent puts down ingredients on table
void putdown (int type) {																					

    // Smokers locked out while the agent puts ingredients on the table
  	pthread_mutex_lock(&table);																			
  	while (a != 1) {

      // Wait conditional statement for the agent that holds it from putting ingredients
   	  pthread_cond_wait (&self, &table);																
  	}

    // Switch case to determine what gets put down on table by agent
  	switch (type) {																							
		case 0: 

      // Tobacco table counter
			t=1;																									
			break;

		case 1:

      // Matches table counter
			m=1;																									
			break;

		case 2:

      // Paper table counter
			p=1;																									
			break;      
  	}

  	pthread_cond_broadcast (&self);

    // Smokers allowed access after the agent has put the ingredients
  	pthread_mutex_unlock (&table);  																		
}

// Smokers pick up the ingredients from the table
void pickup (int smoker_type) {																			

    // Locks the table from the agent while the smokers pick up ingredients from the table
  	pthread_mutex_lock (&table); 																			

    // Switch case to determine which smoker picks up the ingredients from the table
  	switch (smoker_type) {																					
		case 0:

      // Wait conditional statement for the first smoker
			while (t != 1) {
				pthread_cond_wait (&self, &table);														
			}

			printf ("Smoker 0 (TOBACCO) has started smoking\n");

      // Tobacco ingredient counter
			t=0;																									

      // Agent counter
			a=1;																									
			break;

		case 1:

      // Wait conditional statement for the second smoker
			while (m != 1) {																					
				pthread_cond_wait (&self, &table);
			}

      printf ("Smoker 1 (MATCHES) has started smoking\n");

      // Matches ingredient counter
			m=0;																									

      // Agent counter
			a=1;																									
			
			break;
		
    case 2:

      // Wait conditional statement for the third smoker
			while (p != 1) {																					
				pthread_cond_wait (&self, &table);
			}

      printf ("Smoker 2 (PAPER) has started smoking\n");

      // Paper ingredient counter
			p=0;																									

      // Agent counter
			a=1;																									
			break;
  	}

  	pthread_cond_broadcast (&self);

    // Unlocks the table for the agent to put down new ingredients
  	pthread_mutex_unlock (&table);																		
}
