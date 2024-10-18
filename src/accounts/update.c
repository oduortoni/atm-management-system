#include "../header.h"

bool dbUpdateAccountDetails(User u, int accountId, Record AccInfo, int option) {
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db) {
        sqliteError(db, "Failed to open database: ", NULL);
        return false;
    }

    int userid = dbRetrieveUserId(u.name);
    if (!dbAccountExistsForUser( userid, accountId)) {
        printf("\n\t✖ The Account Number You Entered does not exist\n\n");
        sqlite3_close(db);
        return false;
    }

    switch (option)
    {
    case 1:
        dbUpdateAccountInformation(accountId, NULL, AccInfo.phone, option);
        break;
    case 2:
        dbUpdateAccountInformation(accountId, AccInfo.country, -1, option);
        break;
    default:
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);

    return true;
}

void accountUpdateInfo(User u) {
    Record r;
    int option;

    system("clear");
Reselect:
    printf("\n\t\tEnter Account Number");
    r.accountNbr = input_number();

    // fetch the user id using the client's name
    r.userId =dbRetrieveUserId(u.name);
    bool exists = dbAccountExistsForUser(r.userId, r.accountNbr);
    if (!exists)
    {
        printf("\n\t\t✖ The Account Number You Entered does not exist\n\n");
        Return(u);
        return;
    }

    printf("\n\t\tWhich Field You would like to Update\n\n\t\t\t[1] - Phone\n\n\t\t\t[2] - Country\n");
    option = input_number();

    switch (option)
    {
    case 1:
        printf("\t\tEnter your new phone number");
        r.phone = input_number();
        break;
    case 2:
        printf("\t\tEnter your new country");
        input_string(u, r.country);
        if (!isalphabet(r.country))
        {
            printf("\n\t\tInvalid input. Please enter a valid country name\n\n");
            Return(u);
            return;
        }
        break;
    default:
        system("clear");
        printf("\n\n\t\tInvalid Input !!!!");
        goto Reselect;
    }

    // Call the function to process the database transaction
    bool updated = dbUpdateAccountDetails(u, r.accountNbr, r, option);
    if (!updated)
    {
        // error
        printf("\n\n\t\tAn error occured while trying to update account information\n");
        Return(u);
        return;
    }
    success(u);
}

// Function to update the user's phone number or country in the database
void dbUpdateAccountInformation(int accountId, const char *new_country, int new_phone, int option) {
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    {
        sqliteError(db, "Failed to open database: ", NULL);
    }

    // Prepare the SQL statement
    if(option == 1) {
        const char *sqlQuery = "UPDATE Records SET phone = ? WHERE accountNbr = ?;";
        if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL) != SQLITE_OK) {
            sqliteError(db, "Failed to prepare statement: ", stmt);
        }
    } else {
        const char *sqlQuery = "UPDATE Records SET country = ? WHERE accountNbr = ?;"; 
        if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL) != SQLITE_OK) {
            sqliteError(db, "Failed to prepare statement: ", stmt);
        }
    }

    // Bind the new values and user ID to the SQL statement
    if (new_country != NULL && strlen(new_country) > 0)
    {
        sqlite3_bind_text(stmt, 1, new_country, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, accountId);
    }
    else
    {
        sqlite3_bind_int(stmt, 1, new_phone);
        sqlite3_bind_int(stmt, 2, accountId);
    }

    // Execute the statement
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqliteError(db, "Execution Failed: ", stmt);
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
}
