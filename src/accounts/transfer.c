#include "../header.h"

// Function to handle the transfer of account ownership
void accountTransfer(User u) {
    int accnt_id;
    int recepientID;
    system("clear");

    printf("\n\t\t\t\t\tAccount Transfer\n\n");
    printf("\t\t\t===========================================\n\n");
    printf("\n\t\tEnter the Account Number you wish to Transfer: ");
    accnt_id = input_number();

    // Get the user ID from the database
    int user_id = dbRetrieveUserId(u.name);
    if (!dbAccountExistsForUser(user_id, accnt_id)) // Check if the account belongs to the user
    {
        printf("\n\t\t\tAccount ID %d does not belong to user %s\n", accnt_id, u.name);
        StayOrReturn(0, accountTransfer, u);
        return;
    }

    printf("\n\t\t\tEnter the recipient's ID: ");
    recepientID = input_number();

    char *recipientName = dbRetrieveUserName(recepientID); // Fetch the recipient's name from the database
    if (recipientName == NULL || strlen(recipientName) == 0)
    { // Check if the recipient exists
        printf("\n\t\t\tUser ID %d does not exist.\n\n", recepientID);
        StayOrReturn(0, accountTransfer, u);
        return;
    }

    dbaccountTransfer(recepientID, accnt_id, recipientName);
    success(u);
}

// transfer account in the database
void dbaccountTransfer(int user_id, int accnt_id, char *name) {
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    {
        sqliteError(db, "Failed to open database: ", NULL);
    }

    const char *sqlQuery = "UPDATE Records SET userId = ?, name = ? WHERE accountNbr = ?";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, 0) != SQLITE_OK)
    {
        sqliteError(db, "Failed to prepare statement: ", stmt);
    }

    // Bind parameters
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, accnt_id);

    // Execute the statement
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqliteError(db, "Failed to update account: ", stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
