#include <iostream>
#include "rsa.h"

using namespace std;

int main()
{
    uint64 n = 1000036000099;
    uint64 e = 65537;
    string ciphertext = "0614157364221 0502078796304 0031583460315";

    uint64 p = 2;

    while (p * p <= n && n % p != 0)
    {
        p++;
    }
    uint64 q = n / p;

    cout << "P: " << p << " | " << "Q: " << q << endl;

    uint64 phi = (p - 1) * (q - 1);
    uint64 d = mod_inverse(e, phi);

    cout << "Decrypted text: " << decrypt_text(ciphertext, n, d) << endl;
    return 0;
}