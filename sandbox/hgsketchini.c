#include<stdio.h>
#include<string.h>

int main () {
    char string[20] = "banana";
    int mask[20] = {0};
    char guess;
    int i;
    
    while (guess != '0') {
        printf("\n---------\n");
		for (i=0; i<strlen(string); i++) {
			if (mask[i]) {
			    printf("%c", string[i]);
		    } else {
		        printf("_");
		    }
		}
		
		printf("\n");
		printf("Make a guess: ");
		scanf(" %c", &guess);
		
		for (i=0; i<strlen(string); i++) {
		    if (string[i] == guess) {
		        mask[i] = 1;
		    }
		}
    }
    
    
    
    printf("\nFIM\n");
    return 0;
}
