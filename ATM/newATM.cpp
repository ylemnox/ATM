#include <iostream>
#include <string> 
#include <cassert>
#include <vector>
#include <map>
using namespace std;
class Bank;
class Account;
class Card;
class ATM;
class Transaction;
class Withdrawl;
class Deposit;
class Transfer;
class Session;

class Bank {
private:
	string bankName_;
	vector<Account*> accounts_; //account DB
	vector<Card*> issuedCards_; //card DB
	//bool primaryBank_; //for fee calculation
	long long funds_;
public:
	Bank(string bankName, /*bool primaryBank*/ long long initialfund);

	//getter, setter
	string getBankName() const { return bankName_; }
	//bool isPrimaryBank() const { return primaryBank_; }

	//Account Management
	void addAccount(Account* account);
	Account* findAccountByNumber(string accountNumber);
	vector<Account*> findAccountsByOwner(string owner);
	string findPasswordOfCard(string cardnumber);

};
class Account {
private:
	string accountBankName_;
	string accountNumber_; //12-digit
	string ownerName_;
	long balance_;
	vector<Transaction*> transactionHistory_;
	//bool primaryBank_;
public:
	Account(string bankName, string accountNumber, string ownerName, long initBalance);

	//getter, setter
	string getAccountBankName() { return accountBankName_; }
	string getAccountNumber() { return accountNumber_; }
	string getOwnerName() { return ownerName_; }
	long getBalance() { return balance_; }
	vector<Transaction*> getTransactionHistory() { return transactionHistory_; }
	//bool isPrimaryBank() const { return (primaryBank_ == 1) ? true : false; }

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
	Card(string cardNumber, string cardBankName, string accountNumber, string pw);
	//getter, setter
	string getCardNumber() { return cardNumber_; }
	string getCardBankName() { return cardBankName_; }
	string getAccountNumber() { return accountNumber_; }
	bool isAdminCard() { return adminCard_; }
	int getFailedAttempts() { return failedAttempts_; }
	void resetFailedAttempts() { failedAttempts_ = 0; }
	string getPassword() { return password_; }

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
	string primaryBankName_;
	Bank* ownerBank_;
	bool unilingual_;
	map<string,int> availableCash_;
	


public:
	ATM(int atmID, bool singleBank, string bankName, Bank* ownerBank, bool unilingual,  int fiftyK, int tenK, int fiveK, int oneK);
	
	//getter, setter
	long getAvailableCash();
	bool isSingleBank() { return singleBank_; } // return true if singlebank(only primary bank card), false if multibank(every bank card)
	//update availableCash when deposit, withdrawl
	bool updateAvailableCash(int fiftyK, int tenK, int fiveK, int oneK); //return update o,x
	//user authorization
	bool isVaildCard(Card* insertedCard);
	string getCardPasswordFromBank(string cardNumber);
};
class Transaction {
protected:
	int transID_;
	string transactionType_;
	ATM* atm_ = nullptr;
	//DateTime timestamp;
	//TransactionStatus status
	double amount_;
	double fee_;

public:
	static int nextID_;
	Transaction(double amount, double fee);
	virtual bool execute() = 0;
	virtual void validate() = 0;
	//virtual void calculateFee() = 0;
	void describe();
};
class Withdrawl :public Transaction {
private:
	map<string, int> withdrawlDenominations;
	long totalWithdralAmount_;
public:
	Withdrawl(double amount, double fee);
	map<string, int> distributeDenom(long amount);
	bool execute() override;
	//void calculateFee() override;
};
class Deposit :public Transaction{
private:
	bool isCash_; //True for cash, False for checks
	map<string, int> depositedCash;
	vector<double> depositedChecks;
	int numOfCashLimit_;
	int numOfCheckLimit_;
public:
	Deposit(bool isCash, double amount, int fiftyK, int tenK, int fiveK, int oneK, double fee);
	bool validateDeposit();
	bool execute() override=0;
	//void calculateFee() override=0;
};
class Transfer :public Transaction {
private:
	string sourceAccount_;
	string sourceBank_;
	string receiveAccount_;
	string receiveBank_;
	bool cashToAccount_; // true: cash->account, false account->account
public:
	Transfer(bool cashtoAccount, long amount, double fee, string receiveAccount, string receiveBank);
	void validateAccounts();
	bool execute() override;
	//void calculateFee() override;
};
class Session {
private:
	Card* insertedCard_;
	vector<Transaction*> transactions_;
	int wrongPasswordAttempts_;
	bool isAuthenticated_;
	ATM* atm_;
public:
	Session(ATM* currATM, Card* card);
	~Session();
	bool authenticate(string pw);
	void addTransaction(Transaction* currTransaction);
	void endSession(); //whenever user wants or no cash available
};

//Bank Member Function Defined-----------------------------------------------
Bank::Bank(string bankName, /*bool primaryBank*/ long long initialfund) {
	bankName_ = bankName;
	//primaryBank_ = primaryBank;
	funds_ = initialfund;
	cout << bankName_ << " created. Initial Funds: " << funds_ << "won\n";
}
void Bank::addAccount(Account* account) {
	accounts_.push_back(account);
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
string Bank::findPasswordOfCard(string cardnumber) {
	for (Card* card : issuedCards_) {
		if (card->getCardNumber() == cardnumber) return card->getPassword();
		cout << "NO card found in this Bank DB\n";
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
Card::Card(string cardNumber, string cardBankName, string accountNumber, string pw ) {
	cardNumber_ = cardNumber;
	cardBankName_ = cardBankName;
	accountNumber_ = accountNumber;
	password_ = pw;
	adminCard_ = false;
	failedAttempts_ = 0;
	cout << "Card Constructed | " << "Card Bank: " << cardBankName_ << " |Card Number: " << cardNumber_ << " | Account Number: " << accountNumber_ << endl;
}
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
ATM::ATM(int atmID, bool singleBank, string bankName, Bank* ownerBank, bool unilingual, int fiftyK, int tenK, int fiveK, int oneK) {
	atmID_ = atmID;
	singleBank_ = singleBank;
	unilingual_ = unilingual;
	primaryBankName_ = bankName;
	ownerBank_ = ownerBank;
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
bool ATM::updateAvailableCash(int fiftyK, int tenK, int fiveK, int oneK) { //input must include +,-
	//check if bills are sufficient
	if (availableCash_["50000won"] + fiftyK < 0) {
		cout << "Insufficient 50000won bills\n";
		return false;
	}
	if (availableCash_["10000won"] + tenK < 0) {
		cout << "Insufficient 10000won bills\n";
		return false;
	}
	if (availableCash_["5000won"] + fiveK < 0) {
		cout << "Insufficient 5000won bills\n";
		return false;
	}
	if (availableCash_["1000won"] + oneK < 0) {
		cout << "Insufficient 1000won bills\n";
		return false;
	}
	//update available cash
	availableCash_["50000won"] += fiftyK;
	availableCash_["10000won"] += tenK;
	availableCash_["5000won"] += fiveK;
	availableCash_["1000won"] += oneK;
	return true;
}
bool ATM::isVaildCard(Card* insertedCard) {
	// card - ATM
	if (isSingleBank()) {
		if (primaryBankName_ == insertedCard->getCardBankName()) {
			cout << "Invalid Card\n";
			cout << "This is a Single Bank ATM. Only card issued by " << primaryBankName_ << "is allowed.\n";
			return true;
		}
		else {
			cout << "Valid Card\n";
			return false;
		}
	}
	else {
		cout << "Valid Card\n";
		return true;
	}
}
string ATM::getCardPasswordFromBank(string cardNumber) {
	return ownerBank_->findPasswordOfCard(cardNumber);
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
	totalWithdralAmount_ = 0;
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
bool Withdrawl::execute() {
	
	//lower atm's available cash
	if (!atm_->updateAvailableCash(-withdrawlDenominations["50000won"], -withdrawlDenominations["10000won"], -withdrawlDenominations["5000won"], -withdrawlDenominations["1000won"])) {
		return false;
	}
	//if withdrawl session > 3 -> no withdrawl allowed, new withdrawl session
	
	// decrease account balance
	// 
	//dispense to user
	for (std::map<std::string, int>::iterator it = withdrawlDenominations.begin(); it != withdrawlDenominations.end(); ++it) {
		std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
	}
	// decrease ATM cash
	
	// fee charging from account
	return true;
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
	numOfCashLimit_ = 50;
	numOfCheckLimit_ = 30;
}
bool Deposit::validateDeposit() {
	if (isCash_) { //cash
		int total_num_of_cash = depositedCash["50000won"] + depositedCash["10000won"] + depositedCash["5000won"] + depositedCash["1000won"];
		if (total_num_of_cash > numOfCashLimit_) {
			cout << "Total amount of cash should be under 50" << endl;
			return false;
		}
		else return true;
	}
	else { //checks
		//total amount of checks should be under 30
	}	
}
bool Deposit::execute() {
	// fee charging by adding new cash
	// fee adding to depositedCash

	// add to account
	
	// add to ATM available cash
	if (isCash_) {
		atm_->updateAvailableCash(depositedCash["50000won"], depositedCash["10000won"], depositedCash["5000won"], depositedCash["1000won"]);
		cout << "Cash Deposit Completed" << endl;
		//account update using amount_
		return true;
	}
	else {
		//depositedCheck update
		//acount update using amount_
		return true;
	}
}

//void calculateFee() {}
//---------------------------------------------------------------------------

//Transfer Member Function Defined
Transfer::Transfer(bool cashtoAccount, long amount, double fee, string receiveAccount, string receiveBank):Transaction(amount, fee) {
	transactionType_ = "Transfer";
	amount_ = amount;
	fee_ = fee;
	receiveAccount_ = receiveAccount;
	receiveBank_ = receiveBank;
	cashToAccount_ = cashtoAccount;
}
void Transfer::validateAccounts() {
	//check if the sourceaccount and receiveaccount is both valid
}
bool Transfer:: execute() {
	if (cashToAccount_) { //cash-account
		// let user to insert cash with denomination + fee
		// (insert cash-fee) == amount?
		// update ATM available
		// update receiveaccount balance
		return true;
	}
	else { //account-account
		// let user to insert sourceAccount and Bank
		//sourceAccount_ = sourceAccount;
		//sourceBank_ = sourceBank;
		// fee charging from sourceaccount
		// update receiveaccount balance
		return true;
	}
}
//void Transfer::calculateFee() {}
//---------------------------------------------------------------------------

//Session Member Function Defined
Session::Session(ATM* currentATM, Card* card) {
	insertedCard_ = card;
	atm_ = currentATM;
	wrongPasswordAttempts_ = 0;
	isAuthenticated_ = false;
}
Session::~Session() {
	delete insertedCard_;
}
bool Session::authenticate(string pw) {
	while (wrongPasswordAttempts_ < 3) {
		if (pw == atm_->getCardPasswordFromBank(insertedCard_->getCardNumber())) {
			cout << "Correct Password\n";
			wrongPasswordAttempts_ = 0;
			return true;
		}
		else {
			cout << "Wrong Password" << endl;
			wrongPasswordAttempts_++;
			return false;
		}
	endSession();
	}
}
void Session::addTransaction(Transaction* currTransaction) {
	transactions_.push_back(currTransaction);
}
void Session::endSession() {
	//if complete transaction termination
	for (Transaction* transaction : transactions_) {
		transaction->describe();
	}

}
//---------------------------------------------------------------------------

int main() {
	cout << "Bank Initialization\n";
	bool bank_status = true;
	vector<Bank> banks;  // Vector to store all bank objects
	int bankCounter = 1; // Counter for naming banks

	while (bank_status) {
		string bankName;
		long long initialfund;
		cout << "Bank Name: ";
		cin >> bankName;
		cout << "Initial Funds of Bank: ";
		cin >> initialfund;

		// Create new bank object and store it in vector
		string objectName = "bank" + to_string(bankCounter);
		banks.push_back(Bank(bankName, initialfund));

		cout << "Created " << objectName << endl;
		cout << "---------------------------------------";
		bankCounter++;

		// More addition
		char continue_input;
		cout << "Add another bank? (y/n): ";
		cin >> continue_input;
		if (continue_input != 'y' && continue_input != 'Y') {
			bank_status = false;
		}
	}

	cout << "Account Initialization\n";
	bool account_status = true;
	vector<Account> accounts;  // Vector to store all bank objects
	int accountCounter = 1; // Counter for naming banks

	while (account_status) {
		string accbankName;
		string accountNumber;
		string ownerName;
		long initBalance;
		cout << "Bank Name: ";
		cin >> accbankName;
		cout << "Account Number: ";
		cin >> accountNumber;
		cout << "Owner Name: ";
		cin >> ownerName;
		cout << "Initial Balance: ";
		cin >> initBalance;

		string objectName = "account" + to_string(accountCounter);
		for (Bank bank : banks) {
			if (bank.getBankName() == accbankName) {
				accounts.push_back(Account(accbankName, accountNumber, ownerName, initBalance));
				bank.addAccount(&accounts[accountCounter-1]);
			}
			else {
				cout << "No Bank for this account exists\n";
			}
		}
		
		cout << "Created " << objectName << endl;
		cout << "---------------------------------------";
		accountCounter++;

		// More addition
		char continue_input;
		cout << "Add another account? (y/n): ";
		cin >> continue_input;
		if (continue_input != 'y' && continue_input != 'Y') {
			account_status = false;
		}
	}

	cout << "ATM Initialization\n";
	bool ATM_status = true;
	vector<ATM> ATMs;  // Vector to store all bank objects
	int ATMCounter = 1; // Counter for naming banks
	while (ATM_status) {
		int atmID;
		string isSingleBank;
		bool singleBank;
		string atmbankName;
		string lingual;
		bool unilingual;
		int fiftyK;
		int tenK;
		int fiveK;
		int oneK;

		cout << "Bank Name: ";
		cin >> atmbankName;
		cout << "ATM serial Number(6-digit serial number): ";
		cin >> atmID;
		cout << "Press S for Single Bank ATM, M for Multi Bank ATM: ";
		cin >> isSingleBank;
		if (isSingleBank == "S") singleBank = true;
		else if (isSingleBank == "M") singleBank = false;
		else {
			cout << "Wrong Selection\n";
			continue;
		}
		cout << "Unilingual or Bilingual (Press U for unilingual, B for Bilingual): ";
		cin >> lingual;
		if (lingual == "U") unilingual = true;
		else if (lingual == "B") unilingual = false;
		else {
			cout << "Wrong Selection\n";
			continue;
		}
		cout << "Number of 50000won bill: ";
		cin >> fiftyK;
		cout << "Number of 10000won bill: ";
		cin >> tenK;
		cout << "Number of 5000won bill: ";
		cin >> fiveK;
		cout << "Number of 1000won bill: ";
		cin >> oneK;

		string objectName = "ATM" + to_string(ATMCounter);

		for (Bank bank : banks) {
			if (bank.getBankName() == atmbankName) {
				Bank* ownerBank = &bank;
				ATMs.push_back(ATM(atmID, singleBank, atmbankName, ownerBank, unilingual, fiftyK, tenK, fiveK, oneK));
			}
			else {
				cout << "No Bank for this ATM exists\n";
			}
		}

		cout << "---------------------------------------";
		ATMCounter++;

		// More addition
		char continue_input;
		cout << "Add another ATM? (y/n): ";
		cin >> continue_input;
		if (continue_input != 'y' && continue_input != 'Y') {
			account_status = false;
		}
	}
	return 0;
}