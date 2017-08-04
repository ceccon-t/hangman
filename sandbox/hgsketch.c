#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_STRING 20
#define MAX_HINT 75
#define TOTAL_TRIES 5
#define QUIT_SIGNAL '0'

/*
TODO: implement verification of winning the game
V

TODO: display tries_left
V

TODO: implement a manual quit mechanism
V

TODO: force only one character choice each time

TODO: Word structure
V

TODO: file storage / bin data

TODO: random access to bin data
*/

typedef struct word {
	char string[MAX_STRING];
	char hint1[MAX_HINT];
	char hint2[MAX_HINT];
	char hint3[MAX_HINT];
} WORD;


WORD choose_word();
void display_match(WORD chosen_word, int *mask, int tries_left);
void reset_mask(int *mask);
int check_win(int *mask, int i_last_char);
int end_of_string(char *string); // TODO: remove eventually

int main () {
    //char string[MAX_STRING] = "banana";
    int mask[MAX_STRING] = {0};
    char guess;
    int i, i_last_char, tries_left, won = 0, play = 1;
    WORD chosen_word;
    
    //printf("%d\n", end_of_string(string));
    //exit(1);
    
    chosen_word = choose_word();
    
    while (play) {
    	won = 0;
    	tries_left = TOTAL_TRIES;
    	reset_mask(mask);
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
		
				for (i=0; i<strlen(chosen_word.string); i++) {
					if (chosen_word.string[i] == guess) {
						mask[i] = 1;
						won = check_win(mask, i_last_char);
					}
				}
				tries_left--;
			
			}
			
    	}
    	
    	display_match(chosen_word, mask, tries_left);
    	
    	if (won) {
    		printf("\n\nYou've won!\n");
    	} else {
    		printf("\n\nYou've lost.\n");
    	}
    	
    	printf("Enter 1 to play again, 0 to end the game: ");
    	scanf(" %d", &play);
    	
    }
    
    
    printf("\nFIM\n");
    return 0;
    
}


WORD choose_word() {
	WORD chosen;
	
	strcpy(chosen.string, "banana");
	strcpy(chosen.hint1, "It's a fruit");
	strcpy(chosen.hint2, "Associated with the tropics");
	strcpy(chosen.hint3, "It's yellow");
	
	return chosen;
}

void display_match(WORD chosen_word, int *mask, int tries_left) { 
	int i;
	
	system("clear"); // TODO: find better solution than calling the system
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

void reset_mask(int *mask) {
	int i;
	
	for (i=0; i < MAX_STRING; i++) {
		mask[i] = 0;
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


