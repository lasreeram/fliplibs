#ifndef _MY_ELLIPTIC_CURVE_H_
#define _MY_ELLIPTIC_CURVE_H_
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/ecdsa.h>

namespace ec_crypto{
class EllipticCurveKeyPair{
	public:
		EllipticCurveKeyPair( const unsigned char priv_byte[32] );
		~EllipticCurveKeyPair();
		unsigned char* getPrivateKeyFromKeyPair();
		

	private:
		EC_KEY* deriveKeyPairFromPrivateKey( const unsigned char* priv_byte );

		EC_KEY* _key;
		unsigned char* _priv_key;
};
}

#endif
