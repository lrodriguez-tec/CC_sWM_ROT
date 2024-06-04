#include "ROT.hpp"
#include <string>

void ROT::prep_query(const int index, const int array_len, const Elgamal::PrivateKey &prvt, const Elgamal::PublicKey &pubt, EncIndex &enc_index){

	for(int j=0; j < array_len; j++){
		Zn zn = (j == index)? 1: 0; 

		Elgamal::CipherText ct;
		pubt.enc( ct, zn);

		enc_index.add_query( ct.getStr() );

		Zn rzn;
		prvt.dec(rzn, ct);
		//log.debug(std::to_string(j) + " - " + "[" + rzn.getStr() + "]" + "[" + ct.getStr() + "]");
		//log.information(std::to_string(j) + " - " + "[" + rzn.getStr() + "]" + "[" + ct.getStr() + "]");
	}
}

std::vector<Elgamal::CipherText> ROT::req_query(EncIndex &enc_index, int r, Elgamal::PrivateKey &prvt){
	
	std::vector<Elgamal::CipherText> ciph_pos( enc_index.query_size() );

	for(int i=0; i<enc_index.query_size() ; i++)
		ciph_pos.at(i).fromStr( enc_index.query(i) );

	int prev_rotate, after_rotate;
	for(int i=0; i<enc_index.query_size(); i++) {
		Zn zres;
		prvt.dec(zres, ciph_pos.at(i) );
		if( 1 == zres.getInt64())
			prev_rotate = i;
	}

	int left_rotate;
	for(int i=0; i< r; i++) 
		std::rotate(ciph_pos.begin(), ciph_pos.begin() + 1, ciph_pos.end());

	for(int i=0; i<enc_index.query_size(); i++) {
		Zn zres;
		prvt.dec(zres, ciph_pos.at(i) );
		if( 1 == zres.getInt64())
			after_rotate = i;
	}
	log.information("==================== Rotate : (" + std::to_string(prev_rotate) + " -> " + std::to_string(after_rotate) + ")");

	return ciph_pos;
}


Elgamal::CipherText ROT::RanOT(std::vector<Elgamal::CipherText> &ciph_pos, int vi, int r, WM &wm){
	Elgamal::CipherText cr;
	cr.mul(0);

	for(int i=0; i<ciph_pos.size(); i++){
		ciph_pos.at(i).mul( (wm.query_rankCF_pos(i, vi) + r) % ciph_pos.size());
		cr.add( ciph_pos.at(i) ); 
	}

	return cr;
}
