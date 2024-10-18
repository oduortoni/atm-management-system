#include "header.h"

// takes in a string of characters from the stdin
void input_string(User u, char *input) {
	char *nlpos; // position of \n (nl)

	printf(">>> ");

	if (fgets(input, maxinputlen, stdin) != NULL) {
		nlpos = strchr(input, '\n'); // end string at \n
		if (nlpos != NULL) {
			*nlpos = '\0'; // Replace newline with null terminator
		}
		trimlinechar(input);
		if (!isstring(input, maxinputlen)) {
			system("clear");
			printf("\n\t\tInvalid input. Please enter a valid Input.\n\n");
			Return(u);
		}
	} else {
		die();
	}
}

// takes in a string of characters from stdin and tries to convert them to a valid number
int input_number() {
	char input[maxinputlen];
	char *end;
	long n;

	printf(">>> ");

	while (1) {
		if (fgets(input, sizeof(input), stdin) != NULL) {
			input[strcspn(input, "\n")] = '\0'; // remove trailing \n

			errno = 0;
			n = strtol(input, &end, 10);
			if (*end != '\0' || input == end) {
				printf("\n\t\tEnter a number: ");
			} else if( ((n == LONG_MAX || n == LONG_MIN) && errno == ERANGE) || (n < INT_MIN || n > INT_MAX) ) {
				printf("\n\t\t[ERROR] Enter a valid number: ");
			} else {
				return (int)n;
			}
		} else {
			printf("\n\t\tError reading input. Please try again...");
		}
	}
}
// sets the echo mode to off
// thereby hiding user input as they type
void input_hide()
{
	struct termios flags;
	tcgetattr(fileno(stdin), &flags);
	flags.c_lflag &= ~ECHO;
	tcsetattr(fileno(stdin), TCSANOW, &flags);
}
// sets the echo mode to on
// thereby showing user input as they type
void input_show()
{
	struct termios flags;
	tcgetattr(fileno(stdin), &flags);
	flags.c_lflag |= ECHO;
	tcsetattr(fileno(stdin), TCSANOW, &flags);
}
