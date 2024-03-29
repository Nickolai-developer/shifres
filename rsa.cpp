#include "shifres.hpp"
#include <boost/integer/common_factor_rt.hpp>
#include <time.h>

using namespace boost::multiprecision;

inline bool RSA::ferma(const uint1024_t &num) { // O(logN)
    Environment &Environment = Environment::Instance();
    if(num == 2)
		return true;
    boost::random::uniform_int_distribution<uint1024_t> rand(1, std::numeric_limits<uint1024_t>::max());
	for(int i=0;i<200;i++){
		uint1024_t a = (rand(Environment.gen) % (num - 2)) + 2;
		if (boost::integer::gcd_evaluator<uint1024_t>()(a, num) != 1)
			return false;
		if( pows(a, num-1, num) != 1)
			return false;
	}
	return true;
}

inline uint1024_t RSA::pows(const uint1024_t &a, const uint1024_t &x, const uint1024_t &p) {
    number<cpp_int_backend<2048, 2048, unsigned_magnitude, unchecked, void> > y = 1;
    uint16_t offset = 1023;
    for(;;offset--) {
        y*=y;
        y%=p;
        if((x>>offset)&0x1) {
            y*=a;
            y%=p;
        }
        if(offset==0)
            break;
    }
    return static_cast<uint1024_t>(y);
}

inline uint1024_t RSA::mod_inverse(const uint1024_t &a, const uint1024_t &p) {
    number<cpp_int_backend<2048, 2048, signed_magnitude, unchecked, void> > u1=p,u2=0,v1=a,v2=1,t1,t2,q;
    while (v1!=0) {
        q=u1/v1; t1=u1%v1; t2=u2-q*v2;
        u1=v1;u2=v2;v1=t1;v2=t2;
    }
    while(u2<0)u2+=p;
    return static_cast<uint1024_t>(u2);
}

RSA::RSA() {
    Environment &Environment = Environment::Instance();
    int t = time(NULL);
    boost::random::uniform_int_distribution<uint1024_t> rand( RSA::pows(2, 510, std::numeric_limits<uint512_t>::max())-t, std::numeric_limits<uint512_t>::max()-t );
    uint1024_t P = rand(Environment.gen)+t, Q = rand(Environment.gen)+t;
    while(!ferma(++P)) if(P >= std::numeric_limits<uint512_t>::max()-2) { P = rand(Environment.gen)+t; }
    while(!ferma(++Q)) if(Q >= std::numeric_limits<uint512_t>::max()-2) { Q = rand(Environment.gen)+t; }
    sharedModulus = P*Q;
    uint1024_t eul = (P-1)*(Q-1);
    rand = boost::random::uniform_int_distribution<uint1024_t>( RSA::pows(2, 510, std::numeric_limits<uint512_t>::max())-t, sharedModulus-t );
    auto gcd = boost::integer::gcd_evaluator<uint1024_t>();
    do { sharedExponent = rand(Environment.gen)+t; } while(gcd(sharedExponent, eul) != 1);
    hiddenExponent = RSA::mod_inverse(sharedExponent, eul);
}

inline void RSA::decode(void* data) {
    uint1024_t n = *((uint1024_t*)data);
    *((uint1024_t*)data) = RSA::pows(n, hiddenExponent, sharedModulus);
}

inline void RSA::encode(void* data) {
    uint1024_t n = static_cast<uint1024_t>( *((int*)data) );
    *((uint1024_t*)data) = RSA::pows(n, takenExponent, takenModulus);
}

void RSA::takeSharedKey() {
    waitTilReady(OREV_LOCKED);
    uint1024_t arr[2]; arr[0] = sharedModulus, arr[1] = sharedExponent;
    putBytes(arr, 272);
    waitTilReady(REV_UNLOCKED);
    readBytes(arr, 272);
    unlock_reverse_channel();
    unlock_straight_channel();
    takenModulus = arr[0], takenExponent = arr[1];
}

void RSA::giveSharedKey() {
    waitTilReady(REV_STRT_UNLOCKED);
    uint1024_t arr[2]; arr[0] = sharedModulus, arr[1] = sharedExponent;
    putBytes(arr, 272);
    waitTilReady(REV_LOCKED);
    unlock_straight_channel();
    waitTilReady(REV_STRT_UNLOCKED);
    readBytes(arr, 272);
    unlock_reverse_channel();
    takenModulus = arr[0], takenExponent = arr[1];
}
