#ifndef WAVQUANT_H
#define WAVQUANT_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <cmath>

class WAVQuant {
private:
    int nBits;
public:
    WAVQuant(const int n) {
        nBits = n;
    }

    void apply_quantitization(std::vector<short> &samples) {
        for (size_t i = 0; i < samples.size(); i++) {
            samples[i] = (samples[i] >> (16 - nBits)) << (16 - nBits);
        }
    }

};

#endif
