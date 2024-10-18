norecompile = objects/sqlite3.o
objects = objects/main.o objects/input.o objects/menu.o objects/system.o objects/users.o objects/register.o objects/sqhelpers.o

atm: $(objects) $(norecompile)
	cc $(objects) $(norecompile) -o atm 

objects/main.o:
	cc -c src/main.c -o objects/main.o

objects/input.o:
	cc -c src/input.c -o objects/input.o

objects/utils.o:
	cc -c src/utils.c -o objects/utils.o

objects/menu.o:
	cc -c src/menu.c -o objects/menu.o

objects/system.o:
	cc -c src/system.c -o objects/system.o

objects/users.o:
	cc -c src/users/users.c -o objects/users.o

objects/register.o:
	cc -c src/users/register.c -o objects/register.o

objects/sqhelpers.o:
	cc -c src/sqlite/helpers.c -o objects/sqhelpers.o

objects/sqlite3.o:
	cc -c src/sqlite/sqlite3.c -o objects/sqlite3.o

clean:
	rm -f objects/*.o atm
