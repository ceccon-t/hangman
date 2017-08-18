/*
	HANGMAN - Version 1.0
	
	By Tiago S. Ceccon
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>

#define MAX_STRING 20
#define MAX_HINT 75
#define MAX_CHARS_USED 28
#define TOTAL_TRIES 5
#define QUIT_SIGNAL '0'

#define COVERED 0
#define UNCOVERED 1

#define DATAFILENAME "words.dat"
#define NUM_WORDS_DEFAULT_FILE 5


/*
=======================
Placeholder for future TODOs
=======================
*/

typedef struct word {
	char string[MAX_STRING];
	char hint1[MAX_HINT];
	char hint2[MAX_HINT];
	char hint3[MAX_HINT];
} WORD;


WORD choose_word();
void draw_gallows(int tries_left);
void clear_screen();
void display_match(WORD chosen_word, int *mask, int tries_left, char *chars_already_used, int won);
void display_chars_used(char *chars_already_used);
void reset_mask(int *mask, int val);
int check_win(int *mask, int i_last_char);
char get_user_input(char *chars_already_used);
int check_datafile();
int create_default_datafile(char * filename);

int main () {
	
	// Variables
    int mask[MAX_STRING] = {COVERED};
    char guess, chars_already_used[MAX_CHARS_USED];
    int i, i_last_char, tries_left, won, play, found;
    WORD chosen_word;
    
    srand(time(0));
    
    clear_screen();
    
    play = check_datafile();
    
    // Game loop
    while (play) {
    	won = 0;
    	tries_left = TOTAL_TRIES;
    	reset_mask(mask, COVERED);
    	chosen_word = choose_word();
    	i_last_char = strlen(chosen_word.string) - 1;
    	strcpy(chars_already_used, "");
    	
    	// Match loop
    	while(tries_left > 0 && !won) {
			
			display_match(chosen_word, mask, tries_left, chars_already_used, won);
		
			guess = get_user_input(chars_already_used);
				
			// Process input		
			if (guess == QUIT_SIGNAL) {
				tries_left = 0;
			} else {
				// Check if char guessed appears in the word
				found = 0;
				for (i=0; i<strlen(chosen_word.string); i++) {
					if (chosen_word.string[i] == guess) {
						mask[i] = UNCOVERED;
						won = check_win(mask, i_last_char);
						found = 1;
					}
				}
				if (!found) {
					tries_left--;
				}
			} // end process input
			
    	} // end match loop
    	
    	reset_mask(mask, UNCOVERED); // show entire word either winning or losing
    	
    	display_match(chosen_word, mask, tries_left, chars_already_used, won);
    	
    	if (won) {
    		printf("\n\nYou've won!\n");
    	} else {
    		printf("\n\nYou've lost.\n");
    	}
    	
    	printf("Enter 1 to play again, 0 to end the game: ");
    	scanf(" %d", &play);
    	
    } // end game loop
    
    clear_screen();
    return 0;
    
}

WORD choose_word() {
	WORD chosen;
	FILE * datafile;
	int entries_on_file, chosen_pos, filesize; // [0,NUM_OF_ENTRIES_ON_FILE]
	
	datafile = fopen(DATAFILENAME, "rb");
	
	fseek(datafile, 0L, SEEK_END);
	filesize = ftell(datafile);
	
	// Pick a random word by getting a position within the range 0 - entries_on_file
	entries_on_file = filesize / sizeof(WORD);
	//printf("======%d======", entries_on_file); // for debug
	chosen_pos = (int) (rand() / (double) RAND_MAX * entries_on_file);
	fseek(datafile, chosen_pos * sizeof(WORD), SEEK_SET); 
	fread(&chosen, sizeof(WORD), 1, datafile);
	
	fclose(datafile);
	
	return chosen;
}

void print_gallow_with_offset(char *part) {
	printf("\t\t%s\n", part);
}

void draw_gallows(int tries_left) {
	
	// Top of the gallows
	print_gallow_with_offset("_________");
	print_gallow_with_offset("|       |");
	
	// Head
	if (tries_left < 5) {
		if (tries_left == 0) {
			print_gallow_with_offset("|       0");
		} else {
			print_gallow_with_offset("|       O");
		}
	} else {
		print_gallow_with_offset("|");
	}
	
	// Arms
	if (tries_left < 4) {
		if (tries_left < 2) {
			print_gallow_with_offset("|      -|-");
		} else {
			print_gallow_with_offset("|       |");
		}
	} else {
		print_gallow_with_offset("|");
	}
	
	// Belly + legs
	print_gallow_with_offset((tries_left < 4) ? "|       |" : "|");
	print_gallow_with_offset((tries_left < 3) ? "|      / \\" : "|");
	
	// Bottom of the gallows
	print_gallow_with_offset("|");
	print_gallow_with_offset("|");
	print_gallow_with_offset("-----------------");
	
	printf("\n");
	
	
	/*
	For reference:
_________
|       |
|       O
|      -|-
|       |
|      / \
|
|
-----------------

	In the code:
TOP
_________
|       |
HEAD
|       O   / 0
ARMS
|      -|-
BELLY + LEGS
|       |
|      / \
BOTTOM
|
|
-----------------


	*/
}

void clear_screen() {
	
	// cls: Windows command to clear prompt; printf "\033c": same for bash
	system("printf \"\\033c\"||cls"); 

}

void display_match(WORD chosen_word, int *mask, int tries_left, char *chars_already_used, int won) { 
	int i;
	
	clear_screen();
	
	// Title
	printf("===============================================================================\n");
	printf("=================================== HANGMAN ===================================\n");
	printf("===============================================================================\n");
	
	// Gallows
	draw_gallows(tries_left);
	
	// Word
	printf("\t\t\t\t");
	for (i=0; i<strlen(chosen_word.string); i++) {
		if (mask[i] == UNCOVERED) {
			printf("%c ", chosen_word.string[i]);
		} else {
		    printf("_ ");
		}
	}
	
	// Hints
	printf("\n\nHint 1: %s\n", chosen_word.hint1);
	printf("Hint 2: %s\n", (tries_left < 4) ? chosen_word.hint2 : "XXXXXXXX" );
	printf("Hint 3: %s\n", (tries_left < 2) ? chosen_word.hint3 : "XXXXXXXX" );
	
	// Match state
	if (tries_left > 0 && !won) {
			printf("\n");
			printf("You have %d guess%s left.\n", tries_left, (tries_left == 1) ? "" : "es");
			printf("You have already guessed the following letters: ");
			display_chars_used(chars_already_used);
	}
	
}

void display_chars_used(char *chars_already_used) {
	int i, len;
	
	len = strlen(chars_already_used);
	
	for (i = 0; i < len; i++) {
		printf("%c%s", chars_already_used[i],  (i == (len - 1 ))? "" : ", ");
	}
	
	printf("\n");
	
}


void reset_mask(int *mask, int val) {
	int i;
	
	for (i=0; i < MAX_STRING; i++) {
		mask[i] = val;
	}
	
}

int check_win(int *mask, int i_last_char) {
	int i;
	
	for (i = 0; i <= i_last_char; i++) {
		if(mask[i] == COVERED) return 0; // still has covered chars
	}
	
	return 1; // won
}

char get_user_input(char *chars_already_used) {
	char valid_inputs[] = "abcdefghijklmnopqrstuvwxyz0";
	char cleansed_input;
	int len;
	
	fflush(stdin); // for windows
	
	// Get the input
	printf("\nGuess ONE letter (enter %c to quit): ", QUIT_SIGNAL);
	scanf(" %c", &cleansed_input);
	cleansed_input = tolower(cleansed_input);
	
	// Validate the input
	while ((strchr(valid_inputs, cleansed_input) == NULL) || (strchr(chars_already_used, cleansed_input) != NULL)) {
	
		printf("\nPlease enter only an UNUSED LETTER. Try again: ");
		scanf(" %c", &cleansed_input);
		cleansed_input = tolower(cleansed_input);
		
	}
	
	// To avoid accepting more than once the same char
	len = strlen(chars_already_used);
	chars_already_used[len] = cleansed_input;
	chars_already_used[len+1] = '\0';
	
	return cleansed_input;
	

}

int create_default_datafile(char * filename) {
	FILE * datafile;
	int i;
	WORD words[] = {
					{ 
					  "banana",
					  "It's a fruit",
					  "Associated with the tropics",
					  "It's yellow"
					},
					{ 
					  "hangman",
					  "A game",
					  "Usually played with pen and paper",
					  "Focused on words"
					},
					{ 
					  "argentina",
					  "A country",
					  "On the southern hemisphere",
					  "Its official language is Spanish"
					},
					{ 
					  "jazz",
					  "A music genre",
					  "Became very popular after the 1920s",
					  "American"
					},
					{ 
					  "dumbledore",
					  "A fictional character",
					  "First appeared in literature, but later also in film and games",
					  "From the Harry Potter universe"
					}
				 };
	
	if ((datafile = fopen(filename, "wb")) != NULL) {
		
		for (i = 0; i < NUM_WORDS_DEFAULT_FILE; i++) {
			fwrite(&words[i], sizeof(WORD), 1, datafile);
		}
		
		fclose(datafile);
		
		return 1;
	
	} else {
		return 0;
	}
	
}

int check_datafile() {
	FILE * datafile;
	int choice;
	
	datafile = fopen(DATAFILENAME, "rb");
	
	if (datafile != NULL) {
		return 1;
	} else {
		printf("Datafile not found. Please download the file -words.dat- from the repository on github and place it on the same folder of the game to play with all possible words.\n");
	    printf("\n\nEnter 1 to create a new default database (only 5 words) and continue playing.");
	    printf("\nEnter 0 to exit.");
		do {
		    printf("\nYour choice (0/1): ");
		    scanf(" %d", &choice);
		} while(choice != 0 && choice != 1);
		
		if (choice == 1) {
			if (create_default_datafile(DATAFILENAME) != 1) {
				printf("\nERROR WHILE TRYING TO INITIALIZE DATAFILE, EXITING NOW.\n\n\n");
				exit(1);
			} 
		}
		
		return choice;
	}
	
}

