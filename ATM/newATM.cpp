#include <iostream>
#include <string> 
#include <cassert>
#include <vector>
#include <map>
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
	bool singleBank_; //ATM type
	bool unilingual_;
	map<string,int> availableCash_;

public:
	ATM(int atmID, bool singleBank, bool unilingual, int fiftyK, int tenK, int fiveK, int oneK);
	
	//getter, setter
	long getAvailableCash();
	//update availableCash when deposit, withdrawl
	void updateAvailableCash(int fiftyK, int tenK, int fiveK, int oneK);
};

class Transaction {
protected:
	int transID_;
	string transactionType_;
	ATM* atm_;
	//DateTime timestamp;
	//TransactionStatus status
	double amount_;
	double fee_;

public:
	static int nextID_;
	Transaction(double amount, double fee);
	virtual void execute() = 0;
	virtual void validate() = 0;
	virtual void calculateFee() = 0;
	void describe();
};
class Withdrawl :public Transaction {
private:
	map<string, int> withdrawlDenominations;
public:
	Withdrawl(double amount, double fee);
	map<string, int> distributeDenom(long amount);
	void execute() override;
	void calculateFee() override;
};
class Deposit :public Transaction{
private:
	bool isCash_; //True for cash, False for checks
	map<string, int> depositedCash;
	vector<double> depositedChecks;
public:
	Deposit(bool isCash, double amount, int fiftyK, int tenK, int fiveK, int oneK, double fee);
	bool validateDeposit();
	void execute() override=0;
	void calculateFee() override=0;
};
class Transfer :public Transaction {
private:
	string sourceAccount_;
	string receiveAccount_;
	//transferType type;
public:
	Transfer(ATM* t_atm, string sourceAccount, string receiveAccount);
	void validateAccounts();
	void execute() override;
	void calculateFee() override;
};

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
	availableCash_["50000won"] = fiftyK;
	availableCash_["10000won"] = tenK;
	availableCash_["5000won"] = fiveK;
	availableCash_["1000won"] = oneK;
	cout << "ATM Constructed (ATM ID: " << atmID_ << ", Unilingual: " << unilingual_ << ")\n";
	cout << "Initial amount of Denomination\n";
	cout << "[50000won]: " << availableCash_["50000won"] << endl;
	cout << "[10000won]: " << availableCash_["10000won"] << endl;
	cout << "[5000won] : " << availableCash_["5000won"] << endl;
	cout << "[1000won] : " << availableCash_["1000won"] << endl;
}
long ATM::getAvailableCash() {
	return availableCash_["50000won"], availableCash_["10000won"], availableCash_["5000won"], availableCash_["1000won"];
}
void ATM::updateAvailableCash(int fiftyK, int tenK, int fiveK, int oneK) { //input must include +,-
	availableCash_["50000won"] += fiftyK;
	availableCash_["10000won"] += tenK;
	availableCash_["5000won"] += fiveK;
	availableCash_["1000won"] += oneK;
}
//---------------------------------------------------------------------------

//Transaction Member Function Defined
int Transaction::nextID_ = 0;
Transaction::Transaction(double amount, double fee) {
	transID_ = nextID_++;
	amount_ = amount;
	fee_ = fee;
}
void Transaction::describe() {
	cout << "Transaction ID: " << transID_ << " " << transactionType_ << "Amount: " << amount_ << endl;
	cout << "Fee: " << fee_ << endl;
}
//---------------------------------------------------------------------------

//Withdrawl Member Function Defined
Withdrawl::Withdrawl(double amount, double fee) :Transaction(amount, fee) {
	transactionType_ = "Withdrawl";
	amount_ = amount;
	fee_ = fee;
}
map<string,int> Withdrawl::distributeDenom(long amount) {
	if (amount < 1000) {
		cout << "The amount must be bigger than 1000" << endl;
		return withdrawlDenominations;
	}

	long temp = amount;
	if (temp > 50000) {
		withdrawlDenominations["50000 won"] = temp / 50000;
		temp -= withdrawlDenominations["50000 won"] * 50000;
	}
	if (temp > 10000) {
		withdrawlDenominations["10000 won"] = temp / 10000;
		temp -= withdrawlDenominations["10000 won"] * 10000;
	}
	if (temp > 5000) {
		withdrawlDenominations["5000 won"] = temp / 5000;
		temp -= withdrawlDenominations["5000 won"] * 5000;
	}
	if (temp > 1000) {
		withdrawlDenominations["1000 won"] = temp / 1000;
		if (temp % 1000 > 0) cout << "No coins serving";
	}

	return withdrawlDenominations;
}
void Withdrawl::execute() {
	//dispense to user
	for (std::map<std::string, int>::iterator it = withdrawlDenominations.begin(); it != withdrawlDenominations.end(); ++it) {
		std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
	}
	// decrease ATM cash
	atm_->updateAvailableCash(-withdrawlDenominations["50000won"], -withdrawlDenominations["10000won"], -withdrawlDenominations["5000won"], -withdrawlDenominations["1000won"]);
	// decrease account balance

}
//---------------------------------------------------------------------------

//Deposit Member Function Defined                                                                                             //how can I handle checks deposit???
Deposit::Deposit(bool isCash, double amount, int fiftyK, int tenK, int fiveK, int oneK, double fee):Transaction(amount, fee){
	transactionType_ = "Deposit";
	amount_ = amount;
	fee_ = fee;
	isCash_ = isCash; //True for cash, False for checks
	depositedCash["50000won"] = 0;
	depositedCash["10000won"] = 0;
	depositedCash["5000won"] = 0;
	depositedCash["1000won"] = 0;
	vector<double> depositedChecks;
}
bool Deposit::validateDeposit() {
	if (isCash_) { //cash
		int total_num_of_cash = depositedCash["50000won"] + depositedCash["10000won"] + depositedCash["5000won"] + depositedCash["1000won"];
		if (total_num_of_cash > 50) {
			cout << "Total amount of cash should be under 50" << endl;
			return false;
		}
		else return true;
	}
	else { //checks
		
	}	
}
void Deposit::execute() {
	depositedCash["50000 won"]
}

void calculateFee();
//---------------------------------------------------------------------------

//Transfer Member Function Defined
Transfer::Transfer(ATM* t_atm, string sourceAccount, string receiveAccount) :Transaction(t_atm) {
	transactionType_ = "Transfer";
}
//---------------------------------------------------------------------------