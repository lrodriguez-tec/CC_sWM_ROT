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

inline void SysInit() {
	const mcl::EcParam& para = mcl::ecparam::secp192k1;
	Zn::init(para.n);
	Fp::init(para.p);
	Ec::init(para.a, para.b);
}
