#include <elliptic_curve.h>
#include <debug.h>

namespace ec_crypto{
EllipticCurveKeyPair::EllipticCurveKeyPair(const unsigned char priv_bytes[32]){
	_priv_key = (unsigned char*) malloc(32);
	_key = deriveKeyPairFromPrivateKey( priv_bytes );
}


EllipticCurveKeyPair::~EllipticCurveKeyPair(){
	EC_KEY_free(_key);
	free(_priv_key);
}

unsigned char* EllipticCurveKeyPair::getPrivateKeyFromKeyPair(){
	return _priv_key;
}

EC_KEY* EllipticCurveKeyPair::deriveKeyPairFromPrivateKey(const unsigned char* priv_bytes){

	EC_KEY* key;
	BIGNUM priv;
	BN_CTX* ctx;
	const EC_GROUP* group;
	EC_POINT* pub;

	
	key = EC_KEY_new_by_curve_name(NID_secp256k1);
	
	//init the BIG num data structure
	BN_init(&priv);
	//copy data from priv_bytes into BIGNUM
	BN_bin2bn(priv_bytes, 32, &priv);
	//set the private key as the big number provided
	EC_KEY_set_private_key(key, &priv);

	
	//create a new Big number context
	ctx = BN_CTX_new();
	//initialize the Big number context?
	BN_CTX_start(ctx);

	//get the group name from the EC key
	group = EC_KEY_get0_group(key);

	//get a new public key structure using the group name. The POINT represents the public key
	pub = EC_POINT_new(group);

	//calculate the public key from the private key as G * priv where G is a generator belonging to the cureve and priv is the private key.
	//The public key is obtained my multiplying the generator priv key times.
	EC_POINT_mul(group, pub, &priv, NULL, NULL, ctx);

	
	//set the public key to the key pair.
	EC_KEY_set_public_key(key, pub);

	//get private key from the key pair
	const BIGNUM *priv_bn;
	priv_bn = EC_KEY_get0_private_key( key );
	if( !priv_bn ){
		debug_lib::throw_error( "unable to decode private key" );
	}
	BN_bn2bin(priv_bn, _priv_key);

	return key;
}

}
