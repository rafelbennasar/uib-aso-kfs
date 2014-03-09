#include <stdio.h>
/* testing the power algorithm - the first version */
long power(int base, int exponent);
int main(void)
{
    int i;
    for (i=0; i <= 15; ++i)
       printf("2^%2d = %6ld,(-3)^%2d = %10d\n",i,power(2,i),i,power(-3,i));

    return 0;
}

/* the function power is to rise base to the power of exponent, exponent >= 0 */
long power(int base,int exponent)
{
    long int result,pow;
	int i = 0;
    result=1L; pow=base;
    for (i=exponent; i > 0; i=i/2) {
       if (i%2 == 1) result=result*pow;
	       pow=pow*pow;
    }
    return result;
}

