Q1. How to create an elliptic curve private key using openssl?
	$ openssl ecparam -name secp256k1 -genkey -out ec-priv.pem


Q2. How to view the EC key in openssl?
The key can be viewed in a readable form using the following command.
	$ openssl ec -in ec-priv.pem -text -noout

In this output -
	* Private key is 32 bytes long
	* Public key is 64 bytes long + 1 byte prefix which is 04
	* name of the elliptic curve used.

Note* The public key can be in compressed for in which case only the x co-ordinate is stored along with the prefix 02 for y being even and 03 for y being odd.

Q3. What are the public and private keys in relation to the elliptic curve?
The curve represented by secp256k1 is 
	y^2 = x^2 + 7

The public key is made of two 32 byte numbers that represent the x,y co-ordinates on the secp256k1 curve.
The private key is a number using which you can infer the public key (the x,y co-ordinates. However you cannot infer the private key using the public key obviously)


Q4. How to get the public key from the private key using openssl?
The following command can be used to obtain the public key from the private key.

	$ openssl ec -in ec-priv.pem -pubout -out ec-pub.pem

You can decode the public key using the command
	$ openssl ec -in ec-pub.pem -pubin -text -noout

Q5. Why is EC hard to solve?
EC is hard to solve because of the discrete logrithm problem. Also there are multitude of solutions for any given problem. This means that even if the solution set is know it will be time consuming to identify which number in the solution set was used.

The discrete logarithm is just the inverse operation. For example, consider the equation 3^k ≡ 13 (mod 17) for k. From the example above, one solution is k = 4, but it is not the only solution. Since 3^16 ≡ 1 (mod 17) — as follows from Fermat's little theorem — it also follows that if n is an integer then 3^(4+16n) ≡ 3^4 × 3^(16n) ≡ 13 × (1^n) ≡ 13 (mod 17). Hence the equation has infinitely many solutions of the form 4 + 16n. Moreover, since 16 is the smallest positive integer m satisfying 3m ≡ 1 (mod 17), i.e. 16 is the order of 3 in (Z17)×, these are the only solutions. Equivalently, the set of all possible solutions can be expressed by the constraint that k ≡ 4 (mod 16).


Q6. What is the nature of the private key in EC cryptography?
In EC cryptography, the private key is an integer that may or may not be prime. The standards for SEC1 state that the private key must be a random number less than the order of G, where G is a generator point. In the case of secp256k1 n would be FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141, in hex. So private key is a random number in [1, n-1]


Q7. What is a generator (or a primitive point) in EC cryptography?
A primitive point P is simply a generator of this group (of integer points): all elements of the group can be expressed as P+P+...+P (k times) for some k. 
