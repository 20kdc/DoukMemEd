#ifndef DOUKUTSU_H
#define DOUKUTSU_H

// can't be 0!
#define CS_health_maximum 0x49E6D0
#define CS_health_displayed 0x49E6CC
#define CS_health_current 0x49E6D4

// Read/WriteProcessMemory(lpBaseAddress: ptr(PTR_DEFINE))
#define ptr(x) reinterpret_cast<LPVOID>(x)

#endif // DOUKUTSU_H
