#include <stdlib.h>
#include <stdio.h>
int main() {
	typedef struct {
		char fname[30];
		char lname[30];
	}Student;

	typedef struct {
		int num;
		char name[30];
		Student stud[50];
	}Course;

	Course *var = malloc(sizeof(Course));
	char name[30];
	strcpy((var->stud[0]).fname,"Alex");

	printf("hello\n");
	printf("%s\n",(var->*stud.fname));
	return 0;
}
