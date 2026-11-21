#include "rsa.h"
using namespace std;

// add exceptions to all
uint64 mul_mod(uint64 a, uint64 b, uint64 m) // B0
{
	// do the moduls defect coorection here
	a = a % m;
	b = b % m;
	uint64 result = 0;
	while (b > 0)
	{
		if (b % 2 == 1)
			result = (result + a) % m;
		a = (a + a) % m;
		b = b / 2;
	}
	return result;
}
uint64 mod_exp(uint64 b, uint64 n, uint64 m) // B1
{
	// 1. Set x = 1 % m(so that m = 1 gives 0) and power = b % m.
	uint64 x = 1 % m;
	uint64 power = b % m;
	// 2. While n > 0:
	while (n > 0)
	{
		// a.If n is odd(its last bit is 1), set x = mul_mod(x, power, m).
		if (n % 2 == 1)
			x = mul_mod(x, power, m);

		// b.Set power = mul_mod(power, power, m).
		power = mul_mod(power, power, m);
		// c.Set n = n / 2.
		n /= 2;
	}
	// 3. Return x.
	return x;
}

uint64 gcd(uint64 a, uint64 b) // B2
{
	if (b == 0)
		return a;
	return gcd(b, a % b);
}
long long extended_gcd(long long a, long long b,
					   long long &s, long long &t); // B3
uint64 mod_inverse(uint64 a, uint64 m);				// B4
bool is_prime(uint64 n)								// B5
{
	// check for n>=0
	if (n < 2)
		return false;
	for (uint64 d = 2; d <= n / d; d++)
	{
		if (n % d == 0)
			return false;
	}
	return true;
}
void generate_keys(uint64 p, uint64 q, uint64 e,
				   uint64 &n, uint64 &d);	  // B6
uint64 encrypt(uint64 M, uint64 n, uint64 e); // B7
uint64 decrypt(uint64 C, uint64 n, uint64 d); // B7
int block_size(uint64 n);					  // B8

std::string encrypt_text(const std::string &text, uint64 n, uint64 e);	 // B8
std::string decrypt_text(const std::string &blocks, uint64 n, uint64 d); // B8