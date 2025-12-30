#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <limits>

using namespace std;

const string FileName = "C:\\Users\\lenovo\\Desktop\\Project01\\ProblemSolving#7\\Clients.txt";


struct stClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance = 0;
	bool MarkForFDelete = false; // not deleted by default
};
enum enMainMenueScreen {
	eShowClientList = 1,
	eAddNewClient = 2,
	eDeleteClient = 3,
	eUpdateClientInfo = 4,
	eFindClient = 5,
	eExit = 6
};

vector<string> SplitSrting(string S1, const string& Delim = "#//#") {

	vector<string> Vstrings;
	size_t pos = 0;
	string word;

	while ((pos = S1.find(Delim)) != string::npos) {
		word = S1.substr(0, pos);
		if (!word.empty()) {
			Vstrings.push_back(word);
		}
		S1.erase(0, pos + Delim.length());
	}
	if (!S1.empty()) {
		Vstrings.push_back(S1);
	}
	return Vstrings;
}

stClient ConvertLineToRecord(const string& Line, const string& Seperator = "#//#") {

	vector<string> Vstrings = SplitSrting(Line, Seperator);
	stClient Client;

	Client.AccountNumber = Vstrings[0];
	Client.PinCode = Vstrings[1];
	Client.Name = Vstrings[2];
	Client.Phone = Vstrings[3];
	Client.AccountBalance = stod(Vstrings[4]);

	return Client;
}

string ConvertRecordToline(stClient Client, const string& Seperator = "#//#") {

	return Client.AccountNumber + Seperator
		+ Client.PinCode + Seperator
		+ Client.Name + Seperator
		+ Client.Phone + Seperator
		+ to_string(Client.AccountBalance);
}

bool ClientExistsByAccountNumber(const string& AccountNumber, const string& fileName) {
	
	vector<stClient> VstClient;
	ifstream MyFile;
	MyFile.open(fileName, ios::in);

	if (MyFile.is_open()) {
		string Line;
		stClient Client;

		while (getline(MyFile, Line)) {

			Client = ConvertLineToRecord(Line);

			if (Client.AccountNumber == AccountNumber) {

				MyFile.close();
				return true;

			}
			VstClient.push_back(Client);
		}

		MyFile.close();
	}
	return false;
}

stClient ReadNewClient() {
	stClient Client;

	cout << "Enter Account Number? ";

	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, FileName)) {

		cout << "\nClient with Account Number [" << Client.AccountNumber << "] already exist. Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);

	}
	cout << "\nEnter Pin Code? ";
	getline(cin, Client.PinCode);

	cout << "\nEnter Name? ";
	getline(cin, Client.Name);

	cout << "\nEnter Phone? ";
	getline(cin, Client.Phone);

	cout << "\nEnter Balance? ";
	cin >> Client.AccountBalance;

	return Client;

}

vector<stClient> LoadClientDataFromFile(const string& fileName) {

	vector<stClient> VstClient;
	fstream MyFile;
	MyFile.open(fileName, ios::in);

	if(MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line)) {
			Client = ConvertLineToRecord(Line);
			VstClient.push_back(Client);
		}
		MyFile.close();
	}
	return VstClient;
}

void PrintClientRecord(stClient Client) {
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(25) << left << Client.Name;
	cout << "| " << setw(15) << left << Client.Phone;
	cout << "| " << setw(15) << left << Client.AccountBalance;
}


void ShowAllClientsScreen() {
    vector<stClient> VstClient = LoadClientDataFromFile(FileName);

    cout << "\n\t\t\t\t\tClient List (" << VstClient.size() << ") Client(s).\n";

    cout << "-------------------------------------------------------------------------------------------\n";
    cout << "| " << setw(15) << left << "Account Number"
         << "| " << setw(10) << left << "Pin Code"
         << "| " << setw(25) << left << "Name"
         << "| " << setw(15) << left << "Phone"
         << "| " << setw(15) << left << "Balance"
         << "\n";
    cout << "-------------------------------------------------------------------------------------------\n";

    for (const stClient& Client : VstClient) {
        PrintClientRecord(Client);
        cout << "\n";
    }

    cout << "-------------------------------------------------------------------------------------------\n";
}

void PrintClientCard(stClient Client) {

	cout << "\n========================================================================";
	cout << "\nAccount Number: " << Client.AccountNumber;
	cout << "\nPin Code: " << Client.PinCode;
	cout << "\nClient Name: " << Client.Name;
	cout << "\nClient Phone: " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n========================================================================";
}

bool FindClientByAccountNumber(const string& AccountNumber, const vector<stClient>& VstClient, stClient& OutClient) {

	for (const stClient& C : VstClient) {
		if (C.AccountNumber == AccountNumber) {
			OutClient = C;
			return true;
		}
	}
	return false;
}

stClient ChangeClientRecord(const string& AccountNumber) {

	stClient Client;

	while (!ClientExistsByAccountNumber(Client.AccountNumber, FileName)) {
		cout << "\nClient with Account Number [" << Client.AccountNumber << "] not exist. Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "\nEnter Pin Code? ";
	getline(cin, Client.PinCode);

	cout << "\nEnter Name? ";
	getline(cin, Client.Name);

	cout << "\nEnter Phone? ";
	getline(cin, Client.Phone);

	cout << "\nEnter Balance? ";
	cin >> Client.AccountBalance;

	return Client;

}

bool MarkClientForDeleteByAccountNumber(const string& AccountNumber, vector<stClient>& VstClient) {

	for (stClient& C : VstClient) {
		if (C.AccountNumber == AccountNumber) {
			C.MarkForFDelete = true;
			return true;
		}
	}

	return false;
}


vector<stClient> SaveClientDataToFile(const string& fileName, const vector<stClient>& VstClient) {
    ofstream MyFile(fileName, ios::out);

    if (!MyFile.is_open()) {
        throw runtime_error("Could not open file for writing: " + fileName);
    }

    for (const stClient& C : VstClient) {
        if (!C.MarkForFDelete) {
            MyFile << ConvertRecordToline(C) << '\n'; // write to file
        }
    }

    return VstClient;
}

void AddDatalineToFile(const string& fileName, const string& DataLine) {

	fstream MyFile;

	MyFile.open(fileName, ios::out | ios::app);

	if (MyFile.is_open()) {
		MyFile << DataLine << endl;

		MyFile.close();
	}
}

void AddNewClient() {

	stClient Client;
	Client = ReadNewClient();
	AddDatalineToFile(FileName, ConvertRecordToline(Client));

}

void AddNewClients() {

	stClient Client;
	char AddMore = 'Y';

	do {
		cout << "\nAdding New Client: ";
		AddNewClient();

		cout << "\nClient Added Successfully, do you want to add more Clients? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(const string& AccountNumber, vector<stClient>& VstClient) {
	stClient Client;
	char Answer = 'n';


	if (FindClientByAccountNumber(AccountNumber, VstClient, Client)) {
		PrintClientCard(Client);
	}
	cout << "\nAre you sure you want to delete this Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		MarkClientForDeleteByAccountNumber(AccountNumber, VstClient);
		SaveClientDataToFile(FileName, VstClient);
		VstClient = LoadClientDataFromFile(FileName); // Refresh Clients.

		cout << "\nClient Deleted Successfully. ";
		return true;
	}
	else {
		cout << "\nClient with Account Number [" << Client.AccountNumber << "] is Not Found!";
		return false;
	}
}

bool UpdateClientInfo(const string& AccountNumber, vector<stClient>& VstClient) {
	stClient Client;
	char Update = 'n';

	if (FindClientByAccountNumber(AccountNumber, VstClient, Client)) {
		PrintClientCard(Client);
	}

	cout << "\nAre you sure you want to update this Client? y/n? ";
	cin >> Update;
	if (toupper(Update) == 'Y') {

		for(stClient& C: VstClient) 
		{
			if(C.AccountNumber == AccountNumber)
			{
				C = ChangeClientRecord(AccountNumber);
				break;
			}
		}
		SaveClientDataToFile(FileName, VstClient);
		cout << "\nClient Upadted Successfully.";
		return true;
	}
	else {
		cout << "\nClient with Account Number [" << Client.AccountNumber << "] is Not Found!";
		return false;
	}
}

string ReadClientAccountNumber() {
	string AccoutNumber;
	cout << "\nPlease Enter Your Account Number? ";
	cin >> AccoutNumber;
	return AccoutNumber;
}

void ShowDeleteScreen() {

	cout << "\n----------------------------------------------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n----------------------------------------------------------------------\n";

	vector<stClient> VstClient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, VstClient);
}

void ShowUpdateClientInfoScreen() {

	cout << "\n----------------------------------------------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n----------------------------------------------------------------------\n";

	vector<stClient> VstClient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientInfo(AccountNumber, VstClient);
}

void ShowAddNewClient() {


	cout << "\n----------------------------------------------------------------------\n";
	cout << "\tAdd New Client Screen";
	cout << "\n----------------------------------------------------------------------\n";

	
	AddNewClients();
}

void ShowFindClientScreen() {

	cout << "\n----------------------------------------------------------------------\n";
	cout << "\t Find Client Screen";
	cout << "\n----------------------------------------------------------------------\n";

	stClient Client;
	vector<stClient> VstClient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();
	if(FindClientByAccountNumber(AccountNumber, VstClient, Client)) {
		PrintClientCard(Client);
	}
	else {
		cout << "\nClient with Account Number [" << AccountNumber << "] is Not found! ";
	}
}

void ShowEndScreen() {

	cout << "\n----------------------------------------------------------------------\n";
	cout << "\t End Program :-); ";
	cout << "\n----------------------------------------------------------------------\n";
	
}

void ShowMainMenue();
short ReadMainMenueOption();
void PerforMainMenueOption(enMainMenueScreen MainMenueScreenOptions);


void GoBackToMainManue() {

	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause > 0");
	ShowMainMenue();
}

short ReadMainMenueOption() {
	short Choice = 0;
	cout << "\nChoose What do you want to do? [1 to 6]? ";
	cin >> Choice;
	return Choice;
}

void PerforMainMenueOption(enMainMenueScreen MainMenueScreenOptions) {
	switch (MainMenueScreenOptions) {
	case enMainMenueScreen::eShowClientList:
	{
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainManue();
		break;
	}

	case enMainMenueScreen::eAddNewClient:
		system("cls");
		AddNewClients();
		GoBackToMainManue();
		break;

	case enMainMenueScreen::eDeleteClient:
		system("cls");
		ShowDeleteScreen();
		GoBackToMainManue();
		break;
	
	case enMainMenueScreen::eUpdateClientInfo:
		system("cls");
		ShowUpdateClientInfoScreen();
		GoBackToMainManue();
		break;

	case enMainMenueScreen::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainManue();
		break;

	case enMainMenueScreen::eExit:
		system("cls");
		ShowEndScreen();
		break;
	}
}

void ShowMainMenue() {

	cout << "=================================================================================\n";
	cout << "\t\t\t\t Main Menue Screen";
	cout << "\n=================================================================================\n";
	cout << "\t [1] Show Client List." << endl;
	cout << "\t [2] Add New Client." << endl;
	cout << "\t [3] Delete Client." << endl;
	cout << "\t [4] Update Client Info." << endl;
	cout << "\t [5] Find Client." << endl;
	cout << "\t [6] Exit." << endl;
	cout << "\=================================================================================\n";

	PerforMainMenueOption(static_cast<enMainMenueScreen>(ReadMainMenueOption()));
}





int main() {



	ShowMainMenue();



	system("pause>0");
	return 0;
}