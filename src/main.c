#include "header.h"

int main() {
    User u;

    sqliteInit("./data/atm.db");
    initMenu(&u);
    mainMenu(u);

    return 0;
}
