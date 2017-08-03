#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_STRING 20
#define TOTAL_TRIES 5

/*
TODO: implement verification of winning the game
V

TODO: display tries_left
V

TODO: implement a manual quit mechanism
V

TODO: Word structure

TODO: file storage / bin data

TODO: random access to bin data
*/

void display_match(char *string, int *mask);
void reset_mask(int *mask);
int end_of_string(char *string);
int check_win(int *mask, int i_last_char);

int main () {
    char string[MAX_STRING] = "banana";
    int mask[MAX_STRING] = {0};
    char guess;
    int i, i_last_char, tries_left, won = 0, play = 1;
    
    //printf("%d\n", end_of_string(string));
    //exit(1);
    
    while (play) {
    	won = 0;
    	tries_left = TOTAL_TRIES;
    	reset_mask(mask);
    	i_last_char = strlen(string) - 1;
    	
    	while(tries_left > 0 && !won) {
    		/*
    		system("clear"); // TODO: find better solution than calling the system
    		
    		printf("\n---------\n");
			for (i=0; i<strlen(string); i++) {
				if (mask[i]) {
					printf("%c", string[i]);
				} else {
				    printf("_");
				}
			}
			*/
			
			display_match(string, mask);
		
			printf("\n");
			printf("You have %d guesses left.\n", tries_left);
			printf("Make a guess (enter 0 to quit): ");
			scanf(" %c", &guess);
			
			if (guess == '0') {
				tries_left = 0;
			} else {
		
				for (i=0; i<strlen(string); i++) {
					if (string[i] == guess) {
						mask[i] = 1;
						won = check_win(mask, i_last_char);
					}
				}
				tries_left--;
			
			}
			
    	}
    	
    	display_match(string, mask);
    	
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

void display_match(char *string, int *mask) { 
	int i;
	
	system("clear"); // TODO: find better solution than calling the system
	printf("\n---------\n");
	for (i=0; i<strlen(string); i++) {
		if (mask[i]) {
			printf("%c", string[i]);
		} else {
		    printf("_");
		}
	}
	
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


