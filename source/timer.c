#include <time.h>
#include "stdio.h"

int timeElapsed = 0;


void time_begin_timer() {
	timeElapsed = time(NULL);
}

int time_is_up() {
	printf("tid %ld", (time(NULL) - timeElapsed));
	if ((time(NULL) - timeElapsed) >= 3) {
		return 1;
	}
	return 0;
}


void time_end_timer() {
	timeElapsed = 0;
}

int get_timeElapsed(){
	return timeElapsed;
}
