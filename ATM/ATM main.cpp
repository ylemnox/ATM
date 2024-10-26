#include <iostream>
#include <string>;
using namespace std;
/*
class ATM {
private:
	const int _serialNumber; //6 digit
	bool _atmType; //True for single bank, False for multi-bank
	bool _language; //True for English only, False for English or Korean
	int _availableCash; //array 1000, 5000, 10000, 50000
	Bank* primaryBank;
	const int maxWithdrawlAmount=500000;
	const int maxWithdrawlRepeat = 3;
	bool _status;
public:
	bool validateCard() {//argument: +card
		if (_atmType) { //single bank
			//if card belongs to primary bank
				//return true;
			//else return "error message"
		}
		else { //multitbank
			//return true;
		}
	}
	void startSession() {//starts when card is inserted
		if (!_language) {
			cout << "Select Language" << endl;
			// what should I do
			if (validateCard()) {
				Session* currentSession; //heap allocation required
				int trial;
				for (trial = 0; trial < 3; trial++) {
					cout << "Write your PIN" << 3 - trial << "chances left\n";
					int inPIN;
					cin >> inPIN;
					if (inPIN != card.PIN) { //card or account?
						cout << "Wrong PIN" << endl;
					}
					else break;
				}
				if (trial == 3) {
					endSession();
				}
				else;
			}
			else endSession();
		}
	}

	void endSession() {
		delete currentSession;
		cout << "Transaction Summary" << endl;
		cout << "Take your card" << endl;
		//how should I implement card returning
		cout << "Thanks for using our ATM" << endl;
		//reset session-speicific counters
	}

	void handleDeposit(int desiredAmount) {
		if (desiredAmount < maxWithdrawlAmount) {
			cout << "Your Deposit Limit is " endl;
			//recursion?? call this function again?
		}
		else {
			//handles denomination input for cash
			_availableCash -= desiredAmount;
			//if deposit for non-primary banks -> 2000 additional insert
			//else if deposit for primary banks -> 1000 add insert
			//else if withdrawal for non-primary bank -> 2000 paid from withdrawal account
			//else if withdrawal for primary bank -> 1000 paid from withdrawal account
			//else if account transfer fee b/w primary banks -> 3000 paid from source account
			//else if account transfer fee b/w prim and non-prim banks ->4000 paid from source account
			//else if case transfer -> 1000 by inserting more cash
			//account balance - desiredAmount
		}
	}
	void handleWithdrawal();



	bool correct_user("pw type", "pw type");
	bool perform_transaction(type, double);
	void end();
	double getBalance();
	void showMenu();
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
*/
class Cash {
private:
	int denomination;
	int quantity;

public:
	Cash(int denomValue, int init_quantityValue);

	int getDenomination() { return denomination; }
	int getQuantity() { return quantity; }
	void setQuantity(int quantityValue) { quantity = quantityValue; }
	bool validate() {
		if (denomination == 1000 || denomination == 5000 || denomination == 10000 || denomination == 50000) return true;
		else return false;
	}

	int totalAmount() { return denomination * quantity; }

	bool add(int count) {
		if (count <= 0) return false;
		quantity += count;
		return true;
	}
	bool subtract(int count) {
		if (count <= 0) return false;
		if (quantity < count) return false;
		quantity -= count;
		return true;
	}
	static bool validateUniversalVer(int denomValue) {
		if (denomValue != 1000) return false;
		if (denomValue != 5000) return false;
		if (denomValue != 10000) return false;
		if (denomValue != 50000) return true;
	}
};
Cash::Cash(int denomValue, int init_qunatityValue) {
	denomination = denomValue;
	quantity = init_qunatityValue;
}
/*Cash Test Case
// Test Case 1: Basic Operations with Valid Denomination
void testCase1() {
	cout << "Test Case 1: Basic Operations with Valid Denomination" << endl;
	cout << "------------------------------------------------" << endl;

	// Setup
	Cash cash1(10000, 5);  // 5 bills of 10000 denomination

	// Test 1.1: Initial state
	cout << "1.1 Initial State Test" << endl;
	cout << "Expected: Denomination = 10000, Quantity = 5, Total = 50000" << endl;
	cout << "Actual: Denomination = " << cash1.getDenomination()
		<< ", Quantity = " << cash1.getQuantity()
		<< ", Total = " << cash1.totalAmount() << endl;

	// Test 1.2: Add operation
	bool addResult = cash1.add(3);
	cout << "\n1.2 Add Operation Test (adding 3 bills)" << endl;
	cout << "Expected: Success = true, New Quantity = 8, New Total = 80000" << endl;
	cout << "Actual: Success = " << (addResult ? "true" : "false")
		<< ", New Quantity = " << cash1.getQuantity()
		<< ", New Total = " << cash1.totalAmount() << endl;

	// Test 1.3: Subtract operation
	bool subtractResult = cash1.subtract(2);
	cout << "\n1.3 Subtract Operation Test (subtracting 2 bills)" << endl;
	cout << "Expected: Success = true, New Quantity = 6, New Total = 60000" << endl;
	cout << "Actual: Success = " << (subtractResult ? "true" : "false")
		<< ", New Quantity = " << cash1.getQuantity()
		<< ", New Total = " << cash1.totalAmount() << endl;
	cout << "------------------------------------------------\n" << endl;
}

// Test Case 2: Invalid Operations and Edge Cases
void testCase2() {
	cout << "Test Case 2: Invalid Operations and Edge Cases" << endl;
	cout << "------------------------------------------------" << endl;

	// Setup
	Cash cash2(5000, 3);  // 3 bills of 5000 denomination

	// Test 2.1: Subtract more than available
	cout << "2.1 Over-subtraction Test" << endl;
	bool overSubtractResult = cash2.subtract(5);
	cout << "Expected: Success = false, Quantity should remain = 3" << endl;
	cout << "Actual: Success = " << (overSubtractResult ? "true" : "false")
		<< ", Quantity = " << cash2.getQuantity() << endl;

	// Test 2.2: Set negative quantity
	cout << "\n2.2 Set Negative Quantity Test" << endl;
	cash2.setQuantity(-1);
	cout << "Expected: Quantity should not be negative, should remain unchanged or be 0" << endl;
	cout << "Actual: Quantity = " << cash2.getQuantity() << endl;

	// Test 2.3: Validation test
	cout << "\n2.3 Denomination Validation Test" << endl;
	cout << "Expected: 5000 = true, 7000 = false" << endl;
	cout << "Actual: 5000 = " << (Cash::validateUniversalVer(5000) ? "true" : "false")
		<< ", 7000 = " << (Cash::validateUniversalVer(7000) ? "true" : "false") << endl;
	cout << "------------------------------------------------\n" << endl;
}

// Test Case 3: Multiple Operations Sequence
void testCase3() {
	cout << "Test Case 3: Multiple Operations Sequence" << endl;
	cout << "------------------------------------------------" << endl;

	// Setup
	Cash cash3(50000, 1);  // 1 bill of 50000 denomination

	// Test 3.1: Sequential operations
	cout << "3.1 Sequential Operations Test" << endl;
	cout << "Initial state: Quantity = " << cash3.getQuantity()
		<< ", Total = " << cash3.totalAmount() << endl;

	cash3.add(2);      // Add 2 bills
	cout << "\nAfter adding 2 bills:" << endl;
	cout << "Expected: Quantity = 3, Total = 150000" << endl;
	cout << "Actual: Quantity = " << cash3.getQuantity()
		<< ", Total = " << cash3.totalAmount() << endl;

	cash3.subtract(1); // Subtract 1 bill
	cout << "\nAfter subtracting 1 bill:" << endl;
	cout << "Expected: Quantity = 2, Total = 100000" << endl;
	cout << "Actual: Quantity = " << cash3.getQuantity()
		<< ", Total = " << cash3.totalAmount() << endl;

	cash3.setQuantity(5); // Set quantity to 5
	cout << "\nAfter setting quantity to 5:" << endl;
	cout << "Expected: Quantity = 5, Total = 250000" << endl;
	cout << "Actual: Quantity = " << cash3.getQuantity()
		<< ", Total = " << cash3.totalAmount() << endl;
	cout << "------------------------------------------------" << endl;
}
int main() {
	testCase1();
	testCase2();
	testCase3();
	return 0;
}*/