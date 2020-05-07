/**
 * Just a few exercises converting integers between bases.
 *
 * @author Saxon McIntosh
 * @version March 19, 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int bin_to_uint(char *input);
unsigned int oct_to_uint(char *input);
unsigned int hex_to_uint(char *input);
unsigned int dec_to_uint(char *input);
void uint_to_hex(unsigned int n, char *output);
void uint_to_oct(unsigned int n, char *output);
void uint_to_bin(unsigned int n, char *output);

int error = 0;

int main(){
  char input[50];
  unsigned int n = 0;
  char output[50];

  printf("Enter a binary, octal, decimal or hexadecimal number\n");
    printf("convert > ");
    // I had to change it to fgets as my compiler would not allow the
    // use of gets.
    fgets(input, sizeof(input), stdin);

    if(input[0] == '0' && input[1] == 'x'){
        n = hex_to_uint(input);
    }
    else if(input[0] >= '0' && input[0] <= '9'){
        n = dec_to_uint(input);
    }
    else if(input[0] == 'o'){
        n = oct_to_uint(input);
    }
    else if(input[0] == 'b'){
        n = bin_to_uint(input);
    }
    else{
        printf("ERROR: Unknown data type: %s\n", input);
    }

    printf("The decimal value of %s is %u\n", input, n);
    uint_to_hex(n, output);
    printf("The hexadecimal value of %s is %s\n", input, output);
    uint_to_oct(n, output);
    printf("The octal value of %s is %s\n", input, output);
    uint_to_bin(n, output);
    printf("The binary value of %s is %s\n", input, output);
  
  return 0;
}

/**
 * Converts a binary string to an unsigned integer.
 *
 * @param input the user's input string
 *
 * @return the user's unsigned int
 */
unsigned int bin_to_uint(char *input){
  char c;
  unsigned int res = 0;
  int mult = 1;
  int i = 0;
  for (int i = strlen(input) - 2; i > 0; i--) {
    c = input[i];
    if (c == 48 || c == 49) {
      res += (c - 48) * mult;
    } else {
      printf("Incorrect binary value. Exiting.\n");
      exit(1);
    }
    mult *= 2;
  }
  return res;
}

/**
 * Converts a hex string to an unsigned integer.
 *
 * @param input the user's input string
 *
 * @return the user's unsigned int
 */
unsigned int hex_to_uint(char *input){
  char c;
  unsigned int res = 0;
  int mult = 1;
  int i = 0;
  for (int i = strlen(input) - 2; i > 1; i--) {
    c = input[i];
    
    if (c >= 65 && c <= 70) {
      c += 32;
    }
    if (c >= 48 && c <= 57) {
      res += (c - 48) * mult;
    }
    else if (c >= 97 && c <= 102) {
      res += (c - 87) * mult;
    } 
    else {
      printf("Incorrect hex value. Exiting.\n");
      exit(1);
    }
    mult *= 16;
  }
  return res;
}

/**
 * Converts a decimal string to an unsigned integer.
 *
 * @param input the user's input string
 *
 * @return the user's unsigned int
 */
unsigned int dec_to_uint(char *input){
  char c;
  unsigned int res = 0;
  int mult = 1;
  int i = 0;
  for (int i = strlen(input) - 2; i >= 0; i--) {
    c = input[i];
    if (c >= 48 && c <= 57) {
      res += (c - 48) * mult;
    }
    else {
      printf("Incorrect decimal value. Exiting.\n");
      exit(1);
    }
    mult *= 10;
  }
  return res;
}

/**
 * Converts an octal string to an unsigned integer.
 *
 * @param input the user's input string
 *
 * @return the user's unsigned int
 */
unsigned int oct_to_uint(char *input){
  char c;
  unsigned int res = 0;
  int mult = 1;
  int i = 0;
  for (int i = strlen(input) - 2; i > 0; i--) {
    c = input[i];
    if (c >= 48 && c <= 55) {
      res += (c - 48) * mult;
    }
    else {
      printf("Incorrect octal value. Exiting.\n");
      exit(1);
    }
    mult *= 8;
  }
  return res;
}

/**
 * Converts an unsigned integer into hex to print.
 *
 * @param n the int to print
 * @param output an address to an array that will store the new string
 */
void uint_to_hex(unsigned int n, char *output){
  unsigned int rem = 0;
  int div = 16;
  char buffer[9];
  int i = 0;
  for (i = 7; i >= 0; i--) {
    rem = n % div;
    if (rem >= 10 && rem <= 15) {
      buffer[i] = rem + 55;
    } else {
      buffer[i] = rem + 48;
    }
    n /= div;
  }
  buffer[8] = '\0';
  strcpy(output, "0x");
  strcat(output, buffer);
  return;
}

/**
 * Converts an unsigned integer into octal to print.
 *
 * @param n the int to print
 * @param output an address to an array that will store the new string
 */
void uint_to_oct(unsigned int n, char *output){
  unsigned int rem = 0;
  int div = 8;
  char buffer[12];
  int i = 0;
  for (i = 10; i >= 0; i--) {
    rem = n % div;
    buffer[i] = rem + 48;
    n /= div;
  }
  buffer[11] = '\0';
  strcpy(output, "o");
  strcat(output, buffer);
  return;
}

/**
 * Converts an unsigned integer into binary to print.
 *
 * @param n the int to print
 * @param output an address to an array that will store the new string
 */
void uint_to_bin(unsigned int n, char *output){
  unsigned int rem = 0;
  int div = 2;
  char buffer[33];
  int i = 0;
  for (i = 31; i >= 0; i--) {
    rem = n % div;
    buffer[i] = rem + 48;
    n /= div;
  }
  buffer[32] = '\0';
  strcpy(output, "b");
  strcat(output, buffer);
  return;
}
