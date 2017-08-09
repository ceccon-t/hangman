#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>

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
EDIT: this seems to be nearly impossible to do nicely with a standard terminal + stdin

TODO: force only unused characters
V

TODO: display used characters
V

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
V

TODO: implement the 'drawings'
V

TODO: refactor the code to clean the main function
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
void display_match(WORD chosen_word, int *mask, int tries_left);
void display_chars_used(char *chars_already_used);
void reset_mask(int *mask, int val);
int check_win(int *mask, int i_last_char);
char get_user_input(char *chars_already_used);
int end_of_string(char *string); // TODO: remove eventually

int main () {
    //char string[MAX_STRING] = "banana";
    int mask[MAX_STRING] = {0};
    char guess, chars_already_used[28];
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
    	strcpy(chars_already_used, "");
    	
    	while(tries_left > 0 && !won) {
			
			display_match(chosen_word, mask, tries_left);
		
			
			printf("\n");
			//printf("You have %d guesses left.\n", tries_left);
			printf("You have %d guess%s left.\n", tries_left, (tries_left == 1) ? "" : "es");
			printf("You have already guessed the following letters: ");
			display_chars_used(chars_already_used);
			printf("\nGuess ONE letter (enter %c to quit): ", QUIT_SIGNAL);
			
			//scanf(" %c", &guess);
			
			
			guess = get_user_input(chars_already_used);
			
			//printf("===%c===", guess);
			//exit(1);
			
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

void print_gallow_with_offset(char *part) {
	printf("\t\t%s\n", part);
}

void draw_gallows(int tries_left) {
	
	print_gallow_with_offset("_________");
	print_gallow_with_offset("|       |");
	//print_gallow_with_offset((tries_left < 5) ? "|       O" : "|");
	if (tries_left < 5) {
		if (tries_left == 0) {
			print_gallow_with_offset("|       0");
		} else {
			print_gallow_with_offset("|       O");
		}
	} else {
		print_gallow_with_offset("|");
	}
	//print_gallow_with_offset((tries_left < 4) ? "|      -|-" : "|");
	if (tries_left < 4) {
		if (tries_left < 2) {
			print_gallow_with_offset("|      -|-");
		} else {
			print_gallow_with_offset("|       |");
		}
	} else {
		print_gallow_with_offset("|");
	}
	print_gallow_with_offset((tries_left < 4) ? "|       |" : "|");
	print_gallow_with_offset((tries_left < 3) ? "|      / \\" : "|");
	print_gallow_with_offset("|");
	print_gallow_with_offset("|");
	print_gallow_with_offset("-----------------");
	
	printf("\n");
	
	
	/*
_________
|       |
|       O
|      -|-
|       |
|      / \
|
|
-----------------

	*/
}


void display_match(WORD chosen_word, int *mask, int tries_left) { 
	int i;
	
	//system("clear||cls"); 
	// cls: Windows command to clear prompt; printf "\033c": same for bash
	system("cls||printf \"\\033c\""); // TODO: find better solution than calling the system
	
	// Title
	printf("================================================================================\n");
	printf("=================================== HANGMAN ====================================\n");
	printf("================================================================================\n");
	
	draw_gallows(tries_left);
	
	printf("\t\t\t\t");
	for (i=0; i<strlen(chosen_word.string); i++) {
		if (mask[i]) {
			printf("%c ", chosen_word.string[i]);
		} else {
		    printf("_ ");
		}
	}
	
	printf("\n\nHint 1: %s\n", chosen_word.hint1);
	printf("Hint 2: %s\n", (tries_left < 4) ? chosen_word.hint2 : "XXXXXXXX" );
	printf("Hint 3: %s\n", (tries_left < 2) ? chosen_word.hint3 : "XXXXXXXX" );
	
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
	
	for (i = 0; i < i_last_char; i++) {
		if(mask[i] == 0) return 0; // TODO: braces, please
	}
	
	return 1;
}

char get_user_input(char *chars_already_used) {
	char valid_inputs[] = "abcdefghijklmnopqrstuvwxyz0", raw_in[3];
	char cleansed_input;
	int len;
	
	fflush(stdin); // TODO: ?
	
	//fgets(raw_in, 2, stdin);
	//cleansed_input = tolower(raw_in[0]);
	scanf(" %c", &cleansed_input);
	cleansed_input = tolower(cleansed_input);
	
//	while ((raw_in[1] != '\0') || (strchr(valid_inputs, cleansed_input) == NULL) || (strchr(chars_already_used, cleansed_input) != NULL)) {

	while ((strchr(valid_inputs, cleansed_input) == NULL) || (strchr(chars_already_used, cleansed_input) != NULL)) {
	
	//while (raw_in[1] != '\0') {
		printf("\nPlease enter only an UNUSED LETTER. Try again: ");
		/*
		fgets(raw_in, 2, stdin);
		cleansed_input = tolower(raw_in[0]);
		*/
		scanf(" %c", &cleansed_input);
		cleansed_input = tolower(cleansed_input);
		
		
	}
	
	
	len = strlen(chars_already_used);
	chars_already_used[len] = cleansed_input;
	chars_already_used[len+1] = '\0';
	
	//strcat(chars_already_used, cleansed_input); 
	
	return cleansed_input;
	

}



// Pure debug
int end_of_string(char *string) {
	int i;
	
	return (int)strlen(string);
}


