#include <algorithm>

#include "./box_data.h"
#include "./util.h"
    
BoxData::BoxData(const uint8_t* raw) {
    std::copy(raw, raw + sizeof(*this), reinterpret_cast<char*>(this));
    ReverseAll(exp, unknown1, counter, unknown2, no, unknown3, skill_lv, unknown4);
    ReverseAll(flag, hp_plus, atk_plus, heal_plus, awoken_count);
}
