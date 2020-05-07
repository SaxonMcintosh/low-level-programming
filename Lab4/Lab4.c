/**
 * An updated sneaky spy script that can now implement steganography.
 *
 * @author Saxon McIntosh
 * @version March 9, 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define HEADER_SIZE 54
#define WIDTH_BEGIN 18
#define HEIGHT_BEGIN 22

unsigned long size;
unsigned long *psize = &size;

typedef struct {
  long unsigned bSize;
  int width;
  int height;
  char *contents;
} Bitmap;

char* read_file(unsigned long *psize, char *file_name);
void write_file(unsigned long size, char *output, char *file_name);
void make_rand_key(unsigned long size, char *rand_key);
void encrypt(char *clear, char *key, char *cipher);
void decrypt(char *key, char *cipher, char *newClear);
Bitmap read_bmp(char *file_name);
unsigned int write_bmp(Bitmap m, char *file_name);
void encode(char *oldFile, char *newFile, char *messageFile);
void decode(char *encoded, char *newFile);

int main() {
  char clear[20];
  char key[20];
  char cipher[20];
  char bitmap[20];
  char secret[20];
  char message[20];

  int user_int;
  printf("Hello and welcome to my spooky spy application!\n");
  printf("It can be used to encrypt and decrypt text files.\n");
  printf("Please enter a number to make a selection.\n");
  do {
    printf("1.Encrypt a file\n");
    printf("2.Decrypt a file\n");
    printf("3.Encode a bitmap\n");
    printf("4.Decode a bitmap\n");
    printf("5.Exit\n> ");

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
      printf("Please enter the name of the bitmap that you'd like to encode:\n> ");
      scanf("%s", bitmap);

      printf("Please enter the name of your new secret message bitmap:\n> ");
      scanf("%s", secret);

      printf("Please enter the name of the message that you'd like to hide:\n> ");
      scanf("%s", message);

      encode(bitmap, secret, message);
      break;

    case 4:
      printf("Please enter the name of the bitmap you'd like to decode:\n> ");
      scanf("%s", secret);

      printf("Please enter the name of your new message file:\n> ");
      scanf("%s", message);

      decode(secret, message);
      break;
      
    case 5:
      printf("\nThanks for playing. Goodbye!\n");
      break;
      
    default:
      printf("\nWell that input was no good - please try again!\n\n");
    }
  } while (user_int != 5);
  
  return 0;
}

/**
 * Uses malloc to assign space for a string that contains the file's
 * text.
 *
 * @param will point to the size of the text
 * @param the location of the first byte of the file name
 *
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
void write_file(unsigned long size, char *output, char *file_name) {
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

/**
 * read_bmp reads the information from a bmp file into the Bitmap struct
 * and populates the new Bitmap's fields.
 *
 * @param file_name the name of the bmp to be processed
 *
 * @return an implementation of the Bitmap struct
 */
Bitmap read_bmp(char *file_name) {
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
  unsigned long bSize = ftell(filep);
  // Sets the file pointer back to the beginning of the file so that
  // it can be read later.
  rewind(filep);

  // buffer is set to the location of the newly allocated
  // memory.
  
  char *contents = malloc(bSize * sizeof(char));
  if (contents == NULL) {
    printf("Malloc unsuccessful. Goodbye!\n");
    exit(0);
  }

  fread(contents, sizeof(char), bSize, filep);
  fclose(filep);
  
  int width = *((int*)&(contents[WIDTH_BEGIN]));
  int height = *((int*)&(contents[HEIGHT_BEGIN]));

  Bitmap bMap = {bSize, width, height, contents};
  return bMap;
}

/**
 * write_bmp writes a Bitmap to a new bmp file.
 *
 * @param bMap the Bitmap to be written to file
 * @param file_name the name of the new bmp
 *
 * @return the size of the new bmp in bytes
 */
unsigned int write_bmp(Bitmap bMap, char *file_name) {
  // Very similar to read_file.
  unsigned int bSize = bMap.bSize;
  FILE *filep = fopen(file_name, "wb");
  if (filep == NULL) {
    printf("Well I'll be darned - there was a problem when writing the file. Goodbye!\n");
    exit(0);
  }
  
  fwrite(bMap.contents, sizeof(char), bMap.bSize, filep);

  fclose(filep);
  return bSize;
}

/**
 * This method takes advantage of the fact that changing the lsb in a
 * bmp does not effect the images appearance to hide messages in the
 * image itself.
 *
 * @param oldFile the original bmp to be modified
 * @param newFile the chosen name of the modified bmp
 * @param messageFile the name of the message that will be encoded
 */
void encode(char *oldFile, char *newFile, char *messageFile) {
  // Reading the message file into a string.
  char *message = read_file(psize, messageFile);
  // Opening the starting bmp into the Bitmap struct.
  Bitmap original = read_bmp(oldFile);

  // Write message length in first 32 chars of bmp
  int bmp_cnt = 0;
  int mSize = size;
  for (bmp_cnt = HEADER_SIZE; bmp_cnt < HEADER_SIZE + 32; bmp_cnt++){
    // Flip LSB based on a bit in message size
    if (mSize & INT_MIN)
      original.contents[bmp_cnt] |= 1;    // Flip LSB to 1
    else
      original.contents[bmp_cnt] &= 254;  // Flip LSB to 0
     mSize <<= 1;
  }
  
  // Encode the message into the subsequent bytes.
  int position = 88;
  int i = 0;
  int j = 0;
  char c;

  // This increments through the positions in the bmp, tests to see if
  // they end in a 1, change that place on the null character (starting
  // at the leftmost bit) and bit-shifting it to the left, then
  // incrementing the position.
  for (i = 0; i < size; i++) {
    c = message[i];
    for (j = 0; j < 8; j++) {
      if (c & CHAR_MIN) {
	original.contents[position] |= 1;
      } else {
	original.contents[position] &= 254;
      }
      c <<= 1;
      position++;
    }
  }

  write_bmp(original, newFile);
  free(original.contents);
  free(message);
}

/**
 * decode parses the modified bmp to output the hidden message in a new
 * file.
 *
 * @param encoded the bmp that contains the hidden message
 * @param newFile the name of the file that the message is printed to
 */
void decode(char *encoded, char *newFile) {
  Bitmap secret = read_bmp(encoded);

  int mSize = 0;
  int bmp_cnt = 0;
  for (bmp_cnt = HEADER_SIZE; bmp_cnt < HEADER_SIZE + 32; bmp_cnt++){
    // preemptively bitshift to the left, then flip the current mSize
    // bit to a 1 if the lsb of the selected byte is 1.
    mSize <<= 1;
    if (secret.contents[bmp_cnt] & 1) {
      mSize |= 1;
    }
  }

  char *message = malloc(mSize * sizeof(char));
  if (message == NULL) {
    printf("Malloc unsuccessful. Goodbye!\n");
    exit(0);
  }

  int i = 0;
  int j = 0;
  int position = 88;
  char c = '\0';
  // Essentially the reverse of the last problem.
  for (i = 0; i < mSize; i++) {
    for (j = 0; j < 8; j++) {
      c <<= 1;
      if (secret.contents[position] & 1) {
	c |= 1;
      }
      position++;
    }
    message[i] = c;
    c = '\0';
  }

  write_file(mSize, message, newFile);
  free(message);
}
