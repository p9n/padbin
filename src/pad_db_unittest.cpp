#include <array>
#include <string>
#include <gtest/gtest.h>

#include "./pad_db.h"

using namespace std;

class PadDbTest : public testing::Test {
public:
    PadDbTest() : db_("f") {
    }

protected:
    PadDb db_;
};

TEST_F(PadDbTest, MonsterData001) {
    const auto& m = db_.monster(1);

    EXPECT_STREQ(u8"ティラ", m.name);
    
    EXPECT_EQ(Element::FIRE, m.element);
    EXPECT_EQ(Element::NONE, m.sub_element);

    EXPECT_EQ(2, m.rare);
    EXPECT_EQ(1, m.no);
    EXPECT_EQ(2, m.cost);
    EXPECT_EQ(5, m.max_lv);

    EXPECT_EQ(52, m.hp_1);
    EXPECT_EQ(144, m.hp_max);
    EXPECT_EQ(1, m.hp_grow);
    EXPECT_EQ(57, m.atk_1);
    EXPECT_EQ(71, m.atk_max);
    EXPECT_EQ(1, m.atk_grow);
    EXPECT_EQ(8, m.heal_1);
    EXPECT_EQ(13, m.heal_max);
    EXPECT_EQ(1, m.heal_grow);
    
    EXPECT_EQ(1500000, m.exp_type);

    EXPECT_EQ(Type::DRAGON, m.type);
    EXPECT_EQ(Type::NONE, m.sub_type);

    EXPECT_EQ(1, m.skill);  // ヒートブレス
    EXPECT_EQ(51, m.leader_skill);  // 火の力

    EXPECT_FLOAT_EQ(32.933333f, m.plus());
}

TEST_F(PadDbTest, MonsterData004) {
    const auto& m = db_.monster(4);

    EXPECT_STREQ(u8"爆炎龍ティラノス", m.name);
    
    EXPECT_EQ(Element::FIRE, m.element);
    EXPECT_EQ(Element::NONE, m.sub_element);

    EXPECT_EQ(5, m.rare);
    EXPECT_EQ(4, m.no);
    EXPECT_EQ(10, m.cost);
    EXPECT_EQ(99, m.max_lv);

    EXPECT_EQ(777, m.hp_1);
    EXPECT_EQ(2145, m.hp_max);
    EXPECT_EQ(1, m.hp_grow);
    EXPECT_EQ(457, m.atk_1);
    EXPECT_EQ(1114, m.atk_max);
    EXPECT_EQ(1, m.atk_grow);
    EXPECT_EQ(100, m.heal_1);
    EXPECT_EQ(231, m.heal_max);
    EXPECT_EQ(1, m.heal_grow);
    
    EXPECT_EQ(1500000, m.exp_type);

    EXPECT_EQ(Type::DRAGON, m.type);
    EXPECT_EQ(Type::NONE, m.sub_type);

    EXPECT_EQ(11, m.skill);  // マグマブレス
    EXPECT_EQ(56, m.leader_skill);  // 爆炎の力

    EXPECT_EQ(3, m.base_monster);
    EXPECT_EQ(
        (array<uint16_t, 5>({154, 153, 152, 155, 147})),
        m.base_maerial);

    EXPECT_EQ(
        (array<Awoken, 9>({Awoken::ENHANCED_HP, Awoken::ENHANCED_ATTACK, Awoken::SKILL_BOOST})),
        m.awoken);

    EXPECT_FLOAT_EQ(514.3f, m.plus());
}

TEST_F(PadDbTest, SkillData) {
    const auto& heat_breath = db_.skill(1);
    const auto& fire_power = db_.skill(51);
    EXPECT_EQ(u8"ヒートブレス", heat_breath.name);
    EXPECT_EQ(u8"自分の攻撃力×3倍の火属性攻撃", heat_breath.description);
    EXPECT_EQ(0, heat_breath.type);
    EXPECT_EQ(6, heat_breath.max_lv);
    EXPECT_EQ(10, heat_breath.turn);
    EXPECT_EQ(65535, heat_breath.is_active_skill);

    EXPECT_EQ(u8"火の力", fire_power.name);
    EXPECT_EQ(u8"火属性の攻撃力が1.5倍になる", fire_power.description);
    EXPECT_EQ(0, fire_power.is_active_skill);
}

TEST_F(PadDbTest, BoxData) {
    const auto& box = db_.box_data();
    EXPECT_EQ(11, box[0].no);
    EXPECT_EQ(109, box[1].no);
}
