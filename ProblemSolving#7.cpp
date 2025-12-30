//
// ProblemSolving#7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <limits>

using namespace std;

const string FileName = "C:\\Users\\lenovo\\Desktop\\Project01\\ProblemSolving#7\\Clients.txt";

enum enMainMenueScreen {
    ShowClientList = 1,
    AddNewClient = 2,
    DeleteClient = 3,
    UpdateClienrInfo = 4,
    FindClient = 5,
    Exit = 6
};

struct stClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false; // active by default
};

void DisplayMainMenueScreen() {
    cout << "==================================================\n";
    cout << "\t\t\tMain Menue Screen\n";
    cout << "==================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Exit.\n";
    cout << "==================================================\n";
}

vector<string> SplitString(string s, const string& delim = "#//#") {
    vector<string> parts;
    size_t pos = 0;

    while ((pos = s.find(delim)) != string::npos) {
        parts.push_back(s.substr(0, pos));
        s.erase(0, pos + delim.length());
    }
    parts.push_back(s);
    return parts;
}

stClient ConvertDataLineToRecord(const string& line, const string& seperator = "#//#") {
    vector<string> fields = SplitString(line, seperator);

    if (fields.size() != 5) {
        throw runtime_error("Invalid record format (expected 5 fields): " + line);
    }

    stClient client;
    client.AccountNumber = fields[0];
    client.PinCode = fields[1];
    client.Name = fields[2];
    client.Phone = fields[3];
    client.AccountBalance = stod(fields[4]);
    client.MarkForDelete = false;

    return client;
}

string ConvertRecordToLine(const stClient& client, const string& seperator = "#//#") {
    return client.AccountNumber + seperator
        + client.PinCode + seperator
        + client.Name + seperator
        + client.Phone + seperator
        + to_string(client.AccountBalance);
}

vector<stClient> LoadClientDatafromFile(const string& fileName) {
    vector<stClient> clients;
    ifstream file(fileName);

    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + fileName);
    }

    string line;
    int lineNumber = 0;
    while (getline(file, line)) {
        ++lineNumber;

        if (line.find_first_not_of(" \t\r\n") == string::npos)
            continue;

        try {
            clients.push_back(ConvertDataLineToRecord(line));
        }
        catch (const exception& ex) {
            throw runtime_error("Line " + to_string(lineNumber) + ": " + ex.what());
        }
    }

    return clients;
}

void SaveClientDataToFile(const string& fileName, const vector<stClient>& clients) {
    ofstream file(fileName, ios::out);

    if (!file.is_open()) {
        throw runtime_error("Could not open file for writing: " + fileName);
    }

    for (const stClient& c : clients) {
        if (!c.MarkForDelete) {
            file << ConvertRecordToLine(c) << '\n';
        }
    }
}

string ReadClientAccountNumber() {
    string accountNumber;
    cout << "Please enter the client account number: ";
    getline(cin >> ws, accountNumber);
    return accountNumber;
}

bool FindClientByAccountNumber(const vector<stClient>& clients, const string& accountNumber, stClient& outClient) {
    for (const stClient& c : clients) {
        if (c.AccountNumber == accountNumber) {
            outClient = c;
            return true;
        }
    }
    return false;
}

void PrintClientCard(const stClient& client) {
    cout << "\nThe Client Card\n";
    cout << "---------------------\n";
    cout << "Account Number : " << client.AccountNumber << "\n";
    cout << "Pin Code       : " << client.PinCode << "\n";
    cout << "Name           : " << client.Name << "\n";
    cout << "Phone          : " << client.Phone << "\n";
    cout << "Account Balance: " << fixed << setprecision(2) << client.AccountBalance << "\n";
    cout << "---------------------\n";
}

void ShowClientListScreen(vector<stClient>& clients) {
    cout << "\nClient List (" << clients.size() << " Clients)\n";
    cout << "=============================================================\n";
    cout << left
        << setw(15) << "Account"
        << setw(10) << "Pin"
        << setw(25) << "Name"
        << setw(15) << "Phone"
        << setw(12) << "Balance"
        << "\n";
    cout << "=============================================================\n";

    for (const stClient& c : clients) {
        cout << left
            << setw(15) << c.AccountNumber
            << setw(10) << c.PinCode
            << setw(25) << c.Name
            << setw(15) << c.Phone
            << setw(12) << fixed << setprecision(2) << c.AccountBalance
            << "\n";
    }

    cout << "=============================================================\n";
}

void AddNewClientScreen(vector<stClient>& clients) {
    string accountNumber = ReadClientAccountNumber();

    stClient existing;
    if (FindClientByAccountNumber(clients, accountNumber, existing)) {
        cout << "Client with account number (" << accountNumber << ") already exists.\n";
        return;
    }

    stClient client;
    client.AccountNumber = accountNumber;
    client.MarkForDelete = false;

    cout << "Please enter pin code: ";
    getline(cin >> ws, client.PinCode);

    cout << "Please enter client name: ";
    getline(cin, client.Name);

    cout << "Please enter phone number: ";
    getline(cin, client.Phone);

    cout << "Please enter account balance: ";
    cin >> client.AccountBalance;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    clients.push_back(client);
    SaveClientDataToFile(FileName, clients);

    cout << "Client added successfully.\n";
}

void DeleteClientScreen(vector<stClient>& clients) {
    const string accountNumber = ReadClientAccountNumber();

    stClient found;
    if (!FindClientByAccountNumber(clients, accountNumber, found)) {
        cout << "Client with account number (" << accountNumber << ") not found.\n";
        return;
    }

    PrintClientCard(found);

    char answer = 'n';
    cout << "\nAre you sure you want to delete this client? y/n? ";
    cin >> answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (answer != 'y' && answer != 'Y') {
        cout << "Delete cancelled.\n";
        return;
    }

    for (stClient& c : clients) {
        if (c.AccountNumber == accountNumber) {
            c.MarkForDelete = true; // mark it
            break;
        }
    }

    SaveClientDataToFile(FileName, clients);
    // optional: physically remove marked clients from memory:
    vector<stClient> kept;
    kept.reserve(clients.size());
    for (const stClient& c : clients) {
        if (!c.MarkForDelete) kept.push_back(c);
    }
    clients.swap(kept);

    cout << "Client deleted successfully.\n";
}

void UpdateClientInfoScreen(vector<stClient>& clients) {
    const string accountNumber = ReadClientAccountNumber();

    size_t idx = clients.size();
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i].AccountNumber == accountNumber) {
            idx = i;
            break;
        }
    }

    if (idx == clients.size()) {
        cout << "Client with account number (" << accountNumber << ") not found.\n";
        return;
    }

    PrintClientCard(clients[idx]);

    char answer = 'n';
    cout << "\nAre you sure you want to update this client? y/n? ";
    cin >> answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (answer != 'y' && answer != 'Y') {
        cout << "Update cancelled.\n";
        return;
    }

    cout << "Enter new pin code: ";
    getline(cin, clients[idx].PinCode);

    cout << "Enter new name: ";
    getline(cin, clients[idx].Name);

    cout << "Enter new phone: ";
    getline(cin, clients[idx].Phone);

    cout << "Enter new balance: ";
    cin >> clients[idx].AccountBalance;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    SaveClientDataToFile(FileName, clients);
    cout << "Client updated successfully.\n";
}

void FindClientScreen(const vector<stClient>& clients) {
    const string accountNumber = ReadClientAccountNumber();

    stClient found;
    if (!FindClientByAccountNumber(clients, accountNumber, found)) {
        cout << "Client with account number (" << accountNumber << ") not found.\n";
        return;
    }

    PrintClientCard(found);
}

enMainMenueScreen ReadMainMenuChoice() {
    int choice = 0;
    cout << "\nChoose what do you want to do? [1 to 6]? ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice < 1 || choice > 6) {
        return enMainMenueScreen::ShowClientList; // fallback (or keep asking)
    }

    return static_cast<enMainMenueScreen>(choice);
}

