#include <iostream>
#include <string> 
#include <cassert>
#include <vector>
using namespace std;

class Bank {
private:
	string bankName_;
	vector<Account*> accounts_; //account DB
	vector<Card*> issuedCards_; //card DB
	bool primaryBank_; //for fee calculation
public:
	Bank(string bankName, bool primaryBank);

	//getter, setter
	string getBankName() const { return bankName_; }
	bool isPrimaryBank() const { return primaryBank_; }

	//Account Management
	Account* findAccountByNumber(string accountNumber);
	vector<Account*> findAccountsByOwner(string owner);
};
class Account {
private:
	string accountBankName_;
	string accountNumber_; //12-digit
	string ownerName_;
	long balance_;
	vector<Transaction*> transactionHistory_;
	bool primaryBank_;
public:
	Account(string bankName, string accountNumber, string ownerName, long initBalance);

	//getter, setter
	string getAccountBankName() { return accountBankName_; }
	string getAccountNumber() { return accountNumber_; }
	string getOwnerName() { return ownerName_; }
	long getBalance() { return balance_; }
	vector<Transaction*> getTransactionHistory() { return transactionHistory_; }
	bool isPrimaryBank() const { return (primaryBank_ == 1) ? true : false; }

	//Balance Management
	void accountBalanceUpdate(string transactionType, long amount);

	//validate account number
	static bool validateAccountNumber(string accountNumber);
};
class Card {
private:
	string cardNumber_;
	string cardBankName_;
	string accountNumber_;
	string password_;
	bool adminCard_;
	int failedAttempts_ = 0;
public:
	Card(string cardNumber, string cardBankName, string accountNumber, string pw, bool isAdmin = false);

	//getter, setter
	string getCardNumber() { return cardNumber_; }
	string getCardBankName() { return cardBankName_; }
	string getAccountNumber() { return accountNumber_; }
	bool isAdminCard() { return adminCard_; }
	int getFailedAttempts() { return failedAttempts_; }
	void resetFailedAttempts() { failedAttempts_ = 0; }

	//validation
	bool validatePassword(string pw);
	bool isLocked();
	bool isValidForATM(string atmBankName, bool isMultiBankATM);
	//isAdmin: in ATM
};
class ATM {
private:
	int atmID_; //6-digit
	bool singleBank_;
	bool unilingual_;
	int denom[4];
public:
	ATM(int atmID, bool singleBank, bool unilingual, int fiftyK, int tenK, int fiveK, int oneK);

};

class Transaction {};
class Withdrawl {};
class Deposit {};
class Transfer {};

void createATM();
void createBank();

//Bank Member Function Defined-----------------------------------------------
Bank::Bank(string bankName, bool primaryBank) {
	bankName_ = bankName;
	primaryBank_ = primaryBank;
}
Account* Bank::findAccountByNumber(string accountNumber) {
	for (Account* acc : accounts_) {
		if (acc->getAccountNumber() == accountNumber) return acc;
	}
	cout << accountNumber << " account is not found" << endl;
	return nullptr;
}
vector<Account*> Bank::findAccountsByOwner(string owner) {
	vector<Account*> temp;
	for (Account* acc : accounts_) {
		if (acc->getOwnerName() == owner) temp.push_back(acc);
	}

	if (!temp.empty()) return temp;
	else {
		cout << owner << "'s account is not found" << endl;
		return temp;
	}
}
//---------------------------------------------------------------------------

//Account Member Function Defined--------------------------------------------
Account::Account(string bankName, string accountNumber, string ownerName, long initBalance) {
	accountBankName_ = bankName;
	accountNumber_ = accountNumber;
	ownerName_ = ownerName;
	balance_ = initBalance;
}
void Account::accountBalanceUpdate(string transactionType, long amount) {
	if (transactionType == "withdrawl") {
		balance_ -= amount;
	}
	if (transactionType == "deposit") {
		balance_ += amount;
	}
	if (transactionType == "transfer") {
		balance_ -= amount;
	}
}
bool Account::validateAccountNumber(string accountNum) {
	if (atoi(accountNum.c_str()) != 0 && accountNum.length() == 12) return true;
	else return false;
}
//---------------------------------------------------------------------------

//Card Member Function Defined-----------------------------------------------
bool Card::validatePassword(string pw) {
	if (pw == password_) return true;
	else {
		failedAttempts_++;
		return false;
	}
}
bool Card::isValidForATM(string atmBankName, bool isMultiBankATM) {
	if (isMultiBankATM) return true;
	else {
		if (atmBankName == cardBankName_) return true;
		else return false;
	}
}
bool Card::isLocked() {
	if (failedAttempts_ >= 3) return true;
	else return false;
}
//---------------------------------------------------------------------------

//ATM Member Function Defined
ATM::ATM(int atmID, bool singleBank, bool unilingual, int fiftyK, int tenK, int fiveK, int oneK) {
	atmID_ = atmID;
	singleBank_ = singleBank;
	unilingual_ = unilingual;
	denom[0] = fiftyK;
	denom[1] = tenK;
	denom[2] = fiveK;
	denom[3] = oneK;
	cout << "ATM Constructed (ATM ID: " << atmID_ << ", Unilingual: " << unilingual_ << ")\n";
	cout << "Initial amount of Denomination\n";
	cout << "[50000]: " << denom[0] << endl;
	cout << "[10000]: " << denom[1] << endl;
	cout << "[5000] : " << denom[2] << endl;
	cout << "[1000] : " << denom[3] << endl;
}