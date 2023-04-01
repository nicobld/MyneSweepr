#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

extern int listenfd;

void error_and_die(const char errormsg[]){
	perror(errormsg);
	close(listenfd);
	exit(EXIT_FAILURE);
}

void die(const char msg[]){
	printf("%s\n", msg);
	close(listenfd);
	exit(EXIT_FAILURE);
}

void ctrlc(int){
	fprintf(stdout,"Bye\n");
	close(listenfd);
	exit(EXIT_SUCCESS);
}