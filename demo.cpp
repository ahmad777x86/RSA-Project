#include <iostream>
#include "rsa.h"
#include <string>
using namespace std;

int main()
{
    uint64 p, q, e, n, d;
    bool flag = true;
    while (flag)
    {
        cout << "Enter p q e: ";
        cin >> p >> q >> e;

        try
        {
            generate_keys(p, q, e, n, d);
            flag = false;
        }
        catch (const std::invalid_argument &ex)
        {
            cout << ex.what();
        }
    }

    cout << "Public Key :" << "n = " << n << " e = " << e << endl;
    cout << "Private Key: d = " << d << endl;

    string message;

    cin.ignore(1000, '\n');
    getline(cin, message);

    string encrypted_text = encrypt_text(message, n, e);
    cout << "Encrypted text: " << encrypted_text << endl;

    string decrypted_text = decrypt_text(encrypted_text, n, d);
    cout << "Decrypted text: " << decrypted_text << endl;
}