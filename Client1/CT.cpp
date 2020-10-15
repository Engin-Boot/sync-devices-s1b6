#include"CT.h""


void PatientData()
{
	struct Patient s;
	string topic = "Patient / Details";
	cout << "Enter Patient Details:\n" << endl;
	cout << "Enter Name:" << endl;
	getline(cin, s.name);
	cout << "Enter Age:" << endl;
	cin >> s.age;
	cout << "Enter Procedure name(PET-CT,MRI,CT):" << endl;
	getline(cin, s.procedure);
	cout << "Enter Gender(Male/Female/Others):" << endl;
	getline(cin, s.gender);

	string patient_details = s.name + "/" + to_string(s.age) + "/" + s.procedure + "/" + s.gender;
	int n = publish(topic, patient_details);



}

void AllotDetails(string details) {

	string param;
	stringstream ss(details);
	vector<string>result;
	while (getline(ss, param, '/'))
	{
		result.push_back(param);
	}
	struct Patient s;
	s.name = (result[0]);
	s.age = stoi(result[1]);
	s.procedure = (result[2]);
	s.gender = (result[3]);
}

int main()
{
	int connection = 0;
	connection = subscribe();
	if (connection != 0)
	{
		cout << "\n Connectition to subscriber not established!";
		return connection;
	}

	ConnectPublisher();

	PatientData();
}


