objects = src/utils.o src/menu.o src/system.o src/constants.o src/input.o src/users/users.o src/users/register.o src/users/login.o src/users/accounts.o src/accounts/transfer.o src/accounts/accounts.o src/accounts/transaction.o src/accounts/update.o  src/accounts/details.o src/accounts/create.o src/accounts/delete.o src/dbsqlite.o src/main.o
norecompile = src/sqlite3.o
database = data/atm.db
testrunner = tests/runner

target = atm

$(target): $(objects) $(norecompile)
	cc -o atm $(objects) $(norecompile)

src/main.o:
	cc -c src/main.c -o src/main.o

src/menu.o:
	cc -c src/menu.c -o src/menu.o

src/system.o:
	cc -c src/system.c -o src/system.o

src/utils.o:
	cc -c src/utils.c -o src/utils.o

src/input.o:
	cc -c src/input.c -o src/input.o

src/constants.o:
	cc -c src/constants.c -o src/constants.o

src/users/users.o:
	cc -c src/users/users.c -o src/users/users.o

src/users/register.o:
	cc -c src/users/register.c -o src/users/register.o

src/users/login.o:
	cc -c src/users/login.c -o src/users/login.o

src/users/accounts.o:
	cc -c src/users/accounts.c -o src/users/accounts.o

src/accounts/accounts.o:
	cc -c src/accounts/accounts.c -o src/accounts/accounts.o

src/accounts/create.o:
	cc -c src/accounts/create.c -o src/accounts/create.o

src/accounts/update.o:
	cc -c src/accounts/update.c -o src/accounts/update.o

src/accounts/details.o:
	cc -c src/accounts/details.c -o src/accounts/details.o

src/accounts/transaction.o:
	cc -c src/accounts/transaction.c -o src/accounts/transaction.o

src/accounts/transfer.o:
	cc -c src/accounts/transfer.c -o src/accounts/transfer.o

src/accounts/delete.o:
	cc -c src/accounts/delete.c -o src/accounts/delete.o

src/dbsqlite.o:
	cc -c src/dbsqlite.c -o src/dbsqlite.o

src/sqlite3.o:
	cc -c src/sqlite3.c -o src/sqlite3.o

clean:
	rm -f $(objects) $(target)

destroy:
	rm -f $(objects)  $(target) $(norecompile) $(database) $(testrunner)

