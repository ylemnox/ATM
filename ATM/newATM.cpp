#include <iostream>
#include <string> 
#include <cassert>
#include <vector>
#include <map>
using namespace std;

class Bank;
class Account;
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
	Account* findAccountByCardNumber(string cardNumber);
	string findPasswordOfCard(string cardnumber);
	void updateAccount(string transactiontype, string accountNumber, long amount);
	//Bank Fund management
	void updateFunds(long amount);
};
class Account {
private:
	string accountBankName_;
	string accountNumber_; //12-digit
	string ownerName_;
	long balance_;
	string pw_;
	vector<Transaction*> transactionHistory_;
	string cardNumber_;
public:
	Account(string bankName, string accountNumber, string ownerName, long initBalance, string pw);

	//getter, setter
	string getAccountBankName() { return accountBankName_; }
	string getAccountNumber() { return accountNumber_; }
	string getOwnerName() { return ownerName_; }
	string getPassword() { return pw_; }
	long getBalance() { return balance_; }
	string getCardNumber() { return cardNumber_; }
	vector<Transaction*> getTransactionHistory() { return transactionHistory_; }
	//bool isPrimaryBank() const { return (primaryBank_ == 1) ? true : false; }

	//Balance Management
	void accountBalanceUpdate(string transactionType, long amount);

	//validate account number
	static bool validateAccountNumber(string accountNumber);
};
/*class Card {
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
};*/
class ATM {
private:
	int atmID_; //6-digit
	bool singleBank_; //ATM type
	string primaryBankName_;
	Bank* ownerBank_;
	bool unilingual_;
	map<string,long> availableCash_;
	vector<Bank*> interactableBanks_;
	
public:
	ATM(int atmID, bool singleBank, string bankName, Bank* ownerBank, bool unilingual,  int fiftyK, int tenK, int fiveK, int oneK, vector<Bank*>interactableBanks);
	
	//getter, setter
	std::map<std::string, long> getAvailableCash();
	int getATMID() { return atmID_; }
	string getOwnerBankName() { return primaryBankName_; }
	Bank* findGetInteractableBank(string bankname);
	bool isSingleBank() { return singleBank_; } // return true if singlebank(only primary bank card), false if multibank(every bank card)
	//update availableCash when deposit, withdrawl
	bool updateAvailableCash(int fiftyK, int tenK, int fiveK, int oneK); //return update o,x
	//user authorization
	bool isVaildCard(string cardNumber);
	string getCardPasswordFromBank(string cardNumber);

	bool updateAccountBalance(string transactionType, string accountnumber,  long amount);
	void updateBankFund(long amount);
};
class Transaction {
protected:
	int transID_;
	string transactionType_;
	ATM* atm_ = nullptr;
	//DateTime timestamp;
	//TransactionStatus status
	long amount_;
	double fee_;
	Session* session_;

public:
	static int nextID_;
	Transaction(ATM* atm, long amount, double fee, Session* session);
	virtual bool execute() = 0;
	virtual void validate() = 0;
	virtual void calculateFee() = 0;
	void describe();
};
class Withdrawl :public Transaction {
private:
	map<string, int> withdrawlDenominations;
	long totalWithdrawlAmount_;
public:
	Withdrawl(ATM* atm, long amount, double fee, Session* session);
	map<string, int> distributeDenom(long amount);
	bool execute() override;
	void calculateFee() override;
};
class Deposit :public Transaction{
private:
	bool isCash_; //True for cash, False for checks
	map<string, int> depositedCash;
	vector<double> depositedChecks;
	int numOfCashLimit_;
	int numOfCheckLimit_;
	string receiveAccount_;
	string receiveBankName_;
public:
	Deposit(ATM* atm, bool isCash, long amount, int fiftyK, int tenK, int fiveK, int oneK, double fee, Session* session, string receiveAccount, string receiveBank);
	bool validateDeposit();
	bool execute() override;
	void calculateFee() override;
};
class Transfer :public Transaction {
private:
	string sourceAccount_;
	string sourceBank_;
	string receiveAccount_;
	string receiveBank_;
	bool cashToAccount_; // true: cash->account, false account->account
public:
	Transfer(ATM* atm, bool cashtoAccount, long amount, double fee, string receiveAccount, string receiveBank, Session* session);
	void validateAccounts();
	bool execute() override;
	void calculateFee() override;
};
class Session {
private:
	vector<Transaction*> transactions_;
	int wrongPasswordAttempts_;
	bool isAuthenticated_;
	ATM* atm_;
	int withdrawlTimes = 0;
	string currCardNumber_;
public:
	Session(ATM* currentATM, string cardNumber);
	//~Session();
	bool authenticate(string pw);
	void addTransaction(Transaction* currTransaction);
	void endSession(); //whenever user wants or no cash available

	string getCurrentSessionCardNumber() { return currCardNumber_; }
	//string getCurrentSessionCardBank();
	//string getCurrentSessionCardAccountNumber();
	
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
	Account* acc = findAccountByCardNumber(cardnumber);
	if (acc->getCardNumber() == cardnumber) return acc->getPassword();
	return "None";
}
Account* Bank::findAccountByCardNumber(string cardNumber) {
	for (Account* acc : accounts_) {
		if (acc->getCardNumber() == cardNumber) return acc;
	}
	return nullptr;
}
void Bank::updateAccount(string transactiontype, string accountNumber, long amount) {
	Account* tempAcc = findAccountByNumber(accountNumber);
	tempAcc->accountBalanceUpdate(transactiontype, amount);
}
void Bank::updateFunds(long amount) { funds_ += amount; }
//---------------------------------------------------------------------------

//Account Member Function Defined--------------------------------------------
Account::Account(string bankName, string accountNumber, string ownerName, long initBalance, string pw) {
	accountBankName_ = bankName;
	accountNumber_ = accountNumber;
	ownerName_ = ownerName;
	balance_ = initBalance;
	cardNumber_ = "1234" + accountNumber + "5678";
	cout << "Account Constructed (Account Bank name: " << accountBankName_ << ", Account Number: " << accountNumber_ <<", Owner: "<<ownerName_<<", Balance : "<<balance_<<")\n";
	cout << "Issued Card Number (connected to " << accountNumber_ << "account is " << cardNumber_;
	
}
void Account::accountBalanceUpdate(string transactionType, long amount) {
	if (transactionType == "Withdrawl") {
		balance_ -= amount;
		cout << "Account Balance Updated for withdrawl\n";
	}
	if (transactionType == "Deposit") {
		balance_ += amount;
		cout << "Account Balance Updated for Deposit\n";
	}
	if (transactionType == "Transfer Send") {
		balance_ -= amount;
		cout << "Sender Account Balance Updated for Transfer \n";
	}
	if (transactionType == "Transfer Receive") {
		balance_ -= amount;
		cout << "Receiver Account Balance Updated for Transfer \n";
	}
}
bool Account::validateAccountNumber(string accountNum) {
	if (atoi(accountNum.c_str()) != 0 && accountNum.length() == 12) return true;
	else return false;
}
//---------------------------------------------------------------------------

/*Card Member Function Defined-----------------------------------------------
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
}*/
//---------------------------------------------------------------------------

//ATM Member Function Defined
ATM::ATM(int atmID, bool singleBank, string bankName, Bank* ownerBank, bool unilingual, int fiftyK, int tenK, int fiveK, int oneK, vector<Bank*> interactableBanks) {
	atmID_ = atmID;
	singleBank_ = singleBank;
	unilingual_ = unilingual;
	primaryBankName_ = bankName;
	ownerBank_ = ownerBank;
	availableCash_["50000won"] = fiftyK;
	availableCash_["10000won"] = tenK;
	availableCash_["5000won"] = fiveK;
	availableCash_["1000won"] = oneK;
	interactableBanks_ = interactableBanks;
	cout << "ATM Constructed (ATM ID: " << atmID_ << ", Unilingual: " << unilingual_ << ")\n";
	cout << "Initial amount of Denomination\n";
	cout << "[50000won]: " << availableCash_["50000won"] << endl;
	cout << "[10000won]: " << availableCash_["10000won"] << endl;
	cout << "[5000won] : " << availableCash_["5000won"] << endl;
	cout << "[1000won] : " << availableCash_["1000won"] << endl;
}
std::map<std::string, long> ATM::getAvailableCash() {
	return availableCash_;
}
Bank* ATM::findGetInteractableBank(string bankname) {
	for (Bank* bank : interactableBanks_) {
		if (bank->getBankName() == bankname) return bank;
	}
	cout << bankname << " is not existing bank\n";
	return nullptr;
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
bool ATM::isVaildCard(string cardNumber) {
	//Find card
	Account* pAcc = nullptr;
	for (Bank* pBank : interactableBanks_) {
		pAcc = pBank->findAccountByCardNumber(cardNumber);
	}
	if (pAcc) {
		cout << "This card is not registered to any bank\n";
		return false;
	}

	//Card - ATM type matching
	if (isSingleBank()) {
		if (primaryBankName_ == pAcc->getAccountBankName()) {
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
bool ATM::updateAccountBalance(string transactionType, string accountnumber, long amount) {
	Account* currAccount = ownerBank_->findAccountByNumber(accountnumber);
	if (currAccount->getBalance() < abs(amount)) {
		cout << "Insufficient Balance\n";
		return false;
	}
	else{
		currAccount->accountBalanceUpdate(transactionType, amount);
		return true;
	}
}
void ATM::updateBankFund(long amount) {
	ownerBank_->updateFunds(amount);
}
//---------------------------------------------------------------------------

//Transaction Member Function Defined
int Transaction::nextID_ = 0;
Transaction::Transaction(ATM* atm, long amount, double fee, Session* session) {
	transID_ = nextID_++;
	amount_ = amount;
	fee_ = fee;
	atm_ = atm;
	session_ = session;
}
void Transaction::describe() {
	cout << "Transaction ID: " << transID_ << " " << transactionType_ << "Amount: " << amount_ << endl;
	cout << "Fee: " << fee_ << endl;
}
//---------------------------------------------------------------------------

//Withdrawl Member Function Defined
Withdrawl::Withdrawl(ATM* atm, long amount, double fee, Session* session) :Transaction(atm, amount, fee, session) {
	transactionType_ = "Withdrawl";
	totalWithdrawlAmount_ = 0;
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
	
	// decrease account balance
	bool success = atm_->updateAccountBalance(transactionType_, session_->getCurrentSessionCardAccountNumber(), amount_);
	if (!success) return false;
	calculateFee();
	//lower atm's available cash
	if (!atm_->updateAvailableCash(-withdrawlDenominations["50000won"], -withdrawlDenominations["10000won"], -withdrawlDenominations["5000won"], -withdrawlDenominations["1000won"])) {
		return false;
	}
	//dispense to user
	for (std::map<std::string, int>::iterator it = withdrawlDenominations.begin(); it != withdrawlDenominations.end(); ++it) {
		std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
	}
	// fee charging from account
	return true;
}	
void Withdrawl::calculateFee() {
	if (atm_->isSingleBank()) { //primary로만 가능
		cout << "1000 won will be paid from your account for withdrawl\n";
		if (session_->getCurrentSessionCardBank() == atm_->getOwnerBankName()) {
			if (atm_->updateAccountBalance(transactionType_, session_->getCurrentSessionCardAccountNumber(), 1000)) {
				cout << "Fee charged\n";
				atm_->updateBankFund(1000);
				atm_->updateAvailableCash(0, 0, 0, 1);
			}
		}
	}
	else { //multibank
		if (session_->getCurrentSessionCardBank() == atm_->getOwnerBankName()) {
			cout << "Insert 1000 won into slot for Deposit. Slot: ";
			cin >> fee_;
			while (fee_ != 1000) {
				cout << "Wrong Amount. Inset 1000 won again into slot. Slot:";
				cin >> fee_;
			}
			if (atm_->updateAccountBalance(transactionType_, session_->getCurrentSessionCardAccountNumber(), 1000)) {
				cout << "Fee charged\n";
				atm_->updateBankFund(1000);
				atm_->updateAvailableCash(0, 0, 0, 1);
			}
		}
		else {
			cout << "Insert 2000 won into slot for Withdrawl. Slot: ";
			cin >> fee_;
			while (fee_ != 2000) {
				cout << "Wrong Amount. Inset 2000 won again into slot. Slot:";
				cin >> fee_;
			}
			if (atm_->updateAccountBalance(transactionType_, session_->getCurrentSessionCardAccountNumber(), 2000)) {
				cout << "Fee charged\n";
				atm_->updateAvailableCash(0, 0, 0, 2);
				atm_->updateBankFund(2000);
			}
		}
	}
}
//---------------------------------------------------------------------------

//Deposit Member Function Defined                                                                                             //how can I handle checks deposit???
Deposit::Deposit(ATM* atm, bool isCash, long amount, int fiftyK, int tenK, int fiveK, int oneK, double fee, Session* session, string receiveAccount, string receiveBank):Transaction(atm, amount, fee, session){
	transactionType_ = "Deposit";
	isCash_ = isCash; //True for cash, False for checks
	depositedCash["50000won"] = 0;
	depositedCash["10000won"] = 0;
	depositedCash["5000won"] = 0;
	depositedCash["1000won"] = 0;
	vector<double> depositedChecks;
	numOfCashLimit_ = 50;
	numOfCheckLimit_ = 30;
	receiveAccount_ = receiveAccount;
	receiveBankName_ = receiveBank;
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
		return false;
	}	
}
bool Deposit::execute() {
	calculateFee();

	// add to account
	
	// add to ATM available cash
	if (isCash_) {
		if (session_->getCurrentSessionCardBank() == atm_->getOwnerBankName()) { //deposit into primary bank
			atm_->updateAvailableCash(depositedCash["50000won"], depositedCash["10000won"], depositedCash["5000won"], depositedCash["1000won"]);
			atm_->updateAccountBalance(transactionType_, session_->getCurrentSessionCardNumber(), amount_);
			cout << "Cash Deposit Completed" << endl;
		}
		else { //deposit to non-primary bank
			atm_->updateAvailableCash(depositedCash["50000won"], depositedCash["10000won"], depositedCash["5000won"], depositedCash["1000won"]);
			atm_->findGetInteractableBank(receiveBankName_)->updateAccount(transactionType_, receiveAccount_, amount_);
		}
	
		return true;
	}
	else {
		//depositedCheck update
		// 
		//acount update using amount_
		return true;
	}
}
void Deposit::calculateFee() {
	if (atm_->isSingleBank()) { //primary로만 가능
		if (session_->getCurrentSessionCardBank() == atm_->getOwnerBankName()) {
			cout << "Insert 1000 won into slot for Deposit. Slot: ";
			cin >> fee_;
			while (fee_ != 1000) {
				cout << "Wrong Amount. Insert 1000 won again into slot. Slot:";
				cin >> fee_;
			}
			cout << "Fee charged\n";
			atm_->updateAvailableCash(0, 0, 0, 1);
			atm_->updateBankFund(1000);
		}
	}
	else { //multibank
		if (session_->getCurrentSessionCardBank() == atm_->getOwnerBankName()) {
			cout << "Insert 1000 won into slot for Deposit. Slot: ";
			cin >> fee_;
			while (fee_ != 1000) {
				cout << "Wrong Amount. Inset 1000 won again into slot. Slot:";
				cin >> fee_;
			}
			cout << "Fee charged\n";
			atm_->updateAvailableCash(0, 0, 0, 1);
			atm_->updateBankFund(1000);
		}
		else {
			cout << "Insert 2000 won into slot for Deposit. Slot: ";
			cin >> fee_;
			while (fee_ != 2000) {
				cout << "Wrong Amount. Inset 2000 won again into slot. Slot:";
				cin >> fee_;
			}
			cout << "Fee charged\n";
			atm_->updateAvailableCash(0, 0, 0, 2);
			atm_->updateBankFund(2000);
		}
	}
}
//---------------------------------------------------------------------------

//Transfer Member Function Defined
Transfer::Transfer(ATM* atm, bool cashtoAccount, long amount, double fee, string receiveAccount, string receiveBank, Session* session):Transaction(atm, amount, fee, session) {
	transactionType_ = "Transfer";
	receiveAccount_ = receiveAccount;
	receiveBank_ = receiveBank;
	cashToAccount_ = cashtoAccount;
}
void Transfer::validateAccounts() {
	//check if the sourceaccount and receiveaccount is both valid
}
bool Transfer:: execute() {
	calculateFee();
	if (cashToAccount_) { //cash-account
		// let user to insert cash with denomination + fee
		// (insert cash-fee) == amount?
		// update ATM available
		atm_->updateAccountBalance(transactionType_ + "Receive", session_->getCurrentSessionCardAccountNumber(), amount_);
		return true;
	}
	else { //account-account
		
		// update sender balance
		atm_->updateAccountBalance(transactionType_ + "Send", session_->getCurrentSessionCardAccountNumber(), amount_);
		// update receiveaccount balance
		atm_->findGetInteractableBank(receiveBank_)->updateAccount(transactionType_+"Receive", receiveAccount_, amount_);
		return true;
	}
}
void Transfer::calculateFee() {
	if (cashToAccount_) {
		cout << "Insert 1000 won into slot for Transfer. Slot: ";
		cin >> fee_;
		while (fee_ != 1000) {
			cout << "Wrong Amount. Insert 1000 won again into slot. Slot:";
			cin >> fee_;
		}
	}
	else {
		//primary to primary
		if (session_->getCurrentSessionCardBank() == atm_->getOwnerBankName() && atm_->getOwnerBankName() == receiveBank_) {
			cout << "2000 won will be paid from your account for transfer\n";
			if (atm_->updateAccountBalance(transactionType_, session_->getCurrentSessionCardAccountNumber(), 1000)) {
				cout << "Fee charged\n";
				atm_->updateBankFund(2000);
				atm_->updateAvailableCash(0, 0, 0, 2);
			}
		}
		//non to non
		else if (session_->getCurrentSessionCardBank() != atm_->getOwnerBankName() && atm_->getOwnerBankName() != receiveBank_) {
			cout << "4000 won will be paid from your account for transfer\n";
			if (atm_->updateAccountBalance(transactionType_, session_->getCurrentSessionCardAccountNumber(), 3000)) {
				cout << "Fee charged\n";
				atm_->updateBankFund(4000);
				atm_->updateAvailableCash(0, 0, 0, 4);
			}
		}

		else {
			cout << "3000 won will be paid from your account for transfer\n";
			if (atm_->updateAccountBalance(transactionType_, session_->getCurrentSessionCardAccountNumber(), 3000)) {
				cout << "Fee charged\n";
				atm_->updateBankFund(3000);
				atm_->updateAvailableCash(0, 0, 0, 3);
			}
		}
	}	
}
//---------------------------------------------------------------------------

//Session Member Function Defined
Session::Session(ATM* currentATM, string cardNumber) {
	atm_ = currentATM;
	currCardNumber_ = cardNumber;
	wrongPasswordAttempts_ = 0;
	isAuthenticated_ = false;
}
Session::~Session() {}
bool Session::authenticate(string input_pw) {
	while (wrongPasswordAttempts_ < 3) {
		if (input_pw == currCardNumber_) {
			cout << "Correct Password\n";
			wrongPasswordAttempts_ = 0;
			return true;
		}
		else {
			cout << "Wrong Password" << endl;
			wrongPasswordAttempts_++;
			return false;
		}
	
	}
	endSession();
	return false;
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
void screenShot(char slash, vector<Account> accounts, vector<ATM>atms) {
	if (slash == 47) {
		for (ATM atm : atms) {
			long fiftyK = atm.getAvailableCash()["50000"];
			long tenK = atm.getAvailableCash()["10000"]; 
			long fiveK= atm.getAvailableCash()["5000"]; 
			long oneK = atm.getAvailableCash()["1000"];
			cout << "ATM[SN: " << atm.getATMID() << "] remaining cash: {KRW 50000: " << fiftyK << ", KRW 10000: " << tenK << ", KRW 5000: " << fiveK << ", KRW 1000: " << oneK << "}\n";
		}
		for (Account acc : accounts) {
			cout << "Account[Bank: " << acc.getAccountBankName() << ", No : " << acc.getAccountNumber() << ", Owner : " << acc.getOwnerName() << "] balance: " << acc.getBalance() << endl;
		}
	}

}
int main() {

	cout << "Bank Initialization\n";
	bool bank_status = true;
	vector<Bank> banks;  // Vector to store all bank objects
	int bankCounter = 1; // Counter for naming banks
	while (bank_status) {
		string bankName;
		long long initialfund;
		cout << "Bank Name(without space): ";
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
	for (Bank bank : banks) {
		cout << bank.getBankName() << " ";
	}
	cout << " \n";
	cout << endl;

	cout << "Account Initialization\n";
	bool account_status = true;
	vector<Account> accounts;  // Vector to store all bank objects
	int accountCounter = 1; // Counter for naming banks
	while (account_status) {
		string accbankName;
		string accountNumber;
		string ownerName;
		long initBalance;
		string pw;
		cout << "Bank Name: ";
		cin >> accbankName;
		cout << "Account Number: ";
		cin >> accountNumber;
		cout << "Owner Name: ";
		cin >> ownerName;
		cout << "Initial Balance: ";
		cin >> initBalance;
		cout << "Initialize Password: ";
		cin >> pw;

		string objectName = "account" + to_string(accountCounter);
		for (Bank bank : banks) {
			if (bank.getBankName() == accbankName) {
				accounts.push_back(Account(accbankName, accountNumber, ownerName, initBalance, pw));
				bank.addAccount(&accounts[accountCounter - 1]);
			}
		}
		if (accounts[accountCounter - 1].getAccountBankName() == "") {
			cout << "No Bank for this account exists\n";
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
		vector<Bank*> interactableBanks;
		Bank* ownerbank;

		cout << "Bank Name: ";
		cin >> atmbankName;
		cout << "ATM serial Number(6-digit serial number): ";
		cin >> atmID;
		cout << "Press S for Single Bank ATM, M for Multi Bank ATM: ";
		cin >> isSingleBank;
		if (isSingleBank == "S") {
			singleBank = true;
			//for (Bank bank : banks) interactableBanks.push_back(&bank);
		}
		else if (isSingleBank == "M") {
			singleBank = false;
			//for (Bank bank : banks) interactableBanks.push_back(&bank);
		}
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
		bool success = false;
		for (Bank bank : banks) {
			if (bank.getBankName() == atmbankName) {
				Bank* ownerBank = &bank;
				for (Bank bank : banks) interactableBanks.push_back(&bank);
				ATMs.push_back(ATM(atmID, singleBank, atmbankName, ownerBank, unilingual, fiftyK, tenK, fiveK, oneK, interactableBanks));
				success = true;
			}
		}
		if (!success) cout << atmbankName << " does not exist.\n";

		cout << "---------------------------------------";
		ATMCounter++;

		// More addition
		char continue_input;
		cout << "Add another ATM? (y/n): ";
		cin >> continue_input;
		if (continue_input != 'y' && continue_input != 'Y') {
			ATM_status = false;
		}
	}

	char screenshot;
	cin >> screenshot;
	screenShot(screenshot, accounts, ATMs);

	cout << "Select ATM by Serial Number: ";
	int atmSN;
	cin >> atmSN;
	ATM* currATM;
	for (ATM atm : ATMs) {
		if (atmSN == atm.getATMID()) {
			currATM = &atm;
		}
	}
	//Start ATM Operation ---------------------------------
	cout << "Welcome! Please insert Card (Write your card number):\n";
	string cardnumber;
	cin >> cardnumber;
	//Session Start
	Session currSession = Session(currATM, cardnumber);

	// 1. Is card valid to this ATM type
	if (!currATM->isVaildCard(cardnumber)) {
		cout << "Return Card: " << cardnumber << endl;
		cout << "End Session\n";
	}
	//2. PW validation
	cout << "Password: ";
	string inputPW;
	cin >> inputPW;
	bool success = currSession.authenticate(inputPW);
	if (!success) {
		cout << "Return Card: " << cardnumber << endl;
		cout << "End Session\n";
		//currSession.endSession();
	}
	//3. Menu Display
	cout << "Choose the Number of Transaction\n";
	cout << "1. Withdrawl\n";
	cout << "2. Deposit\n";
	cout << "3. Transfer\n";


	

	




	return 0;
}