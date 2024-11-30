#include <iostream>
#include <string> 
#include <cassert>
#include <vector>
#include <map>
#include <fstream>
#include <stdexcept>
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
	vector<ATM*> atms_;
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

	Bank& operator+(Account* account);
	Bank& operator+(ATM* atm);

	Account* findAccountByNumber(string accountNumber);
	vector<Account*> findAccountsByOwner(string owner);
	Account* findAccountByCardNumber(string cardNumber);
	string findPasswordOfCard(string cardnumber);
	void updateAccount(string transactiontype, string accountNumber, long amount);

	vector<Account*> getAccounts() { return accounts_; }
	vector<ATM*>getATMs() { return atms_; }
};
class Account {
private:
	string accountBankName_;
	string accountNumber_; //12-digit
	string ownerName_;
	string korOwnerName_;
	long balance_;
	string pw_;
	vector<string> transactionHistoryOfAccount_;
	string cardNumber_;
public:
	Account(string bankName, string accountNumber, string ownerName, string korOwnerName, long initBalance, string pw);
	//getter, setter
	string getAccountBankName() { 
		if (!this) return "NULL";
		return accountBankName_; }
	string getAccountNumber() { 
		if (!this) return "NULL";
		return accountNumber_; }
	string getOwnerName() { 
		if (!this) return "NULL";
		return ownerName_; 
	}
	string getKorOwnerName(){
		if (!this) return "NULL";
		return korOwnerName_;
	}
	string getPassword() { 
		if (!this) return "NULL";
		return pw_; }
	long getBalance() { return balance_; }
	string getCardNumber() { 
		if (!this) return "NULL";
		return cardNumber_; }
	vector<string> getTransactionHistory() { return transactionHistoryOfAccount_; }

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
	vector<string> kortransactionHistoryOfATM_;
public:
	ATM(string atmID, bool singleBank, string bankName, Bank* ownerBank, bool unilingual,  long fiftyK, long tenK, long fiveK, long oneK, vector<Bank*>interactableBanks);
	
	//getter, setter
	std::map<std::string, long> getAvailableCash();
	string getATMID() { return atmID_; }
	string getOwnerBankName() { return primaryBankName_; }
	Bank* getCardBank(string cardNumber);
	vector<string> gettransactionHistoryOfATM() { return transactionHistoryOfATM_; }
	vector<string>getkorTransactionHistoryOfATM() { return kortransactionHistoryOfATM_; }
	Bank* findGetInteractableBank(string bankname);
	vector<Bank*> getInteractableBank() { return interactableBanks_; }
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
	void addKorTransactionHistoryOfATM(string korhistory);
	void changeToKorean() { isEnglish_ = false; }
	void changeToEnglish() { isEnglish_ = true; }
	void screenShot();
};
class Transaction {
protected:
	int transID_;
	string transactionType_;
	long amount_;
	Session* session_;
	long fee_;

public:
	static int nextID_;

	Transaction(Session* session);
	virtual void execute() = 0;
	virtual bool calculateFee() = 0;
	virtual void describeAndSave() = 0;
};
class Withdrawl :public Transaction {
private:
	map<string, long> withdrawlDenominations_;
	
public:
	Withdrawl(Session* session);
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
public:
	Deposit(Session* session);
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

public:
	Transfer(Session* session);
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
	long withdrawlAmount_;
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
	long getWithdrawlAmount() { return withdrawlAmount_; }
	void addTransactionOfSession(string history);
	void endSession(); //whenever user wants or no cash available
	void adMin();
	void IsNotActive() { isActive = false; }
	ATM* getATM() { return atm_; }
	string getCurrentSessionCardNumber() { return currCardNumber_; }
	string getCurrAccountNumber() { return currCardAccountNumber_; }
	void updateWithdrawlAmount(long amount) { withdrawlAmount_ += amount; }
	
};

//Bank Member Function Defined-----------------------------------------------
Bank::Bank(string bankName, string korBankName) {
	bankName_ = bankName;
	korBankName_ = korBankName;
}
Bank& Bank::operator+(Account* account) {
	accounts_.push_back(account);
	return *this; 
}
Bank& Bank::operator+(ATM* atm) {
	atms_.push_back(atm);
	return *this;
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
	if (transactiontype == "Withdrawal") {
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
Account::Account(string bankName, string accountNumber, string ownerName, string korOwnerName, long initBalance, string pw) {
	accountBankName_ = bankName;
	accountNumber_ = accountNumber;
	ownerName_ = ownerName;
	korOwnerName_ = korOwnerName;
	balance_ = initBalance;
	cardNumber_ = "00" + accountNumber + "00";
	pw_ = pw;
	cout << "Account Constructed (Account Bank name: " << accountBankName_ << ", Account Number: " << accountNumber_ <<", Owner: "<<ownerName_<<", Balance : "<<balance_<<")\n";
	cout << "Issued Card Number (connected to " << accountNumber_ << "account) is " << cardNumber_<<"\n";
	
}
void Account::accountBalanceUpdate(string transactionType, long amount) {
	if (transactionType == "Withdrawal") {
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

//ATM Member Function Defined------------------------------------------------
ATM::ATM(string atmID, bool singleBank, string bankName, Bank* ownerBank, bool unilingual, long fiftyK, long tenK, long fiveK, long oneK, vector<Bank*> interactableBanks) {
	atmID_ = atmID;
	singleBank_ = singleBank;
	unilingual_ = unilingual;
	isEnglish_ = true;
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
		if (bank->getBankName() == bankname || bank->getKorBankName() == bankname) return bank;
	}
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
void ATM::addKorTransactionHistoryOfATM(string korhistory) {
	kortransactionHistoryOfATM_.push_back(korhistory);
}
string ATM::getCardPasswordFromBank(string cardNumber) {
	return getCardBank(cardNumber)->findPasswordOfCard(cardNumber);
}
void ATM::screenShot() {
	cout << "Enter / for screenshot: ";
	char slash;
	cin >> slash;
	if (slash == 47) {
		cout << "##----------------------------------------------------##\n";
		for (Bank* bank : interactableBanks_) {
			for (ATM* atm : bank->getATMs()) {
				long fiftyK = atm->getAvailableCash()["50000won"];
				long tenK = atm->getAvailableCash()["10000won"];
				long fiveK = atm->getAvailableCash()["5000won"];
				long oneK = atm->getAvailableCash()["1000won"];
				long total = fiftyK * 50000 + tenK * 10000 + fiveK * 5000 + oneK * 1000;
	
				cout << "ATM[SN: " << atm->getATMID() << ", Bank: " << atm->getOwnerBankName() << ", Remaining Cash: (50000 Won Bills: " << fiftyK << ", 10000 Won Bills: " << tenK << ", 5000 Won Bills: " << fiveK << ", 1000 Won Bills: " << oneK << "]\n";
			}
		}
		cout << endl;
		for (Bank* bank : interactableBanks_) {
			for (Account* acc : bank->getAccounts()) {
				cout << "Account[Bank: " << acc->getAccountBankName() << ", No : " << acc->getAccountNumber() << ", Owner : " << acc->getOwnerName() << "] balance: " << acc->getBalance() << endl;
			}
		}
		cout << "##----------------------------------------------------##\n";
	}
	return;
}
//---------------------------------------------------------------------------

//Transaction Member Function Defined----------------------------------------
int Transaction::nextID_ = 0;
Transaction::Transaction(Session* session) {
	transID_ = nextID_++;
	//atm_ = atm;
	session_ = session;
	amount_ = 0;
	fee_ = 0;

}
//---------------------------------------------------------------------------

//Withdrawl Member Function Defined
Withdrawl::Withdrawl(Session* session) :Transaction(session) {
	transactionType_ = "Withdrawal";
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
	if (session_->getATM()->isEnglish()) cout << "<Withdrawal>\n";
	else cout << "<인출>\n";

	if (!calculateFee()) return;

	if (session_->getATM()->isEnglish()) cout << "Please Enter the desired amount of cash you want to withdraw (No coins are dispensed). AMOUNT(won): ";
	else cout << "인출하고자 하는 현금의 금액을 입력하십시오. (동전 인출 불가) 금액(원): ";

	cin >> amount_;
	if (session_->getWithdrawlAmount() + amount_ > 500000) {
		if (session_->getATM()->isEnglish()) cout << "Exceeded withdrawal limit per session(KRW 500,000).\n";
		else cout << "세션 당 인출 금액 한도 초과(500,000원).\n";
		session_->IsNotActive();
		return;
	}
	if (amount_ < 1000 || amount_ % 1000 != 0) {
		if (session_->getATM()->isEnglish()) cout << "Wrong Input. Only amount that can be dispensed by 1000, 5000, 10000, 50000won bills is allowed.\n";
		else cout << "잘못된 금액 입력. 천원권, 오천원권, 만원권, 오만원권으로 수령할 수 있는 금액만 허용됩니다.\n";
		session_->IsNotActive();
		return;
	}
	if (amount_ + fee_ > session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance()) {
		if (session_->getATM()->isEnglish()) {
			cout << "Insufficient Account Balance. Current Balance: " <<
				session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << " won.\n";
		}
		else cout << "계좌 잔액 부족. 잔액: " << session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << " 원\n";
		session_->IsNotActive();
		return ;
	}
	if (distributeDenom(amount_)["50000won"] > session_->getATM()->getAvailableCash()["50000won"]) {
		if (session_->getATM()->isEnglish()) cout << "Insufficient 50000won bills\n";
		else cout << "오만원권 부족\n";
		session_->IsNotActive();
		return ;
	}
	if (distributeDenom(amount_)["10000"] > session_->getATM()->getAvailableCash()["10000"]) {
		if (session_->getATM()->isEnglish())cout << "Insufficient 10000won bills\n";
		else cout<<"만원권 부족\n";
		session_->IsNotActive();
		return ;
	}
	if (distributeDenom(amount_)["5000won"] > session_->getATM()->getAvailableCash()["5000won"]) {
		if (session_->getATM()->isEnglish()) cout << "Insufficient 5000won bills\n";
		else cout << "오천원권 부족\n";
		session_->IsNotActive();
		return ;
	}
	if (distributeDenom(amount_)["1000won"] > session_->getATM()->getAvailableCash()["1000won"]) {
		if (session_->getATM()->isEnglish()) cout << "Insufficient 1000won bills\n";
		else cout << "천원권 부족\n";
		session_->IsNotActive();
		return ;
	}
	
	session_->updateWithdrawlAmount(amount_);
	session_->getATM()->getCardBank(session_->getCurrCardNumber())->updateAccount(transactionType_, session_->getCurrAccountNumber(), amount_+fee_);
	session_->getATM()->updateAvailableCash(-distributeDenom(amount_)["50000won"], -distributeDenom(amount_)["10000won"], -distributeDenom(amount_)["5000won"], -distributeDenom(amount_)["1000won"]);
	cout << " \n";
	if (session_->getATM()->isEnglish()) {
		cout << "Get the Cash. 50000won bill: " << distributeDenom(amount_)["50000won"] << ", 10000won bill: " << distributeDenom(amount_)["10000won"]
			<< ", 5000won bill : " << distributeDenom(amount_)["5000won"] << ", 1000won bill : " << distributeDenom(amount_)["1000won"] << endl;
	}
	else {
		cout << "현금을 수령하십시오. 오만원권: " << distributeDenom(amount_)["50000won"] << ", 만원권: " << distributeDenom(amount_)["10000won"]
			<< ", 오천원권: " << distributeDenom(amount_)["5000won"] << ", 천원권: " << distributeDenom(amount_)["1000won"] << endl;
	}
	
	describeAndSave();
	
	if (session_->getATM()->isEnglish()) {
		cout << "\nWithdrawal " << amount_ << " won from (" << session_->getCurrCardBankName() << " , NO: " << session_->getCurrAccountNumber() << ")\n";
		cout << "Balance: " << session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
	}
	else {
		cout << "(" << session_->getCurrCardBankName() << ", 계좌번호: " << session_->getCurrAccountNumber() << ")에서 "<< amount_ << "원 인출됨. \n";
		cout << "잔액: " << session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
	}
	amount_ = 0;
}	
bool Withdrawl::calculateFee() { //update fee
	if (session_->getATM()->getOwnerBankName() == session_->getCurrCardBankName()) {
		if (session_->getATM()->isEnglish()) cout << "1000won fee will be paid from your account when withdrawal is completed.\n";
		else cout << "인출이 완료되면 수수료 1000원이 고객님 계좌에서 출금됩니다.\n";
		fee_ = 1000;
	}
	else {
		if (session_->getATM()->isEnglish()) cout << "2000won fee will be paid from your account when withdrawal is completed.\n";
		else cout << "인출이 완료되면 수수료 2000원이 고객님 계좌에서 출금됩니다.\n";
		fee_ = 2000;
	}
	return true;
}
void Withdrawl::describeAndSave() {
	string history = "ATM SN: " + session_->getATM()->getATMID() + " |TransactionID: " + to_string(transID_) + " |Card Number : " + session_->getCurrCardNumber() + " |Account Number : " + session_->getCurrAccountNumber() + " |" + transactionType_ + " |Amount : " + to_string(amount_) + " |Fee: " + to_string(fee_);
	string korHistory = "ATM 고유번호 : " + session_->getATM()->getATMID() + " |거래번호 : " + to_string(transID_) + " |카드번호 : " + session_->getCurrCardNumber() + " |계좌번호 : " + session_->getCurrAccountNumber() + " |" + "인출" + " |금액 : " + to_string(amount_) + " |수수료: " + to_string(fee_);
	//atm
	if (session_->getATM()->isUnilingual()) session_->getATM()->addTransactionHistoryOfATM(history); //english ver saved only
	else {
		session_->getATM()->addTransactionHistoryOfATM(history);
		session_->getATM()->addKorTransactionHistoryOfATM(korHistory);
	}
	
	//session
	if (session_->getATM()->isEnglish()) session_->addTransactionOfSession(history);
	else session_->addTransactionOfSession(korHistory);
	//account
	session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->addTransactionHistoryOfAccount(history);
	//cout << history << endl;
	cout << " \n";
}
//---------------------------------------------------------------------------

//Deposit Member Function Defined                                                                                            
Deposit::Deposit(Session* session):Transaction(session){
	transactionType_ = "Deposit";
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
			if (session_->getATM()->isEnglish()) cout << "Total amount of cash should be less or equal to 50" << endl;
			else cout << "전체 현금은 50장 이하여야 합니다." << endl;
			return false;
		}
		if (totalsheet_ == 0) {
			if (session_->getATM()->isEnglish()) cout << "No cash inserted.\n";
			else cout << "현금이 투입되지 않았습니다.\n";
			return false;
		}
		else return true;
	}
	return false;
}
void Deposit::execute() {
	if (session_->getATM()->isEnglish()) cout << "<Deposit>\n";
	else cout << "<입금>\n";
	int pick = 0;
	while (pick != 1 && pick != 2) {
		if (session_->getATM()->isEnglish()) cout << "Enter 1 for Cash Deposit. Enter 2 for Check Deposit.: ";
		else cout << "현금 입금은 1, 수표 입금은 2를 누르시오.: ";
		cin >> pick;
		if (pick == 1) isCash_ = true;
		else if (pick == 2) isCash_ = false;
		else {
			if (session_->getATM()->isEnglish()) cout << "Wrong Input. Enter 1 or 2.\n";
			else cout << "잘못된 입력. 1과 2 중 입력하십시오.\n";
		}
	}
	if (isCash_) { //cash deposit
		long fiftyK;
		long tenK;
		long fiveK;
		long oneK;
		bool success = false;

		if (session_->getATM()->isEnglish()) cout << "Place the cash into Slot.\n";
		else cout << "현금을 투입구에 넣어주십시오.\n";

		if (session_->getATM()->isEnglish()) cout << "KRW 50000 bills: ";
		else cout << "오만원권: ";
		cin >> fiftyK;
		depositedCash["50000won"] = fiftyK;

		if (session_->getATM()->isEnglish()) cout << "KRW 10000 bills: ";
		else cout << "만원권: ";
		cin >> tenK;
		depositedCash["10000won"] = tenK;

		if (session_->getATM()->isEnglish()) cout << "KRW 5000 bills: ";
		else cout << "오천원권: ";
		cin >> fiveK;
		depositedCash["5000won"] = fiveK;

		if (session_->getATM()->isEnglish()) cout << "KRW 1000 bills: ";
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
		session_->getATM()->getCardBank(session_->getCurrCardNumber())->updateAccount(transactionType_, session_->getCurrAccountNumber(), amount_);
		if (!session_->getATM()->updateAvailableCash(fiftyK, tenK, fiveK, oneK)) {
			session_->IsNotActive();
			return;
		}
		
		if (session_->getATM()->isEnglish()) {
			cout << "\nDeposit " << amount_ << " won to (" << session_->getCurrCardBankName() << " , NO: " << session_->getCurrAccountNumber() << ")\n";
			cout << "Balance: " << session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
		else{
			cout << "(" << session_->getCurrCardBankName() << ", 계좌번호: " << session_->getCurrAccountNumber() << ")으로 " << amount_ << "원 입금됨. \n";
			cout << "잔액: " << session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
	}

	else { //check deposit
		if (session_->getATM()->isEnglish()) cout << "Place the checks into Slot(Limit: 30 sheets)\n";
		else cout << "수표를 투입구에 넣어주십시오.(한도: 30장)\n";
		long totalsheet=0;
		while (true) {
			if (session_->getATM()->isEnglish()) cout << "Amount of this check(must be greater or equal to KRW 10000) (Enter 'Stop' for Termination of Input): ";
			else cout << "수표 금액(10만원 이상의 금액) (수표 입금을 완료하였으면 '종료'를 입력하십시오.): ";
			string check;
			cin >> check;
			if (check == "Stop"||check=="종료") break;
			if (stol(check) < 10000) {
				if (session_->getATM()->isEnglish()) cout << "Wrong Check\n";
				else cout << "잘못된 수표 금액\n";
				session_->IsNotActive();
				return;
			}
			if (session_->getATM()->isEnglish()) cout << "How many sheets of this check: ";
			else cout << "해당 금액의 수표 장수: ";
			long sheet;
			cin >> sheet;
			totalsheet += sheet;
			if (totalsheet > 30) {
				if (session_->getATM()->isEnglish()) cout << "Exceeded sheet limit(30 sheets). \n";
				else cout << "수표 투입 한도 장수 초과(30장).\n";
				session_->IsNotActive();
				return;
			}
			session_->getATM()->depositChecks(check, sheet);
			amount_ += (stol(check) * sheet);
		}
		
		if (!calculateFee()) {
			session_->IsNotActive();
			return;
		}
		session_->getATM()->getCardBank(session_->getCurrCardNumber())->updateAccount(transactionType_, session_->getCurrAccountNumber(), amount_);

		if (session_->getATM()->isEnglish()) {
			cout << "\nDeposit " << amount_ << " won to (" << session_->getCurrCardBankName() << " , NO: " << session_->getCurrAccountNumber() << ")\n";
			cout << "Balance: " << session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
		else {
			cout << "(" << session_->getCurrCardBankName() << ", 계좌번호: " << session_->getCurrAccountNumber() << ")으로 " << amount_ << "원 입금됨. \n";
			cout << "잔액: " << session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
		
	}
	describeAndSave();
	amount_ = 0;
}
bool Deposit::calculateFee() {
	fee_ = 0;
	if (session_->getATM()->getOwnerBankName() == session_->getCurrCardBankName()) {
		if (session_->getATM()->isEnglish()) cout << "Pay 1000won for deposit fee into slot. SLOT: [1000won bills]: ";
		else cout << "입금 수수료 1000원을 투입구에 넣어주십시오. [천원권]: ";

		cin >> fee_;
		fee_ *= 1000;
		if (fee_ != 1000) {
			if (session_->getATM()->isEnglish()) cout << "Wrong amount is paid for fee. Cash Return: [1000won bills]: " << fee_ / 1000 << endl;
			else cout<<"잘못된 수수료 금액이 투입되었습니다. 현금 반환: [천원권]: " << fee_ / 1000 << endl;
			return false;
		}
		session_->getATM()->updateAvailableCash(0, 0, 0, 1);
		return true;
	}
	else {
		if (session_->getATM()->isEnglish()) cout << "First, Pay 2000won for deposit fee into slot.SLOT: [1000won bills]: ";
		else cout << "입금 수수료 2000원을 투입구에 넣어주십시오. [천원권]: ";
		cin >> fee_;
		fee_ *= 1000;
		if (fee_ != 2000) {
			if (session_->getATM()->isEnglish()) cout << "Wrong amount is paid for fee. Cash Return: [1000won bills]: " << fee_ / 1000 << endl;
			else cout << "잘못된 수수료 금액이 투입되었습니다. 현금 반환: [천원권]: " << fee_ / 1000 << endl;
			return false;
		}
		session_->getATM()->updateAvailableCash(0, 0, 0, 2);
		return true;
	}
}
void Deposit::describeAndSave() {
	if (isCash_) {
		string history = "ATM SN: " + session_->getATM()->getATMID() + " |TransactionID: " + to_string(transID_)+" |Card Number : " + session_->getCurrCardNumber() + " |Account Number : " + session_->getCurrAccountNumber() + " |Cash " + transactionType_ + " |Amount : " + to_string(amount_) + " |Fee: " + to_string(fee_);
		string korHistory = "ATM 고유번호 : " + session_->getATM()->getATMID() + " |거래번호 : " + to_string(transID_) + " |카드번호 : " + session_->getCurrCardNumber() + " |계좌번호 : " + session_->getCurrAccountNumber() + " |" + "현금 입금" + " |금액 : " + to_string(amount_) + " |수수료: " + to_string(fee_);
	
		if (session_->getATM()->isUnilingual()) session_->getATM()->addTransactionHistoryOfATM(history); //english ver saved only
		else {
			session_->getATM()->addTransactionHistoryOfATM(history);
			session_->getATM()->addKorTransactionHistoryOfATM(korHistory);
		}

		if (session_->getATM()->isEnglish()) {
			session_->addTransactionOfSession(history);
		}
		else {
			session_->addTransactionOfSession(korHistory);
		}
		//account
		session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->addTransactionHistoryOfAccount(history);
		//cout << history << endl;
	}
	else {
		string history = "ATM SN: " + session_->getATM()->getATMID() + " |TransactionID: " + to_string(transID_) + " |Card Number : " + session_->getCurrCardNumber() + " |Account Number : " + session_->getCurrAccountNumber() + " |Check " + transactionType_ + " |Amount : " + to_string(amount_) + " |Fee: " + to_string(fee_);
		string korHistory = "ATM 고유번호 : " + session_->getATM()->getATMID() + " |거래번호 : " + to_string(transID_) + " |카드번호 : " + session_->getCurrCardNumber() + " |계좌번호 : " + session_->getCurrAccountNumber() + " |" + "수표 입금" + " |금액 : " + to_string(amount_) + " |수수료: " + to_string(fee_);
		//atm
		if (session_->getATM()->isUnilingual()) session_->getATM()->addTransactionHistoryOfATM(history); //english ver saved only
		else {
			session_->getATM()->addTransactionHistoryOfATM(history);
			session_->getATM()->addKorTransactionHistoryOfATM(korHistory);
		}

		//session
		if (session_->getATM()->isEnglish()) session_->addTransactionOfSession(history);
		else session_->addTransactionOfSession(korHistory);
		//account
		session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->addTransactionHistoryOfAccount(history);
		//cout << history << endl;
		cout << " \n";
	}
}

//---------------------------------------------------------------------------

//Transfer Member Function Defined
Transfer::Transfer(Session* session):Transaction(session) {
	transactionType_ = "Transfer";
	fee_ = 0;
	cashToAccount_ = 0;
}
bool Transfer::validateReceiverAccounts() {
	/*
	if (!session_->getATM()->findGetInteractableBank(receiveBankName_)) {
		return false;
	}
	if (!session_->getATM()->findGetInteractableBank(receiveBankName_)->findAccountByNumber(receiveAccountNumber_)) {
		if (session_->getATM()->isEnglish()) cout << receiveAccountNumber_ << " account is not found in " << receiveBankName_ << " DB.\n";
		else cout << receiveAccountNumber_ << " 계좌는 " << receiveBankName_ << " 데이터베이스에 존재하지 않습니다.\n";
		return false;
	}*/
	try {
		// Check if the bank exists
		auto interactableBank = session_->getATM()->findGetInteractableBank(receiveBankName_);
		if (!interactableBank) {
			if (session_->getATM()->isEnglish()) throw runtime_error("Bank " + receiveBankName_ + " not found.");
			else throw runtime_error(receiveBankName_ + "는 존재하지 않습니다.");
		}

		// Check if the account exists in the bank
		auto account = interactableBank->findAccountByNumber(receiveAccountNumber_);
		if (!account) {
			if (session_->getATM()->isEnglish()) {
				throw runtime_error(receiveAccountNumber_ + " account is not found in " + receiveBankName_ + " DB.");
			}
			else {
				throw runtime_error(receiveAccountNumber_ + " 계좌는 " + receiveBankName_ + " 데이터베이스에 존재하지 않습니다.");
			}
		}

		string receiver = session_->getATM()->findGetInteractableBank(receiveBankName_)->findAccountByNumber(receiveAccountNumber_)->getOwnerName();
		string korReceiver = session_->getATM()->findGetInteractableBank(receiveBankName_)->findAccountByNumber(receiveAccountNumber_)->getKorOwnerName();
		if (session_->getATM()->isEnglish()) cout << "Is the receiver's name is " << receiver << "? Enter Y for yes, N for no. : ";
		else cout << "받는 이 성함이 " << korReceiver << "이(가) 맞습니까? 맞으면 '예', 틀리면 '아니오'를 입력하십시오. : ";
		string yn;
		cin >> yn;
		if (yn == "N") {
			if (session_->getATM()->isEnglish()) cout << "Please check account number and bank name of receiver again.\n";
			else cout << "받는 이의 계좌번호와 은행을 다시 한번 확인하십시오.\n";
			return false;
		}
		return true;

	}
	catch (const runtime_error& e) {
		cout << e.what() << endl;
		return false;
	}
}
void Transfer:: execute() {
	if (session_->getATM()->isEnglish()) cout << "<Transfer>\n";
	else cout << "<송금>\n";
	int pick = 0;
	while (pick != 1 && pick != 2) {
		if (session_->getATM()->isEnglish()) cout << "Enter 1 for Cash-to-Account Transfer. Enter 2 for Account-to-Account Transfer.: ";
		else cout << "현금 송금은 1, 계좌 이체는 2를 입력하십시오. : ";
		cin >> pick;
		if (pick == 1) cashToAccount_ = true;
		else if (pick == 2) cashToAccount_ = false;
		else {
			if (session_->getATM()->isEnglish()) cout << "Wrong Input. Enter 1 or 2.\n";
			else cout << "잘못된 입력. 1과 2 중에 입력하십시오.\n";
		}
	}
	if (session_->getATM()->isEnglish()) cout << "Enter Receiver's Bank: ";
	else cout << "받는 이 은행: ";
	cin >> receiveBankName_;

	if (session_->getATM()->isEnglish()) cout << "Enter Receiver's Account Number: ";
	else cout << "받는 이 계좌번호: ";
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
		if (session_->getATM()->isEnglish()) cout << "Place the cash to transfer into Slot\n";
		else cout << "현금 투입구에 송금 금액을 투입하십시오.\n";

		if (session_->getATM()->isEnglish())cout << "KRW 50000 bills: ";
		else cout << "오만원권: ";
		long fiftyK;
		cin >> fiftyK;

		if (session_->getATM()->isEnglish()) cout << "KRW 10000 bills: ";
		else cout << "만원권: ";
		long tenK;
		cin >> tenK;

		if (session_->getATM()->isEnglish()) cout << "KRW 5000 bills: ";
		else cout << "오천원권: ";
		long fiveK;
		cin >> fiveK;

		if (session_->getATM()->isEnglish()) cout << "KRW 1000 bills: ";
		else cout << "천원권: ";
		long oneK;
		cin >> oneK;

		amount_ = fiftyK * 50000 + tenK * 10000 + fiveK * 5000 + oneK * 1000 - fee_;
		if (amount_-fee_<=0) {
			if (session_->getATM()->isEnglish()) cout << "Insufficient input for transfer.\n";
			else cout << "송금 금액 부족.\n";
			session_->IsNotActive();
			return;
		}
		session_->getATM()->updateAvailableCash(fiftyK, tenK, fiveK, oneK);
		session_->getATM()->findGetInteractableBank(receiveBankName_)->updateAccount(transactionType_ + "Receive", receiveAccountNumber_, amount_);
		describeAndSave();
		if (session_->getATM()->isEnglish()) {
			cout << "\nTransfer " << amount_ << " won to (" << receiveBankName_ << " , NO: " << receiveAccountNumber_ << ")\n";
			//cout << "Balance: " << session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
		else {
			cout << "(" << receiveBankName_ << " , 계좌번호: " << receiveAccountNumber_ << ") 으로 " << amount_ << "원 송금됨.\n";
			//cout << "잔액: " << session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
		return;
	}
	else {
		if (session_->getATM()->isEnglish()) cout << "Enter Amount to Transfer: ";
		else cout << "계좌 이체할 금액을 입력하세요.: ";
		cin >> amount_;

		if (amount_ <= 0) {
			if (session_->getATM()->isEnglish()) cout << "Wrong amount. Amount must be bigger than 0won.\n";
			else cout << "잘못된 금액. 금액은 0보다 커야 합니다.\n";
			session_->IsNotActive();
			return;
		}
		
		if (amount_+fee_ > session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance()) {
			if (session_->getATM()->isEnglish()) cout << "Insufficient Balance.\n";
			else cout << "잔액 부족.\n";

			session_->IsNotActive();
			return;
		}
		session_->getATM()->findGetInteractableBank(receiveBankName_)->updateAccount(transactionType_ + "Receive", receiveAccountNumber_, amount_);
		session_->getATM()->findGetInteractableBank(session_->getCurrCardBankName())->updateAccount(transactionType_ + "Send", session_->getCurrAccountNumber(), amount_+fee_);
		describeAndSave();

		if (session_->getATM()->isEnglish()) {
			cout << "\nTransfer " << amount_ << " won to (" << receiveBankName_ << " , NO: " << receiveAccountNumber_ << ")\n";
			cout << "Balance: " << session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
		else {
			cout << "(" << receiveBankName_ << " , 계좌번호: " << receiveAccountNumber_ << ") 으로 " << amount_ << "원 이체됨.\n";
			cout << "잔액: " << session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->getBalance() << endl;
		}
		return;
	}
	
	
}
bool Transfer::calculateFee() {
	if (cashToAccount_) {
		if (session_->getATM()->isEnglish()) cout << "When you insert the cash to transfer into slot, you must include 1000won for fee.\n";
		else cout << "송금할 금액을 투입시, 수수료 1000원을 포함하여 투입하십시오.\n";
		fee_ = 1000;
	}
	else{
		if (session_->getATM()->getOwnerBankName()==receiveBankName_ && session_->getATM()->getOwnerBankName() == session_->getCurrCardBankName()) {
			if (session_->getATM()->isEnglish()) cout << "2000won will be paid from your account.\n";
			else cout << "고객님 계좌에서 수수료 2000원이 출금됩니다.\n";
			fee_ = 2000;
		}
		else if (session_->getATM()->getOwnerBankName() != receiveBankName_ && session_->getATM()->getOwnerBankName() != session_->getCurrCardBankName()) {
			if (session_->getATM()->isEnglish()) cout << "4000won will be paid from your account.\n";
			else cout << "고객님 계좌에서 수수료 4000원이 출금됩니다.\n";
			fee_ = 4000;
		}
		else {
			if (session_->getATM()->isEnglish()) cout << "3000won will be paid from your account.\n";
			else cout << "고객님 계좌에서 수수료 3000원이 출금됩니다.\n";
			fee_ = 3000;
		}
	}
	return true;
}
void Transfer::describeAndSave() {
	if (cashToAccount_) {
		string history = "ATM SN: " + session_->getATM()->getATMID() + " |TransactionID: " + to_string(transID_) + " |Card Number : " + session_->getCurrCardNumber() + " |Receiver Account Number: " + session_->getATM()->findGetInteractableBank(receiveBankName_)->getBankName() + " " + receiveAccountNumber_ + " |" + "Cash " + transactionType_ + " |Amount: " + to_string(amount_) + " |Fee: " + to_string(fee_);
		string korHistory = "ATM 고유번호 : " + session_->getATM()->getATMID() + " |거래번호 : " + to_string(transID_) + " |카드번호 : " + session_->getCurrCardNumber() + " |받는 이 계좌번호 : " + receiveBankName_ + " " + receiveAccountNumber_ + " | " + "현금 송금" + " | 금액 : " + to_string(amount_) + " |수수료 : " + to_string(fee_);
		if (session_->getATM()->isUnilingual()) session_->getATM()->addTransactionHistoryOfATM(history); //english ver saved only
		else {
			session_->getATM()->addTransactionHistoryOfATM(history);
			session_->getATM()->addKorTransactionHistoryOfATM(korHistory);
		}

		//Session history saving
		if (session_->getATM()->isEnglish()) session_->addTransactionOfSession(history);					
		else session_->addTransactionOfSession(korHistory);				
	

		string reAccHistory = "Received " + to_string(amount_) + " from " + session_->getCurrCardBankName() + session_->getCurrCardNumber();
		session_->getATM()->findGetInteractableBank(receiveBankName_)->findAccountByNumber(receiveAccountNumber_)->addTransactionHistoryOfAccount(reAccHistory);
	
		//cout << history << endl;
		cout << " \n";
	}
	else {
		string history = "ATM SN: " + session_->getATM()->getATMID() + " |TransactionID: " + to_string(transID_) + " |Card Number: " + session_->getCurrCardNumber() + " |Source Account Number : " + session_->getCurrAccountNumber() + " |Receiver Account Number : " + session_->getATM()->findGetInteractableBank(receiveBankName_)->getBankName() +" "+ receiveAccountNumber_ + " |" + "Account " + transactionType_ + " |Amount : " + to_string(amount_) + " |Fee: " + to_string(fee_);
		string korHistory = "ATM 고유번호 : " + session_->getATM()->getATMID() + " |거래번호 : " + to_string(transID_) + " |카드번호 : " + session_->getCurrCardNumber() + " |보내는 이 계좌번호 : " + session_->getCurrAccountNumber() + "  |받는 이 계좌번호 : " + receiveBankName_+" "+ receiveAccountNumber_ +" | " + "계좌 이체" + " |금액 : " + to_string(amount_) + " |수수료 : " + to_string(fee_);
		if (session_->getATM()->isUnilingual()) session_->getATM()->addTransactionHistoryOfATM(history); //english ver saved only
		else {
			session_->getATM()->addTransactionHistoryOfATM(history);
			session_->getATM()->addKorTransactionHistoryOfATM(korHistory);
		}

		if (session_->getATM()->isEnglish()) session_->addTransactionOfSession(history);
		else session_->addTransactionOfSession(korHistory);

		string reAccHistory = "Received " + to_string(amount_) + " from " + session_->getCurrCardBankName() + session_->getCurrCardNumber();
		session_->getATM()->findGetInteractableBank(receiveBankName_)->findAccountByNumber(receiveAccountNumber_)->addTransactionHistoryOfAccount(reAccHistory);
		string seAccHistory = "Sent" + to_string(amount_) + " to " + receiveBankName_ + receiveAccountNumber_;
		session_->getATM()->getCardBank(session_->getCurrCardNumber())->findAccountByCardNumber(session_->getCurrCardNumber())->addTransactionHistoryOfAccount(seAccHistory);
		//cout << history << endl;
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
	if (currCardNumber_ == "Admin") return;
	currCardBankName_ = atm_->getCardBank(cardNumber)->getBankName();
	if (currCardBankName_ == "NULL") {
		IsNotActive();
		return;
	}
	currCardAccountNumber_ = atm_->getCardBank(cardNumber)->findAccountByCardNumber(cardNumber)->getAccountNumber();
	wrongPasswordAttempts_ = 0;
	isAuthenticated_ = false;
	isActive = true;
	withdrawlAmount_ = 0;
	
	

	
}
void Session::execute() {
	// 1. Is card valid to this ATM type
	if (currCardBankName_ == "NULL") {
		//cout << "Card Not Found.\n";
		endSession();
		return;
	}
	if (currCardNumber_ == "Admin") {
		adMin();
		endSession();
		return;
	}
	if (!atm_->isVaildCard(currCardBankName_)) {
		if (atm_->isEnglish()) cout << "INVALID CARD. This is a Single Bank ATM. Only card issued by " << atm_->getOwnerBankName() << " is allowed.\n";
		else cout << "잘못된 카드입니다. 이 ATM은 " << atm_->getOwnerBankName() << "의 카드만 사용 가능합니다.\n";
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
	while (isActive && (cont_ == "Y"||cont_=="예")) {
		if (atm_->isEnglish()) {
			cout << "Choose the Number of Transaction\n";
			cout << "1. Withdrawal\n";
			cout << "2. Deposit\n";
			cout << "3. Transfer\n";
			cout << "4. Cancel\n";
		}
		else {
			cout << "원하시는 거래의 번호를 선택하십시오.\n";
			cout << "1. 인출\n";
			cout << "2. 입금\n";
			cout << "3. 송금\n";
			cout << "4. 취소\n";
		}
		cout << "->";

		cin >> menuPick;
		if (menuPick == 1) {
			if (withdrawlTimes >= 3) {
				if (atm_->isEnglish()) cout << "Only 3 times of Withdrawal are allowed per session.\n";
				else cout << "세션당 최대 3회의 인출만 가능합니다.\n";

				IsNotActive();
				endSession();
				return;
			}
			withdrawl = new Withdrawl(this);
			withdrawl->execute();
			delete withdrawl;
			if (!isActive) {
				endSession();
				return;
			}
			withdrawlTimes++;
			
		}
		else if (menuPick == 2) {
			deposit = new Deposit(this);
			deposit->execute();
			delete deposit;
			if (!isActive) {
				endSession();
				return;
			}
		}
		else if (menuPick == 3) {
			transfer = new Transfer(this);
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
		if (atm_->isEnglish()) cout << "Another Transaction? Press Y / N :";
		else cout << "다른 거래를 원하십니까? 예/아니오 중 입력하십시오.: ";
		cin >> cont_;
		atm_->screenShot();
	}
	endSession();
	return;
}
void Session::authenticate() {
	wrongPasswordAttempts_ = 0;
	string input_pw;
	while (wrongPasswordAttempts_ < 3) {
		if (atm_->isEnglish()) cout << "Password: ";
		else cout << "비밀번호: ";

		cin >> input_pw;
		if (input_pw == atm_->getCardBank(currCardNumber_)->findPasswordOfCard(currCardNumber_)) {
			if (atm_->isEnglish()) cout << "Correct Password\n";
			else cout << "올바른 비밀번호입니다.\n";
			isAuthenticated_ = true;
			return;
		}
		else {
			wrongPasswordAttempts_++;
			if (atm_->isEnglish()) cout << "Wrong Password. Remaining Chance: " << 3 - wrongPasswordAttempts_ << endl;
			else cout << "잘못된 비밀번호입니다. 남은 시도: " << 3 - wrongPasswordAttempts_ << endl;
		}
	}
	return;
}
void Session::addTransactionOfSession(string history) {
	transactionHistoryOfSession.push_back(history);
}
void Session::endSession() {
	if (atm_->isEnglish()) cout << "Card Return. Get your card: " << currCardNumber_ << ".\n";
	else cout<<"카드가 반환되었습니다. 카드를 수령하십시오: " << currCardNumber_ << ".\n";
	cout << "\n------------------------------\n";
	if (atm_->isEnglish()) cout << "Transactions During current session:\n";
	else cout << "현재 세션에서의 모든 거래내역:\n";
	for (string history : transactionHistoryOfSession) {

		cout << history << endl;
	}
	isActive = false;
	if (atm_->isEnglish()) cout << "Thank you for using our ATM.\n";
	else cout << "저희 ATM을 이용해주셔서 감사합니다.\n";
	cout << "-----------------------------\n";
}
void Session::adMin() {
	if (atm_->isEnglish()) {
		cout << "Menu for Admin\n";
		cout << "1. Transaction History\n";
	}
	else {
		cout << "관리자 메뉴\n";
		cout << "1. 거래 내역\n";
	}
	cout << "->";
	int menupick;
	cin >> menupick;
	static int filenumber;
	if (menupick == 1) {
		string fileName = "TransactionHistory" + to_string(filenumber) + ".txt";
		filenumber++;
		ofstream outFile(fileName);
		// Check if the file is open and writable
		if (outFile.is_open()) {
			for (Bank* bank: atm_->getInteractableBank()){
				for (ATM* atm : bank->getATMs()) {
					if (atm_->isEnglish()) {
						for (string history : atm->gettransactionHistoryOfATM()) {
							// Write to file
							cout << history << endl;
							outFile << history << "\n"; // Add a newline for formatting
						}
					}
					else {
						for (string history : atm->getkorTransactionHistoryOfATM()) {
							// Write to file
							cout << history << endl;
							outFile << history << "\n"; // Add a newline for formatting
						}
					}
				}
			}
			outFile.close();
			if (atm_->isEnglish()) cout << "Transaction history successfully saved to " << fileName << endl;
			else cout << "거래내역이 " << fileName << "에 성공적으로 저장되었습니다." << endl;
		}
		else {
			// Handle the error if the file cannot be opened
			cerr << "Error: Could not open the file for writing." << endl;
		}
	}
	return;
}
//---------------------------------------------------------------------------
void screenShot1(vector<Bank*> banks) {
	cout << "Enter / for screenshot: ";
	char slash;
	cin >> slash;
	if (slash == 47) {
		cout << "##----------------------------------------------------##\n";
		for (Bank* bank : banks) {
			for (ATM* atm : bank->getATMs()) {
				long fiftyK = atm->getAvailableCash()["50000won"];
				long tenK = atm->getAvailableCash()["10000won"];
				long fiveK = atm->getAvailableCash()["5000won"];
				long oneK = atm->getAvailableCash()["1000won"];
				long total = fiftyK * 50000 + tenK * 10000 + fiveK * 5000 + oneK * 1000;

				cout << "ATM[SN: " << atm->getATMID() << ", Bank: " << atm->getOwnerBankName() << ", Remaining Cash: (50000 Won Bills: " << fiftyK << ", 10000 Won Bills: " << tenK << ", 5000 Won Bills: " << fiveK << ", 1000 Won Bills: " << oneK << "]\n";

			}
		}
		cout << endl;
		for (Bank* bank : banks) {
			for (Account* acc : bank->getAccounts()) {
				cout << "Account[Bank: " << acc->getAccountBankName() << ", No : " << acc->getAccountNumber() << ", Owner : " << acc->getOwnerName() << "] balance: " << acc->getBalance() << endl;
			}
		}
		cout << "##----------------------------------------------------##\n";
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

	cout << "<Account Initialization>\n";
	bool account_status = true;
	vector<Account*> accounts;  // Vector to store pointers to Account objects
	while (account_status) {
		string accbankName, accountNumber, ownerName, korOwnerName, pw;
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

		cout << "Owner Name[ENGLISH]: ";
		cin >> ownerName;
		cout << "Owner Name[KOREAN]: ";
		cin >> korOwnerName;

		cout << "Initial Balance: ";
		cin >> initBalance;

		cout << "Initialize Password: ";
		cin >> pw;

		
		
		

		// Link account to the bank
		bool linked = false;
		for (Bank* bank : banks) {
			if (bank->getBankName() == accbankName) {
				Account* newAccount = new Account(accbankName, accountNumber, ownerName, korOwnerName, initBalance, pw);
				*bank + newAccount;
				accounts.push_back(newAccount);
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

	cout << "<ATM Initialization>\n";
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
		cout << "ATM SN: ";
		cin >> atmID;
		try {
			int id = stoi(atmID);
			if (atmID.length() != 6) throw invalid_argument("ATM ID must be exactly 6 digits long");
			for (ATM* atm : ATMs) {
				if (atmID == atm->getATMID()) throw runtime_error("ATM with this ID already exists");
			}
		}
		catch (const invalid_argument& e) {
			cout << "Invalid ATM ID. Please enter a 6-digit number." << endl;
			continue;
		}
		catch (const out_of_range& e) {
			// Handles cases where the number is too large for an integer
			cout << "ATM ID is out of valid range" << endl;
			continue;
		}
		catch (const runtime_error& e) {
			// Handles case where an ATM with the given ID is found
			cout << e.what() << endl;
			continue;
		}

		
		cout << "Press S for Single Bank ATM, M for Multi Bank ATM: ";
		cin >> isSingleBank;
		singleBank = (isSingleBank == "S");

		cout << "Unilingual or Bilingual (Press U for unilingual, B for Bilingual): ";
		cin >> lingual;
		if (lingual == "U") unilingual = true;
		else unilingual = false;

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
		ATM* newATM = new ATM(atmID, singleBank, atmbankName, ownerBank, unilingual, fiftyK, tenK, fiveK, oneK, interactableBanks);
		ATMs.push_back(newATM);

		for (Bank* bank : banks) {
			if (bank->getBankName() == atmbankName) {
				*bank + newATM;
			}
		}

		if (!ownerBank) {
			cout << atmbankName << " does not exist.\n";
			continue;
		}
		cout << "---------------------------------------";

		// More addition
		char continue_input;
		cout << "Add another ATM? (y/n): ";
		cin >> continue_input;
		if (continue_input != 'y' && continue_input != 'Y') {
			ATM_status = false;
		}
	}

	screenShot1(banks);

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
				cout << "\nSelect Language 언어를 선택하십시오. 한국어는 0을 누르십시오. Enter 1 for English. :";
				bool lang;
				cin >> lang;
				if (lang == false) currATM->changeToKorean();
				else currATM->changeToEnglish();
			}
			//Start ATM Operation ---------------------------------

			if (currATM->isEnglish()) cout << "Welcome! Please insert Card (Write your card number):\n";
			else cout << "어서오십시오. 카드를 투입구에 넣어주십시오. (카드 번호 입력): \n";
			string cardnumber;
			cin >> cardnumber;

			//Session Start
			Session* currSession = new Session(currATM, cardnumber);
			currSession->execute();
			delete currSession;
			screenShot1(banks);

			if (currATM->isEnglish()) cout << "Enter 'EXIT' to exit this ATM. Enter any key to continue using this ATM. : ";
			else cout << "이 ATM 사용을 중단하려면 '나가기'를 입력하십시오. 계속하려면 아무 키나 누르십시오. : ";
			string exit;
			cin >> exit;
			cout << endl;
			if (exit == "EXIT" || exit == "나가기") break;
		}
		if (currATM->isEnglish()) cout << "Enter END to exit this program. Enter any key to continue. : ";
		else cout << "전체 프로그램 종료를 위해서는 '종료'를 입력하십시오. 계속하려면 아무 키나 누르십시오. : ";
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