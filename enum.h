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
    std::ostream& operator<<(std::ostream& os, name e) { \
        static const std::unordered_map<base, const char*> m = {BOOST_PP_SEQ_FOR_EACH(STRING_DEF, base, members)}; \
        auto it = m.find((base)e); \
        return os << (it == m.end() ? "" : it->second); \
    }

#define MEMBER_DEF(s, data, elem) BOOST_PP_TUPLE_ELEM(0, elem) = BOOST_PP_TUPLE_ELEM(1, elem),

#define STRING_DEF(s, data, elem) {(data)decltype(e)::BOOST_PP_TUPLE_ELEM(0, elem), BOOST_PP_TUPLE_ELEM(2, elem)},

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
    ((EVOLUTION, 0, u8"進化用"))
    ((BALANCE, 1, u8"バランス"))
    ((VITALITY, 2, u8"体力"))
    ((HEAL, 3, u8"回復"))
    ((DRAGON, 4, u8"ドラゴン"))
    ((GOD, 5, u8"神"))
    ((ATTACK, 6, u8"攻撃"))
    ((DEMON, 7, u8"悪魔"))
    ((AWAKEN, 12, u8"覚醒用"))
    ((PROTECT, 13, u8"特別保護"))
    ((STRONG, 14, u8"強化合成"))
    ((NONE, 255, u8"なし"))
)

#undef ENUM_DEF
#undef MEMBER_DEF
#undef STRING_DEF

