#include "../header.h"

void accountDelete(User u) {
    int userId = dbRetrieveUserId(u.name);

    int accountId;
    system("clear");
    printf("\n\t\t\tEnter the Account Number you want to delete : ");
    accountId = input_number();

    if (!dbAccountExistsForUser(userId, accountId))
    {
        printf("\n\t\t\tAccount ID %d does not Exist for this User", accountId);
        StayOrReturn(1, accountDelete, u);
        return;
    }

    // Confirm deletion
    char confirm[5];
    printf("\n\t\tConfirm deletion of account number '%d'? (y/n): ", accountId);
    if (fgets(confirm, sizeof(confirm), stdin) != NULL)
    {
        trimlinechar(confirm);
        if (!isstring(confirm, 5) || !isalphabet(confirm))
        {
            // system("clear");
            printf("\n\t\t\tInvalid input. Please enter a valid option.\n\n");
            StayOrReturn(0, accountDelete, u);
            return;
        }
    }
    else
    {
        printf("\n\t\t\tError reading input.\n\n");
        die();
        return;
    }

    if (confirm[0] == 'n' || confirm[0] == 'N')
    {
        printf("\t\t\tAccount deletion cancelled.\n");
        StayOrReturn(0, accountDelete, u);
        return;
    }

    if (confirm[0] != 'y' && confirm[0] != 'Y' && confirm[0] != 'n' && confirm[0] != 'N')
    {
        printf("\t\t\tInvalid input.\n");
        StayOrReturn(0, accountDelete, u);
        return;
    }

    bool deleted = dbAccountDelete(userId, accountId);
    if (deleted)
    {
        success(u);
    }
    else
    {
        printf("\t\t\tAn error occured while trying to delete account.\n");
        StayOrReturn(0, mainMenu, u);
        return;
    }
};

bool dbAccountDelete(int userId, int accountId) {
        sqlite3_stmt *stmt = NULL;
        sqlite3 *db = sqliteHandler(dbpath);
        if (!db)
        {
            sqliteError(db, "error opening database...", NULL);
            return false;
        }

        const char *sqlquery = "DELETE FROM Records WHERE accountNbr = ? AND userId = ?";
        int succes = sqlite3_prepare_v2(db, sqlquery, -1, &stmt, NULL);
        if (succes != SQLITE_OK)
        {
            sqliteError(db, "error preparing statement...", stmt);
            return false;
        }

        // Bind parameters
        sqlite3_bind_int(stmt, 1, accountId);
        sqlite3_bind_int(stmt, 2, userId);

        // Execute the statement
        succes = sqlite3_step(stmt);
        if (succes != SQLITE_DONE)
        {
            sqliteError(db, "error executing statement...", stmt);
            return false;
        }

        printf("\n\t\t\tSuccessfuly deleted account number '%d'.\n", accountId);

        // Clean up
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
}
