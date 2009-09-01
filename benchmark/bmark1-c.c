#include <gmp.h>

int main()
{
    mpz_t a,b;
    int i;
    
    mpz_init_set_ui (a, 1000);
    mpz_init_set_ui (b, 1000);

    for (i=0; i<12; i++)
    {
	mpz_mul(a,a,b);
	mpz_mul(b,b,a);    
    }

    return 0;
}
