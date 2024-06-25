#include "Time.h"

void Time::setTime(int h, int m, int s) {
	hours = h;
	minutes = m;
	seconds = s;
}


int Time::getHours() {
	return hours;
}

void Time::increaseTime() {
	if (seconds == 59) {
		if (minutes == 59) {
			hours++;
			minutes = 0;
		}
		else {
			minutes++;
		}
		seconds = 0;
	}
	else {
		seconds++;
	}
}

int main() {

	Time t;
	t.setTime(10, 26, 39);
	
	// cout << t.hours << endl;		// can not access since "hours" is private
	cout << t.getHours() << endl;	// can access "getHours()" since it is public
	
	return 0;
}