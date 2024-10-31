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
		if (transaction) {
			transactionHistory.push_back(transaction);
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

void testCashClass() {
	// Test case 1: Valid cash denomination
	Cash cash1(10000, 5);
	assert(cash1.validate() == true);
	assert(cash1.totalAmount() == 50000);

	// Test case 2: Invalid cash denomination
	Cash cash2(100, 10);
	assert(cash2.validate() == false);

	// Test case 3: Add and subtract cash
	assert(cash1.add(2) == true);
	assert(cash1.getQuantity() == 7);
	assert(cash1.subtract(3) == true);
	assert(cash1.getQuantity() == 4);
	assert(cash1.subtract(5) == false);
}

// Test Case 2: Validate Card Class
void testCardClass() {
	// Test case 1: Validate password
	Card card1("123456789012", "Kakao", "111222333444", "password123");
	assert(card1.validatePassword("password123") == true);
	assert(card1.validatePassword("wrongpassword") == false);
	assert(card1.getFailedAttempts() == 1);

	// Test case 2: Validate for ATM
	assert(card1.isValidForATM("Kakao", false) == true);
	assert(card1.isValidForATM("Shinhan", false) == false);
	assert(card1.isValidForATM("Kakao", true) == true);

	// Test case 3: Lock card
	for (int i = 0; i < 2; i++) {
		card1.validatePassword("wrongpassword");
	}
	cout << card1.isLocked() << endl;
	//assert(card1.isLocked() == false);
	card1.validatePassword("wrongpassword");
	assert(card1.isLocked() == true);
	card1.resetFailedAttempts();
	assert(card1.isLocked() == false);
}

// Test Case 3: Validate Account Class
void testAccountClass() {
	// Test case 1: Validate account number
	Account account1("Kakao", "111222333444", "John Doe", 10000);
	assert(account1.validateAccountNumber() == true);

	// Test case 2: Deposit and withdraw
	assert(account1.deposit(5000, nullptr) == true);
	assert(account1.getBalance() == 15000);
	assert(account1.withdraw(3000, nullptr) == true);
	assert(account1.getBalance() == 12000);
	assert(account1.withdraw(15000, nullptr) == false);

	// Test case 3: Transfer
	Account account2("Shinhan", "555666777888", "Jane Smith", 20000);
	assert(account1.transfer(&account2, 5000, nullptr) == true);
	assert(account1.getBalance() == 7000);
	assert(account2.getBalance() == 25000);
}

int main() {
	testCashClass();
	testCardClass();
	testAccountClass();
	return 0;
}

/*Card Test Cases
void testCase1_BasicCardValidation() {
	cout << "\n=== Test Case 1: Basic Card Validation ===\n";

	// Create a regular card
	Card card1("1234567890", "Kakao", "111111111111", "1234");

	// Test basic getters
	cout << "1.1 Testing Card Number:\n";
	cout << "Expected: 1234567890\n";
	cout << "Actual  : " << card1.getCardNumber() << "\n";
	assert(card1.getCardNumber() == "1234567890");

	cout << "\n1.2 Testing Bank Name:\n";
	cout << "Expected: Kakao\n";
	cout << "Actual  : " << card1.getCardBankName() << "\n";
	assert(card1.getCardBankName() == "Kakao");

	cout << "\n1.3 Testing Account Number:\n";
	cout << "Expected: 111111111111\n";
	cout << "Actual  : " << card1.getAccountNumber() << "\n";
	assert(card1.getAccountNumber() == "111111111111");

	cout << "\n1.4 Testing Admin Status:\n";
	cout << "Expected: false\n";
	cout << "Actual  : " << (card1.isAdminCard() ? "true" : "false") << "\n";
	assert(card1.isAdminCard() == false);

	cout << "\n1.5 Testing Password Validation:\n";
	cout << "Expected: Correct password (1234) -> true\n";
	cout << "Actual  : " << (card1.validatePassword("1234") ? "true" : "false") << "\n";
	cout << "Expected: Wrong password (wrong) -> false\n";
	cout << "Actual  : " << (card1.validatePassword("wrong") ? "true" : "false") << "\n";

	cout << "\nBasic card validation tests passed!\n";
}

void testCase2_FailedAttemptsLocking() {
	cout << "\n=== Test Case 2: Failed Attempts and Card Locking ===\n";

	Card card2("9876543210", "Shinhan", "222222222222", "5678");

	cout << "2.1 Testing Initial Failed Attempts:\n";
	cout << "Expected: 0\n";
	cout << "Actual  : " << card2.getFailedAttempts() << "\n";
	assert(card2.getFailedAttempts() == 0);

	cout << "\n2.2 Testing Initial Lock Status:\n";
	cout << "Expected: false\n";
	cout << "Actual  : " << (card2.isLocked() ? "true" : "false") << "\n";
	assert(card2.isLocked() == false);

	// Simulate three wrong password attempts
	cout << "\n2.3 Testing Wrong Password Attempts:\n";
	cout << "Attempt 1 - Expected: false\n";
	cout << "Actual  : " << (card2.validatePassword("wrong1") ? "true" : "false") << "\n";
	cout << "Attempt 2 - Expected: false\n";
	cout << "Actual  : " << (card2.validatePassword("wrong2") ? "true" : "false") << "\n";
	cout << "Attempt 3 - Expected: false\n";
	cout << "Actual  : " << (card2.validatePassword("wrong3") ? "true" : "false") << "\n";

	cout << "\n2.4 Testing Failed Attempts Count:\n";
	cout << "Expected: 3\n";
	cout << "Actual  : " << card2.getFailedAttempts() << "\n";
	assert(card2.getFailedAttempts() == 3);

	cout << "\n2.5 Testing Lock Status After Failed Attempts:\n";
	cout << "Expected: true\n";
	cout << "Actual  : " << (card2.isLocked() ? "true" : "false") << "\n";
	assert(card2.isLocked() == true);

	cout << "\n2.6 Testing Reset Functionality:\n";
	card2.resetFailedAttempts();
	cout << "Expected: 0\n";
	cout << "Actual  : " << card2.getFailedAttempts() << "\n";

	cout << "\nFailed attempts and locking tests passed!\n";
}

void testCase3_ATMCompatibility() {
	cout << "\n=== Test Case 3: ATM Compatibility ===\n";

	Card kakaoBankCard("1111222233", "Kakao", "333333333333", "9999");
	Card adminCard("9999999999", "Admin", "000000000000", "0000", true);

	cout << "3.1 Testing Single-Bank ATM (Same Bank):\n";
	cout << "Expected: true\n";
	cout << "Actual  : " << (kakaoBankCard.isValidForATM("Kakao", false) ? "true" : "false") << "\n";
	assert(kakaoBankCard.isValidForATM("Kakao", false) == true);

	cout << "\n3.2 Testing Single-Bank ATM (Different Bank):\n";
	cout << "Expected: false\n";
	cout << "Actual  : " << (kakaoBankCard.isValidForATM("Shinhan", false) ? "true" : "false") << "\n";
	assert(kakaoBankCard.isValidForATM("Shinhan", false) == false);

	cout << "\n3.3 Testing Multi-Bank ATM:\n";
	cout << "Expected: true\n";
	cout << "Actual  : " << (kakaoBankCard.isValidForATM("Shinhan", true) ? "true" : "false") << "\n";
	assert(kakaoBankCard.isValidForATM("Shinhan", true) == true);

	cout << "\n3.4 Testing Admin Card:\n";
	cout << "Expected: true (should work in any ATM)\n";
	cout << "Actual  : " << (adminCard.isValidForATM("Kakao", false) ? "true" : "false") << "\n";
	assert(adminCard.isValidForATM("Kakao", true) == true);

	cout << "\nATM compatibility tests passed!\n";
}

int main() {
	cout << "Starting Card Class Tests...\n";

	testCase1_BasicCardValidation();
	testCase2_FailedAttemptsLocking();
	testCase3_ATMCompatibility();

	cout << "\nAll card tests completed successfully!\n";
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