#include <fstream>
#include <bitset>
#include "bitStream.h"

using namespace std;
char stored;
bitset<8> b;

int main() {
    BitStream b {"binfile"};
    BitStream be {"binfileEncoded"};
    char array[8];
    b.read_Nbit(array, 8);
    for ( int i = 0; i < 8; i++){
        cout << array[i] << endl;
    }

    be.write_Nbit(array, 8);

    b.close_files();
}

