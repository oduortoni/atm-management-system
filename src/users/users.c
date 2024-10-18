#include "../header.h"

int dbRetrieveUserId(const char *username)
{
    sqlite3_stmt *stmt = NULL;
    int id;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db) {
        sqliteError(db, "Failed to open database: ", NULL);
    }

    const char *sqlQuery = "SELECT user_id FROM USERS WHERE username = ?;";
    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL) != SQLITE_OK) {
        sqliteError(db, "Failed to Prepare statement", stmt);
    }

    // Bind the username parameter to the SQL statement
    if (sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) != SQLITE_OK) {
        sqliteError(db, "Failed to bind username", stmt);
    }

    // Execute the statement and process the results
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    } else {
        // id not resolved from the name
        id = -1;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return id;
}

char *dbRetrieveUserName(int user_id) {
    sqlite3_stmt *stmt = NULL;
    char *name = NULL;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db)
    {
        sqliteError(db, "Failed to open database: ", NULL);
    }

    const char *sqlQuery = "SELECT username  FROM USERS WHERE user_id = ?;";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL) != SQLITE_OK)
    {
        sqliteError(db, "Failed to prepare statement: ", stmt);
    }

    // bind user_id to statement
    if (sqlite3_bind_int(stmt, 1, user_id) != SQLITE_OK)
    {
        sqliteError(db, "Failed to bind user id: ", stmt);
    }

    // Execute the statement and check the result
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        // Extract the name from the result row
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (text)
        {
            name = strdup((const char *)text); // Duplicate the string
            if (!name)
            {
                sqliteError(db, "Memory Allocation Failed: ", stmt);
            }
        }
    }
    else if (rc == SQLITE_DONE) {
        name = NULL;
    } else {
        sqliteError(db, "Execution Failed: ", stmt);
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return name;
}

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

bool dbUsernameExists(const char *username) {
    sqlite3_stmt *stmt = NULL;
    int rc;

    sqlite3 *db = sqliteHandler(dbpath);
    if (!db) {
        sqliteError(db, "error opening database: ", stmt);
    }

    bool exists = false;

    const char *sqlQuery = "SELECT 1 FROM USERS WHERE username = ? LIMIT 1;";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL) != SQLITE_OK) {
        sqliteError(db, "error preparing statement: ", stmt);
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        exists = true;
    }
    else if (rc != SQLITE_DONE)
    {
        sqliteError(db, "error on executing statement: ", stmt);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return exists;
}
