////////////////////////////////////////////////////////////////////////////////
// Main File:        intdate.c
// This File:        intdate.c
// Other Files:      sendsig.c, division.c
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
#include <time.h>

int alrmTime = 3;
int sigUsr1;
void handler_SIGALRM();
void handler_SIGUSR1();
void handler_SIGINT();


/* The main function enters an infinite loop, which may be interrupted by
 * SIGALRM, SIGUSR1, SIGINT.
 * According to the respective signal, one of the handlers withing the
 * program is triggered, and a response is provided.
 *
 * Return value is 0 if the function reaches the end
 */
int main() {
  // Introductory statements about the program intdate
  printf("Pid and time will be printed every 3 seconds.\n");
  printf("Enter ^C to end the program.\n");

  // Counter for the number of time user signal 1 is caught
  sigUsr1 = 0;

  // set up the sigaction struct for SIGALRM
  struct sigaction sa;
  memset (&sa, 0, sizeof(sa)); 
  sa.sa_handler = &handler_SIGALRM;   
  sa.sa_flags = 0;

  // Check the return value of sigaction, and make the call to handler
  if (sigaction(SIGALRM, &sa, NULL) != 0)
  {
    printf("Error: Binding SIGALRM error");
  }

  // set up the sigaction struct for SIGUSR1
  struct sigaction sa_usr1;
  memset (&sa_usr1, 0, sizeof(sa_usr1));  
  sa_usr1.sa_handler = &handler_SIGUSR1;
  sa_usr1.sa_flags = 0;

  // Check the return value of sigaction, and make the call to handler
  if (sigaction(SIGUSR1, &sa_usr1, NULL) != 0)
  {
    printf("Error: Binding SIGUSR1 error");
  }

  // set up the sigaction struct for SIGINT
  struct sigaction sa_int;
  memset (&sa_int, 0, sizeof(sa_int));  
  sa_int.sa_handler = &handler_SIGINT;
  sa_int.sa_flags = 0;

  // Check the return value of sigaction, and make the call to handler
  if (sigaction(SIGINT, &sa_int, NULL) != 0)
  {
    printf("Error: Binding SIGINT error");
  }

  // Set the alarm to go off in (alrmTime) seconds
  if (alarm(alrmTime) != 0)
  {
    printf("Unable to set alarm");
    exit(0);
  }

  // Set up an infinite loop to be interrupted by SIGALRM, SIGUSR1, SIGINT
  while (1)
  {
  }

  return 0;
}

/* The SIGALRM handler responds to a signal sent by the alarm() func.
 * Every alrmTime seconds, the handler will be triggered:
 * The process ID and current time are pronted out.
 */
void handler_SIGALRM()
{
  // time variable (in order to retrieve and print current time)
  time_t ltime;

  // Check the return value of the time() function
  if (time(&ltime) == -1)
  {
    printf("Unable to retrieve calendar time\n");
    exit(0);
  }

  // Check the return value of the ctime() function
  if (ctime(&ltime) == NULL)
  {
    printf("Unable to retrieve date and time string\n");
    exit(0);
  }

  // Print a line with the process id and the current time
  printf("PID: %d | Current Time: %s", getpid(), ctime(&ltime));

  // Reset the alarm
  alarm(alrmTime);
} 

/* The SIGUSR1 handler responds to a user signal
 * Every time the handler is triggered, a counter is incremented
 */
void handler_SIGUSR1()
{
  // Increment the user signal counter
  sigUsr1++;   
  printf("SIGUSR1 caught!\n"); 
}

/* The SIGINT handler responds to an INTERRUPT (ctrl + C)
 * Once the handler is triggered, the user signal counter value is printed,
 * and the process is terminated.
 */
void handler_SIGINT()
{
  // Print exit messages with user signal count and terminate
  printf("\nSIGINT received.\n");
  printf("SIGUSR1 was received %d times. Exiting now.\n", sigUsr1);
  exit(0);
}

