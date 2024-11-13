#include <iostream>
#include <string>
#include <cassert>
#include <vector>
using namespace std;

class Bank {
private:
	string _bankName;
	vector<Account*> _accounts; //account DB
	vector<Card*> _issuedCards; //card DB
	bool _primaryBank; //for fee calculation
public:
	Bank(string bankName, bool primaryBank);

	//getter, setter
	string getBankName() const { return _bankName; }
	bool isPrimaryBank() const { return _primaryBank; }

	//Account Management
	Account* findAccountByNumber(string account_number);
	vector<Account*> findAccountsByOwner(string owner);
	bool validateAccountNumber(string accountnum); //called in findAccountByNumber
};
class Account{
private:
	string _accountBankName;
	string _accountNumber;
	string _ownerName;
	long _balance;
	vector <Transaction*> transactionHistory; 
	bool _primaryBank;
public:
	Account(string bankname, string accountNumber, string ownerName, long initbalance);
	
	//getter, setter
	string getAccountBankName() { return _accountBankName; }
	string getAccountNumber() { return _accountNumber; }
	string getOwnerName() { return _ownerName; }
	long getBalance() { return _balance; }
	vector<Transaction*> getTransactionHistory() { return transactionHistory; }
	bool isPrimaryBank() const { return (_primaryBank == 1) ? true : false; }

	//Balance Management
	void accountBalanceUpdate(string transactiontype, long amount);

};
class Card{
private:
	string _cardNumber;
	string _cardBankName;
	string _accountNumber;
	string _password;
	bool _AdminCard;
	int _failedAttempts = 0;
public:
	Card(string cardNumber, string cardBankName, string accountNumber, string pw, bool isAdmin = false);
	
	//getter, setter
	string getCardNumber() { return _cardNumber; }
	string getCardBankName() { return _cardBankName; }
	string getAccountNumber() { return _accountNumber; }
	bool isAdminCard() { return _AdminCard; }
	int getFailedAttempts() { return _failedAttempts; }
	void resetFailedAttempts() { _failedAttempts = 0; }

	bool validatePassword(string pw) {
		if (pw == _password) return true;
		else {
			_failedAttempts++;
			return false;
		}
	}

	bool isValidForATM(string atmBankName, bool isMultiBankATM) {
		if (isMultiBankATM) return true;
		else {
			if (atmBankName == _cardBankName) return true;
			else return false;
		}
	}

	bool isLocked() {
		if (_failedAttempts >= 3) return true;
		else return false;
	}
};
class ATM{};

class Transaction {};
class Withdrawl{};
class Deposit{};
class Transfer{};


void createATM();
void createBank();