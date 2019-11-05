#include "shifres.hpp"

Shamir::Shamir() {  }

inline void Shamir::decode(void* data) {
    *((int*)data) = pows(*((int*)data), hiddenKey2, Environment::Instance().encrMaxNumber);
}

inline void Shamir::encode(void* data) {
    *((int*)data) = pows(*((int*)data), hiddenKey1, Environment::Instance().encrMaxNumber);
}

void Shamir::recipient_protocol() {
    Environment &Environment = Environment::Instance();
    takeSharedKey();
    while(true){
        choose_c_d(&hiddenKey1, &hiddenKey2, Environment.encrMaxNumber);
        waitTilReady(OREV_LOCKED);
        lock_straight_channel();
        waitTilReady(REV_UNLOCKED);
        int byte;
        if(readBytes(&byte, 4) == EOF) { write(EOF); break; }
        encode(&byte);
        unlock_straight_channel();
        putBytes(&byte, 4);
        unlock_reverse_channel();
        unlock_straight_channel();
        waitTilReady(REV_LOCKED);
        lock_straight_channel();
        waitTilReady(REV_UNLOCKED);
        readBytes(&byte, 4);
        unlock_reverse_channel();
        unlock_straight_channel();
        decode(&byte);
        write(byte);
    }
}

void Shamir::dispatcher_protocol() {
    Environment &Environment = Environment::Instance();
    giveSharedKey();
    try {
        while(true) {
            choose_c_d(&hiddenKey1, &hiddenKey2, Environment.encrMaxNumber);
            int byte = read();
            encode(&byte);
            waitTilReady(REV_STRT_UNLOCKED);
            putBytes(&byte, 4);
            waitTilReady(REV_LOCKED);
            unlock_straight_channel();
            waitTilReady(REV_STRT_UNLOCKED);
            int answer;
            readBytes(&answer, 4);
            decode(&answer);
            unlock_reverse_channel();
            waitTilReady(REV_STRT_UNLOCKED); // as a precaution
            putBytes(&answer, 4);
            waitTilReady(REV_LOCKED);
            unlock_straight_channel();
        }
    } catch(FileStat) {
        sendEOF();
    }
}
