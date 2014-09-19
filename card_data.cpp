#include <algorithm>
#include <cassert>
#include <fstream>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <type_traits>
#include <vector>

#include "./bin_decode.h"
#include "./enum.h"

#include <boost/locale.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/filtered.hpp>

using namespace std;

template<class T>
T Reverse(T& t) {
    char *p = reinterpret_cast<char*>(&t);
    reverse(p, p + sizeof(T));
    return t;
}

void ReverseAll() {
}

template<class T, class... Targs>
void ReverseAll(T& t, Targs& ...args) {
    Reverse(t);
    ReverseAll(args...);
}

#pragma pack(1)
struct MonsterData {
    explicit MonsterData(const uint8_t* raw) {
        copy(raw, raw + sizeof(MonsterData), reinterpret_cast<char*>(this));
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

    MonsterData() = default;

    char name[97];
    Element element;
    uint8_t rare;
    uint16_t no;
    uint16_t cost;
    uint8_t unknown1[2];
    uint8_t max_lv;
    uint32_t compose_exp;  // Lv4合成exp(?)
    uint8_t unknown2[4];
    uint32_t sell_value;  // Lv10賣價(?)

    float hp_1;
    float hp_max;
    float hp_grow;
    uint8_t unknown3[4];
    float atk_1;
    float atk_max;
    float atk_grow;
    uint8_t unknown4[4];
    float heal_1;
    float heal_max;
    float heal_grow;
    uint8_t unknown5[4];

    float exp_type;
    uint8_t unknown6[8];
    Type type;
    Type sub_type;

    uint16_t skill;
    uint8_t unknown7[4];
    uint16_t leader_skill;

    uint8_t unknown8[4];
    uint16_t dungeon_turn;

    float dungeon_hp_1;
    float dungeon_hp_max;
    float dungeon_hp_grow;
    uint8_t unknown9[4];
    float dungeon_atk_1;
    float dungeon_atk_max;
    float dungeon_atk_grow;
    uint8_t unknown10[4];
    float dungeon_def_1;
    float dungeon_def_max;
    float dungeon_def_grow;
    uint8_t unknown11[4];
    
    uint8_t unknown12[4];
    uint16_t dungeon_exp;  // Lv2關卡exp (maybe 4 byte?)

    uint16_t base_monster;  // 進化前
    uint16_t base_maerial[5];  // 進化素材

    uint8_t unknown13[142];  // lots of zeros?

    Element sub_element;

    uint8_t unknown14[11];  // 究極退化?
    uint16_t kakusei[9];
    uint8_t unknown15[6];

    float plus() const {
        return hp_max / 10 + atk_max / 5 + heal_max / 3;
    }

    int count_kakusei(uint16_t x) const {
        return std::count(std::begin(kakusei), std::end(kakusei), x);
    }
};


static_assert(is_pod<MonsterData>::value, "MonsterData is not pod");
static_assert(sizeof(MonsterData) == 438, "Incorrect MonsterData struct size");

template<class T>
void HexDump(const T& t, ostream& os) {
    const char *p = reinterpret_cast<const char*>(&t);
    for (size_t i = 0; i < sizeof(T); i++) {
        os << setw(2) << hex << setfill('0') << (int)(unsigned char)p[i];
        if (i % 10 == 9) os << endl;
        else os << ' ';
    }
    os << dec << endl;
}

size_t string_width(const string& s) {
    size_t ret = 0;
    for (auto c : boost::locale::conv::utf_to_utf<char32_t>(s)) {
        ret += ((int)c >= 128) ? 2 : 1;
    }
    return ret;
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

int main() {
    ifstream fin("f/data021.bin");
    fin >> noskipws;
    vector<uint8_t> v{istream_iterator<char>(fin), istream_iterator<char>()};
   
    BinDecode(v);

    uint32_t monster_count = Reverse(*reinterpret_cast<uint32_t*>(&v[24]));
    cout << "monster count: " << monster_count << endl;

    assert(v.size() == 32 + monster_count * sizeof(MonsterData));

    vector<MonsterData> m;
    for (size_t i = 0; i < monster_count; i++) {
        m.emplace_back(v.data() + 32 + i * sizeof(MonsterData));
    }

    auto key = [](const MonsterData& x) {
        return x.plus();
    };
    sort(m.begin(), m.end(), [key](const MonsterData& x, const MonsterData& y){ return key(x) < key(y); });

    auto pred = [] (const MonsterData& m) {
        return m.element == Element::WOOD and (m.type == Type::DEMON or m.sub_type == Type::DEMON);
    };

    boost::copy(
        m | boost::adaptors::filtered(pred),
        ostream_iterator<MonsterData>(cout, "\n")
    );
}
