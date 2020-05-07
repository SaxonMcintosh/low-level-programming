/**
 * A program intended to outline the rigors of copying text from a file
 * into memory, and from there into a second file.
 *
 * @author Saxon McIntosh
 * @version February 3, 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned long size;
unsigned long *psize = &size;

char* read_file(unsigned long *size, char *file_name);
int write_file(unsigned long size, char *output, char *file_name);
void make_rand_key(unsigned long size, char *rand_key);
void encrypt(char *clear, char *key, char *cipher);
void decrypt(char *key, char *cipher, char *newClear);

int main() {
  char clear[20];
  char key[20];
  char cipher[20];
  
  int user_int;
  printf("Hello and welcome to my spooky spy application!\n");
  printf("It can be used to encrypt and decrypt text files.\n");
  printf("Please enter a number to make a selection.\n");
  do {
    printf("1.Encrypt a file\n");
    printf("2.Decrypt a file\n");
    printf("3.Exit\n> ");

    // Accepts user input and converts it to an integer using atoi().
    char user_in[1];
    scanf("%s", user_in);
    user_int = atoi(user_in);
    
    switch (user_int) {
    case 1:
      printf("Please enter the name of the file that you'd like to encrypt:\n> ");
      scanf("%s", clear);

      printf("Please enter the desired name of your new encryption key:\n> ");
      scanf("%s", key);
      
      printf("Please enter the desired name of your new cipher:\n> ");
      scanf("%s", cipher);

      encrypt(clear, key, cipher);

      printf("\nCipher and key successfully created.\n\n");
      break;
      
    case 2:
      printf("Please enter the name of the file that you'd like to decrypt:\n> ");
      scanf("%s", cipher);

      printf("Please enter the name of the key for this encryption:\n> ");
      scanf("%s", key);
      
      printf("Please enter the desired name of your new message file:\n> ");
      scanf("%s", clear);

      decrypt(key, cipher, clear);

      printf("\nMessage successfully created.\n\n");
      break;
      
    case 3:
      printf("\nThanks for playing. Goodbye!\n");
      break;
      
    default:
      printf("\nWell that input was no good - please try again!\n\n");
    }
  } while (user_int != 3);
  
  return 0;
}

/**
 * Uses malloc to assign space for a string that contains the file's
 * text.
 *
 * @param will point to the size of the text
 * @param the location of the first byte of the file name
 * @return the location of the allocated space
 */
char* read_file(unsigned long *psize, char *file_name) {
  // A file pointer is used while opening the file.
  FILE *filep = fopen(file_name, "rb");
  // In the event the file name doesn't exist:
  if (filep == NULL) {
    printf("That's an invalid file name. Later!\n");
    exit(0);
  }

  // Sets the file pointer to the end of the file, then assigns the
  // files length to size.
  fseek(filep, 0, SEEK_END);
  *psize = (unsigned long) ftell(filep);
  // Sets the file pointer back to the beginning of the file so that
  // it can be read later.
  rewind(filep);

  // buffer is set to the location of the newly allocated
  // memory.
  char *buffer = malloc(*psize * sizeof(char));
  if (buffer == NULL) {
    printf("Malloc unsuccessful. Goodbye!\n");
    exit(0);
  }

  // buffer is filled with the contents of the file, and the file is
  // closed.
  fread(buffer, sizeof(char), *psize, filep);
  fclose(filep);

  return buffer;
}

/**
 * Writes an existing string into a file.
 *
 * @param the size of the string to write
 * @param the location of the string to be written
 * @param the name of the new file
 */
int write_file(unsigned long size, char *output, char *file_name) {
  // Very similar to read_file.
  FILE *filep = fopen(file_name, "wb");
  if (filep == NULL) {
    printf("Well I'll be darned - there was a problem when writing the file. Goodbye!\n");
    exit(0);
  }
  
  fwrite(output, sizeof(char), size, filep);

  fclose(filep);
}

/**
 * Generates a random key using time to encrypt/decrypt a message.
 *
 * @param the length of the key
 * @param the char array that will hold the new key
 */
void make_rand_key(unsigned long size, char *rand_key) {
  // The number of seconds that have passed since Jan 1, 1970.
  time_t seconds;
  // srand initialized a seed for rand() to use, in this case since
  // the argument changes over time, it will never be repeated
  // (unless the program is run more than once in a second
  srand((unsigned) time(&seconds));
  
  int i = 0;
  for (i = 0; i < size; i++) {
    // I wanted to use familiar ASCII values, so my floor is set at the
    // first real character '!' with a ceiling of '~'.
    rand_key[i] = '!' + (rand() % 94);
  }
  rand_key[size] = '\0';
}

/**
 * Cypher creates two files based on user input - a key of random chars,
 * and a cipher of the original clear text.
 *
 * @param the filename of the clear text
 * @param the filename of the new key file
 * @param the filename of the new cipher file
 */
void encrypt(char *clear, char *key, char *cipher) {
  // buffer is made using malloc - must be freed later.
  char *buffer = read_file(psize, clear);

  // size + 1 to accommodate the new '\0'
  char rand_key[size + 1];
  make_rand_key(size, rand_key);

  char encrypted[size + 1];
  encrypted[size] = '\0';

  // Next up is the heart of this assignment:
  int i = 0;
  for (i = 0; i < size; i++) {
    encrypted[i] = buffer[i] ^ rand_key[i];
  }
  free(buffer);

  write_file(size, rand_key, key);
  write_file(size, encrypted, cipher);
}

/**
 * Decrypt gathers a key and cipher from user input, then processes
 * them and writes a message of the decrypted text.
 *
 * @param the filename of the key file
 * @param the filename of the cipher file
 * @param the filename of the new clear text
 */
void decrypt(char *key, char *cipher, char *newClear) {
  char *rand_key = read_file(psize, key);
  char *encrypted = read_file(psize, cipher);

  int i = 0;
  for (i = 0; i < size; i++) {
    encrypted[i] = encrypted[i] ^ rand_key[i];
  }

  write_file(size, encrypted, newClear);
  
  free(rand_key);
  free(encrypted);
}
