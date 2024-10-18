#include "../header.h"

// function to open the database and return the handler
int sqliteInit(char *dbname) {
    sqlite3 *db = NULL;
    int succes;

    succes = sqlite3_open(dbname, &db);
    if (succes != SQLITE_OK) {
        return 0;
    }

    // create each table
    const char *sqlUsersTable = "CREATE TABLE IF NOT EXISTS USERS (user_id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT NOT NULL UNIQUE, password TEXT NOT NULL);";
    sqliteExecute(db, sqlUsersTable);
    const char *sqlRecordsTable = "CREATE TABLE IF NOT EXISTS Records (id INTEGER PRIMARY KEY AUTOINCREMENT, userId INTEGER, name TEXT NOT NULL, country TEXT NOT NULL, phone INTEGER, accountType TEXT NOT NULL, accountNbr INTEGER, amount REAL, deposit_month INTEGER, deposit_day INTEGER, deposit_year INTEGER, withdraw_month INTEGER, withdraw_day INTEGER, withdraw_year INTEGER, Accnt_Balance, FOREIGN KEY (userId) REFERENCES USERS (user_id));";
    sqliteExecute(db, sqlRecordsTable);
    const char *sqlTsTable = "CREATE TABLE IF NOT EXISTS transactions (transaction_id INTEGER PRIMARY KEY AUTOINCREMENT, account_id INTEGER, transaction_type TEXT, amount REAL, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, FOREIGN KEY (account_id) REFERENCES Records(accountNbr));";
    sqliteExecute(db, sqlTsTable);

    return 1;
}

// function to open the database
sqlite3 *sqliteHandler(const char *dbName) {
    sqlite3 *db = NULL;
    int succes;

    // open database
    succes = sqlite3_open(dbName, &db);

    // Check for errors
    if (succes != SQLITE_OK)
    {
        sqliteError(db, "Failed to open Database", NULL);
    }

    // return an open databse
    return db;
}

// execute non effectingrequests
void sqliteExecute(sqlite3 *db, const char *sql)
{
    sqlite3_stmt *stmt = NULL;
    int succes;

    succes = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (succes != SQLITE_OK)
    {
        sqliteError(db, "Failed to Prepare statement", stmt);
    }

    succes = sqlite3_step(stmt); // exec prepared sttmt
    if (succes != SQLITE_DONE)
    {
        sqliteError(db, "Execution Failed", stmt);
    }

    // finalize a statement
    sqlite3_finalize(stmt);

    return;
}

// centralized error handling
void sqliteError(sqlite3 *db, const char *message, sqlite3_stmt *stmt)
{
    fprintf(stderr, "%s: %s\n", message, sqlite3_errmsg(db));
    if (db)
    {
        sqlite3_close(db);
    }
    if (stmt != NULL)
    {
        sqlite3_finalize(stmt);
    }
    die();
}
