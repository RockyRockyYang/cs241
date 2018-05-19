/**
 * Overworked Interns Lab
 * CS 241 - Fall 2016
 */
#include "company.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/**
void *work_interns(void *p) { 
	Company *company = (Company *)p;
  	pthread_mutex_t *left_intern, *right_intern;
  	while (running) {
    	left_intern = Company_get_left_intern(company);
    	right_intern = Company_get_right_intern(company);
    	pthread_mutex_lock(left_intern);
    	int fail = pthread_mutex_trylock(right_intern);
    	if(!fail){
    		Company_hire_interns(company);
    		pthread_mutex_unlock(right_intern);
    	}else{
			Company_have_board_meeting(company);
		}
    	pthread_mutex_unlock(left_intern);
    	Company_have_board_meeting(company);
  	}
	return NULL; 
}*/
void *work_interns(void *p) { 
	Company *company = (Company *)p;
  	pthread_mutex_t *left_intern, *right_intern;
	int lock_left;
  	while (running) {
	lock_left = 0;
    	left_intern = Company_get_left_intern(company);
    	right_intern = Company_get_right_intern(company);
    	if(left_intern < right_intern){lock_left = 1;}
    	if(lock_left){
    		pthread_mutex_lock(left_intern);
    		int fail = pthread_mutex_trylock(right_intern);
    		if(!fail){
    			Company_hire_interns(company);
    			pthread_mutex_unlock(right_intern);
    		}
    		pthread_mutex_unlock(left_intern);
		Company_have_board_meeting(company);	
    	}else{	
    		int right = pthread_mutex_trylock(right_intern);
    		if(!right){
    			int left = pthread_mutex_trylock(left_intern);
    			if(!left){
    				Company_hire_interns(company);
    				pthread_mutex_unlock(left_intern);
    			}
    			pthread_mutex_unlock(right_intern);
			Company_have_board_meeting(company);	
    		}
    	}
    	
  	}
	return NULL; 
}
