#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>

#include "./bin_decode.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: ./decode <filename>\n";
        return 1;
    }
    
    ifstream fin(argv[1]);
    ofstream fout((string(argv[1]) + ".decode").c_str());
    if (not fin or not fout) {
        cout << "Cannot open file\n";
        return 1;
    }
    fin >> noskipws;
    vector<unsigned char> v{istream_iterator<unsigned char>(fin), istream_iterator<unsigned char>()};
    BinDecode(v);
    fout.write(reinterpret_cast<char*>(v.data()), v.size());
}
