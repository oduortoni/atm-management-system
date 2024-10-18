#include "header.h"

// when given the date struct, it checks whether it is comprised of legal numbers
// takes into account leap years and awkward months like february
bool isdate(Date date) {
	if (date.month < 1 || date.month > 12) { return false; }
	if (date.day < 1) { return false; }
	time_t tm = time(NULL);
	struct tm *tm_info = localtime(&tm);

	int currenYear = tm_info->tm_year + 1900;
	if (date.year < 1472 || date.year > currenYear) { return false; }

	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (date.month == 2 && isleapyear(date.year)) {
		daysInMonth[1] = 29; // feb days in a leapYr
	}
	if (date.day > daysInMonth[date.month - 1]) { return false; }

	return true;
}

void trimlinechar(char *str) {
	size_t len = strlen(str);
	if (len > 0 && str[len - 1] == '\n') {
		str[len - 1] = '\0';
	}
}

bool ispositive(const char *str) {
	bool hasdot = false;
	const char *str_p = str;

	if (str == NULL || *str == '\0' || *str_p == '-') { return false; }

	while (*str_p != '\0') {
		if (*str_p == '.') {
			if (hasdot) { return false; } // already had a dot and we found another hence, there are many dots
			else { hasdot = true; } // found our first dot, continue
		} else if (!isdigit(*str_p)) {
			return false;
		}
		str_p++;
	}

	return true;
}

// the converted double will be stored in the dnum variable
bool isvaliddouble(const char *str, double *dnum) {
	char *end;
	errno = 0;

	if (!ispositive(str)) {
		return false;
	}

	*dnum = strtod(str, &end);
	if (end == str || *end != '\0' || errno == ERANGE) {
		return false;
	}

	return true;
}

bool isalphabet(char *str) {
	int i;
	for (i = 0; i < strlen(str); i++) {
		if (!isalpha((unsigned char)str[i]) && str[i] != ' ') { return false; }
	}
	return true;
}

bool isleapyear(int year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isstring(const char *str, size_t n) {
	if (str == NULL || strlen(str) == 0) {
		// NULL pointer or empty
		return false;
	} else if (strlen(str) >= n) {
		// exceeds maxinputlen
		return false;
	}
	return true;
}
