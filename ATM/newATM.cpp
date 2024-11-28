#include <iostream>
#include <string> 
#include <cassert>
#include <vector>
#include <map>
#include <fstream>
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
	string korBankName_;
	vector<Account*> accounts_; //account DB
public:
	Bank(string bankName, string KorBankName_);
	//getter, setter
	string getBankName() const { 
		if (!this) return "NULL";
		return bankName_; 
	}
	string getKorBankName() const {
		if (!this) return "NULL";
		return korBankName_;
	}

	//Account Management
	void addAccount(Account* account);
	Account* findAccountByNumber(string accountNumber);
	vector<Account*> findAccountsByOwner(string owner);
	Account* findAccountByCardNumber(string cardNumber);
	string findPasswordOfCard(string cardnumber);
	void updateAccount(string transactiontype, string accountNumber, long amount);
	//Bank Fund management
};
class Account {
private:
	string accountBankName_;
	string accountNumber_; //12-digit
	string ownerName_;
	long balance_;
	string pw_;
	vector<string> transactionHistoryOfAccount_;
	string cardNumber_;
public:
	Account(string bankName, string accountNumber, string ownerName, long initBalance, string pw);

	//getter, setter
	string getAccountBankName() { 
		if (!this) return "NULL";
		return accountBankName_; }
	string getAccountNumber() { 
		if (!this) return "NULL";
		return accountNumber_; }
	string getOwnerName() { 
		if (!this) return "NULL";
		return ownerName_; }
	string getPassword() { 
		if (!this) return "NULL";
		return pw_; }
	long getBalance() { return balance_; }
	string getCardNumber() { 
		if (!this) return "NULL";
		return cardNumber_; }
	vector<string> getTransactionHistory() { return transactionHistoryOfAccount_; }
	//bool isPrimaryBank() const { return (primaryBank_ == 1) ? true : false; }

	//Balance Management
	void accountBalanceUpdate(string transactionType, long amount);

	//validate account number
	static bool validateAccountNumber(string accountNumber);

	void addTransactionHistoryOfAccount(string history);
};
class ATM {
private:
	string atmID_; //6-digit
	bool singleBank_; //ATM type
	string primaryBankName_;
	Bank* ownerBank_;
	bool unilingual_;
	bool isEnglish_ = true;
	map<string,long> availableCash_;
	map<string, long> inputChecks_;
	vector<Bank*> interactableBanks_;
	vector<string> transactionHistoryOfATM_;
public:
	ATM(string atmID, bool singleBank, string bankName, Bank* ownerBank, bool unilingual,  long fiftyK, long tenK, long fiveK, long oneK, vector<Bank*>interactableBanks);
	
	//getter, setter
	std::map<std::string, long> getAvailableCash();
	string getATMID() { return atmID_; }
	string getOwnerBankName() { return primaryBankName_; }
	Bank* getCardBank(string cardNumber);
	vector<string> gettransactionHistoryOfATM() { return transactionHistoryOfATM_; }
	Bank* findGetInteractableBank(string bankname);
	bool isSingleBank() { return singleBank_; } // return true if singlebank(only primary bank card), false if multibank(every bank card)
	bool isEnglish() { return isEnglish_; }
	//update availableCash when deposit, withdrawl
	bool updateAvailableCash(long fiftyK, long tenK, long fiveK, long oneK); //return update o,x
	void depositChecks(string check, long amount);
	//user authorization
	bool isVaildCard(string cardBank);
	bool isUnilingual() { return unilingual_; }
	string getCardPasswordFromBank(string cardNumber);
	void addTransactionHistoryOfATM(string history);
	void changeToKorean() { isEnglish_ = false; }
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
	virtual bool calculateFee() = 0;
	virtual void describeAndSave() = 0;
};
class Withdrawl :public Transaction {
private:
	map<string, long> withdrawlDenominations_;
	//long totalWithdrawlAmount_;
	long fee_;
	
public:
	Withdrawl(ATM* atm, Session* session);
	map<string, long> distributeDenom(long amount);
	void execute() override;
	bool calculateFee() override;
	void describeAndSave() override;
};
class Deposit :public Transaction{
private:
	bool isCash_; //True for cash, False for checks
	map<string, long> depositedCash;
	long numOfCashLimit_;
	long numOfCheckLimit_;
	long fee_;
public:
	Deposit(ATM* atm, Session* session);
	bool validateDeposit();
	void execute() override;
	bool calculateFee() override;
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
	bool calculateFee() override;
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
	bool isActive;
public:
	Session(ATM* currentATM, string cardNumber);
	void authenticate();
	void execute();
	string getCurrCardNumber() { return currCardNumber_; }
	string getCurrCardBankName() { return currCardBankName_; }
	void addTransactionOfSession(string history);
	void endSession(); //whenever user wants or no cash available
	//void adMin();
	void IsNotActive() { isActive = false; }

	string getCurrentSessionCardNumber() { return currCardNumber_; }
	string getCurrAccountNumber() { return currCardAccountNumber_; }
	//string getCurrentSessionCardAccountNumber();
	
};

//Bank Member Function Defined-----------------------------------------------
Bank::Bank(string bankName, string korBankName) {
	bankName_ = bankName;
	korBankName_ = korBankName;
}
void Bank::addAccount(Account* account) {
	accounts_.push_back(account);
}
Account* Bank::findAccountByNumber(string accountNumber)
 {
	for (Account* acc : accounts_) {
		if (acc->getAccountNumber() == accountNumber) return acc;
	}
	//cout << accountNumber << " account is not found in " << bankName_ << " DB.\n";
	return nullptr;
}
vector<Account*> Bank::findAccountsByOwner(string owner) {
	vector<Account*> temp;
	for (Account* acc : accounts_) {
		if (acc->getOwnerName() == owner) temp.push_back(acc);
	}

	if (!temp.empty()) return temp;
	else {
		//cout << owner << "'s account is not found" << endl;
		return temp;
	}
}
string Bank::findPasswordOfCard(string cardnumber) {
	if (!this) return "NULL";
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
		tempAcc->accountBalanceUpdate(transactiontype,amount);
	}
	else if (transactiontype == "Deposit") {
		Account* tempAcc = findAccountByNumber(accountNumber);
		tempAcc->accountBalanceUpdate(transactiontype, amount);
	}
	else if (transactiontype == "TransferSend") {
		Account* tempAcc = findAccountByNumber(accountNumber);
		tempAcc->accountBalanceUpdate(transactiontype, amount);
	}
	else if (transactiontype == "TransferReceive") {
		Account* tempAcc = findAccountByNumber(accountNumber);
		tempAcc->accountBalanceUpdate(transactiontype, amount);
	}
}
//---------------------------------------------------------------------------

//Account Member Function Defined--------------------------------------------
Account::Account(string bankName, string accountNumber, string ownerName, long initBalance, string pw) {
	accountBankName_ = bankName;
	accountNumber_ = accountNumber;
	ownerName_ = ownerName;
	balance_ = initBalance;
	cardNumber_ = "00" + accountNumber + "00";
	pw_ = pw;
	cout << "Account Constructed (Account Bank name: " << accountBankName_ << ", Account Number: " << accountNumber_ <<", Owner: "<<ownerName_<<", Balance : "<<balance_<<")\n";
	cout << "Issued Card Number (connected to " << accountNumber_ << "account) is " << cardNumber_<<"\n";
	
}
void Account::accountBalanceUpdate(string transactionType, long amount) {
	if (transactionType == "Withdrawl") {
		balance_ -= amount;
	}
	if (transactionType == "Deposit") {
		balance_ += amount;
	}
	if (transactionType == "TransferSend") {
		balance_ -= amount;
	}
	if (transactionType == "TransferReceive") {
		balance_ += amount;
	}
}
bool Account::validateAccountNumber(string accountNum) {
	if (atoi(accountNum.c_str()) != 0 && accountNum.length() == 12) return true;
	else return false;
}
void Account::addTransactionHistoryOfAccount(string history) {
	transactionHistoryOfAccount_.push_back(history);
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
ATM::ATM(string atmID, bool singleBank, string bankName, Bank* ownerBank, bool unilingual, long fiftyK, long tenK, long fiveK, long oneK, vector<Bank*> interactableBanks) {
	atmID_ = atmID;
	singleBank_ = singleBank;
	unilingual_ = unilingual;
	primaryBankName_ = bankName;
	ownerBank_ = ownerBank;
	availableCash_.insert(pair<string,long>("50000won", fiftyK));
	availableCash_.insert(pair<string, long>("10000won", tenK));
	availableCash_.insert(pair<string, long>("5000won", fiveK));
	availableCash_.insert(pair<string, long>("1000won", oneK));
	interactableBanks_ = interactableBanks;
	if (unilingual_) cout << "ATM Constructed (ATM SN: " << atmID_ << ", Unilingual" << ")\n";
	else cout << "ATM Constructed (ATM SN: " << atmID_ << ",Bilingual" << ")\n";
}
map<std::string, long> ATM::getAvailableCash() {
	return availableCash_;
}
Bank* ATM::findGetInteractableBank(string bankname) {
	for (Bank* bank : interactableBanks_) {
		if (bank->getBankName() == bankname) return bank;
	}
	if (isEnglish_) cout << bankname << " is not existing bank\n";
	else cout << bankname << "은 존재하지 않습니다.\n";
	return nullptr;
}
bool ATM::updateAvailableCash(long fiftyK, long tenK, long fiveK, long oneK) { //input must include +,-
	//check if bills are sufficient
	if (availableCash_["50000won"] + fiftyK < 0) {
		if (isEnglish_) cout << "Insufficient 50000won bills\n";
	    else cout << "오만원권 부족\n";
		return false;
	}
	if (availableCash_["10000won"] + tenK < 0) {
		if (isEnglish_) cout << "Insufficient 10000won bills\n";
		else cout << "만원권 부족\n";
		return false;
	}
	if (availableCash_["5000won"] + fiveK < 0) {
		if (isEnglish_) cout << "Insufficient 5000won bills\n";
		else cout << "오천원권 부족\n";
		return false;
	}
	if (availableCash_["1000won"] + oneK < 0) {
		if (isEnglish_) cout << "Insufficient 1000won bills\n";
		else cout << "천원권 부족\n";
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
		if (primaryBankName_ == cardBank) {
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
	if (isEnglish_) cout << "This Card does not below to any Bank\n";
	else cout << "이 카드는 어느 은행의 카드도 아닙니다.\n";
	return nullptr;
}
void ATM::addTransactionHistoryOfATM(string history) {
	transactionHistoryOfATM_.push_back(history);
}	
string ATM::getCardPasswordFromBank(string cardNumber) {
	return getCardBank(cardNumber)->findPasswordOfCard(cardNumber);
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
	fee_ = 0;
}
map<string,long> Withdrawl::distributeDenom(long amount) {
	long temp = amount;
	if (temp >= 50000) {
		withdrawlDenominations_["50000won"] = temp / 50000;
		temp -= withdrawlDenominations_["50000won"] * 50000;
	}
	if (temp >= 10000) {
		withdrawlDenominations_["10000won"] = temp / 10000;
		temp -= withdrawlDenominations_["10000won"] * 10000;
	}
	if (temp >= 5000) {
		withdrawlDenominations_["5000won"] = temp / 5000;
		temp -= withdrawlDenominations_["5000won"] * 5000;
	}
	if (temp >= 1000) {
		withdrawlDenominations_["1000won"] = temp / 1000;
	}

	return withdrawlDenominations_;
}
void Withdrawl::execute() {
	if (atm_->isEnglish()) cout << "<Withdrawl>\n";
	else cout << "<인출>\n";

	if (!calculateFee()) return;

	if (atm_->isEnglish()) cout << "Please Enter the desired amount of cash you want to withdrawl (No coins are dispensed). AMOUNT(won): ";
	else cout << "인출하고자 하는 현금의 금액을 입력하십시오. (동전 인출 불가) 금액(원): ";

	cin >> amount_;
	if (amount_ < 1000 || amount_ % 1000 != 0) {
		if (atm_->isEnglish()) cout << "Wrong Input. Only amount that can be dispensed by 1000, 5000, 10000, 50000won bills is allowed.\n";
		else cout << "잘못된 금액 입력. 천원권, 오천원권, 만원권, 오만원권으로 수령할 수 있는 금액만 허용됩니다.\n";
		session_->IsNotActive();
		return;
	}
	if (amount_ + fee_ > atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance()) {
		if (atm_->isEnglish()) {
			cout << "Insufficient Account Balance. Current Balance: " <<
				atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << " won.\n";
		}
		else cout << "계좌 잔액 부족. 잔액: " << atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << " 원\n";
		session_->IsNotActive();
		return ;
	}
	if (distributeDenom(amount_)["50000won"] > atm_->getAvailableCash()["50000won"]) {
		if (atm_->isEnglish()) cout << "Insufficient 50000won bills\n";
		else cout << "오만원권 부족\n";
		session_->IsNotActive();
		return ;
	}
	if (distributeDenom(amount_)["10000"] > atm_->getAvailableCash()["10000"]) {
		if (atm_->isEnglish())cout << "Insufficient 10000won bills\n";
		else "만원권 부족\n";
		session_->IsNotActive();
		return ;
	}
	if (distributeDenom(amount_)["5000won"] > atm_->getAvailableCash()["5000won"]) {
		if (atm_->isEnglish()) cout << "Insufficient 5000won bills\n";
		else cout << "오천원권 부족\n";
		session_->IsNotActive();
		return ;
	}
	if (distributeDenom(amount_)["1000won"] > atm_->getAvailableCash()["1000won"]) {
		if (atm_->isEnglish()) cout << "Insufficient 1000won bills\n";
		else cout << "천원권 부족\n";
		session_->IsNotActive();
		return ;
	}
	atm_->getCardBank(session_->getCurrCardNumber())->updateAccount(transactionType_, session_->getCurrAccountNumber(), amount_+fee_);
	atm_->updateAvailableCash(-distributeDenom(amount_)["50000won"], -distributeDenom(amount_)["10000won"], -distributeDenom(amount_)["5000won"], -distributeDenom(amount_)["1000won"]);
	cout << " \n";
	if (atm_->isEnglish()) {
		cout << "Get the Cash. 50000won bill: " << distributeDenom(amount_)["50000won"] << ", 10000won bill: " << distributeDenom(amount_)["10000won"]
			<< ", 5000won bill : " << distributeDenom(amount_)["5000won"] << ", 1000won bill : " << distributeDenom(amount_)["1000won"] << endl;
	}
	else {
		cout << "현금을 수령하시오. 오만원권: " << distributeDenom(amount_)["50000won"] << ", 만원권: " << distributeDenom(amount_)["10000won"]
			<< ", 오천원권: " << distributeDenom(amount_)["5000won"] << ", 천원권: " << distributeDenom(amount_)["1000won"] << endl;
	}
	
	describeAndSave();
	
	if (atm_->isEnglish()) {
		cout << "\nWithdrawl " << amount_ << " won from (" << session_->getCurrCardBankName() << " , NO: " << session_->getCurrAccountNumber() << ")\n";
		cout << "Balance: " << atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
	}
	else {
		cout << "(" << session_->getCurrCardBankName() << ", 계좌번호: " << session_->getCurrAccountNumber() << ")에서 "<< amount_ << "원 인출됨. \n";
		cout << "잔액: " << atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
	}
	amount_ = 0;
}	
bool Withdrawl::calculateFee() { //update fee
	if (atm_->getOwnerBankName() == session_->getCurrCardBankName()) {
		if (atm_->isEnglish()) cout << "1000won fee will be paid from your account when withdrawl is completed.\n";
		else cout << "인출이 완료되면 수수료 1000원이 고객님 계좌에서 출금됩니다.\n";
		fee_ = 1000;
	}
	else {
		if (atm_->isEnglish()) cout << "2000won fee will be paid from your account when withdrawl is completed.\n";
		else cout << "인출이 완료되면 수수료 2000원이 고객님 계좌에서 출금됩니다.\n";
		fee_ = 2000;
	}
	return true;
}
void Withdrawl::describeAndSave() {
	string history = "ATM SN: " + atm_->getATMID() + " |TransactionID: " + to_string(transID_) + " |Card Number : " + session_->getCurrCardNumber() + " |Account Number : " + session_->getCurrAccountNumber() + " |" + transactionType_ + " |Amount : " + to_string(amount_) + " |Fee: " + to_string(fee_);
	string korHistory = "ATM 고유번호 : " + atm_->getATMID() + " |거래번호 : " + to_string(transID_) + " |카드번호 : " + session_->getCurrCardNumber() + " |계좌번호 : " + session_->getCurrAccountNumber() + " |" + "인출" + " |금액 : " + to_string(amount_) + " |수수료: " + to_string(fee_);
	atm_->addTransactionHistoryOfATM(history);
	if (atm_->isEnglish()) session_->addTransactionOfSession(history);
	else session_->addTransactionOfSession(korHistory);
	atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->addTransactionHistoryOfAccount(history);
	//cout << history << endl;
	cout << " \n";
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
	fee_ = 0;
}
bool Deposit::validateDeposit() {
	if (isCash_) { //cash
		long totalsheet_= depositedCash["50000won"] + depositedCash["10000won"] + depositedCash["5000won"] + depositedCash["1000won"];
		if (totalsheet_ > numOfCashLimit_) {
			if (atm_->isEnglish()) cout << "Total amount of cash should be less or equal to 50" << endl;
			else cout << "전체 현금은 50장 이하여야 합니다." << endl;
			return false;
		}
		if (totalsheet_ == 0) {
			if (atm_->isEnglish()) cout << "No cash inserted.\n";
			else cout << "현금이 주입되지 않았습니다.\n";
			return false;
		}
		else return true;
	}
	return false;
}
void Deposit::execute() {
	if (atm_->isEnglish()) cout << "<Deposit>\n";
	else cout << "<입금>\n";
	int pick = 0;
	while (pick != 1 && pick != 2) {
		if (atm_->isEnglish()) cout << "Enter 1 for Cash Deposit. Enter 2 for Check Deposit.: ";
		else cout << "현금 입금은 1, 수표 입금은 2를 누르시오.: ";
		cin >> pick;
		if (pick == 1) isCash_ = true;
		else if (pick == 2) isCash_ = false;
		else {
			if (atm_->isEnglish()) cout << "Wrong Input. Enter 1 or 2.\n";
			else cout << "잘못된 입력. 1과 2 중 입력하시오.\n";
	}
	if (isCash_) { //cash deposit
		long fiftyK;
		long tenK;
		long fiveK;
		long oneK;
		bool success = false;

		if (atm_->isEnglish()) cout << "Place the cash into Slot.\n";
		else cout << "현금을 투입구에 넣어주십시오.\n";

		if (atm_->isEnglish()) cout << "KRW 50000 bills: ";
		else cout << "오만원권: ";
		cin >> fiftyK;
		depositedCash["50000won"] = fiftyK;

		if (atm_->isEnglish()) cout << "KRW 10000 bills: ";
		else cout << "만원권: ";
		cin >> tenK;
		depositedCash["10000won"] = tenK;

		if (atm_->isEnglish()) cout << "KRW 5000 bills: ";
		else cout << "오천원권: ";
		cin >> fiveK;
		depositedCash["5000won"] = fiveK;

		if (atm_->isEnglish()) cout << "KRW 1000 bills: ";
		else cout << "천원권: ";
		cin >> oneK;
		depositedCash["1000won"] = oneK;

		amount_ = fiftyK * 50000 + tenK * 10000 + fiveK * 5000 + oneK * 1000;
		if (!validateDeposit()) {
			session_->IsNotActive();
			return;
		}
		if (!calculateFee()) {
			session_->IsNotActive();
			return;
		}
		atm_->getCardBank(session_->getCurrCardNumber())->updateAccount(transactionType_, session_->getCurrAccountNumber(), amount_);
		if (!atm_->updateAvailableCash(fiftyK, tenK, fiveK, oneK)) {
			session_->IsNotActive();
			return;
		}
		
		if (atm_->isEnglish()) {
			cout << "\nDeposit " << amount_ << " won to (" << session_->getCurrCardBankName() << " , NO: " << session_->getCurrAccountNumber() << ")\n";
			cout << "Balance: " << atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
		else{
			cout << "(" << session_->getCurrCardBankName() << ", 계좌번호: " << session_->getCurrAccountNumber() << ")으로 " << amount_ << "원 입금됨. \n";
			cout << "잔액: " << atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
	}

	else { //check deposit
		if (atm_->isEnglish()) cout << "Place the checks into Slot(Limit: 30 sheets)\n";
		else cout << "수표를 투입구에 넣어주십시오.(상한: 30장)\n";
		long totalsheet=0;
		while (true) {
			if (atm_->isEnglish()) cout << "Amount of this check(must be greater or equal to KRW 10000) (Enter 'Stop' for Termination of Input): ";
			else cout << "수표 금액(10만원 이상의 금액) (수표 입금을 완료하였으면 '종료'를 입력하십시오.): ";
			string check;
			cin >> check;
			if (check == "Stop") break;
			if (stol(check) < 10000) {
				if (atm_->isEnglish()) cout << "Wrong Check\n";
				else cout << "잘못된 수표 금액\n";
				session_->IsNotActive();
				return;
			}
			if (atm_->isEnglish()) cout << "How many sheets of this check: ";
			else cout << "해당 금액의 수표 장수: ";
			long sheet;
			cin >> sheet;
			totalsheet += sheet;
			if (totalsheet > 30) {
				if (atm_->isEnglish()) cout << "Exceeded sheet limit(30 sheets). \n";
				else cout << "수표 투입 상한 장수 초과(30장).\n";
				session_->IsNotActive();
				return;
			}
			atm_->depositChecks(check, sheet);
			amount_ += (stol(check) * sheet);
		}
		
		if (!calculateFee()) {
			session_->IsNotActive();
			return;
		}
		if (atm_->isEnglish()) {
			cout << "\nDeposit " << amount_ << " won to (" << session_->getCurrCardBankName() << " , NO: " << session_->getCurrAccountNumber() << ")\n";
			cout << "Balance: " << atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
		else {
			cout << "(" << session_->getCurrCardBankName() << ", 계좌번호: " << session_->getCurrAccountNumber() << ")으로 " << amount_ << "원 입금됨. \n";
			cout << "잔액: " << atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
		atm_->getCardBank(session_->getCurrCardNumber())->updateAccount(transactionType_, session_->getCurrAccountNumber(), amount_);
	}
	describeAndSave();
	amount_ = 0;
}
bool Deposit::calculateFee() {
	fee_ = 0;
	if (atm_->getOwnerBankName() == session_->getCurrCardBankName()) {
		if (atm_->isEnglish()) cout << "Pay 1000won for deposit fee into slot. SLOT: [1000won bills]: ";
		else cout << "입금 수수료 1000원을 투입구에 넣어주십시오. [천원권]: ";

		cin >> fee_;
		fee_ *= 1000;
		if (fee_ != 1000) {
			if (atm_->isEnglish()) cout << "Wrong amount is paid for fee. Cash Return: [1000won bills]: " << fee_ / 1000 << endl;
			else cout<<"잘못된 수수료 금액이 투입되었습니다. 현금 반환: [천원권]: " << fee_ / 1000 << endl;
			return false;
		}
		atm_->updateAvailableCash(0, 0, 0, 1);
		return true;
	}
	else {
		if (atm_->isEnglish()) cout << "First, Pay 2000won for deposit fee into slot.SLOT: [1000won bills]: ";
		else cout << "입금 수수료 2000원을 투입구에 넣어주십시오. [천원권]: ";
		cin >> fee_;
		fee_ *= 1000;
		if (fee_ != 2000) {
			if (atm_->isEnglish()) cout << "Wrong amount is paid for fee. Cash Return: [1000won bills]: " << fee_ / 1000 << endl;
			else cout << "잘못된 수수료 금액이 투입되었습니다. 현금 반환: [천원권]: " << fee_ / 1000 << endl;

			return false;
		}
		atm_->updateAvailableCash(0, 0, 0, 2);
		return true;
	}
}
void Deposit::describeAndSave() {
	if (isCash_) {
		string history = "ATM SN: " + atm_->getATMID() + " |TransactionID: " + to_string(transID_)+" |Card Number : " + session_->getCurrCardNumber() + " |Account Number : " + session_->getCurrAccountNumber() + " |Cash " + transactionType_ + " |Amount : " + to_string(amount_) + " |Fee: " + to_string(fee_);
		string korHistory = "ATM 고유번호 : " + atm_->getATMID() + " |거래번호 : " + to_string(transID_) + " |카드번호 : " + session_->getCurrCardNumber() + " |계좌번호 : " + session_->getCurrAccountNumber() + " |" + "현금 입금" + " |금액 : " + to_string(amount_) + " |수수료: " + to_string(fee_);
		atm_->addTransactionHistoryOfATM(history);
		if (atm_->isEnglish()) session_->addTransactionOfSession(history);
		else session_->addTransactionOfSession(korHistory);
		atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->addTransactionHistoryOfAccount(history);
		//cout << history << endl;
	}
	else {
		string history = "ATM SN: " + atm_->getATMID() + " |TransactionID: " + to_string(transID_) + " |Card Number : " + session_->getCurrCardNumber() + " |Account Number : " + session_->getCurrAccountNumber() + " |Check " + transactionType_ + " |Amount : " + to_string(amount_) + " |Fee: " + to_string(fee_);
		string korHistory = "ATM 고유번호 : " + atm_->getATMID() + " |거래번호 : " + to_string(transID_) + " |카드번호 : " + session_->getCurrCardNumber() + " |계좌번호 : " + session_->getCurrAccountNumber() + " |" + "수표 입금" + " |금액 : " + to_string(amount_) + " |수수료: " + to_string(fee_);

		atm_->addTransactionHistoryOfATM(history);
		session_->addTransactionOfSession(history);
		atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->addTransactionHistoryOfAccount(history);
		//cout << history << endl;
		cout << " \n";
	}
}

//---------------------------------------------------------------------------

//Transfer Member Function Defined
Transfer::Transfer(ATM* atm, Session* session):Transaction(atm, session) {
	transactionType_ = "Transfer";
	fee_ = 0;
	cashToAccount_ = 0;
}
bool Transfer::validateReceiverAccounts() {
	if (!atm_->findGetInteractableBank(receiveBankName_)) {
		return false;
	}
	if (!atm_->findGetInteractableBank(receiveBankName_)->findAccountByNumber(receiveAccountNumber_)) {
		//[4] cout << receiveAccountNumber_ << " account is not found in " << receiveBankName_ << " DB.\n";
		return false;
	}
	string receiver = atm_->findGetInteractableBank(receiveBankName_)->findAccountByNumber(receiveAccountNumber_)->getOwnerName();
	//[5] cout << "Is the receiver's name is " << receiver << [5-1]"? Enter Y for yes, N for no. : ";
	string yn;
	cin >> yn;
	if (yn == "N") {
		//[6]cout << "Please check account number and bank name of receiver again.\n";
		return false;
	}
	return true;
}
void Transfer:: execute() {
	cout << "<Transfer>\n";
	int pick = 0;
	while (pick != 1 && pick != 2) {
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
	if (!success) {
		session_->IsNotActive();
		return;
	}
	if (!calculateFee()) {
		session_->IsNotActive();
		return;
	}
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
		if (amount_-fee_<=0) {
			cout << "Insufficient input for transfer.\n";
			session_->IsNotActive();
			return;
		}
		atm_->updateAvailableCash(fiftyK, tenK, fiveK, oneK);
		atm_->findGetInteractableBank(receiveBankName_)->updateAccount(transactionType_ + "Receive", receiveAccountNumber_, amount_);
		describeAndSave();

		cout << "\nTransfer " << amount_ << " won to (" << receiveBankName_ << " , NO: " << receiveAccountNumber_ << ")\n";
		cout << "Balance: " << atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		return;
	}
	else {
		cout << "Enter Amount to Transfer: ";
		cin >> amount_;

		if (amount_+fee_ > atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance()) {
			cout << "Insufficient Balance.\n";
			session_->IsNotActive();
			return;
		}
		atm_->findGetInteractableBank(receiveBankName_)->updateAccount(transactionType_ + "Receive", receiveAccountNumber_, amount_);
		atm_->findGetInteractableBank(session_->getCurrCardBankName())->updateAccount(transactionType_ + "Send", session_->getCurrAccountNumber(), amount_+fee_);
		describeAndSave();

		cout << "\nTransfer " << amount_ << " won to (" << receiveBankName_ << " , NO: " << receiveAccountNumber_ << ")\n";
		cout << "Balance: " << atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		return;
	}
	
	
}
bool Transfer::calculateFee() {
	if (cashToAccount_) {
		cout << "When you insert the cash to transfer into slot, you must include 1000won for fee.\n";
		fee_ = 1000;
	}
	else{
		if (atm_->getOwnerBankName()==receiveBankName_ && atm_->getOwnerBankName() == session_->getCurrCardBankName()) {
			cout << "2000won will be paid from your account.\n";
			fee_ = 2000;
		}
		else if (atm_->getOwnerBankName() != receiveBankName_ && atm_->getOwnerBankName() != session_->getCurrCardBankName()) {
			cout << "4000won will be paid from your account.\n";
			fee_ = 4000;
		}
		else {
			cout << "3000won will be paid from your account.\n";
			fee_ = 3000;
		}
	}
	return true;
}
void Transfer::describeAndSave() {
	if (cashToAccount_) {
		string history = "ATM SN: " + atm_->getATMID() + " |TransactionID: " + to_string(transID_) + " |Card Number : " + session_->getCurrCardNumber() + " |Receiver Account Number: " + receiveBankName_ + " " + receiveAccountNumber_ + " |" + "Cash " + transactionType_ + " |Amount: " + to_string(amount_) + " |Fee: " + to_string(fee_);
		atm_->addTransactionHistoryOfATM(history);
		session_->addTransactionOfSession(history);

		string reAccHistory = "Received " + to_string(amount_) + " from " + session_->getCurrCardBankName() + session_->getCurrCardNumber();
		atm_->findGetInteractableBank(receiveBankName_)->findAccountByNumber(receiveAccountNumber_)->addTransactionHistoryOfAccount(reAccHistory);
	
		cout << history << endl;
		cout << " \n";
	}
	else {
		string history = "ATM SN: " + atm_->getATMID() + " |TransactionID: " + to_string(transID_) + " |Card Number: " + session_->getCurrCardNumber() + " |Source Account Number : " + session_->getCurrAccountNumber() + " |Receiver Account Number : " + receiveBankName_+" "+ receiveAccountNumber_ + " |" + "Account " + transactionType_ + " |Amount : " + to_string(amount_) + " |Fee: " + to_string(fee_);
		atm_->addTransactionHistoryOfATM(history);
		session_->addTransactionOfSession(history);

		string reAccHistory = "Received " + to_string(amount_) + " from " + session_->getCurrCardBankName() + session_->getCurrCardNumber();
		atm_->findGetInteractableBank(receiveBankName_)->findAccountByNumber(receiveAccountNumber_)->addTransactionHistoryOfAccount(reAccHistory);
		string seAccHistory = "Sent" + to_string(amount_) + " to " + receiveBankName_ + receiveAccountNumber_;
		atm_->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->addTransactionHistoryOfAccount(seAccHistory);
		cout << history << endl;
		cout << " \n";
	}
}
//---------------------------------------------------------------------------

//Session Member Function Defined
Session::Session(ATM* currentATM, string cardNumber) {
	atm_ = currentATM;
	/*/if (cardNumber == "Admin") {
		currCardNumber_ = cardNumber;
		adMin();
	}
	else {*/
		currCardNumber_ = cardNumber;
		currCardBankName_ = atm_->getCardBank(cardNumber)->getBankName();
		if (currCardBankName_ == "NULL") {
			IsNotActive();
			return;
		}
		currCardAccountNumber_ = atm_->getCardBank(cardNumber)->findAccountByCardNumber(cardNumber)->getAccountNumber();
		wrongPasswordAttempts_ = 0;
		isAuthenticated_ = false;
		isActive = true;
	
	

	
}
void Session::execute() {
	// 1. Is card valid to this ATM type
	if (currCardBankName_ == "NULL") {
		//cout << "Card Not Found.\n";
		return;
	}
	//if (currCardNumber_ == "Admin") return;

	if (!atm_->isVaildCard(currCardBankName_)) {
		cout << "INVALID CARD. This is a Single Bank ATM. Only card issued by " << atm_->getOwnerBankName() << " is allowed.\n";
		endSession();
		return;
	}

	//2. PW validation

	authenticate();
	if (!isAuthenticated_) {
		endSession();
		IsNotActive();
		return;
	}
	int menuPick;
	string cont_ = "Y";
	while (isActive && cont_ == "Y") {
		cout << "Choose the Number of Transaction\n";
		cout << "1. Withdrawl\n";
		cout << "2. Deposit\n";
		cout << "3. Transfer\n";
		cout << "4. Cancel\n";
		cout << "->";

		cin >> menuPick;
		if (menuPick == 1) {
			if (withdrawlTimes >= 3) {
				cout << "Only 3 times of Withdrawl are allowed per session.\n";
				IsNotActive();
				endSession();
				return;
			}
			withdrawl = new Withdrawl(atm_, this);
			withdrawl->execute();
			delete withdrawl;
			if (!isActive) {
				endSession();
				return;
			}
			withdrawlTimes++;
			
		}
		else if (menuPick == 2) {
			deposit = new Deposit(atm_, this);
			deposit->execute();
			delete deposit;
			if (!isActive) {
				endSession();
				return;
			}
		}
		else if (menuPick == 3) {
			transfer = new Transfer(atm_, this);
			transfer->execute();
			delete transfer;
			if (!isActive) {
				endSession();
				return;
			}
		}
		else if (menuPick == 4) {
			endSession();
			IsNotActive();
			return;
		}
		cout << "Another Transaction? Press Y / N :";
		cin >> cont_;
	}
}
void Session::authenticate() {
	wrongPasswordAttempts_ = 0;
	string input_pw;
	while (wrongPasswordAttempts_ < 3) {
		cout << "Password: ";
		cin >> input_pw;
		if (input_pw == atm_->getCardBank(currCardNumber_)->findPasswordOfCard(currCardNumber_)) {
			cout << "Correct Password\n";
			isAuthenticated_ = true;
			return;
		}
		else {
			wrongPasswordAttempts_++;
			cout << "Wrong Password. Remaining Chance: " << 3 - wrongPasswordAttempts_<< endl;
		}
	}
	return;
}
void Session::addTransactionOfSession(string history) {
	transactionHistoryOfSession.push_back(history);
}
void Session::endSession() {
	cout << "Card Return. Get your card: " << currCardNumber_ << ".\n";
	cout << "\n------------------------------\n";
	for (string history : transactionHistoryOfSession) {
		cout << "Transactions During current session:\n";
		cout << history << endl;
	}
	isActive = false;
	cout << "Thank you for using our ATM.\n";
}
/*void Session::adMin() {
	cout << "Menu for Admin\n";
	cout << "1. Transaction History\n";
	cout << "->";
	int menupick;
	cin >> menupick;
	if (menupick == 1) {
		for (string history : atm_->gettransactionHistoryOfATM()) cout << history << "\n";
	}
	string fileName = "TransactionHistory.txt";

	// Create an ofstream object to handle file writing
	ofstream outFile(fileName);

	// Check if the file is open and writable
	if (outFile.is_open()) {
		for (string history : atm_->gettransactionHistoryOfATM()) {
			// Write to file
			outFile << history << "\n"; // Add a newline for formatting
		}
		// Close the file
		outFile.close();
		cout << "Transaction history successfully saved to " << fileName << endl;
	}
	else {
		// Handle the error if the file cannot be opened
		cerr << "Error: Could not open the file for writing." << endl;
	}
}*/
//---------------------------------------------------------------------------
void screenShot(vector<Account*> accounts, vector<ATM*>atms) {
	cout << "Enter / for screenshot: ";
	char slash;
	cin >> slash;
	if (slash == 47) {
		for (ATM* atm : atms) {
			long fiftyK = atm->getAvailableCash()["50000won"];
			long tenK = atm->getAvailableCash()["10000won"];
			long fiveK = atm->getAvailableCash()["5000won"];
			long oneK = atm->getAvailableCash()["1000won"];
			long total = fiftyK * 50000 + tenK * 10000 + fiveK * 5000 + oneK * 1000;

			cout << "ATM[SN: " << atm->getATMID() << ", Bank: " << atm->getOwnerBankName() << ", Remaining Cash: (50000 Won Bills: " << fiftyK << ", 10000 Won Bills: " << tenK << ", 5000 Won Bills: " << fiveK << ", 1000 Won Bills: " << oneK << "]\n";
		}
		for (Account* acc : accounts) {
			cout << "Account[Bank: " << acc->getAccountBankName() << ", No : " << acc->getAccountNumber() << ", Owner : " << acc->getOwnerName() << "] balance: " << acc->getBalance() << endl;
		}
	}
	return;
}


int main() {
	vector<Bank*> banks;  // Vector to store pointers to Bank objects
	bool bank_status = true;

	cout << "<Bank Initialization>\n";
	while (bank_status) {
		string bankName;
		cout << "Bank Name[ENGLISH](without space): ";
		cin >> bankName;
		string korBankName;
		cout << "Bank Name[KOREAN](without space): ";
		cin >> korBankName;

		// Dynamically allocate a new Bank object and store its pointer in the vector
		Bank* newBank = new Bank(bankName, korBankName);
		banks.push_back(newBank);

		cout << "---------------------------------------";

		// More addition
		char continue_input;
		cout << "Add another bank? (y/n): ";
		cin >> continue_input;
		if (continue_input != 'y' && continue_input != 'Y') {
			bank_status = false;
		}
	}
	for (Bank* bank : banks) {
		cout << bank->getBankName() << " ";
	}
	cout << " \n";
	cout << endl;

	cout << "Account Initialization\n";
	bool account_status = true;
	vector<Account*> accounts;  // Vector to store pointers to Account objects
	while (account_status) {
		string accbankName, accountNumber, ownerName, pw;
		long initBalance;

		cout << "Bank Name: ";
		cin >> accbankName;

		cout << "Account Number: ";
		cin >> accountNumber;

		bool inputtype = true;
		for (char c : accountNumber) {
			if (!isdigit(c)) {
				cout << "Account number is 12-digit.";
				inputtype = false;
				break;
			}
		}
		if (accountNumber.length() != 12) {
			cout << "Wrong length of account number.\n";
			continue;
		}

		for (Account* acc : accounts) {
			if (acc->getAccountNumber() == accountNumber) {
				cout << "Same account number exists.\n";
				inputtype = false;
			}
		}
		if (!inputtype) continue;

		cout << "Owner Name: ";
		cin >> ownerName;

		cout << "Initial Balance: ";
		cin >> initBalance;

		cout << "Initialize Password: ";
		cin >> pw;

		
		Account* newAccount = new Account(accbankName, accountNumber, ownerName, initBalance, pw);
		accounts.push_back(newAccount);

		// Link account to the bank
		bool linked = false;
		for (Bank* bank : banks) {
			if (bank->getBankName() == accbankName) {
				bank->addAccount(newAccount);
				linked = true;
				break;
			}
		}

		if (!linked) {
			cout << "No Bank for this account exists\n";
		}

	
		cout << "---------------------------------------";

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
	vector<ATM*> ATMs;  // Vector to store pointers to ATM objects
	while (ATM_status) {
		string atmID, isSingleBank, atmbankName, lingual;
		bool singleBank, unilingual;
		long fiftyK, tenK, fiveK, oneK;
		vector<Bank*> interactableBanks;
		Bank* ownerBank = nullptr;

		cout << "Bank Name: ";
		cin >> atmbankName;
		cout << "ATM serial Number(6-digit serial number): ";
		cin >> atmID;
		cout << "Press S for Single Bank ATM, M for Multi Bank ATM: ";
		cin >> isSingleBank;
		singleBank = (isSingleBank == "S");

		cout << "Unilingual or Bilingual (Press U for unilingual, B for Bilingual): ";
		cin >> lingual;
		unilingual = (lingual == "U");

		cout << "Number of 50000won bill: ";
		cin >> fiftyK;
		cout << "Number of 10000won bill: ";
		cin >> tenK;
		cout << "Number of 5000won bill: ";
		cin >> fiveK;
		cout << "Number of 1000won bill: ";
		cin >> oneK;

		// Find the owner bank
		for (Bank* bank : banks) {
			interactableBanks.push_back(bank);
			if (bank->getBankName() == atmbankName) {
				ownerBank = bank;
			}
		}

		if (!ownerBank) {
			cout << atmbankName << " does not exist.\n";
			continue;
		}


		ATM* newATM = new ATM(atmID, singleBank, atmbankName, ownerBank, unilingual, fiftyK, tenK, fiveK, oneK, interactableBanks);
		ATMs.push_back(newATM);

	
		cout << "---------------------------------------";

		// More addition
		char continue_input;
		cout << "Add another ATM? (y/n): ";
		cin >> continue_input;
		if (continue_input != 'y' && continue_input != 'Y') {
			ATM_status = false;
		}
	}

	screenShot(accounts, ATMs);
	
	



	bool atmON = true;
	bool exeStatus = true;
	while (exeStatus) {
		cout << " \nSelect ATM by Serial Number: ";
		string atmSN;
		cin >> atmSN;

		ATM* currATM = nullptr;
		for (ATM* atm : ATMs) {
			if (atmSN == atm->getATMID()) {
				currATM = atm;
			}
		}

		while (atmON) {
			//Select Language
			if (!currATM->isUnilingual()) {
				cout << "Select Language 언어를 선택하시오. 한국어는 0을 누르시오. Enter 1 for English. :";
				bool lang;
				cin >> lang;
				if (lang == false) currATM->changeToKorean();
			}
			//Start ATM Operation ---------------------------------

			//[1] cout << "Welcome! Please insert Card (Write your card number):\n";
			string cardnumber;
			cin >> cardnumber;

			//Admin Card inserted
			if (cardnumber == "Admin") {
				cout << "Menu for Admin\n";
				cout << "1. Transaction History\n";
				cout << "->";
				int menupick;
				cin >> menupick;
				static int filenumber;
				if (menupick == 1) {
					string fileName = "TransactionHistory"+to_string(filenumber)+".txt";
					filenumber++;
					ofstream outFile(fileName);
					// Check if the file is open and writable
					if (outFile.is_open()) {
						for (ATM* atm : ATMs) {
							for (string history : atm->gettransactionHistoryOfATM()) {
								// Write to file
								cout << history << endl;
								outFile << history << "\n"; // Add a newline for formatting
							}
						}
						outFile.close();
						cout << "Transaction history successfully saved to " << fileName << endl;
					}
					else {
						// Handle the error if the file cannot be opened
						cerr << "Error: Could not open the file for writing." << endl;
					}	
				}
				continue;
			}

			//Session Start
			Session* currSession = new Session(currATM, cardnumber);
			currSession->execute();
			screenShot(accounts, ATMs);

			//[2] cout << "Enter 'EXIT' to exit this ATM. Enter any key to continue using this ATM. : ";
			string exit;
			cin >> exit;
			if (exit == "EXIT" || exit == "나가기") break;
		}
		//[3]cout << "Enter END to exit this program. Enter any key to continue. : ";
		string end;
		cin >> end;
		if (end == "END" || end == "종료") break;
	}
	

	// Clean up dynamically allocated memory at the end
	for (Bank* bank : banks) delete bank;
	for (Account* account : accounts) delete account;
	for (ATM* atm : ATMs) delete atm;

	return 0;
}