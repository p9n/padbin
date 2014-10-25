#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

#include <boost/locale.hpp>

template<class T>
T Reverse(T& t) {
    char *p = reinterpret_cast<char*>(&t);
    std::reverse(p, p + sizeof(T));
    return t;
}

template<class T>
void ReverseAll(T& t) {
    Reverse(t);
}

template<class T, class... Targs>
void ReverseAll(T& t, Targs& ...args) {
    Reverse(t);
    ReverseAll(args...);
}

template<class T>
void HexDump(const T& t, std::ostream& os) {
    const unsigned char *p = reinterpret_cast<const unsigned char*>(&t);
    for (size_t i = 0; i < sizeof(T); i++) {
        os << std::setw(2) << std::hex << std::setfill('0') << static_cast<unsigned int>(p[i]);
        if (i % 10 == 9) os << std::endl;
        else os << ' ';
    }
    os << std::dec << std::endl;
}

inline size_t string_width(const std::string& s) {
    size_t ret = 0;
    for (auto c : boost::locale::conv::utf_to_utf<char32_t>(s)) {
        ret += (static_cast<int>(c) >= 128) ? 2 : 1;
    }
    return ret;
}
