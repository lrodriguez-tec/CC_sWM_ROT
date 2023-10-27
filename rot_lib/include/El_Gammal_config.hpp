#pragma once

#include <mcl/fp.hpp>
#include <mcl/ec.hpp>
#include <mcl/elgamal.hpp>
#include <mcl/ecparam.hpp>

typedef mcl::FpT<mcl::FpTag, 192> Fp;
typedef mcl::FpT<mcl::ZnTag, 192> Zn; // use ZnTag because Zn is different class with Fp
typedef mcl::EcT<Fp> Ec;
typedef mcl::ElgamalT<Ec, Zn> Elgamal;

struct CipherTextVec : public std::vector<Elgamal::CipherText> {};

inline void Elgamal_sysInit() {
	const mcl::EcParam& para = mcl::ecparam::secp192k1;
	Zn::init(para.n);
	Fp::init(para.p);
	Ec::init(para.a, para.b);
}


inline Elgamal::PrivateKey generate_prvt_key(){

	Elgamal_sysInit();

	const mcl::EcParam& para = mcl::ecparam::secp192k1;
	const Fp x0(para.gx);
	const Fp y0(para.gy);
	const Ec P(x0, y0);
	const size_t bitLen = para.bitSize;

	Elgamal::PrivateKey prvt;
	prvt.init(P, bitLen); //, rg);

	return prvt;
}
