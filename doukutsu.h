#ifndef DOUKUTSU_H
#define DOUKUTSU_H

#include <QtGlobal>

// MACROS
#define BIT(x) 1 << x

namespace Doukutsu {
    // STRUCTS
    #pragma pack(push, 1)
    typedef struct {
        quint32 id;
        quint32 level;
        quint32 energy;
        quint32 ammoMax;
        quint32 ammo;
    } Weapon;

    typedef struct {
        quint32 id;
        quint32 event;
    } StagePermit;

    typedef struct {
        qint32 collision;
        quint32 shotID;
        qint32 flags;
        qint32 inuse;
        qint32 x, y;
        qint32 movex, movey;
        qint32 unk0, unk1;
        qint32 wasSetup;
        qint32 unk2, unk3;
        qint32 frameID;
        qint32 direction;
        qint32 displayL, displayU, displayR, displayD;
        qint32 distance;
        qint32 unk4;
        qint32 maxdistance;
        qint32 damage;
        qint32 numimpacts;
        qint32 hitrectL, hitrectU, hitrectR, hitrectD;
        qint32 display_nofsX, display_nofsY, unk7, unk8;
    } Bullet;

    typedef struct {
        qint32 inuse;
        qint32 collision;
        qint32 x, y;
        qint32 movey, movey;
        qint32 altvelx,altvely;
        qint32 CurlyMacro1, CurlyMacro2;
        qint32 npcType, entityID;
        qint32 EventNum;
        qint32 tileset;
        qint32 hurtsound;
        qint32 deathsound;
        qint32 health;
        qint32 EXP;
        qint32 deathgraphic;
        qint32 direction;
        qint32 flags;
        qint32 displayL, displayU, displayR, displayD;
        qint32 frametimer, framenum;
        qint32 objecttimer;
        qint32 directive;
        qint32 scriptstate, scripttimer;
        qint32 hitrectL, hitrectU, hitrectR, hitrectD;
        qint32 display_ofsX;
        qint32 display_ofsY;
        qint32 display_smokeRadius;
        qint32 display_unknown;
        qint32 hitTrue;
        qint32 damageTaken;
        qint32 damage;
        qint32 parent;
    } NPC;
    #pragma pack(pop)

    #define off(addr) static_cast<quint32>((addr))
    // MEMORY OFFSETS
    // from http://cave-story-modding.wikia.com/wiki/Noxid's_Assembly_Compendium
        enum Offsets : quint32 {
        // quint8[5] : is 0xA1, 0x20, 0x8B, 0x49, 0x00 in Doukutsu.exe
        VerifyExe = off(0x412429),
        // quint32[0x20] : first element in item array
        InventoryStart = off(0x499B40),
        // Weapon[8] : first element in weapon array
        WeaponsStart = off(0x499BC8),
        // quint32 : index into Weapons
        // CurWeaponOff = WeaponsStart + CurWeaponSlot * sizeof(Weapon)
        CurWeaponSlot = off(0x499C68),
        // quint16 : bitfield (one bit per equip)
        Equips = off(0x49E650),
        // qint32
        PlayerX = off(0x49E654),
        // qint32
        PlayerY = off(0x49E658),
        // qint32 : <UNI value - 0 for normal, 1 for Ironhead, 2 for no movement
        PlayerUniverse = off(0x49E64C),
        // qint32
        PlayerXVel = off(0x49E66C),
        // qint32
        PlayerYVel = off(0x49E670),
        // quint32 : timer for mercy invuln after taking damage
        InvulnTimer = off(0x49E6C8),
        // quint16
        HealthCurrent = off(0x49E6CC),
        // quint16
        WhimStarNum = off(0x49E6CE),
        // quint32
        HealthMaximum = off(0x49E6D0),
        // quint32 : used for yellow bar when taking damage
        HealthDisplayed = off(0x49E6D4),
        // quint32 ??? unknown unit
        AirLeft = off(0x49E6DC),
        // quint32
        BoosterFuel = off(0x49E6E8),
        // StagePermit[8] : first element in Arthur's House teleporter destination (StagePermit) array
        StagePermitsStart = off(0x4A54FC),
        // quint16[2]
        MapSize = off(0x49E586),
        // quint8[w * h]* : This is NOT collision data
        MapPXMOff = off(0x49E480),
        // quint8[256] : this is NOT a pointer!
        MapPXA = off(0x49E484),
        // NPC[0x200] : first element in NPC array
        NPCsStart = off(0x4A6220),
        // Bullet[???] : first element in bullet array
        //BulletsStart = off(???),
        // ...I don't know where it is, or how many it stores
        // ask in the CSMC

        // a message to 20kdc:
        // DON'T USE THIS ENUM FOR NON-OFFSETS!!!
        // total size of NPC array
        // this could probably be a "magic number" (since it's unlikely to change and this thing is probably already incompatible with txin's stuff)
        //NPCCount = off(0x200),
        // size of NPC struct?
        // use sizeof(NPC) instead, it's probably somehow more efficent
        //NPCSize = off(0xAC)
    };
    #undef off

}
#endif
