from ctypes import *
import json

class MonsterData(Structure) :
    _pack_ = 1
    _fields_ = [
        ('name', c_char * 97),
        ('element', c_ubyte),
        ('rare', c_ubyte),
        ('no', c_ushort),
        ('cost', c_ushort),
        ('unknown1', c_ubyte * 2),
        ('max_lv', c_ubyte),
        ('compose_exp', c_uint),
        ('unknown2', c_ubyte * 4),
        ('sell_value', c_uint),

        ('hp_1', c_float),
        ('hp_max', c_float),
        ('hp_grow', c_float),
        ('unknown3', c_ubyte * 4),
        ('atk_1', c_float),
        ('atk_max', c_float),
        ('atk_grow', c_float),
        ('unknown4', c_ubyte * 4),
        ('heal_1', c_float),
        ('heal_max', c_float),
        ('heal_grow', c_float),
        ('unknown5', c_ubyte * 4),

        ('exp_type', c_float),
        ('unknown6', c_ubyte * 8),
        ('type', c_ubyte),
        ('sub_type', c_ubyte),

        ('skill', c_ushort),
        ('unknown7', c_ubyte * 4),
        ('leader_skill', c_ushort),

        ('unknown8', c_ubyte * 4),
        ('dungeon_turn', c_ushort),

        ('dungeon_hp_1', c_float),
        ('dungeon_hp_max', c_float),
        ('dungeon_hp_grow', c_float),
        ('unknown9', c_ubyte * 4),
        ('dungeon_atk_1', c_float),
        ('dungeon_atk_max', c_float),
        ('dungeon_atk_grow', c_float),
        ('unknown10', c_ubyte * 4),
        ('dungeon_def_1', c_float),
        ('dungeon_def_max', c_float),
        ('dungeon_def_grow', c_float),
        ('unknown11', c_ubyte * 4),
        
        ('unknown12', c_ubyte * 4),
        ('dungeon_exp', c_ushort),

        ('base_monster', c_ushort),
        ('base_maerial', c_ushort * 5),

        ('unknown13', c_ubyte * 142),

        ('sub_element', c_ubyte),

        ('unknown14', c_ubyte * 11),
        ('awoken', c_ushort * 9),
        ('unknown15', c_ubyte * 6),
    ]

assert sizeof(MonsterData) == 438

class BoxData(Structure) :
    _pack_ = 1
    _fields_ = [
        ('exp', c_uint),
        ('unknown1', c_uint),
        ('counter', c_uint),
        ('unknown2', c_uint),
        ('no', c_ushort),
        ('unknown3', c_ushort),
        ('skill_lv', c_ushort),
        ('unknown4', c_ushort),
        ('flag', c_ushort),
        ('hp_plus', c_ubyte),
        ('atk_plus', c_ubyte),
        ('heal_plus', c_ubyte),
        ('awoken_count', c_ubyte),
    ]

assert sizeof(BoxData) == 30

dll = CDLL("out/libpadbin.so")
dll.PadDb_new.restype = c_void_p
dll.PadDb_monster_data_size.restype = c_size_t
dll.PadDb_monster_data.restype = POINTER(MonsterData)
dll.PadDb_box_data_size.restype = c_size_t
dll.PadDb_box_data.restype = POINTER(BoxData)

db = dll.PadDb_new("f/")
monster_data = dll.PadDb_monster_data(db)
box_data = dll.PadDb_box_data(db)

monsters = []
for i in range(dll.PadDb_monster_data_size(db)) :
    m = monster_data[i]
    obj = {}
    for name, _ in MonsterData._fields_ :
        if name[:7] == "unknown" :
            continue
        elif name == "base_maerial" or name == "awoken" :
            obj[name] = [x for x in m.__getattribute__(name)]
            pass
        else :
            obj[name] = m.__getattribute__(name)
    monsters.append(obj)

boxes = []
for i in range(dll.PadDb_box_data_size(db)) :
    b = box_data[i]
    obj = dict([(name, b.__getattribute__(name)) for name, _ in BoxData._fields_ if name[:7] != "unknown"])
    boxes.append(obj)

dll.PadDb_delete(db)

print json.dumps({"monster": monsters, "box": boxes}, indent = 2)
