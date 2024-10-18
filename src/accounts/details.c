#include "../header.h"

// Function to calculate interest based on account type
double interest(double amount, char *AccountType) {
    double intrestRate;

    // Set interest rate based on account type
    if (strcmp(AccountType, "saving") == 0)
    {
        intrestRate = 0.07; // 7%
    }
    else if (strcmp(AccountType, "fixed01") == 0)
    {
        intrestRate = 0.04; // 4%
    }
    else if (strcmp(AccountType, "fixed02") == 0)
    {
        intrestRate = 0.05; // 5%
    }
    else if (strcmp(AccountType, "fixed03") == 0)
    {
        intrestRate = 0.08; // 8%
    }

    return amount * intrestRate;
}

// fetches the details of an account from the database
bool dbFetchAccountDetails(User u, int Accntid, Record *r, double *balance) {
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    {
        sqliteError(db, "Failed to open database: ", NULL);
        return false;
    }

    sqlite3_stmt *stmt = NULL;
    int userid = dbRetrieveUserId(u.name);

    if (!dbAccountExistsForUser( userid, Accntid))
    {
        printf("\n\t✖ The Account ID You Entered does not exist for this user\n\n");
        sqlite3_close(db);
        return false; // Account does not exist
    }

    *balance = accountBalance(userid, Accntid, u);

    const char *sqlQuery = "SELECT id, country, userId, accountType, accountNbr, amount,deposit_month ,deposit_day ,deposit_year, phone FROM Records WHERE accountNbr = ? AND userId = ?";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL) != SQLITE_OK)
    {
        sqliteError(db, "Failed to prepare statement: ", stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, Accntid);
    sqlite3_bind_int(stmt, 2, userid);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        r->id = sqlite3_column_int(stmt, 0);
        strcpy(r->country, (const char *)sqlite3_column_text(stmt, 1));
        r->userId = sqlite3_column_int(stmt, 2);
        strcpy(r->accountType, (const char *)sqlite3_column_text(stmt, 3));
        r->accountNbr = sqlite3_column_int(stmt, 4);
        r->amount = sqlite3_column_double(stmt, 5);
        r->deposit.month = sqlite3_column_int(stmt, 6);
        r->deposit.day = sqlite3_column_int(stmt, 7);
        r->deposit.year = sqlite3_column_int(stmt, 8);
        r->phone = sqlite3_column_int(stmt, 9);

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true; // Successfully fetched account details
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return false; // Account does not exist in database
}

// check the details of a single account
void accountDetails(User u) {
    int Accntid;
    double amount = 0.0;
    Record r;

    system("clear");
    printf("\t\t====== Welcome %s =====\n\n", u.name);
    printf("\n\t\tEnter The Account Number you want to check");
    Accntid = input_number();

    double balance;
    if (!dbFetchAccountDetails(u, Accntid, &r, &balance))
    {
        StayOrReturn(0, accountDetails, u);
        return;
    }

    printf("\n\n\t\t==================================\n\n");
    printf("\n\t\t====== [ Account details ] ========\n\n");
    printf("\n\t\tID:      %d\n", r.id);
    printf("\n\t\tNumber:  %d\n", r.accountNbr);
    printf("\n\t\tType:    %s\n", r.accountType);
    printf("\n\t\tAmount:  $%.2f\n", r.amount);
    printf("\n\t\tBalance: $%.2f\n", balance);
    printf("\n\t\tCountry: %s\n", r.country);
    printf("\n\t\tPhone:   %d\n", r.phone);

    // Interest calculations
    if (strncmp(r.accountType, "current", 7) == 0)
    {
        printf("\n\t\tYou will not get interests because the account is of type current\n\n");
        StayOrReturn(1, accountDetails, u);
    }
    else
    {
        double interestRate = interest(balance, r.accountType);
        if (strncmp(r.accountType, "fixed01", 7) == 0)
        {
            printf("\n\t\tYou will get $%.2f as interest on day %d/%d/%d.\n\n",
                   interestRate, r.deposit.month, r.deposit.day, r.deposit.year + 1);
        }
        else if (strncmp(r.accountType, "fixed02", 7) == 0)
        {
            printf("\n\t\tYou will get $%.2f as interest on day %d/%d/%d.\n\n",
                   interestRate * 2, r.deposit.month, r.deposit.day, r.deposit.year + 2);
        }
        else if (strncmp(r.accountType, "fixed03", 7) == 0)
        {
            printf("\n\t\tYou will get $%.2f as interest on day %d/%d/%d.\n\n",
                   interestRate * 3, r.deposit.month, r.deposit.day, r.deposit.year + 3);
        }
        else if (strncmp(r.accountType, "saving", 6) == 0)
        {
            printf("\n\t\tYou will get $%.2f as interest on day 10 of every month.\n\n", interestRate / 12);
        }
    }
    printf("\n\t\t==================================\n");

    StayOrReturn(1, accountDetails, u);
}
