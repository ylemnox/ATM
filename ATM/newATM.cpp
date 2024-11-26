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
	map<string, long> inputChecks_;
	vector<Bank*> interactableBanks_;
	vector<string> transactionHistoryOfATM_;
public:
	ATM(int atmID, bool singleBank, string bankName, Bank* ownerBank, bool unilingual,  int fiftyK, int tenK, int fiveK, int oneK, vector<Bank*>interactableBanks);
	
	//getter, setter
	std::map<std::string, long> getAvailableCash();
	int getATMID() { return atmID_; }
	string getOwnerBankName() { return primaryBankName_; }
	Bank* getCardBank(string cardNumber);
	Bank* findGetInteractableBank(string bankname);
	bool isSingleBank() { return singleBank_; } // return true if singlebank(only primary bank card), false if multibank(every bank card)
	//update availableCash when deposit, withdrawl
	bool updateAvailableCash(int fiftyK, int tenK, int fiveK, int oneK); //return update o,x
	void depositChecks(string check, long amount);
	//user authorization
	bool isVaildCard(string cardBank);
	string getCardPasswordFromBank(string cardNumber);
	void addTransactionHistoryOfATM(string history);
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
	Session* session_;

public:
	static int nextID_;
	Transaction(ATM* atm, Session* session);
	virtual void execute() = 0;
	//virtual void validate() = 0;
	virtual void calculateFee() = 0;
	virtual void describeAndSave() = 0;
};
class Withdrawl :public Transaction {
private:
	map<string, int> withdrawlDenominations;
	//long totalWithdrawlAmount_;
	long fee_;
	
public:
	Withdrawl(ATM* atm, Session* session);
	map<string, int> distributeDenom(long amount);
	void execute() override;
	void calculateFee() override;
	void describeAndSave() override;
};
class Deposit :public Transaction{
private:
	bool isCash_; //True for cash, False for checks
	map<string, int> depositedCash;
	int numOfCashLimit_;
	int numOfCheckLimit_;
public:
	Deposit(ATM* atm, Session* session);
	bool validateDeposit();
	void execute() override;
	void calculateFee() override;
	void describeAndSave() override;
};
class Transfer :public Transaction {
private:
	bool cashToAccount_; // true: cash->account, false account->account
	string receiveBankName_;
	string receiveAccountNumber_;
	long fee_;
public:
	Transfer(ATM* atm, Session* session);
	bool validateReceiverAccounts();
	void execute() override;
	void calculateFee() override;
	void describeAndSave() override;
};
class Session {
private:
	Transaction* currTransaction;
	Transaction* withdrawl;
	Transaction* deposit;
	Transaction* transfer;
	vector<string> transactionHistoryOfSession;
	int wrongPasswordAttempts_;
	bool isAuthenticated_;
	ATM* atm_;
	int withdrawlTimes = 0;
	string currCardNumber_;
	string currCardBankName_;
	string currCardAccountNumber_;
public:
	Session(ATM* currentATM, string cardNumber);
	void authenticate(string pw);
	void execute();
	string getCurrCardNumber() { return currCardNumber_; }
	string getCurrCardBankName() { return currCardBankName_; }
	void addTransactionOfSession(string history);
	void endSession(); //whenever user wants or no cash available

	string getCurrentSessionCardNumber() { return currCardNumber_; }
	string getCurrAccountNumber() { return currCardAccountNumber_; }
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
	if (transactiontype == "Withdrawl") {
		Account* tempAcc = findAccountByNumber(accountNumber);
		tempAcc->accountBalanceUpdate(transactiontype, -amount);
	}
	else if (transactiontype == "Deposit") {
		Account* tempAcc = findAccountByNumber(accountNumber);
		tempAcc->accountBalanceUpdate(transactiontype, amount);
	}
	else if (transactiontype == "TransferSend") {
		Account* tempAcc = findAccountByNumber(accountNumber);
		tempAcc->accountBalanceUpdate(transactiontype, -amount);
	}
	else if (transactiontype == "TransferReceive") {
		Account* tempAcc = findAccountByNumber(accountNumber);
		tempAcc->accountBalanceUpdate(transactiontype, amount);
	}
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
void ATM::depositChecks(string check, long amount) {
	inputChecks_[check] = amount;
}
bool ATM::isVaildCard(string cardBank) {
	if (isSingleBank()) {
		if (primaryBankName_ != cardBank) {
			//cout << "Invalid Card\n";
			//cout << "This is a Single Bank ATM. Only card issued by " << primaryBankName_ << "is allowed.\n";
			return true;
		}
		else {
			//cout << "Valid Card\n";
			return false;
		}
	}
	else {
		return true;
	}
}
Bank* ATM::getCardBank(string cardNumber) {
	Account* pAcc;
	for (Bank* pBank : interactableBanks_) {
		pAcc = pBank->findAccountByCardNumber(cardNumber);
		if (pAcc) return pBank;
	}
	cout << "This Card does not below to any Bank\n";
	return nullptr;
}
void ATM::addTransactionHistoryOfATM(string history) {
	transactionHistoryOfATM_.push_back(history);
}
	
string ATM::getCardPasswordFromBank(string cardNumber) {
	return getCardBank(cardNumber)->findPasswordOfCard(cardNumber);
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
Transaction::Transaction(ATM* atm, Session* session) {
	transID_ = nextID_++;
	atm_ = atm;
	session_ = session;
}
//---------------------------------------------------------------------------

//Withdrawl Member Function Defined
Withdrawl::Withdrawl(ATM* atm, Session* session) :Transaction(atm, session) {
	transactionType_ = "Withdrawl";
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
	cout << "<Deposit>\n";
	cout << "Please Enter the desired amount of cash you want to withdrawl. AMOUNT(won): ";
	cin >> amount_;
	calculateFee();
	if (amount_ + fee_ > atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance()) {
		cout << "Insufficient Account Balance. Current Balance: " <<
			atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << " won.\n";
		return ;
	}
	if (distributeDenom(amount_)["50000"] > atm_->getAvailableCash()["50000"]) {
		cout << "Insufficient 50000won bills\n";
		return ;
	}
	if (distributeDenom(amount_)["10000"] > atm_->getAvailableCash()["10000"]) {
		cout << "Insufficient 10000won bills\n";
		return ;
	}
	if (distributeDenom(amount_)["5000"] > atm_->getAvailableCash()["5000"]) {
		cout << "Insufficient 5000won bills\n";
		return ;
	}
	if (distributeDenom(amount_)["1000"] > atm_->getAvailableCash()["1000"]) {
		cout << "Insufficient 1000won bills\n";
		return ;
	}
	atm_->getCardBank(session_->getCurrCardNumber())->updateAccount(transactionType_, session_->getCurrAccountNumber(), amount_);
	cout << "Get the Cash. 50000won bill: " << distributeDenom(amount_)["50000"] << ", 10000won bill: " << distributeDenom(amount_)["10000"]
		<< "5000won bill : " << distributeDenom(amount_)["5000"] << "1000won bill : " << distributeDenom(amount_)["1000"] << endl;
	describeAndSave();

}	
void Withdrawl::calculateFee() { //update fee
	if (atm_->getOwnerBankName() == session_->getCurrCardBankName()) {
		cout << "1000won will be paid from your account.";
		fee_ = 1000;
	}
	else {
		cout << "2000won will be paid from your account.";
		fee_ = 2000;
	}
}
void Withdrawl::describeAndSave() {
	string history = "TransactionID: " + to_string(transID_) + "Card Number : " + session_->getCurrCardNumber() + " | Account Number : " + session_->getCurrAccountNumber() + " |" + transactionType_ + " | Amount : " + to_string(amount_);
	atm_->addTransactionHistoryOfATM(history);
	session_->addTransactionOfSession(history);
	cout << "Card Number: " << session_->getCurrCardNumber() << " |Account Number: " << session_->getCurrAccountNumber() << " |" << transactionType_ << " |Amount: " << amount_ << "\n";
}
//---------------------------------------------------------------------------

//Deposit Member Function Defined                                                                                            
Deposit::Deposit(ATM* atm, Session* session):Transaction(atm, session){
	transactionType_ = "Deposit";
	//isCash_ = isCash; //True for cash, False for checks
	depositedCash["50000won"] = 0;
	depositedCash["10000won"] = 0;
	depositedCash["5000won"] = 0;
	depositedCash["1000won"] = 0;
	numOfCashLimit_ = 50;
	numOfCheckLimit_ = 30;
}
bool Deposit::validateDeposit() {
	if (isCash_) { //cash
		long totalsheet_= depositedCash["50000won"] + depositedCash["10000won"] + depositedCash["5000won"] + depositedCash["1000won"];
		if (totalsheet_ > numOfCashLimit_) {
			cout << "Total amount of cash should be under 50" << endl;
			return false;
		}
		else return true;
	}
}
void Deposit::execute() {
	cout << "<Deposit>\n";
	int pick = 0;
	while (pick != 1 || pick != 2) {
		cout << "Enter 1 for Cash Deposit. Enter 2 for Check Deposit.: ";
		cin >> pick;
		if (pick == 1) isCash_ = true;
		else if (pick == 2) isCash_ = false;
		else cout << "Wrong Input. Enter 1 or 2.\n";
	}
	calculateFee();
	if (isCash_) { //cash deposit
		cout << "Place the cash into Slot\n";
		cout << "KRW 50000 bills: ";
		long fiftyK;
		cin >> fiftyK;
		cout << "KRW 10000 bills: ";
		long tenK;
		cin >> tenK;
		cout << "KRW 5000 bills: ";
		long fiveK;
		cin >> fiveK;
		cout << "KRW 1000 bills: ";
		long oneK;
		cin >> oneK;
		
		//success ==false이면 어떵할거
		bool success;
		success = validateDeposit();
		if (!success) return ;
		atm_->getCardBank(session_->getCurrCardNumber())->updateAccount(transactionType_, session_->getCurrAccountNumber(), amount_);
		success = atm_->updateAvailableCash(depositedCash["50000"], depositedCash["10000"], depositedCash["5000"], depositedCash["1000"]);
		if (!success) return ;
		cout << "Deposit " << amount_ << " won to (" << session_->getCurrCardBankName() << " , NO: " << session_->getCurrAccountNumber() << ")\n";
	}
	else { //check deposit
		cout << "Place the checks into Slot\n";
		long totalsheet=0;
		while (totalsheet<=30) {
			cout << "Written Amount of this check: ";
			string check;
			cin >> check;
			cout << "How many sheets of this check: ";
			long sheet;
			cin >> sheet;
			atm_->depositChecks(check, sheet);
			totalsheet += sheet;
			if (totalsheet == 30) cout << "No more checks input is allowed\n";
			amount_ += (stol(check) * sheet);
		}
		atm_->getCardBank(session_->getCurrCardNumber())->updateAccount(transactionType_, session_->getCurrAccountNumber(), amount_);
		describeAndSave();
	}
}
void Deposit::calculateFee() {
	if (atm_->getOwnerBankName() == session_->getCurrCardBankName()) {
		long fee_ = 0;
		cout << "First, Pay 1000won for deposit fee into slot.";
		while (fee_ != 1000) {
			cout << "SLOT: ";
			cin >> fee_;
			if (fee_ == 1000) break;
			cout << "Insert exactly 1000won into SLOT.\n";
		}
	}
	else {
		long fee_ = 0;
		cout << "First, Pay 2000won for deposit fee into slot.";
		while (fee_ != 2000) {
			cout << "SLOT: ";
			cin >> fee_;
			if (fee_ == 2000) break;
			cout << "Insert exactly 2000won into SLOT.\n";
		}
		atm_->updateAvailableCash(0, 0, 0, 2);
	}
}
void Deposit::describeAndSave() {
	if (isCash_) {
		string history = "TransactionID: "+ to_string(transID_)+"Card Number : " + session_->getCurrCardNumber() + " | Account Number : " + session_->getCurrAccountNumber() + " |Cash " + transactionType_ + " | Amount : " + to_string(amount_);
		atm_->addTransactionHistoryOfATM(history);
		session_->addTransactionOfSession(history);
		cout << "Card Number: " << session_->getCurrCardNumber() << " |Account Number: " << session_->getCurrAccountNumber() << " |" << "Cash " << transactionType_ << " |Amount: " << amount_ << "\n";
	}
	else {
		string history = "TransactionID: " + to_string(transID_) + "Card Number : " + session_->getCurrCardNumber() + " | Account Number : " + session_->getCurrAccountNumber() + " |Check " + transactionType_ + " | Amount : " + to_string(amount_);
		atm_->addTransactionHistoryOfATM(history);
		session_->addTransactionOfSession(history);
		cout << "Card Number: " << session_->getCurrCardNumber() << " |Account Number: " << session_->getCurrAccountNumber() << " |" << "Check " << transactionType_ << " |Amount: " << amount_ << "\n";
	}
}

//---------------------------------------------------------------------------

//Transfer Member Function Defined
Transfer::Transfer(ATM* atm, Session* session):Transaction(atm, session) {
	transactionType_ = "Transfer";
}
bool Transfer::validateReceiverAccounts() {
	if (!atm_->findGetInteractableBank(receiveBankName_)) {
		cout << "This Bank does not exist.\n";
		return false;
	}
	if (!atm_->findGetInteractableBank(receiveBankName_)->findAccountByNumber(receiveAccountNumber_)) {
		cout << "Wrong Account Number\n";
		return false;
	}
	string receiver = atm_->findGetInteractableBank(receiveBankName_)->findAccountByNumber(receiveAccountNumber_)->getOwnerName();
	cout << "Is the receiver's name is " << receiver << "? Enter Y for yes, N for no. : ";
	string yn;
	cin >> yn;
	if (yn == "N") return false;
	return true;
}
void Transfer:: execute() {
	cout << "<Transfer>\n";
	int pick = 0;
	while (pick != 1 || pick != 2) {
		cout << "Enter 1 for Cash-to-Account Transfer. Enter 2 for Account-to-Account Transfer.: ";
		cin >> pick;
		if (pick == 1) cashToAccount_ = true;
		else if (pick == 2) cashToAccount_ = false;
		else cout << "Wrong Input. Enter 1 or 2.\n";
	}
	cout << "Enter Receiver's Bank: ";
	cin >> receiveBankName_;
	cout << "Enter Receiver's Account Number: ";
	cin >> receiveAccountNumber_;
	bool success = validateReceiverAccounts();
	if (success) {
		calculateFee();
		if (cashToAccount_) {
			cout << "Place the cash to transfer into Slot\n";
			cout << "KRW 50000 bills: ";
			long fiftyK;
			cin >> fiftyK;
			cout << "KRW 10000 bills: ";
			long tenK;
			cin >> tenK;
			cout << "KRW 5000 bills: ";
			long fiveK;
			cin >> fiveK;
			cout << "KRW 1000 bills: ";
			long oneK;
			cin >> oneK;

			amount_ = fiftyK * 50000 + tenK * 10000 + fiveK * 5000 + oneK * 1000 - fee_;
			atm_->updateAvailableCash(fiftyK, tenK, fiveK, oneK);
			atm_->findGetInteractableBank(receiveBankName_)->updateAccount(transactionType_ + "Receive", receiveAccountNumber_, amount_);
			return ;
		}
		else {
			cout << "Enter Amount to Transfer: ";
			cin >> amount_;

			if (amount_ > atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance()) {
				cout << "Insufficient Balance.\n";
				return ;
			}
			atm_->findGetInteractableBank(receiveBankName_)->updateAccount(transactionType_ + "Receive", receiveAccountNumber_, amount_);
			atm_->findGetInteractableBank(session_->getCurrCardBankName())->updateAccount(transactionType_ + "Send", session_->getCurrAccountNumber(), amount_);
			return ;
		}
	}
	describeAndSave();
}
void Transfer::calculateFee() {
	if (cashToAccount_) {
		cout << "When you insert the cash to transfer into slot, you must include 1000won for fee.\n";
		fee_ = 1000;
	}
	else{
		if (atm_->getOwnerBankName()==receiveBankName_ && atm_->getOwnerBankName() == session_->getCurrCardBankName()) {
			cout << "When you insert the cash to transfer into slot, you must include 2000won for fee.\n";
			fee_ = 2000;
		}
		else if (atm_->getOwnerBankName() != receiveBankName_ && atm_->getOwnerBankName() != session_->getCurrCardBankName()) {
			cout << "When you insert the cash to transfer into slot, you must include 4000won for fee.\n";
			fee_ = 4000;
		}
		else {
			cout << "When you insert the cash to transfer into slot, you must include 3000won for fee.\n";
			fee_ = 3000;
		}
	}
}
void Transfer::describeAndSave() {
	if (cashToAccount_) {
		string history = "TransactionID: " + to_string(transID_) + "Card Number : " + session_->getCurrCardNumber() + " |Receiver Account Number: " + receiveAccountNumber_ + " |" + "Cash " + transactionType_ + " |Amount: " + to_string(amount_);
		atm_->addTransactionHistoryOfATM(history);
		session_->addTransactionOfSession(history);
		cout << "Card Number: " << session_->getCurrCardNumber() << " |Receiver Account Number: " << receiveAccountNumber_ << " |" << "Cash " << transactionType_ << " |Amount: " << amount_ << "\n";
	}
	else {
		string history = "TransactionID: " + to_string(transID_) + "Card Number: " + session_->getCurrCardNumber() + " | Sender Account Number : " + session_->getCurrAccountNumber() + " | Receiver Account Number : " + receiveAccountNumber_ + " | " + "Account " + transactionType_ + " | Amount : " + to_string(amount_);
		atm_->addTransactionHistoryOfATM(history);
		session_->addTransactionOfSession(history);
		cout << "Card Number: " << session_->getCurrCardNumber() << " |Sender Account Number: " << session_->getCurrAccountNumber() << " |Receiver Account Number: " << receiveAccountNumber_ << " |" << "Account " << transactionType_ << " |Amount: " << amount_ << "\n";
	}
}
//---------------------------------------------------------------------------

//Session Member Function Defined
Session::Session(ATM* currentATM, string cardNumber) {
	atm_ = currentATM;
	currCardNumber_ = cardNumber;
	currCardBankName_ = atm_->getCardBank(cardNumber)->getBankName();
	currCardAccountNumber_ = atm_->getCardBank(cardNumber)->findAccountByCardNumber(cardNumber)->getAccountNumber();
	wrongPasswordAttempts_ = 0;
	isAuthenticated_ = false;
	
	withdrawl = new Withdrawl(atm_, this);
	deposit = new Deposit(atm_, this);
	transfer = new Transfer(atm_, this);
	
}
void Session::execute() {
	// 1. Is card valid to this ATM type
	if (!atm_->isVaildCard(currCardNumber_)) {
		cout << "INVALID CARD> This is a Single Bank ATM. Only card issued by " << atm_->getOwnerBankName() << "is allowed.\n";
		cout << "Return Card: " << currCardNumber_ << endl;
		endSession();
	}

	//2. PW validation
	cout << "Password: ";
	string inputPW;
	cin >> inputPW;
	authenticate(inputPW);
	if (!isAuthenticated_) {
		cout << "Return Card: " << currCardNumber_ << endl;
		endSession();
		//currSession.endSession();
	}
	int menuPick;
	string cont_ = "Y";
	while (isAuthenticated_&&cont_=="Y") {
		cout << "Choose the Number of Transaction\n";
		cout << "1. Withdrawl\n";
		cout << "2. Deposit\n";
		cout << "3. Transfer\n";
		cout << "->";
		
		cin >> menuPick;
		if (menuPick == 1) withdrawl->execute();
		else if (menuPick == 2) deposit->execute();
		else if (menuPick == 3) transfer->execute();
		cout << "Another Transaction? Press Y / N :";
		cin >> cont_;
	}
	endSession();
	
}
void Session::authenticate(string input_pw) {
	wrongPasswordAttempts_ = 0;
	while (wrongPasswordAttempts_ < 3) {
		if (input_pw == atm_->getCardPasswordFromBank(currCardNumber_)) {
			cout << "Correct Password\n";
			isAuthenticated_ = true;
			return;
		}
		else {
			wrongPasswordAttempts_++;
			cout << "Wrong Password. Remaining Chance: " << 3 - wrongPasswordAttempts_<< endl;
			return;
		}
	}
	endSession();
	return;
}
void Session::addTransactionOfSession(string history) {
	transactionHistoryOfSession.push_back(history);
}
void Session::endSession() {
	for (string history : transactionHistoryOfSession) {
		cout << "Transactions During current session:\n";
		cout << history << endl;
	}
	delete withdrawl;
	delete deposit;
	delete transfer;
	cout << "Current Session End\n";
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
	ATM* currATM=nullptr;
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
	return 0;
}