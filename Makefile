norecompile = objects/sqlite3.o
objects = objects/main.o objects/input.o objects/utils.o objects/accounts.o objects/details.o objects/constants.o objects/menu.o objects/system.o objects/users.o objects/register.o objects/login.o objects/sqhelpers.o objects/create.o

atm: $(objects) $(norecompile)
	cc $(objects) $(norecompile) -o atm 

objects/main.o:
	cc -c src/main.c -o objects/main.o

objects/input.o:
	cc -c src/input.c -o objects/input.o

objects/utils.o:
	cc -c src/utils.c -o objects/utils.o

objects/constants.o:
	cc -c src/constants.c -o objects/constants.o

objects/menu.o:
	cc -c src/menu.c -o objects/menu.o

objects/system.o:
	cc -c src/system.c -o objects/system.o

objects/users.o:
	cc -c src/users/users.c -o objects/users.o

objects/register.o:
	cc -c src/users/register.c -o objects/register.o

objects/login.o:
	cc -c src/users/login.c -o objects/login.o

objects/create.o:
	cc -c src/accounts/create.c -o objects/create.o

objects/accounts.o:
	cc -c src/accounts/accounts.c -o objects/accounts.o

objects/details.o:
	cc -c src/accounts/details.c -o objects/details.o

objects/sqhelpers.o:
	cc -c src/sqlite/helpers.c -o objects/sqhelpers.o

objects/sqlite3.o:
	cc -c src/sqlite/sqlite3.c -o objects/sqlite3.o

clean:
	rm -f $(objects) atm
