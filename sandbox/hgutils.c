#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_STRING 20
#define MAX_HINT 75
#define TOTAL_TRIES 5
#define QUIT_SIGNAL '0'

/*
TODO: Word structure
V

TODO: display_word

TODO: create bin file for Words

TODO: random access to bin data
*/

typedef struct word {
	char string[MAX_STRING];
	char hint1[MAX_HINT];
	char hint2[MAX_HINT];
	char hint3[MAX_HINT];
} WORD;


void display_word(WORD w);

int main() {
	WORD w = {"banana",
			  "It's a fruit",
			  "Associated with the tropics",
			  "It's yellow"};
			  
	display_word(w);
	
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
