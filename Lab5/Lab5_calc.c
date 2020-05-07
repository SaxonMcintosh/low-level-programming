/**
 * A calculator that performs all of its operations with the assistance
 * of a single bitwise add operation and warns the user of overflow and
 * underflow related errors. I had to modify a few things to compensate
 * for my incredibly strict compiler.
 *
 * @author Saxon McIntosh
 * @version March 21 2020
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void menu();
int _add(int a, int b);
int add(int a, int b);
int sub(int a, int b);
int neg(int a);
int mul(int a, int b);
// My compiler got mad at me becuase of 'div' in the standard library.
int divi(int a, int b);
int mod(int a, int b);
// My compiler got mad at me becuase of 'pow' in the standard library.
int powr(int a, int b);
int convert(char *input);

int main(int argc, char *argv[]){
  
  int res = 0;
  int n = 0;
  char input[50];
  input[0] = '\0';

  while(input[0] != 'q' && input[0] != 'Q'){
    menu();
    printf("\nres = %d > ", res);
    // I had to change gets to fgets as my compiler would not allow the
    // use of it.
    fgets(input, sizeof(input), stdin);
    // I changed this to 'clear' for my linux box.
    system("clear");

    switch (input[0]){
    case '+':
      res = add(res, convert(input));
      break;
    case '-':
      res = sub(res, convert(input));
      break;
    case '*':
      res = mul(res, convert(input));
      break;
    case '/':
      res = divi(res, convert(input));
      break;
    case '%':
      res = mod(res, convert(input));
      break;
    case '~':
      res = neg(res);
      break;
    case '^':
      res = powr(res, convert(input));
      break;
    case 'c':
    case 'C':
      res = 0;
      break;
    case 'q':
    case 'Q':
      printf("Good-bye!\n");
      break;
    default:
      printf("Enter a valid operator and operand\n");
    }
  }
  return 0;
}

/**
 * A helper function that prints the menu options.
 */
void menu(){
  printf("\nSafe Integer Calculator\n");
  printf("+ x to add\n");
  printf("- x to subtract\n");
  printf("* x to multiply\n");
  printf("/ x to divide\n");
  printf("%% x to modulus\n");
  printf("~ x to negate\n");
  printf("^ x to raise by power x\n");
  printf("c x to clear result\n");
  printf("q x to quit\n");
  return;
}

/**
 * This add option uses only bitwise operators.
 *
 * @param an element to add
 * @param another element to add
 *
 * @return the sum of the operation
 */
int _add(int a, int b){
  while (b != 0) {
    // Whenever two ones are added together, you must carry it to the
    // next place. carry marks the places for this.
    int carry = a & b;
    // The non-carry places that take a one get it added here.
    a = a ^ b;
    // b holds carry bitshifted to the left, to perform the next
    // operation.
    b = carry << 1;
  }
  return a;
}

/**
 * This is essentially a wrapper for the _add function that contributes
 * to the calculator's 'safe' aspect.
 *
 * @param a an element to add
 * @param b another element to add
 *
 * @return the sum of the operation
 */
int add(int a, int b){
  int res = 0;
  res = _add(a, b);
  if (a > 0 && b > 0 && res < 0) {
    printf("Warning: Overflow Detected\n");
  } else if (a < 0 && b < 0 && res > 0) {
    printf("Warning: Underflow Detected\n");
  }
  return res;
}

/**
 * This returns the negated version of the argument.
 *
 * @param a the integer to be negated
 *
 * @return the negation of the original integer
 */
int neg(int a){
  /* neg operates based on two's complement. If 'a' is the maximum or
   * minimum value for int, the calculator will produce an error
   * statement.*/
  return add(a ^ -1, 1);
}

/**
 * Subtract. Name's on the box.
 *
 * @param a the integer to be subtracted from
 * @param b the amount being subtracted
 *
 * @return the difference of a and b
 */
int sub(int a, int b){
  return add(a, neg(b));
}

/**
 * mul works by adding the value a set number of times.
 *
 * @param a one integer to multiply
 * @param b another integer to multiply
 *
 * @return the product of the multiplication
 */
int mul(int a, int b){
  int res = 0;
  int sign = 1;
  // To perform as few operations as possible, the values are saved
  // based on size.
  int biggie;
  int smalls;
  if (a > b) {
    biggie = a;
    smalls = b;
  } else {
    biggie = b;
    smalls = a;
  }
  if (biggie < 0) {
    biggie = neg(biggie);
    sign = neg(sign);
  }
  if (smalls < 0) {
    smalls = neg(smalls);
    sign = neg(sign);
  }
  int i = 0;
  while (i < smalls) {
    res = add(res, biggie);
    i = add(i, 1);
  }
  if (sign < 0) {
    res = neg(res);
  }
  return res;
}

/**
 * divi works the same way mul does, but in reverse.
 *
 * @param a the integer to be divided
 * @param b the integer that divides 'a'
 *
 * @return the quotient of a and b
 */
int divi(int a, int b){
  int cnt = 0;
  int sign = 1;
  if (a < 0) {
    a = neg(a);
    sign = neg(sign);
  }
  if (b < 0) {
    b = neg(b);
    sign = neg(sign);
  }
  while (a >= b) {
    a = sub(a, b);
    cnt = add(cnt, 1);
  }
  if (sign < 0) {
    cnt = neg(cnt);
  }
  return cnt;
}

/**
 * mod is like divi, but providing the remainder instead of the quotient
 *
 * @param a the integer to be divided
 * @param b the integer that divides 'a'
 *
 * @return the remainder of a and b
 */
int mod(int a, int b){
  if (a < 0) {
    a = neg(a);
  }
  if (b < 0) {
    b = neg(b);
  }
  while (a >= b) {
    a = sub(a, b);
  }
  return a;
}

/**
 * powr performs mul times the exponent.
 *
 * @param n the base
 * @param exp the exponent
 *
 * @return the product of the multiplications
 */
int powr(int n, int exp){
  if (exp < 0) {
    printf("This isn't that kind of calculator, pal.");
    return 0;
  }
  if (exp == 0) {
    return 1;
  }
  int res = 1;
  while (exp != 0) {
    res = mul(res, n);
    exp = sub(exp, 1);
  }
  return res;
}

/**
 * convert pulls the numeric string and converts it into an integer.
 *
 * @param input the user's operand and numeric string
 *
 * @return the integer from the string, or 0 if there's an error
 */
int convert(char *input){
  printf("%s", input);
  int res = 0;
  int sign = 1;
  // i should be placed at the start of the numeric string.
  int i = 2;
  int j = 0;

  // 11 is big enough to hold max int and a null char.
  char buffer[11];
  int error = 0;

  if (input[1] != 32) {
    printf("Please use a disambiguating space between operator and operand.\n");
    error = add(error, 1);
  }
  if (input[2] == 45) {
    sign = neg(sign);
    i = 3;
  }

  // If no limit is imposed on j, the program breaks.
  while (input[i] != '\n' && j != 12) {
    if (input[i] >= 48 && input[i] <= 57) {
      buffer[j] = input[i];
      j = add(j, 1);
    } else {
      printf("Non-integer detected in operand.\n");
      error = add(error, 1);
    }
    i = add(i, 1);
  }
  buffer[j] = '\0';
  if (error > 0) {
    return 0;
  }
  i = 0;
  while (buffer[i] != '\0') {
    j = sub(j, 1);
    res = add(res, mul(sub(buffer[i], 48), powr(10, j)));
    i = add(i, 1);
  }
  if (sign < 0) {
    res = neg(res);
  }
  return res;
}
