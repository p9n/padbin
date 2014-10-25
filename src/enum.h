#pragma once

#include <iostream>
#include <unordered_map>

#ifdef BOOST_PP_VARIADICS
#undef BOOST_PP_VARIADICS
#endif
#define BOOST_PP_VARIADICS 1

#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

static_assert(BOOST_PP_VARIADICS == 1, "no variadic macro support");

#define ENUM_DEF(name, base, members) \
    enum class name : base { \
        BOOST_PP_SEQ_FOR_EACH(MEMBER_DEF, , members) \
    }; \
    \
    inline std::ostream& operator<<(std::ostream& os, name e) { \
        static const std::unordered_map<base, const char*> m = {BOOST_PP_SEQ_FOR_EACH(STRING_DEF, base, members)}; \
        auto it = m.find(static_cast<base>(e)); \
        return os << (it == m.end() ? "" : it->second); \
    }

#define MEMBER_DEF(s, data, elem) BOOST_PP_TUPLE_ELEM(0, elem) = BOOST_PP_TUPLE_ELEM(1, elem),

#define STRING_DEF(s, data, elem) {static_cast<data>(decltype(e)::BOOST_PP_TUPLE_ELEM(0, elem)), BOOST_PP_TUPLE_ELEM(2, elem)},

ENUM_DEF(
    Element, uint8_t,
    ((FIRE, 0, u8"\x1B[1;31m火\x1B[m"))
    ((WATER, 1, u8"\x1B[1;36m水\x1B[m"))
    ((WOOD, 2, u8"\x1B[1;32m木\x1B[m"))
    ((LIGHT, 3, u8"\x1B[1;33m光\x1B[m"))
    ((DARK, 4, u8"\x1B[1;35m闇\x1B[m"))
    ((NONE, 255, u8"  "))
)

ENUM_DEF(
    Type, uint8_t,
    ((EVO_MATERIAL, 0, u8"進化用"))
    ((BALANCED, 1, u8"バランス"))
    ((PHYSICAL, 2, u8"体力"))
    ((HEALER, 3, u8"回復"))
    ((DRAGON, 4, u8"ドラゴン"))
    ((GOD, 5, u8"神"))
    ((ATTACKER, 6, u8"攻撃"))
    ((DEVIL, 7, u8"悪魔"))
    ((AWOKEN_MATERIAL, 12, u8"覚醒用"))
    ((PROTECTED, 13, u8"特別保護"))
    ((ENHANCE_MATERIAL, 14, u8"強化合成"))
    ((NONE, 255, u8"なし"))
)

ENUM_DEF(
    Awoken, uint16_t,
    ((ENHANCED_HP, 1, u8"HP強化"))
    ((ENHANCED_ATTACK, 2, u8"攻撃強化"))
    ((ENHANCED_HEAL, 3, u8"回復強化"))
    ((REDUCE_FIRE_DAMAGE, 4, u8"火ダメージ軽減"))
    ((REDUCE_WATER_DAMAGE, 5, u8"水ダメージ軽減"))
    ((REDUCE_WOOD_DAMAGE, 6, u8"木ダメージ軽減"))
    ((REDUCE_LIGHT_DAMAGE, 7, u8"光ダメージ軽減"))
    ((REDUCE_DARK_DAMAGE, 8, u8"闇ダメージ軽減"))
    ((AUTO_RECOVER, 9, u8"自動回復"))
    ((RESISTANCE_BIND, 10, u8"バインド耐性"))
    ((RESISTANCE_DARK, 11, u8"暗闇耐性"))
    ((RESISTANCE_JAMMERS, 12, u8"お邪魔耐性"))
    ((RESISTANCE_POISON, 13, u8"毒耐性"))
    ((ENHANCED_FIRE_ORBS, 14, u8"火ドロップ強化"))
    ((ENHANCED_WATER_ORBS, 15, u8"水ドロップ強化"))
    ((ENHANCED_WOOD_ORBS, 16, u8"木ドロップ強化"))
    ((ENHANCED_LIGHT_ORBS, 17, u8"光ドロップ強化"))
    ((ENHANCED_DARK_ORBS, 18, u8"闇ドロップ強化"))
    ((EXTEND_TIME, 19, u8"操作時間延長"))
    ((RECOVER_BIND, 20, u8"バインド回復"))
    ((SKILL_BOOST, 21, u8"スキルブースト"))
    ((ENHANCED_FIRE_ATTRIBUTE, 22, u8"火属性強化"))
    ((ENHANCED_WATER_ATTRIBUTE, 23, u8"水属性強化"))
    ((ENHANCED_WOOD_ATTRIBUTE, 24, u8"木属性強化"))
    ((ENHANCED_LIGHT_ATTRIBUTE, 25, u8"光属性強化"))
    ((ENHANCED_DARK_ATTRIBUTE, 26, u8"闇属性強化"))
    ((TWO_PRONGED_ATTACK, 27, u8"2体攻撃"))
    ((RESISTANCE_SKILL_BIND, 29, u8"封印耐性"))
)

#undef ENUM_DEF
#undef MEMBER_DEF
#undef STRING_DEF

