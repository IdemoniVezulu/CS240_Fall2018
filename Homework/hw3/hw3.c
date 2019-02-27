/*
 * Name, hw3.c, CS 24000, Fall 2018
 * Last updated September 13, 2018
 */

/*
* Include files are below
*/

#include "hw3.h"
#include <math.h>

/*
* Function definitions are below
*/

/*
* This function takes in two complex numbers and
* returns the sum of the two complex numbers
*/

complex complex_add(complex num_one, complex num_two) {
  complex add_num;

  /* The two arguments have their components added seperately for the result */

  add_num.x = num_one.x + num_two.x;
  add_num.y = num_one.y + num_two.y;

  return add_num;
}/*complex_add()*/

/*
* This function takes in two complex numbers and
* returns the difference of the two complex numbers
*/

complex complex_sub(complex num_one, complex num_two) {
  complex sub_num;

  /* Function Prototype for complex_neg() */

  complex complex_neg(complex);

  /* Subtract the two arguments by adding one with the negative of the other*/

  sub_num = complex_add(num_one, complex_neg(num_two));

  return sub_num;
}/*complex_sub()*/

/*
* This function takes in two complex numbers and
* returns the product of the two complex numbers
*/

complex complex_mul(complex num_one, complex num_two) {
  complex prod_num;

  prod_num.x = (num_one.x * num_two.x) - (num_one.y * num_two.y);
  prod_num.y = (num_one.x * num_two.y) + (num_one.y * num_two.x);

  return prod_num;
}/*complex_mul()*/

/*
* This function takes in two complex numbers and
* returns the quotient from the division of the two complex numbers.
*/

complex complex_div(complex num_one, complex num_two) {
  complex div_num;

  /* Function prototype for complex_inv() */

  complex complex_inv(complex);

  /* Division of complex numbers is calculated as the product of one complex
     number with the inverse of the other
  */

  div_num = complex_mul(num_one, complex_inv(num_two));

  return div_num;
}/*complex_div()*/

/*
* This function takes in one complex number and
* returns the negative version of the complex number
*/

complex complex_neg(complex num) {
  complex neg_num;

  neg_num.x = num.x * -1;
  neg_num.y = num.y * -1;

  return neg_num;
}/*complex_neg()*/

/*
* This function takes in a complex number and
* it returns the reciprocal of the complex number.
*
* The reciprocal is determined by dividing each of
* its components by the square of the magnitude of
* the complex number and negating the imaginary part.
*/

complex complex_inv(complex num) {
  complex inv_num;

  inv_num.x = num.x / (pow(num.x, 2) + pow(num.y, 2));
  inv_num.y = -1 * (num.y / (pow(num.x, 2) + pow(num.y, 2)));

  return inv_num;
}/*complex_inv()*/

/*
* This function takes in a complex number and
* returns the exponential function result of the complex number;
*/

complex complex_exp(complex num) {
  complex exp_num;

  exp_num.x = exp(num.x) * cos(num.y);
  exp_num.y = exp(num.x) * sin(num.y);

  return exp_num;
}/*complex_exp()*/

/*
* This function takes in two complex numbers and
* returns the dot product of the two complex numbers.
*
* The dot product is the sum of the product of like terms
* in the complex numbers.
*/

double complex_dot(complex num_one, complex num_two) {
  double dot_res;

  dot_res = ((num_one.x * num_two.x) + (num_one.y * num_two.y));

  return dot_res;
}/*complex_dot()*/


/*
* This function takes in one complex number as an argument and
* returns the number of calculations is takes to make the magnitude
* of a 0 + 0i > 2.
*/

int mandelbrot(complex num) {
  complex temp = { 0, 0 };

  /* count keeps track of the number of calculations required to make the
     magnitude of temp > 2
  */

  int count = 0;

  /* The calculation happens only for MAX_MANDERBLOT times */

  while (count < MAX_MANDELBROT) {

    /* The calculation stops if the magnitude of temp > 2 */

    if (complex_dot(temp, temp) > 4) {
      break;
    }
    else {

      /* If the magnitude is less than 2, the calculation happens */

      temp = complex_add(complex_mul(temp, temp), num);
      count++;
    }
  }

  return count;
}/*mandelbrot()*/
