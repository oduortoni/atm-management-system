#include "../header.h"

// Function to create a new account for a user
void accountCreate(User u) {
    Record r;
    char input[maxinputlen];

    system("clear");
    printf("\t\t\t========= New record ========\n");

    printf("\n\tEnter the account number");
    r.accountNbr = input_number();
    // check if account nbr is available even if not for the current user
    if(dbAccountExistsInDatabase(r.accountNbr)) {
        printf("\n\t\t✖ The Account Number You Entered already exists\n\n");
        StayOrReturn(0, accountCreate, u);
    }

    // fetch the user id using the client's name
    r.userId = dbRetrieveUserId(u.name);

    // check if account nbr is available, specifically, for the current user
    if (dbAccountExistsForUser(r.userId, r.accountNbr))
    {
        printf("\n\t\t✖ The Account Number You Entered already exists\n\n");
        StayOrReturn(0, accountCreate, u);
    }

    printf("\nEnter today's date (mm/dd/yyyy): ");
    if (scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year) != 3 || !isdate(r.deposit))
    {
        printf("\n\t\tInvalid date format or invalid date. Please try again.\n\n");
        StayOrReturn(0, accountCreate, u);
    }
    while (getchar() != '\n')
        ; // Clear input buffer

    printf("\nEnter the phone number");
    r.phone = input_number();

    printf("\nEnter the country");
    input_string(u, r.country);
    if (!isalphabet(r.country))
    {
        printf("\n\tInvalid input. Please enter a valid country name.\n\n");
        StayOrReturn(0, accountCreate, u);
    }

    printf("\nEnter amount to deposit $");
    if (fgets(input, sizeof(input), stdin) != NULL)
    {
        input[strcspn(input, "\n")] = '\0'; // Clear input buffer
        if (!isvaliddouble(input, &r.amount))
        {
            printf("\n\t\tInvalid input. Please enter a valid amount.\n\n");
            StayOrReturn(0, accountCreate, u);
        }
    }
    else
    {
        die();
    }

    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice");
    input_string(u, r.accountType);
    bool valid = false;
    char *type = r.accountType;
    valid = (strcmp(type, "fixed01") == 0 || strcmp(type, "fixed02") == 0 || strcmp(type, "fixed03") == 0 || strcmp(type, "current") == 0 || strcmp(type, "saving") == 0);
    if (!valid) {
        printf("\n\t\tInvalid Account Type!!!\n");
        StayOrReturn(0, accountCreate, u);
    }

    r.Accnt_Balance = r.amount;

    if (!dbAccountCreate(u, r))
    {
        printf("\n\t\tAn error occurred while trying to create a new account\n");
        StayOrReturn(0, accountCreate, u);
    }

    system("clear");
    printf("\n\n\t\t\tAccount Created Successfully!!!\n");
    success(u);
}

bool dbAccountCreate(User u, Record r) {
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    {
        printf("Failed to open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    const char *sqlQuery = "INSERT INTO Records (userId, name, country, phone, accountType, accountNbr, amount, deposit_month, deposit_day, deposit_year, Accnt_Balance) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    int ok = sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL);
    if (ok != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, r.userId);
    sqlite3_bind_text(stmt, 2, u.name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, r.country, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, r.phone);
    sqlite3_bind_text(stmt, 5, r.accountType, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, r.accountNbr);
    sqlite3_bind_double(stmt, 7, r.amount);
    sqlite3_bind_int(stmt, 8, r.deposit.month);
    sqlite3_bind_int(stmt, 9, r.deposit.day);
    sqlite3_bind_int(stmt, 10, r.deposit.year);
    sqlite3_bind_double(stmt, 11, r.Accnt_Balance);

    ok = sqlite3_step(stmt);
    if (ok != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        sqliteError(db, "error creating user", stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
