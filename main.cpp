#include <algorithm>
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
    PadDb db("f/");
    
    cout << "monster count: " << db.monster_data().size() << endl;
    cout << "skill count: " << db.skill_data().size() << endl;
    cout << "box_count: " << db.box_data().size() << endl;

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
        return m.element == Element::WOOD and (m.type == Type::DEVIL or m.sub_type == Type::DEVIL);
    };

    for (auto& x : m | boost::adaptors::filtered(pred)) {
        cout << *x << endl;
        // cout << db.skill(x->skill).name << endl;
        // cout << db.skill(x->leader_skill).name << endl;
    }
}
