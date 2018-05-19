/**
 * Mini Valgrind Lab
 * CS 241 - Fall 2016
 */

#include "mini_valgrind.h"
#include <stdio.h>
#include <stdlib.h>
int main() {
  // your tests here using malloc and free
 	char* str = malloc (10);
        sprintf (str, "%s", "Hello");
        printf ("%s\n", str);
        free (str);
        char* str2 = malloc(30);
	sprintf (str2, "shabi");

	char* str4 = malloc (10);
        free (str4);
        free (str4);
	free (str4);
        free (NULL);

	char* str5 = malloc(20);
	free(str5 + sizeof(int) *2);

	char* str6 = malloc(20);	
	str6 = "hello";
	free(str6);
	
	char *ptr = (char *)malloc(20);
	free(ptr);
	ptr = NULL;
	free(ptr);
  // Do NOT modify this line
  atexit(print_report);
  return 0;
}
