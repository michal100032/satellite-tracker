#include "Time.hpp"

#include <iostream>
#include <iomanip>
#include <ctime>
#include <format>
#include <cmath>

double Time::gmstNow() {
	
	tm now = utc();
	double jd = getJulianDay(now);

	double ut = jd + 0.5 - truncf(jd + 0.5);
	jd -= ut;
	double du = jd - 2451545.0;
	double Tu = du / 36525;

	double midGmst =  24110.54841 + 8640184.812866 * Tu + 0.093104 * Tu * Tu - 6.2e-6 * Tu * Tu * Tu;
	double gmst = fmod(midGmst + 86400 * 1.00273790934 * ut, 86400);
		
	return gmst;
}

tm Time::utc() {
	time_t now = time(0);
	return *gmtime(&now);
}

double Time::getJulianDay(tm& t) {
	
	int D = t.tm_mday;
	int M = t.tm_mon + 1;
	int Y = t.tm_year + 1900;

	int JDN = (1461 * (Y + 4800 + (M - 14) / 12)) / 4 + (367 * (M - 2 - 12 * ((M - 14) / 12))) / 12 - (3 * ((Y + 4900 + (M - 14) / 12) / 100)) / 4 + D - 32075;
	return (double)JDN + ((double)t.tm_hour - 12.0) / 24 + (double)t.tm_min / 1440 + (double)t.tm_sec / 86400;
}

double Time::julianDayNow() {
	tm now = utc();
	return getJulianDay(now);
}

double Time::julianSinceEquinox() {
	return julianDayNow() - 2460024.39201;
}