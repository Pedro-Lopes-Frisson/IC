#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;
	std::map<short, size_t> countsMID;
	std::map<short, size_t> countsSIDE;
  std::string filename;

  public:
	WAVHist(const SndfileHandle& sfh ) {
		counts.resize(sfh.channels());
	}

	void update(const std::vector<short> & samples) {
		size_t n { };
		for(auto s : samples){
			counts[n++ % counts.size()][s]++;
    }
	}
  void update_MID(const std::vector<short> & samples){
    // ir buscar 2 de cada vez
		size_t n { };
		size_t n1 { 1 };
    int value ;
    for ( ; n < samples.size() - 1 && n1 < samples.size();
            n = n + 2 , n1 = n1 + 2){
      value = samples[n] + samples[n1] / 2;
			countsMID[value]++;
    }
  }


  void update_SIDE(const std::vector<short> & samples){
    // ir buscar 2 de cada vez
		size_t n { };
		size_t n1 { 1 };
    int value ;
    for ( ; n < samples.size() - 1 && n1 < samples.size();
            n = n + 2 , n1 = n1 + 2){
      value = samples[n] - samples[n1] / 2;
			countsSIDE[value]++;
    }
  }

	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}

	void dump_SIDE( std::ofstream& file) const {
		for(auto [value, counter] : countsSIDE)
			file << value << '\t' << counter << '\n';
	}
	void dump_MID(std::ofstream& file) const {
		for(auto [value, counter] : countsMID)
      file << value << '\t' << counter << '\n';
	}


};

#endif
