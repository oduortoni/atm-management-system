#include "../header.h"

// fetches the user data from the keyboard
// updates the parameters with these credentials
void loginMenu(char a[50], char pass[50]) {
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login >>>_");
    if (fgets(a, 50, stdin) != NULL) {
        trimlinechar(a);

        // Validate the input
        if (!isstring(a, 50)) {
            system("clear");
            printf("\n\t\t\tInvalid input. Please enter a valid Name.\n\n");
            exit(0);
        }
        if (!isalphabet(a)) {
            system("clear");
            printf("\n\tInvalid input. Please enter a valid name\n\n");
            exit(0);
        }
    }
    else
    {
        die();
    }

    input_hide();
    printf("\n\n\n\n\n\t\t\t\tUser Password >>>_");
    if (fgets(pass, 50, stdin) != NULL) {
        trimlinechar(pass);

        // Validate the input
        if (!isstring(pass, 50)) {
            system("clear");
            printf("\n\t\t\tInvalid input. Please enter a valid Input.\n\n");
            exit(0);
        }
    } else {
        die();
    }
    input_show();
};
