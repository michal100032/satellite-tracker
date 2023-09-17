#pragma once

#include <ctime>

class Time {
public:
	static double gmstNow();

	static tm utc();
	static double getJulianDay(tm& time);
	static double julianDayNow();
	static double julianSinceEquinox();

};