#include <iostream>
#include <unistd.h>
#include <bcc_helper.h>
#include <random>

int main(){
	MyBccHelper helper;

	/*
	 */
	  char* ptr = NULL;
	  sleep(10);
	  *ptr = '\0'; //causes signal SIGSEV
}
