

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

#include <stdint-gcc.h>

#define EOF (-1)

enum WaitMode { OREV_LOCKED=1, REV_LOCKED=2, REV_UNLOCKED=3, REV_STRT_UNLOCKED=5 };

#ifdef __cplusplus
extern "C"
{
#endif
void DLL_EXPORT init(const char* straightChannel, const char* inverseChannel);
int DLL_EXPORT readByte(); /// only from reverse channel
int DLL_EXPORT readBytes(void *bytes, int cnt); /// only from reverse channel
void DLL_EXPORT putByte(int data); /// only to straight channel
void DLL_EXPORT putBytes(void *bytes, int cnt); /// only to straight channel
void DLL_EXPORT unlock_straight_channel();
void DLL_EXPORT unlock_reverse_channel();
void DLL_EXPORT lock_reverse_channel();
void DLL_EXPORT lock_straight_channel();
void DLL_EXPORT trylock_reverse_channel(); /// this function don`t tell, if file locked 100% probability
void DLL_EXPORT trylock_straight_channel(); /// this function don`t tell, if file locked 100% probability
#ifdef __cplusplus
}
#endif

