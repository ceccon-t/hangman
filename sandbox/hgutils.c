#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_STRING 20
#define MAX_HINT 75
#define TOTAL_TRIES 5
#define QUIT_SIGNAL '0'

#define DATAFILENAME "words.dat"
#define NUM_WORDS_DEFAULT_FILE 5

/*
TODO: Word structure
V

TODO: display_word
V

TODO: create bin file for Words
V

TODO: random access to bin data
*/

typedef struct word {
	char string[MAX_STRING];
	char hint1[MAX_HINT];
	char hint2[MAX_HINT];
	char hint3[MAX_HINT];
} WORD;


void display_word(WORD w);
int create_default_datafile(char * filename);
int inspect_file(char * filename);

int main() {
	/*
	WORD w = {"banana",
			  "It's a fruit",
			  "Associated with the tropics",
			  "It's yellow"};
			  
	display_word(w);
	*/
	
	//create_default_datafile("new_default_file.dat");
	
	inspect_file(DATAFILENAME);
	
	return 0;
}

void display_word(WORD w) {
	
	printf("\n===== DISPLAY WORD ===== \n");
	printf("string: %s\n", w.string);
	printf("hint1: %s\n", w.hint1);
	printf("hint2: %s\n", w.hint2);
	printf("hint3: %s\n", w.hint3);
	printf("\n===== END DISPLAY WORD ===== \n");
	
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
					  "Became very popular after the 20s",
					  "American"
					},
					{ 
					  "dumbledore",
					  "A fictional character",
					  "First appeared in literature, but later in film and games",
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

int inspect_file(char * filename) { 
	FILE * datafile;
	WORD w;
	char op;
	
	if ((datafile = fopen(filename, "rb")) != NULL) {
	
		do {
			
			if (fread(&w, sizeof(WORD), 1, datafile) == 1) {
				display_word(w);
				printf("\nEnter q to quit or any other char to continue: ");
				scanf(" %c", &op);
			} else {
				printf("\nEnd of the file.\n");
			}	
			
		
		} while (op != 'q' && !feof(datafile));
		
		fclose(datafile);
		
		return 1;
	} else {
		return 0;
	}
	
}

