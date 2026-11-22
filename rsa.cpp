#include <iostream>
#include <vector>
#include "rsa.h"
using namespace std;

uint64 mul_mod(uint64 a, uint64 b, uint64 m) // B0
{
	if (m == 0)
		throw invalid_argument("modulus must not be zero");
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
	if (m == 0)
		throw invalid_argument("modulus must not be zero");

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
					   long long &s, long long &t) // B3
{
	stack<long long> quotients;
	long long r0 = a, r1 = b;
	// 	Step 1. Divide forward, run the Euclidean algorithm
	while (r1 != 0)
	{
		quotients.push(r0 / r1);

		long long temp = r0;
		r0 = r1;
		r1 = temp % r1;
	}
	long long g = r0;
	long long x = 1, y = 0;

	// Step 2. Substitute backward, replace (x, y) by (y, x − q·y) for each quotient
	while (!quotients.empty())
	{

		long long temp = x;
		x = y;
		y = temp - quotients.top() * y;
		quotients.pop();
	}

	s = x;
	t = y;

	return g;
}
uint64 mod_inverse(uint64 a, uint64 m) // B4
{
	if (m == 0)
		throw invalid_argument("modulus must not be zero");

	long long s, t;
	if (extended_gcd(a, m, s, t) != 1)
		throw invalid_argument("No inverse");

	long long r = s % (long long)m;
	if (r < 0)
		r += m;

	return r;
}
bool is_prime(uint64 n) // B5
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
				   uint64 &n, uint64 &d) // B6
{
	// 	1. If p or q is not prime, throw std::invalid_argument.
	if (!is_prime(p) || !is_prime(q))
		throw invalid_argument("p and q must be prime\n");
	// 2. If p == q, throw std::invalid_argument.
	if (p == q)
		throw invalid_argument("p and q must not be equal\n");
	// 3. Let φ = (p − 1)(q − 1). If e ≤ 1, e ≥ φ, or gcd(e, φ) ≠ 1, throw std::invalid_argument.
	uint64 phi = (p - 1) * (q - 1);
	if (e <= 1 || e >= phi || gcd(e, phi) != 1)
		throw invalid_argument("e or gcd are not in range\n");
	// 4. Set n = p·q and d = mod_inverse(e, φ).
	uint64 new_n = p * q;
	uint64 new_d = mod_inverse(e, phi);
	n = new_n;
	d = new_d;
}
uint64 encrypt(uint64 M, uint64 n, uint64 e) // B7
{
	if (M >= n)
		throw invalid_argument("M exceeded n\n");
	// encrypt returns M^e mod n
	return mod_exp(M, e, n);
}
uint64 decrypt(uint64 C, uint64 n, uint64 d) // B7
{
	if (C >= n)
		throw invalid_argument("C exceeded n\n");

	// decrypt returns C^d mod n,
	return mod_exp(C, d, n);
}
int block_size(uint64 n) // B8
{
	uint64 z = 25;
	uint64 k = 0;
	for (int i = 1; z < n; i++)
	{
		k++;
		z = z * 100 + 25;
	}
	if (k > 9)
		throw invalid_argument("K exceeds 9\n");
	if (k == 0)
		throw invalid_argument("n is too small to hold even one letter");
	return k;
}

std::string encrypt_text(const std::string &text, uint64 n, uint64 e) // B8
{

	uint64 k = block_size(n);
	cout << "Block size: " << k << endl;
	cout << "Message: " << text << endl;
	string temp = "";

	// cleaning
	for (int i = 0; text[i] != '\0'; i++)
	{
		unsigned char ch = text[i];
		if (std::isalpha(ch))
		{
			temp += toupper(ch);
		}
	}

	cout << "Cleaned: " << temp << endl;

	// padding for multiple of k
	while (temp.size() % k != 0)
	{
		temp += 'X';
	}

	string C;
	string C_block;
	uint64 M = 0;

	// sliding window
	for (int i = 0; i < temp.size(); i += k)
	{
		for (int j = 0; j < k; j++)
		{
			M = M * 100 + (temp[i + j] - 'A');
		}
		C_block += to_string(encrypt(M, n, e));
		while (C_block.size() < 2 * k)
		{
			C_block = '0' + C_block;
		}
		C += C_block;
		C_block = "";
		C += ' ';
		M = 0;
	}
	// empty string case
	if (!C.empty())
		C.pop_back();
	return C;
}
std::string decrypt_text(const std::string &blocks, uint64 n, uint64 d) // B8
{
	char c;
	uint64 num = 0;
	string M = "";
	uint64 message_num;
	int k = block_size(n);
	string Blocks = blocks;
	Blocks += ' ';

	// empty string case
	if (blocks.empty())
		return M;

	for (int i = 0; i < Blocks.size(); i++)
	{
		c = Blocks[i];
		if (c == ' ')
		{
			message_num = decrypt(num, n, d);
			stack<int> st;

			for (int j = 0; j < k; j++)
			{
				st.push(message_num % 100);
				message_num = message_num / 100;
			}
			// turning number to ascii, then converting to the corresponding character
			while (!st.empty())
			{
				int n;
				n = st.top();
				n = n + 'A';
				M += static_cast<char>(n);
				st.pop();
			}
			num = 0;
		}
		else
		{
			num = num * 10 + (c - '0');
		}
	}
	return M;
}
