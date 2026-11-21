#include <iostream>
#include "rsa.h"
#include <string>
using namespace std;

int main()
{
    uint64 p,q,e,n,d;
    bool flag = true;
    while(flag)
    {
        cout << "Enter p q e: ";
        cin>> p >> q >> e;

        try{
            generate_keys(p,q,e,n,d);
            flag = false;
        }
        catch(const std::invalid_argument &ex)
        {
            cout << ex.what();
        }
    }

    cout << "Public Key :" << "n = " << n << " e = "<< e <<endl;
    cout << "Private Key: d = " << d << endl;

    cout << "Block Size: 2 letters\n";
    string message;

    getline(cin,message);

    cout << "Message: " << message << endl;
    
    
    cout << "Cleaned: " << message << endl;








}