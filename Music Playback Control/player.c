#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

typedef enum {STANDBY,PLAY,PAUSE}mpc_state;

struct mpc {
	pthread_mutex_t mpc_mutex; // thread lock
	pthread_cond_t mpc_cond; // thread condition
	pthread_t mpc_tid; // thread id
	mpc_state mpc_state; // current state value of the thread
};

// Functions that set up the control menu and perform actions */
static void player_init(struct mpc *control);
static void player_menu(void);
static void player_load(struct mpc *control);
static int player_play(struct mpc *control);
static int player_rewind(struct mpc *control);
static int player_seek(struct mpc *control);
static int player_rate(struct mpc *control);
void* start_pthread(void *control);

// Global variables
FILE *file;				// File chosen
char buf[256]; 			// string entered by user
char *array[50];		// Array the string is written to and used for play back
int seektime; 			// Seek time the user sets the player to
int rate = 1000000; 	// Default seek time is set to 1
int count;				// Total line count of the file being loaded
int i = 0;				// global i counter for array iteration
int c;

int main() {
	struct mpc player;	// defines an MPC struct named player
	player_init(&player);	// pass the player struct to the initialization function
	player_menu();			// display the menu options

	while(1) {			
		char input = getchar();
		switch(input) {	
			case 'l':						// if user types L or l
				player_load(&player);			// pass player struct to loadfile function
				break;
			case ' ':						// if user types a blank space ' '				
				player_play(&player);			// pass player struct to start function
				break;
			case 'r':						// if user types R or r
				player_rewind(&player);		// pass player struct to rewind function
				break;
			case 's':						// if user types S or s
				player_seek(&player);			// pass player struct to seek function
				break;		
			case 't':						// if user types T or t			
				player_rate(&player);			// pass player to set rate
				break;		
		}
	}
}

// Player struct initializer function
static void player_init(struct mpc *control) {
	control->mpc_state = STANDBY;
	control->mpc_tid;
	pthread_cond_init(&control->mpc_cond,NULL);
	pthread_mutex_init(&control->mpc_mutex,NULL);
}

// Menu for user to choose from
static void player_menu(void) {
	printf("Load File: l\n");
	printf("Play / Pause: ' '\n");
	printf("Rewind: r\n");
	printf("Seek To: s\n");
	printf("Set Rate: t\n");
}

// Allows user to select a file to play, gets line count, assigns it to an array, and calls menu to display
static void player_load(struct mpc *control) {
	control->mpc_state = STANDBY;
	printf("Enter the file to be played: ");
	char filename[256];
	char line[50];
	scanf("%s",&filename);
	file = fopen(filename, "r");

	// Loops through the file to get total size of array
	while (fgets(line, sizeof line, file) != NULL) {
		count++;		
	}
	// Sets file pointer back to the start
	rewind(file);
	
	// Looops through array and assigns each line to an array 
	while( i < count && fgets( buf, 1024, file ) != NULL )  { 
	    array[i] = malloc( strlen(buf) + 1 ); /*maybe you haven't allocated the strings*/
	    strcpy( array[i++], buf ); 
	}
	// Loops through the file to get total size of array
	rewind(file);
	
	printf("File %s has been loaded\n\n", filename);
	player_menu();
}

// Function to determine what the curent state of the player is, changes state depending on current player state
static int player_play(struct mpc *control) {
	if(control->mpc_state == STANDBY) {
		control->mpc_state = PLAY;
		return pthread_create(&control->mpc_tid,NULL,start_pthread,control);
	}
	if(control->mpc_state == PAUSE) {
		control->mpc_state = PLAY;
		return pthread_cond_signal(&control->mpc_cond);
	}
	if(control->mpc_state == PLAY) {
		control->mpc_state = PAUSE;
		return pthread_cond_signal(&control->mpc_cond);
	}
}

// Rewinds Selected File by setting i to 0
static int player_rewind(struct mpc *control) {
	printf("\nFile was rewound\n\n");
	i = 0;										// assigns i back to 0
	c = count-1;
	player_menu();								// Calls back the menu options
}

// Allows user to choose a seek time
static int player_seek(struct mpc *control) {
	printf("Enter the seek location: ");
	int seek;									// seek variable for user input of seek time
	scanf("%d",&seek);							// checks to see if user inputted new seek value
	i = seek - 1;								// assigns seek to i, subs 1 to account for array starting at 0
	c = seek - 1;
	printf("\nSeek Time: %d\n\n", seek);
	player_menu();								// calls back the menu options
}

// Allows user to choose a playback rate
static int player_rate(struct mpc *control) {
	printf("\nEnter the playback rate: ");
	float newrate;								// newrate variable for user input of playback rate
	float temprate = rate;						// assigns value of rate to temprate to use in rate calculation
	scanf("%f",&newrate);						// checks to see if user inputted new playback rate
	rate = (int) (temprate / newrate);			// assigns newrate to rate 
	printf("\nPlay Rate: %f\n\n", newrate);
	player_menu();								// calls back the menu options
}

typedef enum { false, true } bool;				// boolean values for paused, determines what the current state of paused is

// Player thrad function controls play and pause function
void* start_pthread(void* control)
{
	if (rate > 0)								// if rate is positive, i is assigned 0
		i = 0;	
	if (rate < 0)
		c = count-1;
	bool paused = false;						// sets paused to false							
	struct mpc* player_state = (struct mpc*) control;
	while (1) {
 		switch (player_state->mpc_state) {
			case PLAY:
 				if (rate > 0) {								// if rate is positive
					while (i < count) {						// while the current value of i is less than total line count
						if(!paused) {						// if player is not paused
							printf("%s", array[i]);
							usleep(rate);					// sleep for given amount of time
							i++;							// increments i
							if (!paused)					// if player has been paused, break
								break;
						} else {
							pthread_mutex_lock(&player_state->mpc_mutex);
							pthread_cond_wait(&player_state->mpc_cond,&player_state->mpc_mutex);
							pthread_mutex_unlock(&player_state->mpc_mutex);
							paused = false;
						}
					}
				}
				else if (rate < 0) {						// if rate of play is negative
					float currate = rate * -1;				// converts negative rate of play to positive for usleep
					while ( c >= 0) {					
						if(!paused) {						// if player is not paused
							printf("%s", array[c]);			
							usleep(currate);				// sleep for given amount of time
							c--;
							if (!paused)
								break;
						} else {
							pthread_mutex_lock(&player_state->mpc_mutex);
							pthread_cond_wait(&player_state->mpc_cond,&player_state->mpc_mutex);
							pthread_mutex_unlock(&player_state->mpc_mutex);
							paused = false;
						}
					}
				}
				break;
				
			case PAUSE:
				printf("\nPlayer paused.\n");
				pthread_mutex_lock(&player_state->mpc_mutex);
				pthread_cond_wait(&player_state->mpc_cond,&player_state->mpc_mutex);
				pthread_mutex_unlock(&player_state->mpc_mutex);
				break;
		}
	}
}