# padbin
======

some notes about pad binary data

-----------------------------------------------

## data021
```
32 byte header
    byte 24 ~ 27: monster data count (X)
438X monster data
```

-----------------------------------------------

## data036
```
32 byte header
    byte 24 ~ 27: skill raw data count (X)
48X byte skill raw data
4 byte unknown
4 byte string table size (Y)
Y byte string table
```

-----------------------------------------------

## data056

```
32 byte header

0x116~0x117 box size
0x118~0x119 monster count
30 byte per monster

uint32 exp
uint32 unknown
uint32 流水號?
uint32 unknown (zeros?)
uint16 monster id
uint16 unknown
uint16 skill_lv
uint16 unknown
uint16 flag (8 = 気に入り, 1 = new)
uint8  hp_plus
uint8  atk_plus
uint8  heal_plus
uint8  覚醒数
```

-----------------------------------------------

## backup command

```
adb backup -nosystem jp.gungho.pad -noapk -noshared -f pad.ab
```
