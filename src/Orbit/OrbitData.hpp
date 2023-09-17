#pragma once

#include <string_view>
#include <vector>
#include <ctime>

#include "Orbit.hpp"

struct SatelliteInfo {
	std::string name;
	int catalogNumber;

	time_t timestamp;

	Orbit orbit;
};

class OrbitData {
public:
	OrbitData();
	std::vector<SatelliteInfo> load();
private:
	void loadFromFile(std::vector<SatelliteInfo>& outData);
	void loadFromWeb(std::vector<SatelliteInfo>& outData);

	void parse(std::string_view, size_t length, std::vector<SatelliteInfo>& outData);
	void saveToFile(const char* data);

	bool isDataUpToDate();
	time_t lastUpdated();
};