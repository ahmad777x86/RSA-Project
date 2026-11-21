#include <iostream>
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
        if(b%2 == 1)
		    result = (result + a) % m;
		a = (a + a) % m;
		b = b / 2;
	}
	return result;
}
uint64 mod_exp(uint64 b, uint64 n, uint64 m) // B1
{
	//1. Set x = 1 % m(so that m = 1 gives 0) and power = b % m.
	uint64 x = 1 % m;
	uint64 power = b % m;
		//2. While n > 0:
	while (n > 0)
	{
		//a.If n is odd(its last bit is 1), set x = mul_mod(x, power, m).
		if (n % 2 == 1) x = mul_mod(x, power, m);
		
		//b.Set power = mul_mod(power, power, m).
		power = mul_mod(power, power, m);
		//c.Set n = n / 2.
		n /= 2;
	}
		//3. Return x.
	return x;
}

uint64 gcd(uint64 a, uint64 b) // B2
{
	while(b!=0)
	{
		uint64 temp = a;
		a = b;
		b = temp % b;
	}
	return a;

}
long long extended_gcd(long long a, long long b,
	long long& s, long long& t) // B3
{
	stack<long long > quotients;
	long long r0 = a , r1 = b;
	while(r1 != 0)
	{
		quotients.push(r0/r1);

		long long temp = r0;
		r0 = r1;
		r1 = temp % r1;
	}
	long long g = r0;
	long long x =1, y =0 ;

	while(!quotients.empty())
	{
		
		long long temp = x;
		x = y;
		y = temp - quotients.top() * y;
		quotients.pop();
	}
	
	s = x; t = y;

	return g;
	
}
uint64 mod_inverse(uint64 a, uint64 m) // B4
{
	long long s, t;
	if(extended_gcd(a, m, s, t) != 1) throw invalid_argument("No inverse");

	long long r = s % (long long)m;
	if (r < 0) r += m;

	return r;
}
bool is_prime(uint64 n) // B5
{
	// check for n>=0
	if (n < 2) return false;
	for (uint64 d = 2; d <= n / d; d++)
	{
		if (n % d == 0) return false;
	}
	return true;
}
void generate_keys(uint64 p, uint64 q, uint64 e,
	uint64& n, uint64& d) // B6
{
	// Note: You may assume p, q < 231, so p·q fits in 64 bits. impliment this shih

// 	1. If p or q is not prime, throw std::invalid_argument.
	if(!is_prime(p) || !is_prime(q)) throw invalid_argument("p and q must be prime\n");
// 2. If p == q, throw std::invalid_argument.
	if(p == q) throw invalid_argument("p and q must not be equal\n");
// 3. Let φ = (p − 1)(q − 1). If e ≤ 1, e ≥ φ, or gcd(e, φ) ≠ 1, throw std::invalid_argument.
	uint64 φ = (p - 1)*(q - 1);
	if(e <= 1 || e>= φ || gcd(e,φ) != 1) throw invalid_argument("e or gcd are not in range\n");
// 4. Set n = p·q and d = mod_inverse(e, φ).
	n = p * q;
	d = mod_inverse(e, φ);
}
uint64 encrypt(uint64 M, uint64 n, uint64 e) // B7
{
	if(M >= n) throw invalid_argument("M exceeded n\n");
	// encrypt returns M^e mod n 
	return mod_exp(M,e,n);
}
uint64 decrypt(uint64 C, uint64 n, uint64 d) // B7
{
	if(C >= n) throw invalid_argument("C exceeded n\n");

	// decrypt returns C^d mod n,
	return mod_exp(C,d,n);
}
int block_size(uint64 n) // B8
{
	uint64 z = 25;
	uint64 k = 0;
	for (int i = 1; z < n ; i++)
	{
		/* code */
		k++;
		z = z*100+25;
	}
	if (k > 9) throw invalid_argument("K exceeds 9\n");
	return k;
}

std::string encrypt_text(const std::string& text, uint64 n, uint64 e) // B8
{

	uint64 k = block_size(n);

	string cleaned = "";
	for (int i = 0; text[i] != '\0'; i++)
	{
		/* code */
		if(isalpha(text[i]))
		{
			cleaned += toupper(text[i]);
		}
	}

	cout << "Cleaned: " << cleaned << endl;

	while(text.size() % k != 0)
	{
		text + 'x';
	}

	





}
std::string decrypt_text(const std::string& blocks, uint64 n, uint64 d) // B8
{
	
}