#include <iostream>
#include <string>;
using namespace std;

class ATM {
private:
	const int _serial_number; //6 digit
	bool _single_bank; //True for single bank, False for multi-bank
	bool _unilingual; //True for English only, False for English or Korean
	int _available_cash;
	string _transaction_history;
	Bank* bank;
	bool _status;
	Account* current_account;
public:
	ATM(int serial_number, bool single_bank, bool unilingual, int deposit_cash);
	bool start();
	bool correct_user("pw type", "pw type");
	bool perform_transaction(type, double);
	void end();
	double getBalance();
	void showMenu();
	int set_fees(string action) {
		//if deposit for non-primary banks -> 2000 additional insert
		//else if deposit for primary banks -> 1000 add insert
		//else if withdrawal for non-primary bank -> 2000 paid from withdrawal account
		//else if withdrawal for primary bank -> 1000 paid from withdrawal account
		//else if account transfer fee b/w primary banks -> 3000 paid from source account
		//else if account transfer fee b/w prim and non-prim banks ->4000 paid from source account
		//else if case transfer -> 1000 by inserting more cash
	}
	int dispense_slot();
	int insert_slot();
	//session management
	// user authentication
	// transaction handling
	// interface operation
	//admin mode for admin card inserted situation
	//customer mode for common card
};

class Bank {
private:
	string _bankname;
	//user: map of all registered users
	bool _primary_bank; //true for primary bank, false for non-primary bank
	int reserve; //total dispensable amount of money
public:
	//all account operations must go through Bank class
	//internal account storage hidden from outside world
	//add account
	//remove account
	//find accoount
	//user validation
	//account updates
	//information retrieval
};

class Account {
private:
	int _number; //12 digit
	string _bank;
	string _user;
	int _balance;
	string _history;
	int _password; //how many digits?? is abc included??

public:
	Account(int, string, string, string);
	//deposit
	//withdrawl
	//balance check
	//password validation
	//history access
};
Account::Account(int number, string bankname, string username, string history) {
}

class Transaction {
private:
	//transaction details
	//timing information
	//type, amount
	//reference detail
public:
	//information retrieval
	//detail formatting
	//transaction validation
};

class User {
private:
	//personal information
	//contact details
	//authenication data
public:
	//profile management
	//information access
	//credential validation
};

cout << "why is nothing happening" << endl;
