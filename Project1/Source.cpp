#pragma warning(disable : 4996)
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <Windows.h>
#include <iomanip>
using namespace std;
struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string PhoneNumber;
    float AccountBalance;
    bool MarkForDelete = false;
};
stClient convertLineToRecord(string, string);
bool isFoundClient(string, stClient&);
void printTraverseToMenu();
void printTraverseToTransactions();
void mainMenu();
int choice(int First, int Last);
void mainTransactions();
vector <stClient> saveCleintsDataToFile(string FileName, vector<stClient> vClient);
float enterDepositAmount();
bool assuringForTransaction();
float enterWithdrawAmount();
vector<stClient> loadDataFromFileToVector(string FileName)
{
    fstream MyFile;
    vector<stClient> vClients;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        stClient Client;
        while (getline(MyFile, Line))
        {
            Client = convertLineToRecord(Line, "||");
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}
void saveVectorToFile(string FileName, vector <string> vFileContent)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        for (string& Lines : vFileContent)
        {
            MyFile << Lines << endl;
        }
        MyFile.close();
    }
}
void UpdateRecordInFile(string FileName, string RecordFrom, string RecordTo)
{
    vector<string> vFileContent;
    loadDataFromFileToVector(FileName);
    for (string& Lines : vFileContent)
    {
        if (Lines == RecordFrom)
        {
            Lines = RecordTo;
        }
    }
    saveVectorToFile(FileName, vFileContent);
}
void printFileContent(string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Lines;
        while (getline(MyFile, Lines)) {
            if (Lines != "")
                cout << Lines << endl;
        }
        MyFile.close();
    }
}
vector <string> splitString(string Str, string Delim)
{
    vector <string> vStr;
    int Position = 0;
    string sWord;
    while ((Position = Str.find(Delim)) != std::string::npos)
    {
        sWord = Str.substr(0, Position);
        if (sWord != "")
            vStr.push_back(sWord);
        Str.erase(0, Position + Delim.length());
    }
    if (Str != "")
    {
        vStr.push_back(Str);
    }
    return vStr;
}
string joinString(vector<string>& vStr, string Delim)
{
    string Str;
    for (string& s : vStr)
    {
        Str += s + Delim;
    }
    Str.erase(Str.length() - Delim.length(), Delim.length());
    return Str;

}
void changeBalanceByAccountNumber(stClient Client, string AccountNumber)
{
    vector<stClient> vClient;
    vClient = loadDataFromFileToVector("ClientsRecord.txt");
    for (stClient& s : vClient)
    {
        if (s.AccountNumber == AccountNumber)
        {
            s.AccountBalance = Client.AccountBalance;
        }
    }
    vClient = saveCleintsDataToFile("ClientsRecord.txt", vClient);
}
string convertRecordToLine(stClient C1, string Delim)
{
    string S = "";
    S += C1.AccountNumber + Delim;
    S += C1.PinCode + Delim;
    S += C1.Name + Delim;
    S += C1.PhoneNumber + Delim;
    S += to_string(C1.AccountBalance);
    return S;
}
stClient readNewClient()
{
    stClient E1;
    cout << "Enter Account Number?" << endl;
    getline(cin >> ws, E1.AccountNumber);
    cout << "Enter Pincode?" << endl;
    getline(cin, E1.PinCode);
    cout << "Enter Name?" << endl;
    getline(cin, E1.Name);
    cout << "Enter Phone?" << endl;
    getline(cin, E1.PhoneNumber);
    cout << "Enter AccountBalance" << endl;
    cin >> (cin, E1.AccountBalance);

    return E1;
}
stClient readNewClient(bool Update)
{
    stClient E1;
    cout << "Enter Pincode?" << endl;
    getline(cin >> ws, E1.PinCode);
    cout << "Enter Name?" << endl;
    getline(cin, E1.Name);
    cout << "Enter Phone?" << endl;
    getline(cin, E1.PhoneNumber);
    cout << "Enter AccountBalance" << endl;
    cin >> (cin, E1.AccountBalance);
    return E1;
}
stClient convertLineToRecord(string Line, string Delim)
{
    stClient C;
    vector<string> vStr = splitString(Line, Delim);
    C.AccountNumber = vStr[0];
    C.PinCode = vStr[1];
    C.Name = vStr[2];
    C.PhoneNumber = vStr[3];
    C.AccountBalance = stof(vStr[4]);
    return C;
}
void printClientRecord(stClient& Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.PhoneNumber;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}
void printClientRecordForBalance(stClient& Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}
bool loadLineToFile(string Filename, string Line)
{
    fstream MyFile;
    MyFile.open(Filename, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << Line << endl;
        MyFile.close();
        return true;
    }
    else
    {
        MyFile.close();
        return false;
    }
}
void addClient()
{
    bool ContinueAddingClients = true;
    while (ContinueAddingClients)
    {
        char K;
        stClient C1;
        do {
            C1 = readNewClient();
            if (isFoundClient(C1.AccountNumber, C1))
            {
                cout << "This client is already in our system! try enter another number\n";
            }
            else
                break;
        } while (1);
        string Line = convertRecordToLine(C1, "||");
        if (loadLineToFile("ClientsRecord.txt", Line)) {
            cout << "Client added to the file successfully! Do you want to add more clients ?Y/N" << endl;
            cin >> K;
            K = tolower(K);
            if (K != 'y')
                ContinueAddingClients = false;
        }
        else {
            cout << "Client is not added to our system!" << endl;
            break;
        }
    }
    printTraverseToMenu();
}
void PrintAllClientsData(vector <stClient> vClients)
{
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (stClient& Client : vClients)
    {
        printClientRecord(Client);
        cout << endl;
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}
void printBalanceMenu(vector<stClient> vClients)
{
    float Sum = 0;
    cout << "\n\t\t\t\t\tBalance List (" << vClients.size() << ") Client(s).";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (stClient& Client : vClients)
    {
        printClientRecordForBalance(Client);
        Sum += Client.AccountBalance;
        cout << endl;
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n";
    cout << "\t\t\tTotal Balances is " << Sum << endl;
}
void showClientList()
{
    vector<stClient>vClients;
    vClients = loadDataFromFileToVector("ClientsRecord.txt");
    PrintAllClientsData(vClients);
    printTraverseToMenu();
}
bool isFoundClient(string AccountNumber, stClient& Client)
{
    vector<stClient> vClients;
    vClients = loadDataFromFileToVector("ClientsRecord.txt");
    for (stClient& s : vClients)
    {
        if (s.AccountNumber == AccountNumber)
        {
            Client = s;
            return true;
        }
    }
    return false;
}
void searchForClient()
{
    string AccountNumber;
    stClient Client;
    cout << "Enter the account number: \n";
    cin >> AccountNumber;
    if (isFoundClient(AccountNumber, Client))
    {
        cout << "The following are the client details: \n";
        printClientRecord(Client);
        printTraverseToMenu();
    }
    else
    {
        cout << "Client with account number (" << AccountNumber << ") is not found!" << endl;
        printTraverseToMenu();
    }
}
void searchForClientToDeposit()
{
    string AccountNumber;
    stClient Client;
    do {
        cout << "Enter the account number: \n";
        cin >> AccountNumber;
        if (isFoundClient(AccountNumber, Client))
        {
            cout << "The following are the client details: \n";
            printClientRecord(Client);
            break;
        }
        else
            cout << "Client with account number (" << AccountNumber << ") is not found!" << endl;
    } while (1);
    float Balance = enterDepositAmount();
    if (assuringForTransaction())
    {
        Client.AccountBalance += Balance;
        changeBalanceByAccountNumber(Client, AccountNumber);
        cout << "Your transcation was done successfully! the new balance now is " << Client.AccountBalance << endl;
        printTraverseToTransactions();
    }
    else
        printTraverseToTransactions();
}
void searchForClientToWithdraw()
{
    string AccountNumber;
    stClient Client;
    do {
        cout << "Enter the account number: \n";
        cin >> AccountNumber;
        if (isFoundClient(AccountNumber, Client))
        {
            cout << "The following are the client details: \n";
            printClientRecord(Client);
            break;
        }
        else
            cout << "Client with account number (" << AccountNumber << ") is not found!" << endl;
    } while (1);
    do {
        float Balance = enterWithdrawAmount();
        if (assuringForTransaction())
        {
            if (Balance <= Client.AccountBalance)
            {
                Client.AccountBalance -= Balance;
                changeBalanceByAccountNumber(Client, AccountNumber);
                cout << "Your transcation was done successfully! the new balance now is " << Client.AccountBalance << endl;
                break;
            }
            else
            {
                cout << "You don't have enough money to withdraw! try to enter another amount!\n";
            }
        }
        else
            break;
    } while (1);
    printTraverseToTransactions();
}
bool markClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vClient)
{
    for (stClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
string readClientAccountNumber()
{
    string AccountNumber = "";
    cout << "Please enter AccountNumber? \n";
    cin >> AccountNumber;
    return AccountNumber;
}
vector <stClient> saveCleintsDataToFile(string FileName, vector<stClient> vClient)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string DataLine;
    if (MyFile.is_open())
    {
        for (stClient C : vClient)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = convertRecordToLine(C, "||");
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClient;
}

bool findClientByAccountNumber(string AccountNumber, vector<stClient> vClient, stClient& Client)
{
    for (stClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
bool deleteClientByAccountNumber(string AccountNumber, vector<stClient>& vClient)
{
    stClient Client;
    char Answer = 'n';
    if (findClientByAccountNumber(AccountNumber, vClient, Client))
    {
        printClientRecord(Client);
        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            markClientForDeleteByAccountNumber(AccountNumber, vClient);
            saveCleintsDataToFile("ClientsRecord.txt", vClient);
            vClient = loadDataFromFileToVector("ClientsRecord.txt");
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber
            << ") is Not Found!";
        return false;
    }
}
vector<stClient> updateClientData(vector<stClient>& vClient, stClient Client)
{
    for (stClient& s : vClient)
    {
        if (s.AccountNumber == Client.AccountNumber)
            s = readNewClient(1);
    }
    return vClient;
}
bool updateClientByAccountNumber(string AccountNumber, vector<stClient>& vClient)
{
    stClient Client;
    char Answer = 'n';
    if (findClientByAccountNumber(AccountNumber, vClient, Client))
    {
        cout << "The client data is:\n";
        printClientRecord(Client);
        cout << "\n\nAre you sure you want to update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            vClient = updateClientData(vClient, Client);
            saveCleintsDataToFile("ClientsRecord.txt", vClient);
            cout << "The client data was updated successfully!\n";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber
            << ") is Not Found!";
        return false;
    }
}
void deleteClient()
{
    string AccountNumber = readClientAccountNumber();
    vector<stClient> vClient = loadDataFromFileToVector("ClientsRecord.txt");
    deleteClientByAccountNumber(AccountNumber, vClient);
    printTraverseToMenu();
}
void updateClient()
{
    string AccountNumber = readClientAccountNumber();
    vector<stClient> vClient = loadDataFromFileToVector("ClientsRecord.txt");
    updateClientByAccountNumber(AccountNumber, vClient);
    printTraverseToMenu();
}
bool assuringForTransaction()
{
    char C;
    cout << "Are you sure you want to perform this transaction? Y/N" << endl;
    cin >> C;
    C = tolower(C);
    if (C == 'y')
        return true;
    else
        return false;
}
float enterDepositAmount()
{
    float Sum;
    cout << "\nPlease enter deposit amount? ";
    cin >> Sum;
    return Sum;
}
float enterWithdrawAmount()
{
    float Sum;
    cout << "\nPlease enter withdraw amount?";
    cin >> Sum;
    return Sum;
}
void deposit()
{
    cout << "--------------------------------\n";
    cout << "\tDeposit screen\n";
    cout << "--------------------------------\n";
    searchForClientToDeposit();
}
void withdraw()
{
    cout << "--------------------------------\n";
    cout << "\tWithdraw screen\n";
    cout << "--------------------------------\n";
    searchForClientToWithdraw();
}
void showBalanceMenu()
{
    vector<stClient> vClient = loadDataFromFileToVector("ClientsRecord.txt");
    printBalanceMenu(vClient);
    printTraverseToTransactions();
}
void transactions()
{
    system("cls");
    cout << "================================\n";
    cout << "\tTransactions menu screen\t\t" << endl;
    cout << "================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main menu.\n";
}
void printTraverseToMenu()
{
    cout << "\nPress any key to go back to main menu" << endl;
    system("pause");
    mainMenu();
}
void printTraverseToTransactions()
{
    cout << "\n Press any key to go back to transactions menu" << endl;
    system("pause");
    mainTransactions();
}
int choice(int First, int Last)
{
    cout << "Choose what do you want to do? [" << First << " to " << Last << "] ?" << endl;
    int Choose;
    cin >> Choose;
    return Choose;
}
void printMenu()
{
    system("cls");
    cout << "================================\n";
    cout << "\tMain menu screen\t\t" << endl;
    cout << "================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add A New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Exit.\n";
    cout << "================================\n";
}
void transactionsChoices()
{
    switch (choice(1, 4))
    {
    case 1:
        system("cls");
        deposit();
        break;
    case 2:
        system("cls");
        withdraw();
        break;
    case 3:
        system("cls");
        showBalanceMenu();
        break;
    case 4:
        printTraverseToMenu();
        break;
    default:
        system("cls");
        printTraverseToMenu();
        break;
    }
}
void menuChoices()
{
    switch (choice(1, 7))
    {
    case 1:
        system("cls");
        showClientList();
        break;
    case 2:
        system("cls");
        addClient();
        break;
    case 3:
        system("cls");
        deleteClient();
        break;
    case 4:
        system("cls");
        updateClient();
        break;
    case 5:
        system("cls");
        searchForClient();
        break;
    case 6:
        system("cls");
        mainTransactions();
        break;
    case 7:
        system("cls");
        break;
    default:
        system("cls");
        break;
    }
}
void mainTransactions()
{
    transactions();
    transactionsChoices();
}
void mainMenu()
{
    printMenu();
    menuChoices();
}
int main()
{
    mainMenu();
    return 0;
}