//
// Created by whoknows on 10/31/22.
//
#include <getopt.h>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    // Flags
    int neg_flag = 0,
    m_flag = 0,
    r_flag = 0,
    decrease_flag = 0,
    increase_flag = 0,
    write_flag = 0;
    int result;
    char* endptr;
    // values
    int mirror, num_rotations;
    double num_decrease, num_increase;

    string filename;
    for (;;) {
        switch (getopt(argc, argv, "hnm:r:d:i:f:")) {
            case 'h':
                cout << "USAGE: " << argv[0] << "[options] <inFile>" << endl;
                cout << "Options:" << endl;
                cout << "-f FILENAME \t\t Write to an external file the result of the flags aplied" << endl;
                cout << "-h Show this help message" << endl;
                cout << "-m 1 for vertically mirrored 0 for horizontally mirrored" << endl;
                cout << "-n generates a negative image" << endl;
                cout << "-r <integer> rotate by a multiple of 90" << endl;
                cout << "-d <real number> decrease brightness" << endl;
                cout << "-i <real number> increase brightness" << endl;
                return 0;
            case 'f':
                cout << "f";
                write_flag = 1;
                filename = optarg;
            case 'n':
                cout << "n";
                neg_flag = 1;
                break;
            case 'm':

                cout << "m";
                m_flag = 1;
                mirror = atoi(optarg);
                if (mirror != 1 && mirror != 0) {
                    cerr << "Only 2 values are possible 1 or 0" << endl;
                    return -1;
                }
                break;
            case 'r':
                cout << "r";
                r_flag = 1;
                num_rotations = atoi(optarg);
                break;
            case 'd':
                cout << "d";
                decrease_flag = 1;
                num_decrease = strtod(optarg, &endptr);
                /* If the result is 0, test for an error */
                if (num_decrease == 0) {
                    /* If the value provided was out of range, display a warning message */
                    if (errno == ERANGE)
                        printf("The value provided was out of range\n");
                }
                break;

            case 'i':
                cout << "i";
                increase_flag = 1;
                num_increase = strtod(optarg, &endptr);
                /* If the result is 0, test for an error */
                if (num_increase == 0) {
                    /* If the value provided was out of range, display a warning message */
                    if (errno == ERANGE)
                        printf("The value provided was out of range\n");
                }
                break;

            default:
                cout << "Invalid opt ";
                abort();
        }

        break;

    }
    return 0;
}
