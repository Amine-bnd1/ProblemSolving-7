#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <limits>

using namespace std;

const string FileName = "C:\\Users\\lenovo\\Desktop\\Project01\\ProblemSolving#7\\Clients.txt";




void ShowMainMenueScreen();
void ShowTransactionsMenueScreen();




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
	eTransactions = 6,
	eExit = 7
};

enum enTransactionsMainScreen {
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBalance = 3,
    eMainMenue = 4
};



vector<string>SplitString(string S1, const string& Delim = "#//#") {

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

	stClient Client;
	vector<string> Vstrings = SplitString(Line, Seperator);

	Client.AccountNumber = Vstrings[0];
	Client.PinCode = Vstrings[1];
	Client.Name = Vstrings[2];
	Client.Phone = Vstrings[3];
	Client.AccountBalance = stod(Vstrings[4]);

	return Client;
}

string ConvertRecordToLine(stClient Client, const string& Seperator = "#//#") {

	return Client.AccountNumber + Seperator
		+ Client.PinCode + Seperator
		+ Client.Name + Seperator
		+ Client.Phone + Seperator
		+ to_string(Client.AccountBalance);
}

bool ClientExistsByAccountNumber(const string& AccountNumber, const string& fileName) {
	vector<stClient> VstClient;
	fstream MyFile;
	MyFile.open(fileName, ios::in);


	string Line;
	stClient Client;

	if (MyFile.is_open()) {
		while (getline(MyFile, Line)) {
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber) {
				MyFile.close();
				return true;
				
			}
		}

		MyFile.close();
	}

	return false;
}

stClient ReadNewClient() {

	stClient Client;

	cout << "\n\nPlease Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);



	while(ClientExistsByAccountNumber(Client.AccountNumber, FileName)) {
		cout << "\nClient with Account Number [" << Client.AccountNumber << "] already exists. Please enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "\nEnter Pin Code? ";
	getline(cin , Client.PinCode);

	cout << "\nEnter Client Name? ";
	getline(cin, Client.Name);

	cout << "\nEnter Client Phone? ";
	getline(cin, Client.Phone);

	cout << "\nEnter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;


}

vector<stClient>LoadClientDataFromFile(const string& fileName) {

	vector<stClient> VstClient;

	fstream MyFile;
	MyFile.open(fileName, ios::in);

	

	if (MyFile.is_open()) {
		string Line;
		stClient Client;

		while(getline(MyFile,Line)) {

			Client = ConvertLineToRecord(Line);
			VstClient.push_back(Client);
		}
		MyFile.close();
	}
	
	return VstClient;
}

void PrintClientRecordLine(const stClient& Client) {
    cout << "| " << left << setw(15) << Client.AccountNumber
         << "| " << left << setw(10) << Client.PinCode
         << "| " << left << setw(40) << Client.Name
         << "| " << left << setw(12) << Client.Phone
         << "| " << right << setw(12) << fixed << setprecision(2) << Client.AccountBalance
         << "|\n";
}

void ShowAllClientList() {
    const int wAcc = 15, wPin = 10, wName = 40, wPhone = 12, wBal = 12;

    // total width = columns + prefix "| " per column (2 chars) + 1 '|' per column at start
    // easiest: just print a long separator that is safely wide enough:
    const int totalWidth = 2 + wAcc + 2 + wPin + 2 + wName + 2 + wPhone + 2 + wBal + 1;

    auto PrintSeparator = [&]() {
        cout << string(totalWidth, '-') << "\n";
    };

    vector<stClient> VstClient = LoadClientDataFromFile(FileName);

    cout << "\n\t\t\t\tClient List (" << VstClient.size() << ") Client(s).\n\n";

    PrintSeparator();

    cout << "| " << left << setw(wAcc) << "Account Number"
         << "| " << left << setw(wPin) << "Pin Code"
         << "| " << left << setw(wName) << "Name"
         << "| " << left << setw(wPhone) << "Phone"
         << "| " << right << setw(wBal) << "Balance"
         << "|\n";

    PrintSeparator();

    if (VstClient.empty()) {
        cout << "No Clients Available In the System.\n";
    } else {
        for (const stClient& C : VstClient) {
            PrintClientRecordLine(C);
        }
    }

    PrintSeparator();
}

void PrintTotalBalancesRecord(stClient Client) {

	cout << "| " <<  setw(15) << left <<  Client.AccountNumber;
	cout << "| " <<  setw(40) << left << Client.Name;
	cout << "| " <<  setw(12) << left << Client.AccountBalance;
	cout << "\n";

}

void ShowTotalBalancesScreen() {
	stClient Client;

	vector<stClient> VstClient = LoadClientDataFromFile(FileName);

	cout << "\n\t\t\t\tBalances List (" << VstClient.size() << ") Client(s).";


	cout << "\n-----------------------------------------------------------------------------------------\n";
	cout << "| " << left << setw(15) << "Account Number ";
	cout << "| " << left << setw(40) << "Client Name ";
	cout << "| " << left << setw(12) << "Balance ";
	cout << "\n-----------------------------------------------------------------------------------------\n";



	double totalBalances = 0;

	if (VstClient.size() == 0) {
		cout << "\nNo Client Available In the System.";
	}
	
	else 
	{

		for (stClient Client : VstClient) {
			PrintTotalBalancesRecord(Client);
			cout << endl;

			totalBalances += Client.AccountBalance;

		}

		cout << "\n\t\t\t\tTotal Balances = " << totalBalances;
		cout << "\n-----------------------------------------------------------------------------------------\n";

	}
}


void PrintClientCard(stClient Client) {

	cout << "\n\nThe Following are the Client details: ";
	cout << "\n-------------------------------------------------------------------------";
	cout << "\nAccount Number: " << Client.AccountNumber;
	cout << "\nPin Code: " << Client.PinCode;
	cout << "\nName: " << Client.Name;
	cout << "\nPhone: " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-------------------------------------------------------------------------";

}

bool FindClientByAccountNumber(const string& AccountNumber,const vector<stClient>& VstClient,stClient& OutClient) {
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
    Client.AccountNumber = AccountNumber;

    cout << "\nEnter Pin Code? ";
    getline(cin >> ws, Client.PinCode);

    cout << "\nEnter Client Name? ";
    getline(cin, Client.Name);

    cout << "\nEnter Client Phone? ";
    getline(cin, Client.Phone);

    cout << "\nEnter Account Balance? ";
    cin >> Client.AccountBalance;

    return Client;
}


bool MarkClientForDeleteByAccountNumber(const string& AccountNumber, vector<stClient>& VstClient) {
    for (stClient& C : VstClient) {
        if (C.AccountNumber == AccountNumber) {
            C.MarkForFDelete = true; // FIX: assignment
            return true;
        }
    }
    return false;
}
 

vector<stClient>SaveClientDataToFile(const string& fileName, vector<stClient>& VstClient) {
	stClient Client;
	
	fstream MyFile;
	MyFile.open(fileName, ios::out);

	string DataLine;

	if (MyFile.is_open()) {

		for (stClient C : VstClient) {
			if(C.MarkForFDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return VstClient;
}


void AddDataLineToFile(const string& fileName, const string& stDataLine) {

	fstream MyFile;
	MyFile.open(fileName, ios::out | ios::app);

	if (MyFile.is_open()) {

		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

void AddNewClient() {
	stClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(FileName, ConvertRecordToLine(Client));
}

void AddNewClients() {

	
	char AddMore = 'n';

	do 
	{
		cout << "\nAdding New Client: ";
		AddNewClient();

		cout << "\n\nClient Added Successfully, do you want to add more Clients? y/n? ";
		cin >> AddMore;
	}
	while(toupper(AddMore) == 'Y');
}

bool DeleteClientByAccountNumber(const string& AccountNumber, vector<stClient>& VstClient) {
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, VstClient, Client)) {
		
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want to delete this client? y/n? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y') {

			MarkClientForDeleteByAccountNumber(AccountNumber, VstClient);
			SaveClientDataToFile(FileName, VstClient);
			VstClient = LoadClientDataFromFile(FileName);
			cout << "\n\nClient Deleted Successfully. ";
			return true;
		}
	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << " ) is Not Found! ";
		return false;
	}
}

bool UpdateClientByAccountNumber(const string& AccountNumber, vector<stClient>& VstClient) {
    stClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, VstClient, Client)) {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want to update this client? y/n? ";
        cin >> Answer;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // important

        if (toupper(Answer) == 'Y') {
            for (stClient& C : VstClient) {
                if (C.AccountNumber == AccountNumber) {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientDataToFile(FileName, VstClient);
            cout << "\nClient Updated Successfully. ";
            return true;
        }
        return false;
    }

    cout << "\nClient with Account Number (" << AccountNumber << " ) is Not Found! ";
    return false;
}

bool DepositByAccountNumber(const string& AccountNumber, double Amount, vector<stClient>& VstClient)
{
	for (stClient& C : VstClient) { // reference -> modifies the real element
		if (C.AccountNumber == AccountNumber) {
			C.AccountBalance += Amount;               // Amount can be + (deposit) or - (withdraw)
			SaveClientDataToFile(FileName, VstClient); // save updated balances
			return true;
		}
	}
	return false;
}

string ReadClientAccountNumber() {
	string AccountNumber;
	cout << "\nPlease Enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}

void ShowDepositScreen() {

	cout << "\n-----------------------------------------------------------------------";
	cout << "\n\t\Deposit Screen ";
	cout << "\n-----------------------------------------------------------------------";

	vector<stClient> VstClient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();

	stClient Client;
	if (!FindClientByAccountNumber(AccountNumber, VstClient, Client)) {
		cout << "\nClient with [" << AccountNumber << "] does not exist.";
		return;
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease Enter deposit amount? ";
	cin >> Amount;

	char Answer = 'n';
	cout << "\nAre You sure you want to perform this transaction? y/n? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y' && Amount > 0) {
		DepositByAccountNumber(AccountNumber, Amount, VstClient);
		cout << "\nDone Successfully.";
	}
	else {
		cout << "\nTransaction Cancelled! ";
	}
}

void ShowWitdrawScreen() {
	cout << "\n-----------------------------------------------------------------------";
	cout << "\n\t\tWithdraw Screen ";
	cout << "\n-----------------------------------------------------------------------";

	stClient Client;
	vector<stClient> VstClient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, VstClient, Client)) {
		cout << "\nClient with Account Number [" << AccountNumber << "] does not exist!";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter withdraw amount? ";
	cin >> Amount;

	while (Amount <= 0) {
		cout << "\nInvalid amount, enter a positive amount? ";
		cin >> Amount;
	}

	while (Amount > Client.AccountBalance) {
		cout << "\nAmount Exceeds the balance, you can withdraw up to: " << Client.AccountBalance;
		cout << "\nPlease enter another amount? ";
		cin >> Amount;
	}

	char Answer = 'n';
	cout << "\n\nAre you sure you want to perform this transaction? y/n? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y') {
		DepositByAccountNumber(AccountNumber, Amount*-1, VstClient); // withdraw = negative deposit
		cout << "\nWithdraw Done Successfully.";
	}
	else {
		cout << "\nTransaction Cancelled! ";
	}
}


void ShowDeleteClientScreen() {

	cout << "\n-----------------------------------------------------------------------";
	cout << "\n\t\tDelete Client Screen ";
	cout << "\n-----------------------------------------------------------------------";

	vector<stClient> VstClient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();

	DeleteClientByAccountNumber(AccountNumber, VstClient);
}

void ShowUpdateClientScreen() {
	cout << "\n-----------------------------------------------------------------------";
	cout << "\n\t\tUpdate Client Info Screen ";
	cout << "\n-----------------------------------------------------------------------";

	vector<stClient> VstClient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();

	UpdateClientByAccountNumber(AccountNumber, VstClient);
}

void ShowAddNewClientsScreen() {
	cout << "\n-----------------------------------------------------------------------";
	cout << "\n\t\Add New Client Screen ";
	cout << "\n-----------------------------------------------------------------------";

	AddNewClients();
}

void ShowFindClientScreen() {
	cout << "\n-----------------------------------------------------------------------";
	cout << "\n\t\Find Client Screen ";
	cout << "\n-----------------------------------------------------------------------";

	stClient Client;
	vector<stClient> VstClient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadClientAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, VstClient, Client)) {
		PrintClientCard(Client);
	}
	else {
		cout << "\n\nClient with Account Number (" << AccountNumber << ") is Not Found! ";
	}
}
void ShowMainMenue() {
	ShowMainMenueScreen();
}

void ShowEndProgramScreen() {
	cout << "\n-----------------------------------------------------------------------";
	cout << "\n\t\End Program; GoodBye! -);";
	cout << "\n-----------------------------------------------------------------------";

}

void GoBackToMainMenue() {
	cout << "\n\nPress any key to go back to Main Menue... ";
	system("pause>0");
	ShowMainMenueScreen();
}
void GoBackToTransactionMenue() {
	cout << "\n\nPress any key to go back to Main Menue... ";
	system("pause>0");
	ShowTransactionsMenueScreen();
}

short ReadMainMenueOptions() {
	short Choice = 0;
	cout << "\nChoose what you want to do? [1 to 7]? ";
	cin >> Choice;
	return Choice;
}

short ReadTransactionMenueOptions() {
	short tChoice = 0;
	cout << "\nChoose what you want to do? [1 to 4]? ";
	cin >> tChoice;
	return tChoice;
}

void PerformTransactionMenue(enTransactionsMainScreen TransactionOptions) {

	switch (TransactionOptions) {
	case enTransactionsMainScreen::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionMenue();
		break;

	case enTransactionsMainScreen::eWithdraw:
		system("cls");
		ShowWitdrawScreen();
		GoBackToTransactionMenue();
		break;

	case enTransactionsMainScreen::eTotalBalance:
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionMenue();
		break;

	case enTransactionsMainScreen::eMainMenue:
		system("cls");
		GoBackToMainMenue();
		break;

	}

}


void PerformMainMenueOptions(enMainMenueScreen MainMenueOptions) {
	switch (MainMenueOptions) {

	case enMainMenueScreen::eShowClientList: {

		system("cls");
		ShowAllClientList();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueScreen::eAddNewClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueScreen::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueScreen::eUpdateClientInfo:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueScreen::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueScreen::eTransactions:
		system("cls");
		ShowTransactionsMenueScreen();
		break;

	case enMainMenueScreen::eExit:
		system("cls");
		ShowEndProgramScreen();
		break;
	}

}


void ShowMainMenueScreen() {

	system("cls");

	cout << "==============================================================================================\n";
	cout << "\t\t\t\tMain Menue Screen\n";
	cout << "==============================================================================================\n";
	cout << "\t [1] Show Client List.\n";
	cout << "\t [2] Add New Client.\n";
	cout << "\t [3] Delete Client.\n";
	cout << "\t [4] Update Client Info.\n";
	cout << "\t [5] Find Client.\n";
	cout << "\t [6] Transactions.\n";
	cout << "\t [7] Exit.\n";
	cout << "==============================================================================================\n";

	PerformMainMenueOptions((enMainMenueScreen)ReadMainMenueOptions());
}

void ShowTransactionsMenueScreen() {

	system("cls");

	cout << "==============================================================================================\n";
	cout << "\t\t\t\Transaction Menue Screen\n";
	cout << "\t [1] Deposit.\n";
	cout << "\t [2] Wirhdraw.\n";
	cout << "\t [3] Total Balances.\n";
	cout << "\t [4] Main Menue.\n";
	cout << "==============================================================================================\n";

	PerformTransactionMenue((enTransactionsMainScreen)ReadTransactionMenueOptions());

}



int main() {


	ShowMainMenueScreen();


	system("pause>0");
	return 0;
}