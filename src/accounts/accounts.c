#include "../header.h"

// Function to get the account balance for a given account ID
double accountBalance(int userId, int accnt_id, User u) {
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    {
        sqliteError(db, "\tFailed to open database: ", NULL);
        return false;
    }

    const char *sqlQuery = "SELECT Accnt_Balance FROM Records WHERE accountNbr = ?";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, 0) != SQLITE_OK) {
        sqliteError(db, "Failed to prepare statement: ", stmt);
    }

    sqlite3_bind_int(stmt, 1, accnt_id);
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqliteError(db, "Account not found: ", stmt);
    }

    double balance = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return balance;
}

// if account number exists in the database
bool dbAccountExistsInDatabase(int accountNbr) {
    sqlite3_stmt *stmt = NULL;
    bool exists = false;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db) {
        sqliteError(db, "\tFailed to open database: ", NULL);
        return false;
    }

    const char *sqlquery = "SELECT COUNT(*) FROM Records WHERE accountNbr = ?;";
    if (sqlite3_prepare_v2(db, sqlquery, -1, &stmt, NULL) != SQLITE_OK) {
        sqliteError(db, "Failed to prepare statement: ", stmt);
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, accountNbr) != SQLITE_OK) {
        sqliteError(db, "Failed to bind parameters: ", stmt);
        return false;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        // printf("Count: %d, id was: %d, account num: %d\n", count, user_id, account_number);
        if (count > 0) {
            exists = true;
        }
    } else {
        printf("An error");
        sqliteError(db, "Failed to execute statement: ", stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return exists;
}

    // if account number already exists for a given user
bool dbAccountExistsForUser(int user_id, int account_number) {
    sqlite3_stmt *stmt = NULL;
    bool exists = false;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    {
        sqliteError(db, "\tFailed to open database: ", NULL);
        return false;
    }

    const char *sqlQuery = "SELECT COUNT(*) FROM Records WHERE userId = ? AND accountNbr = ?;";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL) != SQLITE_OK)
    {
        sqliteError(db, "Failed to prepare statement: ", stmt);
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, user_id) != SQLITE_OK || sqlite3_bind_int(stmt, 2, account_number) != SQLITE_OK) {
        sqliteError(db, "Failed to bind parameters: ", stmt);
        return false;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        // printf("Count: %d, id was: %d, account num: %d\n", count, user_id, account_number);
        if (count > 0)
        {
            exists = true;
        }
    } else {
        printf("An error");
        sqliteError(db, "Failed to execute statement: ", stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return exists;
}
