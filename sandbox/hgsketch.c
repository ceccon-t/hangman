#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define MAX_STRING 20
#define MAX_HINT 75
#define TOTAL_TRIES 5
#define QUIT_SIGNAL '0'

#define DATAFILENAME "words.dat"
#define NUM_WORDS_DEFAULT_FILE 5


/*
=======================
// Gameplay
TODO: implement verification of winning the game
V

TODO: implement a manual quit mechanism
V

TODO: decrement tries_left only when no matches were found for the guess
V
=======================

=======================
// Validation
TODO: force only one character choice each time

TODO: force only unused characters

TODO: display used characters

=======================

=======================
// Data structure
TODO: Word structure
V

TODO: file storage / bin data
V

TODO: random access to bin data
V
- TODO: find number of entries on the file (to determine a random position)
V
=======================

=======================
// Interface
TODO: display tries_left
V

TODO: check clear screen
V

TODO: define the visual presentation of the game

TODO: implement the 'drawings'
=======================
*/

typedef struct word {
	char string[MAX_STRING];
	char hint1[MAX_HINT];
	char hint2[MAX_HINT];
	char hint3[MAX_HINT];
} WORD;


WORD choose_word();
void display_match(WORD chosen_word, int *mask, int tries_left);
void reset_mask(int *mask, int val);
int check_win(int *mask, int i_last_char);
int end_of_string(char *string); // TODO: remove eventually

int main () {
    //char string[MAX_STRING] = "banana";
    int mask[MAX_STRING] = {0};
    char guess;
    int i, i_last_char, tries_left, won = 0, play = 1, found;
    WORD chosen_word;
    
    srand(time(0));
    
    //printf("%d\n", end_of_string(string));
    //exit(1);
    
    
    while (play) {
    	won = 0;
    	tries_left = TOTAL_TRIES;
    	reset_mask(mask, 0);
    	chosen_word = choose_word();
    	i_last_char = strlen(chosen_word.string) - 1;
    	
    	while(tries_left > 0 && !won) {
			
			display_match(chosen_word, mask, tries_left);
		
			printf("\n");
			printf("You have %d guesses left.\n", tries_left);
			printf("Make a guess (enter %c to quit): ", QUIT_SIGNAL);
			scanf(" %c", &guess);
			
			if (guess == QUIT_SIGNAL) {
				tries_left = 0;
			} else {
		
				found = 0;
				for (i=0; i<strlen(chosen_word.string); i++) {
					if (chosen_word.string[i] == guess) {
						mask[i] = 1;
						won = check_win(mask, i_last_char);
						found = 1;
					}
				}
				if (!found) {
					tries_left--;
				}
			
			}
			
    	}
    	
    	reset_mask(mask, 1);
    	
    	display_match(chosen_word, mask, tries_left);
    	
    	if (won) {
    		printf("\n\nYou've won!\n");
    	} else {
    		printf("\n\nYou've lost.\n");
    	}
    	
    	printf("Enter 1 to play again, 0 to end the game: ");
    	scanf(" %d", &play);
    	
    }
    
    
    printf("\nEND\n");
    return 0;
    
}


WORD choose_word() {
	WORD chosen;
	FILE * datafile;
	int entries_on_file, chosen_pos, filesize; // [0,NUM_OF_ENTRIES_ON_FILE]
	
	//strcpy(chosen.string, "banana");
	//strcpy(chosen.hint1, "It's a fruit");
	//strcpy(chosen.hint2, "Associated with the tropics");
	//strcpy(chosen.hint3, "It's yellow");
	
	datafile = fopen(DATAFILENAME, "rb");
	
	fseek(datafile, 0L, SEEK_END);
	filesize = ftell(datafile);
	
	//fread(&chosen, sizeof(WORD), 1, datafile);
	//entries_on_file = 4;
	entries_on_file = filesize / sizeof(WORD);
	//printf("======%d======", entries_on_file);
	//chosen_pos = 3;
	chosen_pos = (int) (rand() / (double) RAND_MAX * entries_on_file);
	fseek(datafile, chosen_pos * sizeof(WORD), SEEK_SET); 
	fread(&chosen, sizeof(WORD), 1, datafile);
	
	fclose(datafile);
	
	
	return chosen;
}

void display_match(WORD chosen_word, int *mask, int tries_left) { 
	int i;
	
	//system("clear||cls"); 
	// cls: Windows command to clear prompt; printf "\033c": same for bash
	system("cls||printf \"\\033c\""); // TODO: find better solution than calling the system
	printf("\n---------\n");
	for (i=0; i<strlen(chosen_word.string); i++) {
		if (mask[i]) {
			printf("%c", chosen_word.string[i]);
		} else {
		    printf("_");
		}
	}
	
	printf("\nHint 1: %s\n", chosen_word.hint1);
	printf("Hint 2: %s\n", (tries_left < 4) ? chosen_word.hint2 : "XXXXXXXX" );
	printf("Hint 3: %s\n", (tries_left < 2) ? chosen_word.hint3 : "XXXXXXXX" );
	
}

void reset_mask(int *mask, int val) {
	int i;
	
	for (i=0; i < MAX_STRING; i++) {
		mask[i] = val;
	}
	
}

int check_win(int *mask, int i_last_char) {
	int i;
	
	for (i = 0; i < i_last_char; i++) {
		if(mask[i] == 0) return 0; // TODO: braces, please
	}
	
	return 1;
}


// Pure debug
int end_of_string(char *string) {
	int i;
	
	return (int)strlen(string);
}


