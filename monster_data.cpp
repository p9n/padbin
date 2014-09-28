#include <algorithm>
#include <iomanip>
#include <iostream>

#include "./monster_data.h"
#include "./bin_decode.h"
#include "./enum.h"
#include "./util.h"

using namespace std;

MonsterData::MonsterData(const uint8_t* raw) {
    copy(raw, raw + sizeof(*this), reinterpret_cast<char*>(this));
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
    for (auto& x: kakusei) ReverseAll(x);
    for (auto& x: base_maerial) ReverseAll(x);
}


float MonsterData::plus() const {
    return hp_max / 10 + atk_max / 5 + heal_max / 3;
}

int MonsterData::count_kakusei(uint16_t x) const {
    return std::count(std::begin(kakusei), std::end(kakusei), x);
}


ostream& operator<<(ostream& os, const MonsterData& m) {
    const static unordered_map<uint16_t, const char*> kakusei_name = {
        {1, u8"HP強化"},
        {2, u8"攻撃強化"},
        {3, u8"回復強化"},
        {4, u8"火ダメージ軽減"},
        {5, u8"水ダメージ軽減"},
        {6, u8"木ダメージ軽減"},
        {7, u8"光ダメージ軽減"},
        {8, u8"闇ダメージ軽減"},
        {9, u8"自動回復"},
        {10, u8"バインド耐性"},
        {11, u8"暗闇耐性"},
        {12, u8"お邪魔耐性"},
        {13, u8"毒耐性"},
        {14, u8"火ドロップ強化"},
        {15, u8"水ドロップ強化"},
        {16, u8"木ドロップ強化"},
        {17, u8"光ドロップ強化"},
        {18, u8"闇ドロップ強化"},
        {19, u8"操作時間延長"},
        {20, u8"バインド回復"},
        {21, u8"スキルブースト"},
        {22, u8"火属性強化"},
        {23, u8"水属性強化"},
        {24, u8"木属性強化"},
        {25, u8"光属性強化"},
        {26, u8"闇属性強化"},
        {27, u8"2体攻撃"},
        {29, u8"封印耐性"},
    };

    os << setw(4) << m.no << ' ';
    os << m.name << setw(33 - string_width(m.name)) << ' ';
    os << m.element << '/' << m.sub_element << ' ';

    ostringstream oss;
    oss << m.type;
    if (m.sub_type != Type::NONE) oss << '/' << m.sub_type;
    os << oss.str() << setw(18 - string_width(oss.str())) << ' ';

    os << setw(4) << (int)m.hp_max << ' ';
    os << setw(4) << (int)m.atk_max << ' ';
    os << setw(4) << (int)m.heal_max << ' ';
    os << setw(7) << fixed << setprecision(3) << m.plus();
    for (auto x : m.kakusei) {
        auto it = kakusei_name.find(x);
        if (it != kakusei_name.end()) os << ' ' << it->second;
    }

    return os;
}

