#ifndef _HEADER_H
#define _HEADER_H

#include <termios.h> // password input (hide and show)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h> // defines ERANGE for floating point conversions
#include <limits.h> // defines the limits of integral types e.g LONG_MAX
#include <stdbool.h>
#include <ctype.h> // e.g is_digit()

#include "./sqlite/sqlite3.h"

struct Date {
	int month, day, year;
};

struct Record {
	int id;
	int userId;
	char name[100];
	char country[100];
	int phone;
	char accountType[100];
	int accountNbr;
	double amount;
	struct Date deposit;
	struct Date withdraw;
	double Accnt_Balance;
};

struct User {
	int uid;
	char name[50];
	char password[50];
};

typedef struct Date Date;
typedef struct Record Record;
typedef struct User User;

extern const int maxinputlen;
extern const char *dbpath;

void registerMenu(User u);
void initMenu(User *u);
void mainMenu(User u);
void loginMenu(char a[50], char pass[50]);


sqlite3 *sqliteHandler(const char *dbName);
void sqliteExecute(sqlite3 *db, const char *sql);
void sqliteError(sqlite3 *db, const char *message, sqlite3_stmt *stmt);
int sqliteInit(char *dbname);

bool dbUsernameExists(const char *username);
const char *dbRetrievePassword(User u);
char *dbRetrieveUserName(int user_id);
int dbRetrieveUserId(const char *username);
bool dbAccountExistsForUser(int user_id, int account_number);
bool dbAccountExistsInDatabase(int accountNbr);
double accountBalance(int userId, int accnt_id, User u);
bool dbAccountCreate(User u, Record r);
void accountCreate(User u);
double interest(double amount, char *AccountType);
bool dbFetchAccountDetails(User u, int Accntid, Record *r, double *balance);
void accountDetails(User u);
bool dbUpdateAccountDetails(User u, int accountId, Record AccInfo, int option);
void accountUpdateInfo(User u);
void dbUpdateAccountInformation(int accountId, const char *new_country, int new_phone, int option);
void accountList(User u);

bool isdate(Date date);
void trimlinechar(char *str);
bool ispositive(const char *str);
bool isvaliddouble(const char *str, double *dnum);
bool isalphabet(char *str);
bool isleapyear(int year);
bool isstring(const char *str, size_t n);

void Return(User u);
void success(User u);
void StayOrReturn(int notGood, void (*f)(User), User u);
void die();

void input_hide();
void input_show();
int input_number();
void input_string(User u, char *input);

#endif  // _HEADER_H
