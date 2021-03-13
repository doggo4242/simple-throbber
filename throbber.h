// Simple command-line progress bar in C
// (c)m1lkweed 2021 GPLv3+
// Works on all ANSI-compliant, UNIX-like terminals with compatible fonts
//
// start_throbber() returns the pid of the child that's displaying the throbber.
// this should be passed to stop_throbber() or it will run until the parent exits.
// start_throbber() returns -1 on error, no child will be created.

#ifndef THROBBER_H_
#define THROBBER_H_

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <stdnoreturn.h>

unsigned _throbber_x, _throbber_y;

noreturn static void _stop_throbber(int signo){
	(void)signo;
	const char *safe_exit = "\x1b[u\x1b[39;49m";
	fprintf(stdout, "\x1b[s\x1b[%d;%dH", _throbber_x, _throbber_y);
	fputs("  ", stdout);
	fputs(safe_exit, stdout);
	fflush(stdout);
	exit(0);
}

int start_throbber(const unsigned x, const unsigned y, const char *color){
	const char *frames[] = {
		"⠯⠃",
		"⠏⠇",
		"⠫⠇",
		"⠭⠇",
		"⠮⠇",
		"⠧⠇",
		"⠯⠆",
		"⠯⠅",
		
	};
	const char *safe_exit = "\x1b[u\x1b[39;49m"; //pop cursor position and reset colors
	if((x <= 0)||(y <= 0))return -1;
	int child;
	unsigned step = 0;
	fflush(stdout);
	if((child = fork())){
		return child;
	}else{
		_throbber_x = x;
		_throbber_y = y;
		signal(SIGUSR1, _stop_throbber);
		prctl(PR_SET_NAME, "throbber");
		while(1){
			fprintf(stdout, "\x1b[s\x1b[%d;%dH%s", x, y, color?color:""); //push and move cursor position
			fputs(frames[++step %8], stdout);
			fputs(safe_exit, stdout);
			fflush(stdout); //prevents visual errors
			usleep(100000L);
		}
	}
}

void stop_throbber(int throbber){
	kill(throbber, SIGUSR1);
	usleep(100L); //ensures the calling thread is delayed for the minimum time needed
	return;
}

#endif //THROBBER_H_
