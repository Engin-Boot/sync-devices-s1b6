#pragma once

#include <string>

struct Patient {
	std::string name;
	std::string procedure;
	int age;
	std::string gender;
};

std::string fillPatientDetails(Patient);
Patient splitPublishedData(std::string);
void displayPatientDetails(Patient);