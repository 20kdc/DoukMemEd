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
        qint32 X,Y;
        qint32 movex,movey;
        qint32 altvelx,altvely;
        qint32 CurlyMacro1,CurlyMacro2;
        qint32 npcType,entityID;
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
        qint32 frametimer,framenum;
        qint32 objecttimer;
        qint32 directive;
        qint32 scriptstate,scripttimer;
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

    // MEMORY OFFSETS
    // from http://cave-story-modding.wikia.com/wiki/Noxid's_Assembly_Compendium
    enum Offsets : quint32 {
        // Leo40Story please correct these - 20kdc
        // quint8[5] : is 0xA1, 0x20, 0x8B, 0x49, 0x00 in Doukutsu.exe
        VerifyExe = static_cast<quint32>(0x412429),
        // quint32[0x20] : first element in item array
        InventoryStart = static_cast<quint32>(0x499B40),
        // Weapon[8] : first element in weapon array
        WeaponsStart = static_cast<quint32>(0x499BC8),
        // quint32 that is an index into Weapons
        CurWeaponSlot = static_cast<quint32>(0x499C68),
        // quint16 bitfield (one bit per equip)
        Equips = static_cast<quint32>(0x49E650),
        // qint32
        PlayerX = static_cast<quint32>(0x49E654),
        // qint32
        PlayerY = static_cast<quint32>(0x49E658),
        // qint32 : <UNI value - 0 for normal, 1 for Ironhead, 2 for no movement
        PlayerUniverse = static_cast<quint32>(0x49E64C),
        // qint32
        PlayerXVel = static_cast<quint32>(0x49E66C),
        // qint32
        PlayerYVel = static_cast<quint32>(0x49E670),
        // quint32 : timer for mercy invuln after taking damage
        InvulnTimer = static_cast<quint32>(0x49E6C8),
        // quint16
        HealthCurrent = static_cast<quint32>(0x49E6CC),
        // quint16
        WhimStarNum = static_cast<quint32>(0x49E6CE),
        // quint32
        HealthMaximum = static_cast<quint32>(0x49E6D0),
        // quint32 : used for yellow bar when taking damage
        HealthDisplayed = static_cast<quint32>(0x49E6D4),
        // quint32 ??? unknown unit
        AirLeft = static_cast<quint32>(0x49E6DC),
        // quint32
        BoosterFuel = static_cast<quint32>(0x49E6E8),
        // StagePermit[8] : first element in Arthur's House teleporter destination (StagePermit) array
        StagePermitsStart = static_cast<quint32>(0x4A54FC),
        // quint16[2]
        MapSize = static_cast<quint32>(0x49E586),
        // quint8[w * h]* : This is NOT collision data
        MapPXMOff = static_cast<quint32>(0x49E480),
        // quint8[256] : this is NOT a pointer!
        MapPXA = static_cast<quint32>(0x49E484),
        // NPC[0x200] : first element in NPC array
        NPCsStart = static_cast<quint32>(0x4A6220),
        // DON'T USE THIS ENUM FOR NON-OFFSETS!!!
        // total size of NPC array
        // this could probably be a "magic number" (since it's unlikely to change and this thing is probably already incompatible with txin's stuff)
        NPCCount = static_cast<quint32>(0x200),
        // size of NPC struct?
        // use sizeof(NPC) instead, it's probably somehow more efficent
        NPCSize = static_cast<quint32>(0xAC)
    };

}
#endif
