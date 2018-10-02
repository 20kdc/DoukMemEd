#pragma once

#include <stdint.h>

// MACROS
#define BIT(x) 1 << x

namespace Doukutsu {
    // STRUCTS
    #pragma pack(push, 1)
    typedef struct {
        uint32_t id;
        uint32_t shotID; // type of bullet fired
        uint32_t level;
        uint32_t energy;
        uint32_t ammoMax;
        uint32_t ammo;
    } Weapon;

    /* NYI
    typedef struct {
        // ???
    } Warp;
    */
    #pragma pack(pop)

    // MEMORY OFFSETS
    // from http://cave-story-modding.wikia.com/wiki/Noxid's_Assembly_Compendium
    enum Offsets : uint32_t {
        WeaponsStart = static_cast<uint32_t>(0x499BC8),
        Equips = static_cast<uint32_t>(0x49E650),
        InvincTimer = static_cast<uint32_t>(0x49E6C8),
        HealthCurrent = static_cast<uint32_t>(0x49E6CC),
        WhimStarNum = static_cast<uint32_t>(0x49E6CE),
        HealthMaximum = static_cast<uint32_t>(0x49E6D0),
        HealthDisplayed = static_cast<uint32_t>(0x49E6D4),
        Oxygen = static_cast<uint32_t>(0x49E6DC),
        BoosterFuel = static_cast<uint32_t>(0x49E6E8),
    };

}