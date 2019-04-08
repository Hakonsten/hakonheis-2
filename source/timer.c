#include <time.h>
#include "timer.h"


void time_begin_timer() {
	timeElapsed = time(NULL);
}

int time_is_up() {
	if (difftime(time(NULL), timeElapsed) >= 3) {
		return 1;
	}
	return 0;
}


void time_end_timer() {
	timeElapsed = 0;
}
