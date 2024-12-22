#ifndef CRYPTOPP_MD5_H
#define CRYPTOPP_MD5_H

#include "iterhash.h"

NAMESPACE_BEGIN(CryptoPP)

namespace Weak1 {

/// \brief MD5 message digest
/// \sa <a href="http://www.cryptolounge.org/wiki/MD5">MD5</a>
/// \since Crypto++ 1.0
class MD5 : public IteratedHashWithStaticTransform<word32, LittleEndian, 64, 16, MD5>
{
public:
    static void InitState(HashWordType *state)
    {
        state[0] = 0x67452301L;
        state[1] = 0xefcdab89L;
        state[2] = 0x98badcfeL;
        state[3] = 0x10325476L;
    }
    static void Transform(word32 *digest, const word32 *data)
    {
        CRYPTOPP_ASSERT(digest);
        CRYPTOPP_ASSERT(data);

        // #define F1(x, y, z) (x & y | ~x & z)

        auto F1 = [&](auto x, auto y, auto z) { return (z ^ (x & (y ^ z))); };
        auto F2 = [&](auto x, auto y, auto z) { return F1(z, x, y); };
        auto F3 = [&](auto x, auto y, auto z) { return (x ^ y ^ z); };
        auto F4 = [&](auto x, auto y, auto z) { return (y ^ (x | ~z)); };

        auto MD5STEP = [&](const auto& f, auto &w, auto x, auto y, auto z, auto data, int s)
        {
            w = rotlFixed(w + f(x, y, z) + data, s) + x;
        };

        word32 a, b, c, d;

        a=digest[0];
        b=digest[1];
        c=digest[2];
        d=digest[3];

        MD5STEP(F1, a, b, c, d, data[0] + 0xd76aa478, 7);
        MD5STEP(F1, d, a, b, c, data[1] + 0xe8c7b756, 12);
        MD5STEP(F1, c, d, a, b, data[2] + 0x242070db, 17);
        MD5STEP(F1, b, c, d, a, data[3] + 0xc1bdceee, 22);
        MD5STEP(F1, a, b, c, d, data[4] + 0xf57c0faf, 7);
        MD5STEP(F1, d, a, b, c, data[5] + 0x4787c62a, 12);
        MD5STEP(F1, c, d, a, b, data[6] + 0xa8304613, 17);
        MD5STEP(F1, b, c, d, a, data[7] + 0xfd469501, 22);
        MD5STEP(F1, a, b, c, d, data[8] + 0x698098d8, 7);
        MD5STEP(F1, d, a, b, c, data[9] + 0x8b44f7af, 12);
        MD5STEP(F1, c, d, a, b, data[10] + 0xffff5bb1, 17);
        MD5STEP(F1, b, c, d, a, data[11] + 0x895cd7be, 22);
        MD5STEP(F1, a, b, c, d, data[12] + 0x6b901122, 7);
        MD5STEP(F1, d, a, b, c, data[13] + 0xfd987193, 12);
        MD5STEP(F1, c, d, a, b, data[14] + 0xa679438e, 17);
        MD5STEP(F1, b, c, d, a, data[15] + 0x49b40821, 22);

        MD5STEP(F2, a, b, c, d, data[1] + 0xf61e2562, 5);
        MD5STEP(F2, d, a, b, c, data[6] + 0xc040b340, 9);
        MD5STEP(F2, c, d, a, b, data[11] + 0x265e5a51, 14);
        MD5STEP(F2, b, c, d, a, data[0] + 0xe9b6c7aa, 20);
        MD5STEP(F2, a, b, c, d, data[5] + 0xd62f105d, 5);
        MD5STEP(F2, d, a, b, c, data[10] + 0x02441453, 9);
        MD5STEP(F2, c, d, a, b, data[15] + 0xd8a1e681, 14);
        MD5STEP(F2, b, c, d, a, data[4] + 0xe7d3fbc8, 20);
        MD5STEP(F2, a, b, c, d, data[9] + 0x21e1cde6, 5);
        MD5STEP(F2, d, a, b, c, data[14] + 0xc33707d6, 9);
        MD5STEP(F2, c, d, a, b, data[3] + 0xf4d50d87, 14);
        MD5STEP(F2, b, c, d, a, data[8] + 0x455a14ed, 20);
        MD5STEP(F2, a, b, c, d, data[13] + 0xa9e3e905, 5);
        MD5STEP(F2, d, a, b, c, data[2] + 0xfcefa3f8, 9);
        MD5STEP(F2, c, d, a, b, data[7] + 0x676f02d9, 14);
        MD5STEP(F2, b, c, d, a, data[12] + 0x8d2a4c8a, 20);

        MD5STEP(F3, a, b, c, d, data[5] + 0xfffa3942, 4);
        MD5STEP(F3, d, a, b, c, data[8] + 0x8771f681, 11);
        MD5STEP(F3, c, d, a, b, data[11] + 0x6d9d6122, 16);
        MD5STEP(F3, b, c, d, a, data[14] + 0xfde5380c, 23);
        MD5STEP(F3, a, b, c, d, data[1] + 0xa4beea44, 4);
        MD5STEP(F3, d, a, b, c, data[4] + 0x4bdecfa9, 11);
        MD5STEP(F3, c, d, a, b, data[7] + 0xf6bb4b60, 16);
        MD5STEP(F3, b, c, d, a, data[10] + 0xbebfbc70, 23);
        MD5STEP(F3, a, b, c, d, data[13] + 0x289b7ec6, 4);
        MD5STEP(F3, d, a, b, c, data[0] + 0xeaa127fa, 11);
        MD5STEP(F3, c, d, a, b, data[3] + 0xd4ef3085, 16);
        MD5STEP(F3, b, c, d, a, data[6] + 0x04881d05, 23);
        MD5STEP(F3, a, b, c, d, data[9] + 0xd9d4d039, 4);
        MD5STEP(F3, d, a, b, c, data[12] + 0xe6db99e5, 11);
        MD5STEP(F3, c, d, a, b, data[15] + 0x1fa27cf8, 16);
        MD5STEP(F3, b, c, d, a, data[2] + 0xc4ac5665, 23);

        MD5STEP(F4, a, b, c, d, data[0] + 0xf4292244, 6);
        MD5STEP(F4, d, a, b, c, data[7] + 0x432aff97, 10);
        MD5STEP(F4, c, d, a, b, data[14] + 0xab9423a7, 15);
        MD5STEP(F4, b, c, d, a, data[5] + 0xfc93a039, 21);
        MD5STEP(F4, a, b, c, d, data[12] + 0x655b59c3, 6);
        MD5STEP(F4, d, a, b, c, data[3] + 0x8f0ccc92, 10);
        MD5STEP(F4, c, d, a, b, data[10] + 0xffeff47d, 15);
        MD5STEP(F4, b, c, d, a, data[1] + 0x85845dd1, 21);
        MD5STEP(F4, a, b, c, d, data[8] + 0x6fa87e4f, 6);
        MD5STEP(F4, d, a, b, c, data[15] + 0xfe2ce6e0, 10);
        MD5STEP(F4, c, d, a, b, data[6] + 0xa3014314, 15);
        MD5STEP(F4, b, c, d, a, data[13] + 0x4e0811a1, 21);
        MD5STEP(F4, a, b, c, d, data[4] + 0xf7537e82, 6);
        MD5STEP(F4, d, a, b, c, data[11] + 0xbd3af235, 10);
        MD5STEP(F4, c, d, a, b, data[2] + 0x2ad7d2bb, 15);
        MD5STEP(F4, b, c, d, a, data[9] + 0xeb86d391, 21);

        digest[0]+=a;
        digest[1]+=b;
        digest[2]+=c;
        digest[3]+=d;
    }
	CRYPTOPP_STATIC_CONSTEXPR const char* StaticAlgorithmName() {return "MD5";}
};

}
#if CRYPTOPP_ENABLE_NAMESPACE_WEAK >= 1
namespace Weak {using namespace Weak1;}		// import Weak1 into CryptoPP::Weak
#else
using namespace Weak1;	// import Weak1 into CryptoPP with warning
#ifdef __GNUC__
#warning "You may be using a weak algorithm that has been retained for backwards compatibility. Please '#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1' before including this .h file and prepend the class name with 'Weak::' to remove this warning."
#else
#pragma message("You may be using a weak algorithm that has been retained for backwards compatibility. Please '#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1' before including this .h file and prepend the class name with 'Weak::' to remove this warning.")
#endif
#endif

NAMESPACE_END

#endif
