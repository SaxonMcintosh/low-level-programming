/**
 * This script is an exploration of the IEEE 754 floating point.
 *
 * @author Saxon McIntosh
 * @version March 27, 2020
 */

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>

#define NORM 0
#define DNORM 1
#define SPEC 2
#define BIAS 127


typedef struct {
  int sign;
  int exp;
  float man;
  int mode;
} flt;


/**
 * To be honest I still don't fully understand indirection, but it seems
 * to be a means of indirectly referring to a certain data type through
 * another, in this case a float through an int.
 *
 * @param f the float whose value is extracted
 *
 * @return the bits of a float, cast to an int
 */
int get_flt_bits(float f) {
  return *(int*)&f;
}

/**
 * This gets the sign of the float from the float bits themselves as a
 * char.
 *
 * @param f the float whose value is extracted
 *
 * @return the character for a string representation of the sign
 */
char get_flt_sign_char(float f) {
  char c;
  int n = get_flt_bits(f);

  // bit shifting the float value so that it is a 1 or 0 with 31 leading
  // zeroes
  n >>= 31;  

  if (n & 1) {
    c = '1';
  } else {
    c = '0';
  }

  return c;
}

/**
 * This gets the sign of the float from the float bits themselves and
 * converts it to an integer value.
 *
 * @param f the float whose value is extracted
 *
 * @return the integer value for the sign of the float
 */
int get_flt_sign_val(float f) {
  int n = get_flt_bits(f);
  int i = 0;

  n >>= 31;
  
  if (n & 1) {
    n = -1;
  } else {
    n = 1;
  }
  
  return n;
}

/**
 * This gets the exponent of the float from the float bits themselves
 * and converts it to a binary string.
 *
 * @param f the float whose value is extracted
 *
 * @return the string representation for the exponent of the float
 */
char* get_flt_exp_str(float f) {
  char *bits = malloc(9 * sizeof(char));
  
  int n = get_flt_bits(f);
  int i = 0;

  // bit shifting past the mantissa to get to the exponent
  n >>= 23;
  
  for (i = 0; i < 8; i++) {
    if (n & 1) {
      bits[7 - i] = '1';
    } else {
      bits[7 - i] = '0';
    }
    n >>= 1;
  }
  bits[i] = '\0';
  
  return bits;
}

/**
 * This gets the exponent of the float from the float bits themselves
 * and converts it to an integer.
 *
 * @param f the float whose value is extracted
 *
 * @return the value for the exponent of the float
 */
int get_flt_exp_val(float f) {
  int n = get_flt_bits(f);
  int i = 0, val = 0;
  
  /* I'm performing the operation for the exponent manually and storing
   * it in exp. I don't know why since it's in math. Glutton for
   * punishment, I guess. */
  int exp = 1;

  n >>= 23;
  
  for (i = 0; i < 8; i++) {
    if (n & 1) {
      val += exp;
    }
    exp *= 2;
    n >>= 1;
  }

  // if the bias is not subtracted, the exponent would vastly exceed its
  // intended value
  return val - BIAS;
}

/**
 * This function also processes the exponent bits of the float, but to
 * ascertain what mode should be used to process the mantissa (or if it
 * should even be bother with. Some comparatively tiny values that have
 * an absolute value close to zero must be denormalized, or not led with
 * a one.
 *
 * @param f the float whose value is extracted
 *
 * @return an int representing the mode with which the mantissa is
 * processed
 */
int get_flt_exp_mode(float f) {
  int n = get_flt_bits(f);
  int i = 0, count = 0;

  n >>= 23;

  /* count accumulates the number of ones in the exponent. If there are
  * 8 of them, the value is a special type that cannot be represented.
  * No ones means the value must be denormalized. */
  for (i = 0; i < 8; i++) {
    if (n & 1) {
      count++;
    }
    n >>= 1;
  }
  
  if (count == 0) {
    return DNORM;
  }
  if (count == 8) {
    return SPEC;
  }
  return NORM;
}

/**
 * This gets the exponent of the float from the float bits themselves
 * and converts it to a binary string.
 *
 * @param f the float whose value is extracted
 *
 * @return the string representation for the exponent of the float
 */
char* get_flt_man_str(float f) {
  int n = get_flt_bits(f);
  int i = 0;
  char *bits = malloc(24 * sizeof(char));
  for (i = 0; i < 23; i++) {
    if (n & 1) {
      bits[22 - i] = '1';
    } else {
      bits[22 - i] = '0';
    }
    n >>= 1;
  }
  bits[i] = '\0';
  return bits;
}

/**
 * The project specification for this said that it should return an int,
 * but that would mean that I would need to repeatedly perform
 * indirection for any method that implemented it as a float - which is
 * all of them. Instead it converts the information from the string
 * method for the mantissa. (It was the best way I could find.
 * Realistically I could have used the same method for the other values
 * and it would have been easier, but I wanted to manipulate the bits
 * more to learn about what I was doing.)
 *
 * @param f the float whose value is extracted
 *
 * @return the float value of the mantissa
 */
float get_flt_man_val(float f) {
  // The mode is used to determine if the mantissa's leading one will be
  // excluded (in the case of denormalization).
  int mode = get_flt_exp_mode(f);

  if (f < 0) {
    f *= -1;
  }
  
  // Utilizing 127 and 128 here helps me flip the bits exactly where I
  // want to.
  int one = 127, n = 0, i = 0;
  for (i = 0; i < 8; i++) {
    n <<= 1;
    if (128 & one) {
      n |= 1;
    }
    one <<= 1;
  }
  
  char *str = get_flt_man_str(f);
  for (i = 0; i < 23; i++) {
    n <<= 1;
    if (str[i] == '1') {
      n |= 1;
    }
  }

  free(str);

  // casting the float bits from the int back into a float to return
  float fman = *(float*) &n;

  // subtracting one from the float value of the mantissa in the case
  // of denormalized floats
  if (mode == DNORM) {
    fman--;
  }
  
  return fman;
}

/**
 * This method just cobbles together a string from all the other binary
 * strings.
 *
 * @param f the float whose value is extracted
 *
 * @return the binary representation of the float
 */
char* get_flt_bits_str(float f) {
  char *exp = get_flt_exp_str(f);
  char *man = get_flt_man_str(f);
  char *bits = malloc(35 * sizeof(char));
  int i = 0;  
  
  bits[0] = get_flt_sign_char(f);
  bits[1] = ' ';
  
  for (i = 0; i < 8; i++) {
    bits[i + 2] = exp[i];
  }
  
  bits[10] = ' ';
  for (i = 0; i < 23; i++) {
    bits[i + 11] = man[i];
  }
  bits[35] = '\0';

  free(exp);
  free(man);
  
  return bits;
}

/**
 * This is a helper method that assembles the flt.
 *
 * @param f the float whose value is extracted
 * 
 * @return the completed flt object
 */
flt get_flt_val_flt(float f) {
  int sign = get_flt_sign_val(f);
  int mode = get_flt_exp_mode(f);
  int exp = get_flt_exp_val(f);
  float man = get_flt_man_val(f);

  flt flo = {sign, exp, man, mode};
  
  return flo;
}

/**
 * A helper function that prints the values found in the float.
 *
 * @param f the flt object to be printed
 */
void print_flt(flt f) {
  printf("\tFloat Values\nSign: %d\nExponent: %d\nMantissa: %f\nMode: ", f.sign, f.exp, f.man);
  switch (f.mode) {
  case 0:
    printf("NORM\n");
    break;
  case 1:
    printf("DNORM\n");
    break;
  default:
    printf("SPEC\n");
  }
  printf("\n");
}

/**
 * This is the culmination of the other methods - it produces the value
 * of the float itself, accounting for edge cases.
 *
 * @param f the float object whose value is extracted
 */
float get_flt_bits_val(flt f) {
  if (f.mode == SPEC) {
    // The mantissa is 1 in the advent that it has a binary value of 0,
    // in which case the value is infinity when the mode is SPEC.
    if (f.man == 1) {
      return INFINITY * f.sign;
    } else {
      // The other option for SPEC is nan.
      return NAN;
    }
  }
  
  float expo = 1.0;
  int i = 0;
  if (f.exp > 0) {
    for (i = 0; i < f.exp; i++) {
      expo *= 2;
    }
  }
  if (f.exp < 0) {
    for (i = 0; i > f.exp; i--) {
      expo /= 2;
    }
  }

  return f.sign * f.man * expo;
}

/* As far as the bit representations and float part value methods are
 * concerned, the only methods that need to be run here are
 * get_flt_bits_str() and print_flt(), as they call all the other
 * methods */
int main(){
  float f = -15.375;
  printf("Float input value: %f\n\n", f);
  
  char *bits = get_flt_bits_str(f);
  printf("Float binary: %s\n\n", bits);
  free(bits);

  flt flobject = get_flt_val_flt(f);
  print_flt(flobject);

  printf("Interpreted float value: %f\n", get_flt_bits_val(flobject));

  return 0;
}
