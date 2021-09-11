// for testing snippets of code
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "defs.h"

using namespace std;

void hex_to_b(U64 hex_num) {
    for (int i = 0; i < 64; ++i) {
        if (i % 8 == 0) {
            cout << "\n";
        }
        cout << (hex_num & 1);
        hex_num = hex_num >> 1;
    }
    cout << "\n";
}

int main(){
    U64 mask = 0;
    hex_to_b(mask);
    
    // for (int i = 0; i < 4; ++i) {
    //     cout << (x >> i) << "\n";
    // }
    // for (int i = 0; i < 16; ++i) {
    //     cout << (y >> i) << "\n";
    // }
}