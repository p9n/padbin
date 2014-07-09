#include "./bin_decode.h"

void BinDecode(std::vector<std::uint8_t>& v) {
    std::uint32_t key = v[1];
    for (std::size_t i = 2; i < v.size(); i++) {
        key = key * 0x000343fdu + 0x00269ec3u;
        v[i] ^= ((key >> 16) & 0xFF);
    }

    std::uint32_t key2 = *reinterpret_cast<std::uint16_t*>(&v[10]);
    for (std::size_t i = 12; i < v.size(); i++) {
        v[i] ^= key2;
        key2 += 166;
    }
}
