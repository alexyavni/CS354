////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c
// This File:        division.c
// Other Files:      sendsig.c, intdate.c
// Semester:         CS 354 Fall 2018
//
// Author:           Alexandra Yavnilovitch
// Email:            yavnilovitch@wisc.edu
// CS Login:         yavnilovitch
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void handler_SIGFPE();
void handler_SIGINT();
int counter;

/* The division function prompts the user enters 2 integer values.
 * Then a division operation is attempted (first/second). The number of
 * successful operations is recorded. 
 * The function can be terminated in two ways:
 * 	- Interrupt signal (SIGINT) ctrl+C
 * 	- Division by zero (SIGFPE)
 */
int main() 
{
  // Vars for the numerator (string is converted to int)
  int num;
  char num_str[100];
  
  // Vars for the denominator (srting is conv. to int)
  int denom;
  char denom_str[100];
  
  // Var for quotient and remainder
  int quot;
  int rem;
  
  // Initialize counter for number of successful division operations
  counter = 0;

  // Sigaction struct to handle division by zero error
  struct sigaction sa;
  memset (&sa, 0, sizeof(sa));
  sa.sa_handler = &handler_SIGFPE;
  sa.sa_flags = 0;

  // Call the SIGFPE handler if division by zero triggers
  if (sigaction(SIGFPE, &sa, NULL) != 0)
  {
    printf("Error: Binding SIGFPE error");
  }

  // Sigaction struct to handle INTERRUPT (ctrl+C)
  struct sigaction sa_int;
  memset (&sa_int, 0, sizeof(sa_int));
  sa_int.sa_handler = &handler_SIGINT;
  sa_int.sa_flags = 0;

  // Call the SIGINT handler when (ctrl+c) or kill() interrupts process.
  if (sigaction(SIGINT, &sa_int, NULL) != 0)
  {
    printf("Error: Binding SIGINT error");
  }

  // Loop body of division function
  // Prompts user for integer values to use in division
  // If division by zero does not occur, the result is displayed,
  // and the loop continues to iterate
  while (1)
  {
    // Prompt for user inputs (first, second ints)
    printf("Enter first integer: ");
    if (fgets(num_str, 100, stdin) == NULL)
    {
      printf("Unable to read user input\n");
      exit(0);
    }
    
    printf("Enter second integer: ");
    if (fgets(denom_str, 100, stdin) == NULL)
    {
      printf("Unable to read user input\n");
      exit(0);
    }

    // Parse user input for the numerator and the denominator
    num = atoi(num_str);
    denom = atoi(denom_str);

    // Determine the quotient and remainder of the operation
    quot = num / denom;
    rem = num % denom;

    // Print out the results
    printf("%d / %d is %d with a remainder of %d\n", num, denom, quot, rem);
    counter++;
  }
}

/* The SIGFPE handler responds to the division by zero operation.
 * Once the handler is triggered, the count of successful division operations
 * is printed, and the process is terminated.
 */
void handler_SIGFPE() 
{
  printf("Error: a division by 0 operation was attempted.\n");
  printf("Total number of operations completed successfully: %d\n", counter);
  printf("The program will be terminated.\n");
  exit(0);
}

/* The SIGINT handler responds to an INTERRUPT (ctrl + C)
 * Once the handler is triggered, the count of successful division operations
 * is printed, and the process is terminated.
 */
void handler_SIGINT()
{
  printf("\nTotal number of operations successfully completed: %d\n", counter);
  printf("The program will be terminated.\n");
  exit(0);
}
