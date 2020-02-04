/**
 * As requested. C is bizarre.
 *
 * @author Saxon McIntosh
 * @version January 28, 2020
 */

#include <stdio.h>
# define MAX 64

int strLength(char str[]);
int isLower(char c);
char toUpper(char c);
void getString(char str[]);
void toReverse(char str[], int len);
int toCaps(char str[], int len);
void eccentricPrint(char str[], int len, int newCaps);

int main() {
  char str[MAX];
  
  getString(str);
  int len = strLength(str);

  toReverse(str, len);
  
  int newCaps = 0;
  newCaps = toCaps(str, len);

  eccentricPrint(str, len, newCaps);
  
  return 0;
}

/**
 * Captures the length of the input by counting the chars until the
 * null character.
 *
 * @param the string whose length will be counted
 */
int strLength(char str[]) {
  int i = 0;
  int count = 0;
  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] != '\n') {
      count++;
    }
  }
  return count;
}

/**
 * Returns a boolean (int) determining whether the char is lowercase
 * or not.
 *
 * @param the char to be vetted
 */
int isLower(char c) {
  return c >= 'a';
}

/**
 * Downgrades the char 32 places in the ASCII table to make it uppercase
 *
 * @param the char to be capitalized
 */
char toUpper(char c) {
  return c - 32;
}

/**
 * Retrieves a string from the user and performs light input validation.
 *
 * @param the empty string meant to be filled by the user input
 */
void getString(char str[]) {
  printf("Please enter some text (and keep it under 64 characters, capisce?): ");
  do {
    str = fgets(str, MAX, stdin);
    if (strLength(str) < 1) {
      printf("Come on, please try actually typing something: ");
    }
  } while (strLength(str) < 1);
}

/**
 * Reverses the string argument - since the function is void I had to
 * a copy of the original to disassemble.
 *
 * @param the string to be reversed
 * @param the length of the string to be reversed
 */
void toReverse(char str[], int len) {
  char temp[len];
  int i = 0;
  for (i = 0; i < len; i++) {
    temp[i] = str[i];
  }
  int j = 0;
  for (i = len - 1; i >= 0; i--, j++) {
    str[j] = temp[i];
  }
}

/**
 * Capitalizes the lowercase chars and returns the tally.
 *
 * @param the string to be capitalized / counted
 * @param the length of said string
 */
int toCaps(char str[], int len) {
  int count = 0;
  int i = 0;
  for (i = 0; i < len; i++) {
    if (isLower(str[i])) {
      str[i] = toUpper(str[i]);
      count++;
    }
  }
  return count;
}

/**
 * Prints the string argument one char per line, then the count of
 * total chars in the string and the number of capitalized chars.
 *
 * @param the string to be dissected
 * @param the length of that string
 * @param the number of chars that were capitalized
 */
void eccentricPrint(char str[], int len, int newCaps) {
  int i = 0;
  for (i = 0; i < len; i++) {
    printf("%c\n", str[i]);
  }

  printf("\nThe string is %d chars and %d chars were capitalized.\n",
	 len, newCaps);
}
