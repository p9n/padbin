#include <cstdint>
#include <cstdlib>

#include "./bin_decode.h"

void BinDecode(std::vector<unsigned char>& v) {
    uint32_t key = v[1];
    for (size_t i = 2; i < v.size(); i++) {
        key = key * 0x000343fdu + 0x00269ec3u;
        v[i] ^= ((key >> 16) & 0xFF);
    }

    uint32_t key2 = reinterpret_cast<uint16_t&>(v[10]);
    for (size_t i = 12; i < v.size(); i++) {
        v[i] ^= key2;
        key2 += 166;
    }
}
