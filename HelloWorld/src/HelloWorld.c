/*
 ============================================================================
 Name        : HelloWorld.c
 Author      : Carlo
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <ourhdr.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
	pid_t pid = getpid();

	if (pid < 0){
		err_sys("Getpid should not fail!!!");
	}

	printf("My id is: %d \n", pid);
	printf("And pid square root is: %f", (double) sqrt(pid));
	return EXIT_SUCCESS;
}
