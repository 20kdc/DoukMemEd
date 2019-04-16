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

    typedef struct {
        int32_t collision;
        uint32_t shotID;
        int32_t flags;
        int32_t inuse;
        int32_t x, y;
        int32_t movex, movey;
        int32_t unk0, unk1;
        int32_t wasSetup;
        int32_t unk2, unk3;
        int32_t frameID;
        int32_t direction;
        int32_t displayL, displayU, displayR, displayD;
        int32_t distance;
        int32_t unk4;
        int32_t maxdistance;
        int32_t damage;
        int32_t numimpacts;
        int32_t hitrectL, hitrectU, hitrectR, hitrectD;
        int32_t display_nofsX, display_nofsY, unk7, unk8;
    } Bullet;

    typedef struct {
        int32_t inuse;
        int32_t collision;
        int32_t X,Y;
        int32_t movex,movey;
        int32_t altvelx,altvely;
        int32_t CurlyMacro1,CurlyMacro2;
        int32_t npcType,entityID;
        int32_t EventNum;
        int32_t tileset;
        int32_t hurtsound;
        int32_t deathsound;
        int32_t health;
        int32_t EXP;
        int32_t deathgraphic;
        int32_t direction;
        int32_t flags;
        int32_t displayL, displayU, displayR, displayD;
        int32_t frametimer,framenum;
        int32_t objecttimer;
        int32_t directive;
        int32_t scriptstate,scripttimer;
        int32_t hitrectL, hitrectU, hitrectR, hitrectD;
        int32_t display_ofsX;
        int32_t display_ofsY;
        int32_t display_smokeRadius;
        int32_t display_unknown;
        int32_t hitTrue;
        int32_t damageTaken;
        int32_t damage;
        int32_t parent;
    } NPC;

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
        // int32_t
        PlayerUniverse = static_cast<uint32_t>(0x49E64C),
        // int32_t
        PlayerXV = static_cast<uint32_t>(0x49E66C),
        // int32_t
        PlayerYV = static_cast<uint32_t>(0x49E670),
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
        MapPXA = static_cast<uint32_t>(0x49E484),
        // NPCs
        NPCBase = static_cast<uint32_t>(0x4A6220),
        // NPC count
        NPCCount = static_cast<uint32_t>(0x200),
        NPCSize = static_cast<uint32_t>(0xAC)
    };

}
#endif
