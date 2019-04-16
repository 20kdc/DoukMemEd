#ifndef DOUKUTSU_H
#define DOUKUTSU_H

#include <stdint.h>

// MACROS
#define BIT(x) 1 << x

namespace Doukutsu {
    // STRUCTS
    #pragma pack(push, 1)
    typedef struct {
        uint32_t id;
        uint32_t level;
        uint32_t energy;
        uint32_t ammoMax;
        uint32_t ammo;
    } Weapon;

    typedef struct {
        uint32_t id;
        uint32_t event;
    } StagePermit;
    #pragma pack(pop)

    // MEMORY OFFSETS
    // from http://cave-story-modding.wikia.com/wiki/Noxid's_Assembly_Compendium
    enum Offsets : uint32_t {
        // Leo40Story please correct these - 20kdc
        // 0xA1, 0x20, 0x8B, 0x49, 0x00
        VerifyExe = static_cast<uint32_t>(0x412429),
        // uint32_t[0x20] according to NAC
        InventoryStart = static_cast<uint32_t>(0x499B40),
        // Weapon[???]
        WeaponsStart = static_cast<uint32_t>(0x499BC8),
        // uint32_t that is an index into Weapons
        CurWeaponSlot = static_cast<uint32_t>(0x499C68),
        // uint16_t bitfield (one bit per equip)
        Equips = static_cast<uint32_t>(0x49E650),
        // int32_t
        PlayerX = static_cast<uint32_t>(0x49E654),
        // int32_t
        PlayerY = static_cast<uint32_t>(0x49E658),
        // uint32_t ??
        InvincTimer = static_cast<uint32_t>(0x49E6C8),
        // uint16_t ???
        HealthCurrent = static_cast<uint32_t>(0x49E6CC),
        // uint16_t ???
        WhimStarNum = static_cast<uint32_t>(0x49E6CE),
        // uint32_t ???
        HealthMaximum = static_cast<uint32_t>(0x49E6D0),
        // uint32_t ???
        HealthDisplayed = static_cast<uint32_t>(0x49E6D4),
        // uint32_t ???
        Oxygen = static_cast<uint32_t>(0x49E6DC),
        // uint32_t
        BoosterFuel = static_cast<uint32_t>(0x49E6E8),
        // ???
        StagePermitsStart = static_cast<uint32_t>(0x4A54FC),
        // uint16_t[2]
        MapSize = static_cast<uint32_t>(0x49E586),
        // uint8_t[w * h]* : This is NOT collision data
        MapPtr = static_cast<uint32_t>(0x49E480),
        // uint8_t[256] : this is NOT a pointer!
        MapPXA = static_cast<uint32_t>(0x49E484)
    };

}
#endif
