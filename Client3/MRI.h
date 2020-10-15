
#pragma once
#include"include/Publisher.h"
#include <iostream>
#include<istream>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

struct Patient {
	string name;
	string procedure;
	int age;
	string gender;
};

extern int subscribe();
extern void ConnectPublisher();
void PatientData();
void AllotDetails(string a);
