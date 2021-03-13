#include <stdio.h>
#include <unistd.h>

#include "throbber.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"

int main(){
	int throbber1 = start_throbber(1, 1, RED);
	int throbber2 = start_throbber(2, 1, GREEN);
	puts("Check the top-left corner");
	sleep(5);
	stop_throbber(throbber1);
	stop_throbber(throbber2);
	puts("Done!");
}
