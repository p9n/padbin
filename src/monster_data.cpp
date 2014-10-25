#include <algorithm>
#include <iomanip>
#include <iostream>

#include "./monster_data.h"
#include "./bin_decode.h"
#include "./enum.h"
#include "./util.h"

using namespace std;

MonsterData::MonsterData(const unsigned char* raw) {
    copy(raw, raw + sizeof(*this), reinterpret_cast<unsigned char*>(this));
    // reverse multibyte values
    ReverseAll(no, cost, compose_exp, sell_value);
    ReverseAll(hp_1, hp_max, hp_grow);
    ReverseAll(atk_1, atk_max, atk_grow);
    ReverseAll(heal_1, heal_max, heal_grow);
    ReverseAll(exp_type, skill, leader_skill, dungeon_turn);
    ReverseAll(dungeon_hp_1, dungeon_hp_max, dungeon_hp_grow);
    ReverseAll(dungeon_atk_1, dungeon_atk_max, dungeon_atk_grow);
    ReverseAll(dungeon_def_1, dungeon_def_max, dungeon_def_grow);
    ReverseAll(dungeon_exp, base_monster);
    for (auto& x: awoken) ReverseAll(x);
    for (auto& x: base_maerial) ReverseAll(x);
}


float MonsterData::plus() const {
    return hp_max / 10 + atk_max / 5 + heal_max / 3;
}

int MonsterData::count_awoken(Awoken x) const {
    return std::count(std::begin(awoken), std::end(awoken), x);
}

bool MonsterData::has_type(Type t) const {
    return type == t or sub_type == t;
}

ostream& operator<<(ostream& os, const MonsterData& m) {
    os << setw(4) << m.no << ' ';
    os << m.name << setw(33 - string_width(m.name)) << ' ';
    os << m.element << '/' << m.sub_element << ' ';

    ostringstream oss;
    oss << m.type;
    if (m.sub_type != Type::NONE) oss << '/' << m.sub_type;
    os << oss.str() << setw(18 - string_width(oss.str())) << ' ';

    os << setw(4) << static_cast<int>(m.hp_max) << ' ';
    os << setw(4) << static_cast<int>(m.atk_max) << ' ';
    os << setw(4) << static_cast<int>(m.heal_max) << ' ';
    os << setw(7) << fixed << setprecision(3) << m.plus();
    for (auto x : m.awoken) os << ' ' << x;

    return os;
}

