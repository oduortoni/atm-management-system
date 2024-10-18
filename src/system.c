#include "header.h"

// Function to prompt the user for an option after an unsuccessful operation
void StayOrReturn(int notGood, void (*f)(User), User u) {
    int option;
    while (1)
    {
        if (notGood == 0)
        {
            printf("\n\n\t\tEnter\n\t\t0) to try again, \n\t\t1) to return to main menu and \n\t\t2) to exit\n\n\t\t");
            option = input_number();
            if (option == 0)
            {
                f(u);
                break;
            }
            else if (option == 1)
            {
                mainMenu(u);
                break;
            }
            else if (option == 2)
            {
                system("clear");
                exit(0);
            }
            else
            {
                printf("\n\n\t\tPlease insert a valid operation!\n");
            }
        }
        else
        {
            printf("\n\n\t\tEnter\n\t\t1) to go to the main menu\n\t\t0) to exit\n\n\t\t");
            option = input_number();
            if (option == 1)
            {
                system("clear");
                mainMenu(u);
                break;
            }
            else if (option == 0)
            {
                system("clear");
                exit(1);
            }
            else
            {
                printf("\n\n\t\tPlease insert a valid operation!\n");
            }
        }
    }
}

// Function to display a success message and prompt user for next action
void success(User u) {
    int option;

    printf("\n\n\t\t✔ Success!\n\n");

    do
    {
        printf("\n\n\t\tEnter\n\t\t1) to go to the main menu\n\t\t2) to exit\n\n\t\t");
        option = input_number();
        system("clear");
        switch (option)
        {
        case 1:
            mainMenu(u);
            break;
        case 2:
            exit(1);
        default:
            printf("\n\n\t\tInsert a valid operation!\n");
            break;
        }
    } while (option != 1 && option != 2);
}

// Function to return to the main menu or exit
void Return(User u) {
    int option;

    do {
        printf("\n\n\tEnter\n\t1) to go to the main menu\n\t2) to exit\n\n\t\t");
        option = input_number(); // Get user input

        switch (option) {
            case 1:
                system("clear");
                mainMenu(u);
                break;
            case 2:
                system("clear");
                exit(1);
            default:
                printf("\n\t[ERROR] Insert a valid operation!\n");
        }
    } while (option != 1 && option != 2); // Loop until a valid option is entered
}

// handle unrecoverable errors
void die() {
    system("clear");
    printf("\n\n\n\n\n\t\tAn unrecoverable error occured. Do try again later...\n");
    getchar();
    exit(1);
}
