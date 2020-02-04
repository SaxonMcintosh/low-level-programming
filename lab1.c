/**
 * Just a small warm-up using maths to ease into C.
 *
 * @author Saxon McIntosh
 * @version January 21, 2020 
 */
 
#include <stdio.h>
// for atoi()
#include <stdlib.h>
// for isdigit()
#include <ctype.h>

int get_input();
// I played around with the idea of using a larger data type, but it
// felt like overkill.
unsigned int sum(int n);
double avg(int n);

int main() {
  int n;
  printf("Input a positive number - and please keep it to 7 digits or less: ");
  n = get_input();
  printf("The sum of the first %d integers is %u.\n", n, sum(n));
  printf("The average of the first %d integers is %.2f.\n", n, avg(n));
  return 0;
}

// This is almost entirely input validation - I'm still not used to
// conventions in C so it feels sloppy but it works.
int get_input() {
  // The size constraint is to prevent overflow later. 99999999 is too
  // large.
  char user_s[7];
  int flag;
  do {
    flag = 1;
    scanf("%s", user_s);
    int i = 0;
    int count = 0;
    for (i = 0; user_s[i] != '\0'; i++) {
      count++;
      // Because '-' and '.' are non-digits, this also conveniently
      // rules out having to deal with negative input or floats.
      if (isdigit(user_s[i]) == 0 || count > 7) {
	flag = 0;
      }
    }
    /*
    if (!atoi(user_s)) {
      flag = 0;
    }
    */
    if (flag == 0) {
      if (count > 7) {
	printf("Well that's far too long - please try again: ");
      } else {
	printf("Sorry, but that's not a positive integer - please give it another try: ");
      }
    }
  } while (flag == 0);
  int n = atoi(user_s);
  return n;
}

unsigned int sum(int n) {
  int i = 0;
  unsigned int count = 0;
  for (i = 0; i < n; i++) {
    count += i + 1;
  }
  return count;
}

double avg(int n) {
  double a = 0.0;
  a = sum(n);
  return a / n;
}
