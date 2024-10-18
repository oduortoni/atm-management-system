#include "../header.h"

// Function to update the account balance
void dbUpdateAccntBalance(int user_id, int accnt_id, double balance, int option) {
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db) {
        sqliteError(db, "Failed to open database: ", NULL);
    }

    if(option == 1) { // deposit
        const char *sqlQuery = "UPDATE Records SET deposit_day = ?, deposit_month = ?, deposit_year = ?, Accnt_Balance = ? WHERE accountNbr = ?";
        if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, 0) != SQLITE_OK) {
            sqliteError(db, "Failed to prepare deposit statement: ", stmt);
        }
    } else { // withdraw
        const char *sqlQuery= "UPDATE Records SET withdraw_day = ?, withdraw_month = ?, withdraw_year = ?, Accnt_Balance = ? WHERE accountNbr = ?";
        if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, 0) != SQLITE_OK) {
            sqliteError(db, "Failed to prepare withdraw statement: ", stmt);
        }
    }
   
    // Get the current date
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    // Get current day, month, and year
    int year = tm_info->tm_year + 1900; // Year since 1900
    int month = tm_info->tm_mon + 1;    // Months are zero-based (0-11)
    int day = tm_info->tm_mday;         // Current day

    // Bind the parameters
    sqlite3_bind_int(stmt, 1, day);
    sqlite3_bind_int(stmt, 2, month);
    sqlite3_bind_int(stmt, 3, year);
    sqlite3_bind_double(stmt, 4, balance);
    sqlite3_bind_int(stmt, 5, accnt_id);

    // Execute the statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqliteError(db, "Failed to update the database: ", stmt);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Function to handle making a transaction (deposit/withdraw)
void accountMakeTransaction(User u) {
    int accnt_id;
    int option = 0;
    double amount;
    char transaction_type[10];

    int user_id = dbRetrieveUserId(u.name);

    system("clear");
    printf("\n\t\t\tEnter your Account Number: ");
    accnt_id = input_number();

    // Confirm the account ownership
    if (!dbAccountExistsForUser(user_id, accnt_id)) {
        printf("\n\t\t\t✖ Account ID %d does not exist for this user %s\n", accnt_id, u.name);
        StayOrReturn(0, accountMakeTransaction, u);
        return;
    }

    // Check if the account type allows transactions
    if(!accountAllowsTransactions(user_id, accnt_id, u)) {
        Return(u); // Return to the menu
        return;
    }
retry:
    printf("\n\t\t\t\tMake Transaction");
    printf("\n\t\t=========================================================\n");
    printf("\n\t\t\t\tDo you Wish to");
    printf("\n\t\t[1]- Deposit\n");
    printf("\n\t\t[2]- Withdraw\n");
    printf("\n\t\t[3]- go to main menu\n");
    option = input_number();

    char input[maxinputlen];
    double balance = accountBalance(user_id, accnt_id, u);

    switch (option) {
    case 1:
        strcpy(transaction_type, "Deposit"); // Set transaction type to deposit
        printf("\n\t\tHow much do you wish to deposit: ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Remove newline character if present
            input[strcspn(input, "\n")] = '\0'; // Clear input buffer

            // Validate the input
            if (!isvaliddouble(input, &amount)) {
                system("clear");
                printf("\n\t\tInvalid input. Please enter a valid amount.\n\n");
                Return(u);
            }
        } else {
            die();
        }
        balance += amount;
        dbUpdateAccntBalance(user_id, accnt_id, balance, option);
        break;
    case 2:
        strcpy(transaction_type, "Withdraw");
        printf("\n\t\tHow much do you wish to withdraw: ");
        if (fgets(input, sizeof(input), stdin) != NULL)
        {
            input[strcspn(input, "\n")] = '\0'; // Clear input buffer
            // Validate the input
            if (!isvaliddouble(input, &amount)) {
                system("clear");
                printf("\n\t\tInvalid input. Please enter a valid amount.\n\n");
                Return(u);
            }
        }
        else
        {
            die();
        }
        // Check if there are sufficient funds
        if (balance < amount) {
            printf("\n\t\t\tInsufficient balance!!!!\n");
            StayOrReturn(0, accountMakeTransaction, u);
            return;
        }
        balance -= amount;
        dbUpdateAccntBalance(user_id, accnt_id, balance, option);
        break;
    case 3:
        mainMenu(u);
        return;
    default:
        system("clear");
        printf("\n\t\tInvalid Input");
        goto retry;
    }

    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    { // TODO**************************************************************************************************
        sqliteError(db, "Failed to open database: ", NULL);
    }
    dbRecordTransaction(accnt_id, transaction_type, amount);
    sqlite3_close(db);
    printf("\n\t\t\tTransaction successful\n");
    success(u);
}

// saves a transaction to the database
void dbRecordTransaction(int accnt_id, char *transaction_type, double amount) {
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    {
        sqliteError(db, "Failed to open database: ", NULL);
    }
    const char *sqlQuery = "INSERT INTO transactions (account_id, transaction_type, amount) VALUES (?, ?, ?)";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, 0) != SQLITE_OK)
    {
        sqliteError(db, "Failed to log transaction: ", stmt);
    }

    sqlite3_bind_int(stmt, 1, accnt_id);
    sqlite3_bind_text(stmt, 2, transaction_type, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, amount);

    // Get the current timestamp
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info); // Format: YYYY-MM-DD HH:MM:SS

    // Bind the timestamp
    sqlite3_bind_text(stmt, 4, timestamp, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqliteError(db, "Failed to log transaction: ", stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// does the account type allow for transactions
bool accountAllowsTransactions(int userId, int accnt_id, User u) {
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    {
        sqliteError(db, "Failed to open database: ", NULL);
    }

    const char *sqlQuery = "SELECT accountType FROM Records WHERE accountNbr = ?";   
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, 0) != SQLITE_OK)
    {
        sqliteError(db, "Failed to fetch account: ", stmt);
    }

    sqlite3_bind_int(stmt, 1, accnt_id);

    if (sqlite3_step(stmt) != SQLITE_ROW)
    {
        sqliteError(db, "Account not found: ", stmt);
    }
    const char *account_type = (const char *)sqlite3_column_text(stmt, 0); // fetch the account type

    if (strcmp(account_type, "saving") != 0)
    {
        // account is not a savings account => doesnt allow transcations
        printf("\n\t\tTransactions are not allowed for account type %s\n", account_type);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
