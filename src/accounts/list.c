#include "../header.h"

void accountList(User u) {
    Record r;
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    {
        sqliteError(db, "Failed to open database: ", stmt);
    }

    int userid = dbRetrieveUserId(u.name);

    const char *sqlQuery = "SELECT id, country, phone, accountType, accountNbr, amount, deposit_month,deposit_day,deposit_year FROM Records WHERE userId = ?";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL) != SQLITE_OK)
    {
        sqliteError(db, "Failed to prepare statement: ", stmt);
    }

    sqlite3_bind_int(stmt, 1, userid);
    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // Retrieve account information from the database
        r.id = sqlite3_column_int(stmt, 0);
        strcpy(r.country, sqlite3_column_text(stmt, 1));
        r.phone = sqlite3_column_int(stmt, 2);
        strcpy(r.accountType, (const char *)sqlite3_column_text(stmt, 3));
        r.accountNbr = sqlite3_column_int(stmt, 4);
        r.amount = sqlite3_column_double(stmt, 5);
        r.deposit.month = sqlite3_column_int(stmt, 6);
        r.deposit.day = sqlite3_column_int(stmt, 7);
        r.deposit.year = sqlite3_column_int(stmt, 8);

        // Display account information
        printf("\n\t\t\t_____________________\n");
        printf("\n\t\t\tAccount number: %d\n\t\t\tDeposit Date: %d/%d/%d \n\t\t\tcountry: %s \n\t\t\tPhone number: %d \n\t\t\tAmount deposited: $%.2f \n\t\t\tType Of Account: %s\n\t\t\tAccount ID: %d", r.accountNbr,r.deposit.day,r.deposit.month,r.deposit.year,r.country,r.phone,r.amount,r.accountType,r.id);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    success(u);
}
