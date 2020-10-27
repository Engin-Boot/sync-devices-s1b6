#include "PatientDetails.h"
#include <iostream>
#include <vector>
#include <sstream>

std::string fillPatientDetails(Patient patient)
{
	std::string topic = "Patient/Details";
	std::cout << "Enter Patient Details: " << std::endl;
	std::cout << "Enter Name: ";
	std::cin >> patient.name;
	std::cout << "Enter Age: ";
	std::cin >> patient.age;
	std::cout << "Enter Procedure name(PET-CT,MRI,CT): ";
	std::cin >> patient.procedure;
	std::cout << "Enter Gender(Male/Female/Others): ";
	std::cin >> patient.gender;
	std::cout << "\n\n";
	std::string patient_details = patient.name + "/" + std::to_string(patient.age) + "/" + patient.procedure + "/" + patient.gender;
	return patient_details;
}

Patient splitPublishedData(std::string details) {

	std::string word;
	std::stringstream ss(details);
	std::vector<std::string>result;
	while (std::getline(ss, word, '/'))
	{
		result.push_back(word);
	}
	Patient patient;
	patient.name = (result[0]);
	patient.age = stoi(result[1]);
	patient.procedure = (result[2]);
	patient.gender = (result[3]);

	return patient;
}

void displayPatientDetails(Patient patient)
{
	std::cout << "Patient-Name : " << patient.name << "\n";
	std::cout << "Patient-Age : " << patient.age << "\n";
	std::cout << "Procedure Chosen : " << patient.procedure << "\n";
	std::cout << "Patient-Gender : " << patient.gender << "\n\n";
}
