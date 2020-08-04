////////////////////////////////////////////////////////////////////////////////
// Main File:        sendsig.c
// This File:        sendsig.c
// Other Files:      intdate.c, division.c
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

/* The program sendsig takes two command line arguments.
 * As a response to the respective arguments, the function will determine
 * whether to send a SIGINT or a SIGUSR1.
 *
 * argc: number of command line arguments
 * argv: array of string arguments
 */
int main(int argc, char **argv) {
  // Check if there are exactly 3 command line arguments
  // String array: (1) "sendsig" (2) signal type (3) Process id
  if (argc != 3)
  {
    printf("Usage: <signal type> <pid>\n");
    exit(0);
  }

  // Get the process id
  int pid = atoi(argv[2]);

  int valid_type = 0;

  // Check signal type (SIGUSR1 or SIGINT)
  // Make a kill() function call accordingly
  if (strcmp(argv[1], "-u") == 0)
  {
    valid_type = 1;
    if (kill(pid, SIGUSR1) == -1)
    {
      printf("Unsuccessful call of kill() function\n");
    }
  }
  if (strcmp(argv[1], "-i") == 0)
  {
    valid_type = 1;
    if (kill(pid, SIGINT) == -1)
    {
      printf("Unsuccessful call of kill() function\n");
    }
  }
  if (valid_type == 0)
  {
    printf("Handled signal types: -u (SIGUSR1), -i (SIGINT)\n");
  }
  return 0;
}
