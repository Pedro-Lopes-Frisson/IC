#include <fstream>
#include <bitset>
#include "bitStream.h"

using namespace std;
char stored;
bitset<8> b;

int main() {
    BitStream b {"binfile"};
    auto bit = b.read_bit();
    char array[8];
    b.read_Nbit(array, 8);
    for ( int i = 0; i < 8; i++){
        cout << array[i] << endl;
    }

    b.close_files();
}

