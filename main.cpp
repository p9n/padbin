#include <algorithm>
#include <cassert>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <type_traits>

#include "./monster_data.h"
#include "./bin_decode.h"
#include "./pad_db.h"

#include <boost/range/adaptor/filtered.hpp>

using namespace std;

int main() {

    PadDb db;
    db.LoadMonsterData("f/data021.bin");

    auto& monster_data = db.monster_data();
    using IterType = vector<MonsterData>::const_iterator;
    vector<IterType> m(monster_data.size());
    for (size_t i = 0; i < monster_data.size(); i++) m[i] = monster_data.begin() + i;

    auto key = [](const IterType& x) {
        return x->plus();
    };
    sort(m.begin(), m.end(), [key](const IterType& x, const IterType& y){ return key(x) < key(y); });

    auto pred = [] (const IterType& it) {
        auto& m = *it;
        return m.element == Element::WOOD and (m.type == Type::DEMON or m.sub_type == Type::DEMON);
    };

    for (auto& x : m | boost::adaptors::filtered(pred)) cout << *x << endl;
}
