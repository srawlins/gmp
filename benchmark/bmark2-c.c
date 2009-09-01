#include <gmp.h>

int main()
{
    mpz_t a,b;
    int i;
    
    mpz_init_set_ui (a, 1234);
    mpz_init_set_ui (b, 5678);

    for (i=0; i<100000; i++)
    {
	mpz_add(a,a,b);
	mpz_add(b,b,a);    
    }

    return 0;
}
