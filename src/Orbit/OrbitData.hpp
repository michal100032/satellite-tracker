#pragma once

#include <string_view>
#include <unordered_map>
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
	void load();
	SatelliteInfo& get(int catalogNumber);
private:
	void loadFromFile();
	void loadFromWeb();

	void parse(std::string_view, size_t length);
	void saveToFile(const char* data);

	bool isDataUpToDate();
	time_t lastUpdated();

	std::unordered_map<int, SatelliteInfo> m_satInfos;
};