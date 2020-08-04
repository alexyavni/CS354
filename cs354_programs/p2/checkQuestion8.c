#include <stdio.h>
#include <stdlib.h>

int main (void) {
	char buf[100], word[20], num[20];
	int course =  0;
	strcpy(buf, "CS 354");

	sscanf(buf, "%s %s", &word, &num);
        printf("%s %s\n", num, word);

// (3)
//	sscanf(buf, "%s %d", word, &course);
//    	printf("%d %s\n", course, word);

// (2)
//	sscanf(buf, "%s %s", word, num);
//	printf("%s %s\n", num, word);

// (1)
//	sscanf(buf, "%s %d", word, course);
//	printf("%d %s\n", course, word);
}
