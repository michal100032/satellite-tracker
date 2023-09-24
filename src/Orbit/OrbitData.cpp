#include "OrbitData.hpp"

#include <string_view>
#include <iostream>
#include <sstream>
#include <fstream>
#include <charconv>
#include <format>

#include <sys/types.h>
#include <sys/stat.h>

#include "../Http.hpp"
#include "../Time.hpp"
#include "../consts.hpp"

const double DEG_TO_RAD = 3.14159 / 180.0;

const char* DATA_FILE_PATH = "resources/data.txt";
// const char* DATA_URL = "celestrak.org/NORAD/elements/gp.php?GROUP=stations&FORMAT=tle";
const char* DATA_URL = "celestrak.org/NORAD/elements/gp.php?GROUP=noaa&FORMAT=tle";
//const char* DATA_URL = "celestrak.org/NORAD/elements/gp.php?GROUP=molniya&FORMAT=tle";

const double DATA_UPDATE_INTERVAL = 7 * 86400.0;

OrbitData::OrbitData() {

}

void OrbitData::load() {
	if (isDataUpToDate())
		loadFromFile();
	else loadFromWeb();
}

SatelliteInfo& OrbitData::get(int catalogNumber) {
	return m_satInfos[catalogNumber];
}

void OrbitData::loadFromFile() {
	std::cout << "Loading data from " << DATA_FILE_PATH << std::endl;

	std::ifstream file(DATA_FILE_PATH, std::ios_base::in | std::ios_base::binary);

	file.seekg(0, std::ios::end);
	int length = file.tellg();
	file.seekg(0, std::ios::beg);
	
	char* buffer = new char[length + 1];
	buffer[length] = '\0';

	file.read(buffer, length);
	file.close();
	
	parse(buffer, length);
	delete[] buffer;
}

void OrbitData::loadFromWeb() {
	std::cout << "Downloading data from http://" << DATA_URL << std::endl;

	Http http;
	HttpResponse response = http.get(DATA_URL);
	saveToFile(response.body);

	parse(response.body, response.length);
}

std::string_view moveLine(std::string_view& data) {
	size_t position = data.find('\r\n');

	std::string_view line;
	if (position == std::string_view::npos || position + 2 >= data.length()) {
		line = data.substr(0, data.length());
		data.remove_prefix(data.length());
	}
	else {
		line = data.substr(0, position);
		data.remove_prefix(position + 2);
	}
	return line;
}

template<typename T> 
inline T readValue(std::string_view line, size_t start, size_t end) {
	while (line[start] == ' ')
		start++;
	T data;
	std::from_chars(line.data() + start, line.data() + end, data);
	return data;
}

std::chrono::utc_clock::time_point readTime(std::string_view line) {

	int epochYear = readValue<int>(line, 18, 20);
	float epochDay = readValue<float>(line, 20, 32);

	if (epochYear < 57)
		epochYear += 100;

	tm time;
	time.tm_year = epochYear;
	time.tm_mon = 0;
	time.tm_mday = static_cast<int>(epochDay);
	time.tm_yday = static_cast<int>(epochDay) - 1;

	float fractionOfDay = epochDay - time.tm_yday - 1;
	time.tm_hour = static_cast<int>(fractionOfDay * 24);
	
	float fractionOfHour = 24 * fractionOfDay - time.tm_hour;
	time.tm_min = static_cast<int>(fractionOfHour * 60);
	float fractionOfMinute = 60 * fractionOfHour - time.tm_min;
	
	time.tm_sec = static_cast<int>(roundf(fractionOfMinute * 60));
	time.tm_isdst = 0;

	using namespace std::chrono;

	time_t sysTime = system_clock::to_time_t(sys_days{ year(time.tm_year + 1900) / month(1) / day(1) } 
	+ hours{ 24 * time.tm_yday + time.tm_hour } + minutes{ time.tm_min } + seconds{ time.tm_sec });

	time_point<utc_clock> utcTime = std::chrono::utc_clock::from_sys(system_clock::from_time_t(sysTime));
	return utcTime;
}

double getSecsBetweenTimeAndJulian(tm& time, double julian) {

	tm firstOfJanuary = time;
	firstOfJanuary.tm_mday = 1;
	firstOfJanuary.tm_yday = 0;

	double timeJulian = Time::getJulianDay(firstOfJanuary) + time.tm_yday;
	double julianOffset = julian - timeJulian;
	return julianOffset * 86400;
}

void OrbitData::parse(std::string_view data, size_t length) {
	double julianNow = Time::julianDayNow();
	size_t position;
	SatelliteInfo satellite;

	std::cout << "Parsing..." << std::endl;
	
	while (data.length() > 2) {
		// line 0
		std::string_view line = moveLine(data);
		satellite.name = std::string(line);

		// line 1
		line = moveLine(data);		
		int catalogNumber = readValue<int>(line, 2, 7);
		auto time = readTime(line);

		// line 3
		line = moveLine(data);

		double incl         = readValue<double>(line, 9, 16) * DEG_TO_RAD;
		double raan         = readValue<double>(line, 17, 25) * DEG_TO_RAD;
		double ecc          = readValue<double>(line, 26, 33) / 10000000;
		double aop          = readValue<double>(line, 34, 42) * DEG_TO_RAD;
		double meanAnomaly  = readValue<double>(line, 43, 51) * DEG_TO_RAD;
		double meanAngular  = readValue<double>(line, 52, 63) * 2.0 * PI / 86400;

		// calculations
		/*
		double timeDifference = getSecsBetweenTimeAndJulian(time, julianNow);
		double meanAnomalyNow = meanAnomaly + meanAngular * timeDifference;
		double trueAnomalyNow = Orbit::trueFromMean(meanAnomalyNow, ecc);
		*/
		double sma = pow(GRAV_PARAM / meanAngular / meanAngular, 1.0 / 3);

		satellite.catalogNumber = catalogNumber;
		satellite.orbit = Orbit(sma, ecc, raan, incl, aop, Orbit::trueFromMean(meanAnomaly, ecc), time);

		m_satInfos.emplace(catalogNumber, satellite);
	}
}

inline bool OrbitData::isDataUpToDate() {
	return difftime(time(nullptr), lastUpdated()) < DATA_UPDATE_INTERVAL;
}

time_t OrbitData::lastUpdated() {
	struct stat result;
	if (stat(DATA_FILE_PATH, &result) == 0) {
		return result.st_mtime;
	}
	return 0;
}

void OrbitData::saveToFile(const char* data) {
	std::ofstream file(DATA_FILE_PATH, std::ios_base::out | std::ios_base::binary);
	if (file.good()) {
		file.write(data, strlen(data));
	}
}
