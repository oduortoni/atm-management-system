#include "header.h"

void initMenu(User *u) {
    int option;
    int r = 0;
    system("clear");

    printf("\n\n\t\t============= ATM =============\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- Login\n");
    printf("\n\t\t[2]- Register\n");
    printf("\n\t\t[3]- Exit\n");
    printf("\n\t\t");

    while (!r) {
        option = input_number();
        switch (option) {
            case 1:
                loginMenu(u->name, u->password);
                if (strcmp(u->password, dbRetrievePassword(*u)) == 0)
                {
                    printf("\n\t\t[Success login] Password Match!\n\n");
                    r = 1; // login success, exit loop on next iteration
                }
                else
                {
                    printf("\n\t\tWrong password or Username\n\n");
                    exit(EXIT_FAILURE);
                }
             break;
            case 2:
                registerMenu(*u);
                r = 1; // registration success, exit loop on next iteration
             break;
            case 3:
                system("clear");
                exit(EXIT_SUCCESS);
            default:
                printf("\n\t\tInsert a valid operation!\n");
             break;
        }
    }
};

void mainMenu(User u) {
    int choice;

    while (1) {
        system("clear");
        printf("\n\n\t\t======= >ATM< =======\n\n");
        printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
        printf("\n\t\t[1]- Create a new account\n");
        printf("\n\t\t[2]- Update account information\n");
        printf("\n\t\t[3]- Check accounts\n");
        printf("\n\t\t[4]- Check list of owned accounts\n");
        printf("\n\t\t[5]- Make Transaction\n");
        printf("\n\t\t[6]- Remove existing account\n");
        printf("\n\t\t[7]- Transfer ownership\n");
        printf("\n\t\t[8]- Exit\n");
        printf("\n\t\t");

        choice = input_number();

        switch (choice) {
            case 1:
                accountCreate(u);
              break;
            case 2:
                accountUpdateInfo(u);
                break;
            case 3:
                accountDetails(u);
                break;
            case 4:
                accountList(u);
                break;
            // case 5:
            //     accountMakeTransaction(u);
            //     break;
            case 6:
                accountDelete(u);
                break;
            // case 7:
            //     accountTransfer(u);
            //     break;
            case 8:
                system("clear");
                exit(EXIT_SUCCESS);
            default:
                printf("\n\n\t\tInvalid option. Please try again.\n");
                break;
        }
    }
}

