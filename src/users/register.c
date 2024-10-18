#include "../header.h"

void dbUserRegister(User u) {
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = sqliteHandler(dbpath);
    if (!db) {
        die();
    }

    const char *sqlquery = "INSERT INTO USERS (username, password) VALUES (?, ?);";

    int success = sqlite3_prepare_v2(db, sqlquery, -1, &stmt, NULL);

    if (success != SQLITE_OK)
    {
        sqliteError(db, "Failed to prepare statement: ", stmt);
    }

    sqlite3_bind_text(stmt, 1, u.name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, u.password, -1, SQLITE_STATIC);

    success = sqlite3_step(stmt);

    if (success != SQLITE_DONE) {
        sqliteError(db, "Execution Failed: ", stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Function to register a new user
void registerMenu(User u)
{
    char confirmPwd[maxinputlen];

    while (1)
    {
        printf("\n\t\tEnter Your Name >>>_");
        if (fgets(u.name, sizeof(u.name), stdin) != NULL)
        {
            trimlinechar(u.name);

            // Validate the name length
            if (!isstring(u.name, maxinputlen))
            {
                system("clear");
                printf("\n\t\t\tName too long. Please input a shorter name.\n\n");
                continue;
            }
            if (!isalphabet(u.name))
            {
                system("clear");
                printf("\n\t\tInvalid input. Please enter a valid name\n\n");
                continue;
            }
        }
        else
        {
            die();
        }
        // Check if username already exists
        if (dbUsernameExists(u.name))
        {
            system("clear");
            printf("\n\t\tUsername exists. Please choose a different name.\n");
            continue;
        }
        break;
    }

    while (1)
    {
        input_hide();

        printf("\n\t\tNew Password >>>_");
        if (fgets(u.password, sizeof(u.password), stdin) != NULL)
        {
            trimlinechar(u.password);
            if (!isstring(u.password, maxinputlen))
            {
                printf("\n\t\t\tInvalid input. Please enter a valid password.\n\n");
                continue;
            }
        }
        else
        {
            die();
        }

        printf("\n\t\tConfirm Password >>>_");
        if (fgets(confirmPwd, sizeof(confirmPwd), stdin) != NULL)
        {
            trimlinechar(confirmPwd);
            if (!isstring(confirmPwd, maxinputlen))
            {
                printf("\n\t\t\tInvalid input. Please enter a valid password.\n\n");
                continue;
            }
        }
        else
        {
            die();
        }

        // Restore terminal settings
        input_show();

        if (strcmp(u.password, confirmPwd) == 0)
        {
            break; // Exit loop if passwords match
        }
        else
        {
            printf("\n\t\tPasswords do not match!!!\n");
            exit(0);
        }
    }

    dbUserRegister(u);

    printf("\n\t\tUser registered successfully!!!\n\n");
    mainMenu(u);
}
