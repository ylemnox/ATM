#include <iostream>
#include <string>
#include <cassert>
#include <vector>
using namespace std;
class Transaction {
private:
	int transactionID;
	string cardNumber;
	string sourceAccountNumber;
	string receiverAccountNumber;
	double amount;
	string transactionType;
	long timeStamp;
	double fee;
public:
	Transaction(int transaction_id, string card_num, string source_account, string receiver, double amount, string transaction_type, long timestamp, double fee_amount);

	int getTransactionID() { return transactionID; }
	string getCardNumber() { return cardNumber; }
	string getSourceAccountNumber() { return sourceAccountNumber; }
	string getReceiverAccountNumber() { return receiverAccountNumber; }
	double getAmount() { return amount; }
	string getTransactionType() { return transactionType; }
	long getTimestamp() { return timeStamp; }
	double getFee() { return fee; }
};
Transaction::Transaction(int transaction_id, string card_num, string source_account, string receiver, double amount_, string transaction_type, long timestamp, double fee_amount) {
	transactionID = transaction_id;
	cardNumber = card_num;
	sourceAccountNumber = source_account;
	receiverAccountNumber = receiver;
	amount = amount_;
	transactionType = transaction_type;
	timeStamp = timestamp;
	fee = fee_amount;
}
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

class Card {
private:
	string cardNumber;
	string cardBankName;
	string accountNumber;
	string password;
	bool AdminCard;
	int failedAttempts=0;
public:
	Card(string cardnum, string bankname, string accountnum, string pw, bool isAdmin=false);

	string getCardNumber() { return cardNumber; }
	string getCardBankName() { return cardBankName; }
	string getAccountNumber() { return accountNumber; }
	bool isAdminCard() { return AdminCard; }
	int getFailedAttempts() { return failedAttempts; }
	void resetFailedAttempts() {
		failedAttempts = 0;
	}

	bool validatePassword(string pw) {
		if (pw == password) return true;
		else {
			failedAttempts++;
			return false;
		}
	}
	
	bool isValidForATM(string atmBankName, bool isMultiBankATM) {
		if (isMultiBankATM) return true;
		else {
			if (atmBankName == cardBankName) return true;
			else return false;
		}
	}

	bool isLocked() {
		if (failedAttempts >= 3) return true;
		else return false;
	}
};
Card::Card(string cardnum, string cardbankname, string accountnum, string pw, bool isadmincard) {
	cardNumber = cardnum;
	cardBankName = cardbankname;
	accountNumber = accountnum;
	password = pw;
	AdminCard = isadmincard;
}
class Account {
private:
	string accountBankName;
	string accountNumber;
	string ownerName;
	long balance;
	vector <Transaction*> transactionHistory; //transaction account implemented on 10/29
	bool primaryBank;

	

	bool deductFee(long feeAmount) {
		if (balance >= feeAmount) {
			balance -= feeAmount;
			return true;
		}
		return false;
	}
public:
	Account(string bankname, string accountnum, string owner, long initbalance);
	string getAccountBankName() { return accountBankName; }
	string getAccountNumber() { return accountNumber; }
	string getOwnerName() { return ownerName; }
	long getBalance() { return balance; }
	vector<Transaction*> getTransactionHistory() { return transactionHistory; }
	bool isPrimaryBank() const { return primaryBank == 0 ? true : false; }
	bool deposit(long in_amount, Transaction* transaction) {
		if (in_amount <= 0) return false;
		balance += in_amount;
		addTransaction(transaction);
		return true;
		
	}
	bool transfer(Account* receiverAccount, long transAmount, Transaction* transaction) {
		if (balance < transAmount || transAmount<=0) return false;
		if (!receiverAccount) return false;

		balance -= transAmount;
		receiverAccount->deposit(transAmount, transaction);
		addTransaction(transaction);
		return true;
	}
	bool withdraw(long outAmount, Transaction* transaction) {
		if (balance < outAmount || outAmount<=0) return false;

		balance -= outAmount;
		addTransaction(transaction);
		return true;
	}

	void addTransaction(Transaction* transaction) {
		if (transaction != nullptr) {
			transactionHistory.push_back(transaction);
			//I wonder there must be some initiation for the appended transaction
		}
	}
	bool validateAccountNumber() {
		if (atoi(accountNumber.c_str()) != 0 && accountNumber.length() == 12) return true;
		else return false;
	}

};
Account::Account(string bankname, string accountnum, string owner, long initbalance) {
	accountBankName = bankname;
	accountNumber = accountnum;
	ownerName = owner;
	balance = initbalance;

}
class Bank {
private:
	string bankName;
	vector<Account*> accounts;        // All accounts in this bank
	vector<Card*> issuedCards;        // All cards issued by this bank
	vector<Transaction*> transactions; // Transaction history
	bool primaryBank;               // If this bank is primary for an ATM
	int lastTransactionID;
public:
	Bank(string bankName, bool isPrimary);
	~Bank(); // Is it necessary?
	Account* createAccount(string owner, string accountnum, long initialbalance);
	Account * findAccount(string account_number);
	vector<Account*> getAccountsByOwner(string owner);
	bool validateAccountNumber(string accountnum); //must be called before constructor called
	//for 241104
	Card* createCard(string accountnum, string cardnum, string pw, bool isAdmin = false);
	Card * findCard(string cardnum);
	bool validateCard(string cardnum, string atmBankName, bool isMultiBankATM);
	bool validateCardPassword(string cardNumber, string password);

	Transaction * createTransaction(string cardNum, string sourceAcc, string receiverAcc,double amount, string transType, double fee);
	bool processTransaction(Transaction * transaction);
	vector<Transaction*> getTransactionHistory(string accountNumber);
	vector<Transaction*> getAllTransactions() const;

	// Setter, Getter
	string getBankName() const { return bankName; }
	bool isPrimaryBank() const { return primaryBank; }
	void setPrimaryBank(bool is_primary) {
		primaryBank = is_primary;
	}

	// Balance Operations
	bool updateBalance(string accountNumber, long amount);
	long getBalance(string accountNumber);
};

Bank::Bank(string bank_name, bool isPrimary) {
	bankName = bank_name;
	primaryBank = isPrimary;
}

Account* Bank::createAccount(string owner, string accountnum, long initialbalance) {
	for (Account* acc : accounts) {
		if (acc->getAccountNumber() == accountnum) { return nullptr; }
	}

	Account* newAccount=new Account(getBankName(), accountnum, owner, initialbalance);

	if (!newAccount->validateAccountNumber()) {
		delete newAccount;
		return nullptr;
	}
	accounts.push_back(newAccount);

	return newAccount;
}

Account* Bank::findAccount(string accountnum) {
	for (Account* acc : accounts) {
		if (acc->getAccountNumber() == accountnum) return acc;
	}
	cout << accountnum << "account is not found" << endl;
	return nullptr;
}

vector<Account*> Bank::getAccountsByOwner(string owner) {
	vector<Account*> temp;
	for (Account* acc : accounts) {
		if (acc->getOwnerName() == owner) temp.push_back(acc);
	}

	if (!temp.empty()) return temp;
	else {
		cout << owner << "'s account is not found" << endl;
		return temp;
	}
}
bool Bank::validateAccountNumber(string accountnum){
	if (atoi(accountnum.c_str()) != 0 && accountnum.length() == 12) return true;
	else false;
}
Card* Bank::createCard(string accountnum, string cardnum, string pw, bool isAdmin = false) {
	if (validateAccountNumber) {
		cout << "Wrong Account Number Format" << endl;
		return nullptr;
	}
	if (!findAccount(accountnum)) {
		cout << "Existing Account" << endl;
		return nullptr;
	}
	if (!findCard(cardnum)) {
		cout << "Existing Card" << endl;
		return nullptr;
	}
	Card* createdCard = new Card(cardnum, getBankName(), accountnum, pw, isAdmin = false);

	issuedCards.push_back(createdCard);
	return createdCard;
}
/* Basic Structure Fully Test
void testCase1() {
	cout << "Test Case 1: Basic Operations with Valid Denominations" << endl;
	cout << "------------------------------------------------------" << endl;

	// Setup
	Cash cash1(10000, 5); // 5 bills of 10000 denomination

	// Test 1.1: Initial state
	cout << "1.1 Initial State Test" << endl;
	cout << "Expected: Denomination = 10000, Quantity = 5, Total = 50000" << endl;
	cout << "Actual: Denomination = " << cash1.getDenomination() << ", Quantity = " << cash1.getQuantity() << ", Total = " << cash1.totalAmount() << endl;

	// Test 1.2: Add quantity
	cash1.add(3);
	cout << "1.2 After Adding 3 Bills" << endl;
	cout << "Expected: Quantity = 8, Total = 80000" << endl;
	cout << "Actual: Quantity = " << cash1.getQuantity() << ", Total = " << cash1.totalAmount() << endl;

	// Test 1.3: Subtract quantity
	cash1.subtract(2);
	cout << "1.3 After Subtracting 2 Bills" << endl;
	cout << "Expected: Quantity = 6, Total = 60000" << endl;
	cout << "Actual: Quantity = " << cash1.getQuantity() << ", Total = " << cash1.totalAmount() << endl;

	// Test 1.4: Validate denomination
	cout << "1.4 Denomination Validation" << endl;
	cout << "Expected: True (10000 is a valid denomination)" << endl;
	cout << "Actual: " << (cash1.validate() ? "True" : "False") << endl;

	cout << endl;
}

void testCase2() {
	cout << "Test Case 2: Card Password Validation and Lockout" << endl;
	cout << "--------------------------------------------------" << endl;

	// Setup
	Card card1("1234567812345678", "Bank A", "123456789012", "pass123");

	// Test 2.1: Validate correct password
	cout << "2.1 Correct Password Validation" << endl;
	cout << "Expected: True" << endl;
	cout << "Actual: " << (card1.validatePassword("pass123") ? "True" : "False") << endl;

	// Test 2.2: Incorrect password attempt and failed attempt count
	card1.validatePassword("wrongpass");
	cout << "2.2 Incorrect Password Attempt" << endl;
	cout << "Expected: Failed Attempts = 1" << endl;
	cout << "Actual: Failed Attempts = " << card1.getFailedAttempts() << endl;

	// Test 2.3: Lockout after 3 incorrect attempts
	card1.validatePassword("wrongpass");
	card1.validatePassword("wrongpass");
	cout << "2.3 Account Lockout After 3 Attempts" << endl;
	cout << "Expected: Is Locked = True" << endl;
	cout << "Actual: Is Locked = " << (card1.isLocked() ? "True" : "False") << endl;

	// Test 2.4: Reset failed attempts
	card1.resetFailedAttempts();
	cout << "2.4 Reset Failed Attempts" << endl;
	cout << "Expected: Failed Attempts = 0" << endl;
	cout << "Actual: Failed Attempts = " << card1.getFailedAttempts() << endl;

	cout << endl;
}
void testCase3() {
	cout << "Test Case 3: Account Operations - Deposit, Transfer, Withdraw" << endl;
	cout << "------------------------------------------------------------" << endl;

	// Setup
	Account account1("Bank A", "123456789012", "Alice", 100000); // Balance: 100,000
	Account account2("Bank B", "210987654321", "Bob", 50000);   // Balance: 50,000

	Transaction depositTransaction(1, "", "123456789012", "", 20000, "deposit", 1633072800, 0.0);

	// Test 3.1: Deposit
	account1.deposit(20000, &depositTransaction);
	cout << "3.1 Deposit Operation" << endl;
	cout << "Expected: Balance = 120000" << endl;
	cout << "Actual: Balance = " << account1.getBalance() << endl;

	// Test 3.2: Transfer between accounts
	Transaction transferTransaction(2, "", "123456789012", "210987654321", 30000, "transfer", 1633072801, 10.0);
	account1.transfer(&account2, 30000, &transferTransaction);
	cout << "3.2 Transfer Operation from Account 1 to Account 2" << endl;
	cout << "Expected: Account 1 Balance = 90000, Account 2 Balance = 80000" << endl;
	cout << "Actual: Account 1 Balance = " << account1.getBalance() << ", Account 2 Balance = " << account2.getBalance() << endl;

	// Test 3.3: Withdraw
	Transaction withdrawTransaction(3, "", "123456789012", "", 10000, "withdraw", 1633072802, 5.0);
	account1.withdraw(10000, &withdrawTransaction);
	cout << "3.3 Withdraw Operation" << endl;
	cout << "Expected: Account 1 Balance = 80000" << endl;
	cout << "Actual: Account 1 Balance = " << account1.getBalance() << endl;

	cout << endl;
}
int main() {
	testCase1();
	testCase2();
	testCase3();
	return 0;
}
*/

/*Cash Test Cases
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