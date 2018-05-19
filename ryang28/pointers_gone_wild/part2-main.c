/**
 * Machine Problem 0
 * CS 241 - Spring 2016
 */

#include "part2-functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * (Edit this function to print out the "Illinois" lines in
 * part2-functions.c in order.)
 */
int main() {
  // your code here
  first_step(81);
//2
  int * value_second = malloc(sizeof(int));
  *value_second= 132;
  second_step(value_second);
	free(value_second);
//3
  int **value_3 = malloc(sizeof(int*));
	*value_3 = malloc(sizeof(int));
	**value_3 = 8942;
	double_step(value_3);
  free(*value_3);
	free(value_3);

//4
  char ch[] = {0,0,0,0,0,0,0,0,0};
  *(int*)(&ch[5]) = 15;
	strange_step( ch );
//5
	char* value_5 = "hey";
	empty_step(value_5);
//6
	char* s = "uuuuuuuu";
	char* s2 = "uuuuuuuu";
	two_step(s,s2);
//7
	char* word = "helloworld";
	char* first_ptr = &word[0];
	char* second_ptr = &word[2];
	char* third_ptr = &word[4];
	three_step(first_ptr, second_ptr, third_ptr);
//8
	char* first_word = "aaaaa";
	char* second_word = "iiiii";
	char* third_word = "qqqqq";
	step_step_step(first_word, second_word, third_word);
//9
	char* odd_char = "a";
	it_may_be_odd(odd_char, 97);
//10
	char str_tok[12] = "CS241,CS241";
	tok_step(str_tok);
//11
	char ch2[] = {1,8,0,0,0};
  the_end(ch2, ch2);
printf("%d",*(int*)ch2);
  return 0;
}
