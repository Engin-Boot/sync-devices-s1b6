#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../Mqtt_C_Client/PatientDetails.cpp"

TEST_CASE("When splitPublishedData called with string having details then returns a Patient object with its fields filled")
{
	Patient patient;
	patient.name = "Name";
	patient.age = 45;
	patient.procedure = "Procedure";
	patient.gender = "Gender";

	std::string details = "Name/45/Procedure/Gender/";
	auto returnedPatient = splitPublishedData(details);
	
	REQUIRE(returnedPatient.name == patient.name);
	REQUIRE(returnedPatient.procedure == patient.procedure);
	REQUIRE(returnedPatient.age ==patient.age);
	REQUIRE(returnedPatient.gender == patient.gender);
}

