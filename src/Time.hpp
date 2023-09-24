#pragma once

#include <ctime>
#include <chrono>

class Time {
public:
	static double gmstNow();

	static tm utc();
	static double getJulianDay(tm& time);
	static double getJulianDay(std::chrono::utc_clock::time_point time);

	static double julianDayNow();
	static double julianSinceEquinox(std::chrono::utc_clock::time_point time);
	static double gmstFromUtc(std::chrono::utc_clock::time_point utc);

};