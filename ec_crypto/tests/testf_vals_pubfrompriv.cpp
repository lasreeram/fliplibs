#include <elliptic_curve.h>
#include <gtest/gtest.h>

using namespace ec_crypto;
using ::testing::TestWithParam;
using ::testing::Bool;
using ::testing::Values;
using ::testing::Combine;
#if GTEST_HAS_COMBINE
namespace{ //put everything into an anonymous namespace so that it does not affect anything outside

	const unsigned char priv_bytes1[32] = {
		0x16, 0x26, 0x07, 0x83, 0xe4, 0x0b, 0x16, 0x73,
		0x16, 0x73, 0x62, 0x2a, 0xc8, 0xa5, 0xb0, 0x45,
		0xfc, 0x3e, 0xa4, 0xaf, 0x70, 0xf7, 0x27, 0xf3,
		0xf9, 0xe9, 0x2b, 0xdd, 0x3a, 0x1d, 0xdc, 0x43
	};
	const unsigned char priv_bytes2[32] = {
		0x11, 0x23, 0x09, 0x83, 0xe4, 0x0b, 0x16, 0x73,
		0x16, 0x73, 0x62, 0x2a, 0xc8, 0xa5, 0xb0, 0x45,
		0xfc, 0x3e, 0xa5, 0xaf, 0x70, 0xf7, 0x27, 0xf3,
		0xf9, 0xe9, 0x2b, 0xdd, 0x3a, 0x1d, 0xdc, 0x43
	};
	class ECurveTest : public TestWithParam< ::testing::tuple<bool, unsigned char*> > {
		public: //or should be protected as it will be derived by google test?

			virtual void SetUp() {

				const unsigned char* test = ::testing::get<1>(GetParam());
				//const unsigned char* pb = NULL;
				//if ( test == 1 )
				//	pb = priv_bytes1;
				//else
				//	pb = priv_bytes2;

				ec_ = new EllipticCurveKeyPair(test);
			}

			virtual void TearDown() {
				delete ec_;
			}


			EllipticCurveKeyPair* ec_;

	};

	INSTANTIATE_TEST_CASE_P(RunWithDifferentValues,
			ECurveTest,
			Combine(Values(1), Values((unsigned char*)priv_bytes1,(unsigned char*)priv_bytes2)));
} 

TEST_P( ECurveTest, getKeyPairFromPrivateKey ){ //first param must be fixture class name
	EC_KEY* ptr = ec_->getKeyPair(); //for ever test Setup and TearDown is called
	int value = EC_KEY_get_flags( ptr );
	EXPECT_EQ( value, 0 );
}


// if you link with gtest_main you do not have to call RUN_ALL_TESTS.
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
#else
	TEST(DummyTest, CombineIsNotSupportedOnThisPlatform) {}
#endif
