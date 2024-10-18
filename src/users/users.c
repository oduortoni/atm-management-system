#include "../header.h"

const char *dbRetrievePassword(User u)
{
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    {
        fprintf(stderr, "Failed to open database\n");
        return NULL;
    }

    sqlite3_stmt *stmt = NULL;

    const char *sqlQuery = "SELECT password FROM USERS WHERE username = ?;";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    // Bind the username parameter to the SQL statement
    if (sqlite3_bind_text(stmt, 1, u.name, -1, SQLITE_STATIC) != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind username: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }

    const char *pass = NULL;
    // Execute the statement and process the results
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // Allocate memory for the password and copy it from the result
        pass = strdup((const char *)sqlite3_column_text(stmt, 0));
    }
    else
    {
        // If no match is found
        pass = "no user found";
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return pass;
}
