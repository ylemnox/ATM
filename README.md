# ATM Simulation System

A comprehensive C++ console application that simulates an ATM (Automated Teller Machine) system with support for multiple banks, accounts, and bilingual operations.

## Features

### Core Functionality
- **Multi-Bank Support**: Manage multiple banks within a single system
- **Account Management**: Create and manage multiple bank accounts with unique account numbers
- **ATM Operations**: Support for both single-bank and multi-bank ATM configurations
- **Bilingual Interface**: Switch between English and Korean languages
- **Transaction Types**:
  - **Withdrawals**: Dispense cash with automatic denomination distribution
  - **Deposits**: Accept both cash and check deposits
  - **Transfers**: Support for account-to-account and cash-to-account transfers

### Advanced Features
- **Session Management**: Secure user sessions with password authentication
- **Transaction History**: Track all transactions for accounts, ATMs, and sessions
- **Fee Calculation**: Automatic fee calculation based on transaction type and bank affiliation
- **Cash Management**: Track available denominations (₩50,000, ₩10,000, ₩5,000, ₩1,000)
- **Input Validation**: Comprehensive validation for account numbers, ATM IDs, and transaction amounts
- **Admin Functions**: Administrative capabilities for system monitoring

## System Architecture

### Classes

#### Bank
Manages bank information and maintains databases of accounts and ATMs.
- Store and retrieve account information
- Find accounts by number, owner name, or card number
- Manage associated ATMs
- Update account balances for transactions

#### Account
Represents individual bank accounts with:
- 12-digit account number validation
- Owner information (English and Korean names)
- Password protection
- Balance management
- Transaction history tracking
- Card number association

#### ATM
Represents ATM machines with configurable features:
- 6-digit unique ATM serial number
- Single-bank or multi-bank operation modes
- Unilingual (English-only) or bilingual (English/Korean) support
- Cash inventory management (multiple denominations)
- Check deposit capabilities
- Transaction logging

#### Transaction (Abstract Base Class)
Base class for all transaction types with:
- Unique transaction IDs
- Fee calculation
- Transaction execution
- History recording

#### Withdrawal
Handles cash withdrawal operations:
- Automatic denomination distribution
- Available cash verification
- Fee calculation based on bank affiliation

#### Deposit
Manages deposit operations for:
- Cash deposits (with denomination counting)
- Check deposits (with limits)
- Validation of deposit amounts

#### Transfer
Facilitates fund transfers:
- Account-to-account transfers
- Cash-to-account transfers
- Cross-bank transfers with fee calculation

#### Session
Manages user interaction sessions:
- Card insertion and authentication
- Password verification (with attempt limits)
- Transaction execution
- Session history tracking
- Secure session termination

## Usage

### Compilation

```bash
g++ -std=c++11 ATM.cpp -o ATM.exe
```

### Running the Application

```bash
./ATM.exe
```

### Initial Setup

The application guides you through a three-step initialization process:

#### 1. Bank Initialization
```
Bank Name[ENGLISH](without space): BankA
Bank Name[KOREAN](without space): 은행A
Add another bank? (y/n): y
```

#### 2. Account Initialization
```
Bank Name: BankA
Account Number: 123456789012
Owner Name[ENGLISH]: JohnDoe
Owner Name[KOREAN]: 홍길동
Initial Balance: 1000000
Initialize Password: 1234
Add another account? (y/n): y
```

#### 3. ATM Initialization
```
Bank Name: BankA
ATM SN: 123456
Press S for Single Bank ATM, M for Multi Bank ATM: M
Unilingual or Bilingual (Press U for unilingual, B for Bilingual): B
Number of 50000won bill: 10
Number of 10000won bill: 50
Number of 5000won bill: 20
Number of 1000won bill: 100
Add another ATM? (y/n): n
```

### Using the ATM

1. **Select ATM**: Enter the 6-digit serial number
2. **Choose Language**: Select English (1) or Korean (0) if bilingual
3. **Insert Card**: Enter your card number
4. **Authenticate**: Enter your password
5. **Select Transaction**: Choose from available operations
6. **Complete Transaction**: Follow on-screen prompts

## Transaction Details

### Withdrawal
- Minimum: ₩1,000
- Amounts must be multiples of ₩1,000
- Automatic denomination distribution based on availability
- Fees apply for non-primary bank cards

### Deposit
- **Cash**: Limited to 50 bills per transaction
- **Check**: Limited to 30 checks per transaction
- Deposited cash is immediately added to ATM inventory

### Transfer
- **Account-to-Account**: Transfer between any accounts
- **Cash-to-Account**: Deposit cash directly to any account
- Cross-bank transfers may incur additional fees

## Fee Structure

Fees are automatically calculated based on:
- Transaction type
- Bank affiliation (primary bank vs. other banks)
- ATM type (single-bank vs. multi-bank)

## Technical Specifications

- **Language**: C++11
- **Account Number Format**: 12 digits
- **ATM Serial Number Format**: 6 digits
- **Card Number Format**: 16 digits
- **Supported Denominations**: ₩50,000, ₩10,000, ₩5,000, ₩1,000

## Data Structures

- Uses STL containers (vector, map) for efficient data management
- Pointer-based object relationships for flexibility
- Dynamic memory management with proper cleanup

## Security Features

- Password authentication with attempt limits
- Card validation against bank databases
- Null pointer checks for safe object access
- Transaction authorization checks

## Future Enhancements

Potential areas for expansion:
- Persistent data storage (file I/O or database)
- GUI implementation
- Network connectivity for real-time bank communication
- Additional transaction types (bill payments, etc.)
- Enhanced security features (PIN encryption, biometrics)
- Transaction receipt printing

## Requirements

- C++11 compatible compiler (GCC, MSVC, Clang)
- Standard C++ library

## Contributing

Contributions, issues, and feature requests are welcome!
