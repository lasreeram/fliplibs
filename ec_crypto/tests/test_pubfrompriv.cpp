#include <elliptic_curve.h>
#include <gtest/gtest.h>

using namespace ec_crypto;
TEST( EllipticCurveKeyPair, Constructor ){
	unsigned char priv_bytes[32] = {
		0x16, 0x26, 0x07, 0x83, 0xe4, 0x0b, 0x16, 0x73,
		0x16, 0x73, 0x62, 0x2a, 0xc8, 0xa5, 0xb0, 0x45,
		0xfc, 0x3e, 0xa4, 0xaf, 0x70, 0xf7, 0x27, 0xf3,
		0xf9, 0xe9, 0x2b, 0xdd, 0x3a, 0x1d, 0xdc, 0x43
	};
	EllipticCurveKeyPair ec(priv_bytes);
	EXPECT_EQ( ec.isInitialized(), true );
}

TEST( EllipticCurveKeyPair, getKeyPairFromPrivateKey ){
	unsigned char priv_bytes[32] = {
		0x16, 0x26, 0x07, 0x83, 0xe4, 0x0b, 0x16, 0x73,
		0x16, 0x73, 0x62, 0x2a, 0xc8, 0xa5, 0xb0, 0x45,
		0xfc, 0x3e, 0xa4, 0xaf, 0x70, 0xf7, 0x27, 0xf3,
		0xf9, 0xe9, 0x2b, 0xdd, 0x3a, 0x1d, 0xdc, 0x43
	};
	EllipticCurveKeyPair ec(priv_bytes);
	char* ptr = (char*)ec.getKeyPair();
	EXPECT_STRNE( NULL, ptr );
}
