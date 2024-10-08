#ifndef VLAK_ELEMENTS_H
#define VLAK_ELEMENTS_H

typedef enum
{
    NIC = 0,    // NIC (EMPTY TILE)
    KRY = 1,    // KRYSTAL (CRYSTAL)
    KOR = 2,    // KORUNA (CROWN)
    STO = 3,    // STROM (TREE)
    JAB = 4,    // JABLKO (APPLE)
    KRA = 5,    // KRAVA (COW)
    TRE = 6,    // TRESNE (CHERRY)
    RYB = 7,    // RYBNIK (POND)
    ZIR = 8,    // ZIRAFA (GIRAFFE)
    ZMR = 9,    // ZMRZLINA (ICE CREAM)
    DOR = 10,   // DORT (CAKE)
    POC = 11,   // POCITAC (COMPUTER)
    AUT = 12,   // AUTO (CAR)
    BAL = 13,   // BALON (BALOON)
    BUD = 14,   // BUDIK (CLOCK)
    SLO = 15,   // SLON (ELEPHANT)
    VIN = 16,   // VINO (WINE)
    PEN = 17,   // PENIZE (MONEY)
    LET = 18,   // LETADLO (AIRPLANE)
    VRA,        // VRATA (DOOR)
    ZED,        // ZED (WALL)
    LOK_BOOM,    // VLAK VYBUCH (TRAIN EXPLOSION)
    LOK_DED,    // VLAK TROSKA (TRAIN RUIN)
    LOK,        // VLAK (TRAIN)
    // VAGONY (WAGONS)

    VAG_KRY,
    VAG_KOR,
    VAG_STO,
    VAG_JAB,
    VAG_KRA,
    VAG_TRE,
    VAG_RYB,
    VAG_ZIR,
    VAG_ZMR,
    VAG_DOR,
    VAG_POC,
    VAG_AUT,
    VAG_BAL,
    VAG_BUD,
    VAG_SLO,
    VAG_VIN,
    VAG_PEN,
    VAG_LET,
    NUM_LEVEL_ELEMENTS
} vlak_element_t;

#endif
