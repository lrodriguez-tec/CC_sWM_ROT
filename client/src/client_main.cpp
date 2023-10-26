#include "El_Gammal_config.hpp"
#include "client.hpp"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {

	SysInit();

	const mcl::EcParam& para = mcl::ecparam::secp192k1;
	const Fp x0(para.gx);
	const Fp y0(para.gy);
	const Ec P(x0, y0);
	const size_t bitLen = para.bitSize;

	Elgamal::PrivateKey prvt;
	prvt.init(P, bitLen); //, rg);
	const Elgamal::PublicKey &pubt = prvt.getPublicKey();

	cout << "prvt: " << prvt << endl;
	cout << "pub : " << pubt << endl;

	int port = 9999;
	std::string file_name{"resources/client/query.txt"};
	std::string server_name{"localhost"};
	
	Client client{server_name, port, file_name, prvt, pubt};
	
	return 0;
}



