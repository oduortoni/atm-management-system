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

void loginMenu(char a[50], char pass[50]);
void registerMenu(User u);
void mainMenu(User u);
void initMenu(User *u);

sqlite3 *sqliteHandler(const char *dbName);
void sqliteExecute(sqlite3 *db, const char *sql);
void sqliteError(sqlite3 *db, const char *message, sqlite3_stmt *stmt);
int sqliteInit(char *dbname);

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
