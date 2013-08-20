#include <iostream>
#include <cstdlib>
#include <string>
#include "MHashEngine.h"
using namespace std;

int main() {
RCF::MHashEngine mhe;
char act;
do {
cout << "(c)alculate hash, (e)nd: ";
cin >> act;
if(act == 'c') {
cout << "Enter text: ";
string s;
cin >> s;
cout << "MD5 hash of this text is: " << mhe.getHash(s) << endl;
}
} while(act != 'e');
return EXIT_SUCCESS;
}
