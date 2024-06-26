#include "client.hpp"
#include "enc_index.pb.h"
#include "ROT.hpp"

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

Client::Client (std::string server_name_, 
				int port_, 
				std::string query_file_name_,
				Elgamal::PrivateKey &prvt_,
			   	const Elgamal::PublicKey &pubt_):	server_name{server_name_}, 
													port{port_},
												   	prvt{prvt_}, 
													pubt{pubt_}{

	nclient_sock = nng::req::open();

	std::ifstream ifs{query_file_name_};
	if(!ifs){
		std::cerr << "Error abriendo archivo : " << query_file_name_ << std::endl;
		exit(0);
	}
	ifs >> query_size;
	ifs >> alph_size;

	for(int i=0; i<query_size; i++){
		int temp;
		ifs >> temp;
		query.push_back(temp);
	}
	
	std::string query_vals;
	for(auto &val: query)
		query_vals += std::to_string(val) + ", ";

	log.information("--------------------------------------------------");
	log.information("Query size: " + std::to_string(query_size));
	log.information("Alph  size: " + std::to_string(alph_size));
	log.information("Query: " + query_vals);

	start_server();
}

void Client::start_server(){

	std::string adress = "tcp://" + server_name + ":" + std::to_string(port);
	nclient_sock.dial( adress.c_str() );

	QueryConfig query_config;
	query_config.set_pub_key( pubt.getStr() );
	query_config.set_priv_key( prvt.getStr() ); //Only for debuging
	query_config.set_query_size(query_size);
	//client_socket.send(query_config.SerializeAsString());

	std::string squery_config = query_config.SerializeAsString();
	nng::view vista(squery_config.c_str(), squery_config.size());
	log.information("NETWORK Size query_config: " + std::to_string( squery_config.size()));
	nclient_sock.send( vista );

	nng::buffer text_config_buffer = nclient_sock.recv();
	char* text_config_char = text_config_buffer.data<char>();
	std::string text_config_str{text_config_char};

	TextConfig text_config; 
	text_config.ParseFromString( text_config_str );

	log.information("Received:");
	log.information("lg_sigma: " + std::to_string(text_config.lg_sigma() ));
	log.information("text_len: " + std::to_string(text_config.text_len() ));

	lg_sigma = text_config.lg_sigma();
	text_len = text_config.text_len();
	array_len = text_len * 2;

	int lpos = 0;
	int rpos = text_len-1;

	int prev_lpos = lpos;
	int prev_rpos = rpos;

	bool mismatch = false;
	bool last_mismatch = false;
	ROT rot;
	std::string query_r{};

	for(auto &v: query){
		log.information("================================================== Query: " + std::to_string(v));

		int query_val = v;

		for(int i=0; i<lg_sigma; i++, query_val >>= 1){
			last_mismatch = false;

			log.information("****************************************************************************************************");
			int real_pos = lpos;
			int real_pos_r = rpos;

			int bit = 1 & query_val;
			lpos += (bit == 0) ? 0: (text_len);
			rpos += (bit == 0) ? 0: (text_len);

			lpos %= array_len;
			rpos %= array_len;

			log.information(std::to_string(i) + " (lvi) ------------------------------> pos: " + std::to_string(real_pos) + " --- query_pos: " + std::to_string(lpos) + " --- " + std::to_string(bit));
			log.debug("Plain \t\tCipher");

			EncIndex enc_index;
			enc_index.set_mismatch( mismatch );
			mismatch = false;

			log.information("===== prep_query lpos");
			rot.prep_query(lpos, array_len, prvt, pubt, enc_index);

			std::string senc_index = enc_index.SerializeAsString();
			nng::view vista(senc_index.c_str(), senc_index.size());
			log.information("NETWORK Size senc_index: " + std::to_string(senc_index.size()));
			nclient_sock.send( vista );
			//client_socket.send( enc_index.SerializeAsString() );

			nng::buffer query_result_buffer = nclient_sock.recv();
			char* query_result_char = query_result_buffer.data<char>();
			std::string query_result_str{query_result_char};

			QueryResult query_result;
			query_result.ParseFromString(query_result_str);

			if(query_result.reached_deletes()){
				log.information( "deletes limit reached: last_r = " + std::to_string(query_result.last_r()));
				log.information("Final result ========================================================================== prevs : " + std::to_string(prev_lpos) + ", " + std::to_string(prev_rpos));
				lpos = (prev_lpos - query_result.last_r() + array_len) % array_len;
				rpos = (prev_rpos - query_result.last_r() + array_len) % array_len;

				log.information("Final result ========================================================================== : " + std::to_string(lpos) + ", " + std::to_string(rpos));

				log.information("Query found: " + query_r);

				exit(0);
			}

			Elgamal::CipherText cipher_res;
			cipher_res.fromStr( query_result.cipher_index() );

			Zn zres;
			prvt.dec(zres, cipher_res);
			cout << "Resultado: (ciph)  " << cipher_res << endl;
			cout << "Resultado: (plain) " << zres << endl;

			lpos = zres.getUint64();
			//===========================================================

			log.information(std::to_string(i) + " (rvi) ------------------------------> pos: ---" + std::to_string(real_pos_r) + "--- --- query_pos: " + std::to_string(rpos) + " --- " + std::to_string(bit));
			log.debug("Plain \t\tCipher");

			EncIndex enc_index_r;
			log.information("===== prep_query rpos");
			rot.prep_query(rpos, array_len, prvt, pubt, enc_index_r);

			std::string senc_index_r = enc_index_r.SerializeAsString();
			nng::view vista_r(senc_index_r.c_str(), senc_index_r.size());
			log.information("NETWORK Size senc_index_r: " + std::to_string(senc_index_r.size()));
			nclient_sock.send( vista_r );
			//client_socket.send( enc_index_r.SerializeAsString() );

			nng::buffer query_result_buffer_r = nclient_sock.recv();
			char* query_result_char_r = query_result_buffer_r.data<char>();
			std::string query_result_str_r{query_result_char_r};

			QueryResult query_result_r;
			query_result_r.ParseFromString(query_result_str_r);

			Elgamal::CipherText cipher_res_r;
			cipher_res_r.fromStr( query_result_r.cipher_index() );

			Zn zres_r;
			prvt.dec(zres_r, cipher_res_r);
			cout << "Resultado: (ciph_r)  " << cipher_res_r << endl;
			cout << "Resultado: (plain_r) " << zres_r << endl;

			rpos = zres_r.getUint64();
		}

		cout << "Resultado Final ========================================================================== : (query: " << v << ") " << lpos << "--" << rpos << endl;

		if( lpos == rpos){
			cout << "Se encontro un mistatch. Se usaran los prev_lpos y prev_rpos: (" << prev_lpos << ", " << prev_rpos << ") para continuar con la siguiente letra" << endl;
			last_mismatch = mismatch = true;

			lpos = prev_lpos;
			rpos = prev_rpos;
			query_r += "<" + std::to_string(v) + ">-";
		}else{
			query_r += std::to_string(v) + "-";
		}

		prev_lpos = lpos;
		prev_rpos = rpos;
	}

	FinishCommunication finish;
	finish.set_request_r( last_mismatch );
	finish.set_testing(100);
	cout << "last mismatch : "  << last_mismatch << endl;


	if(last_mismatch){
		cout << "It is required to get penultimate_r" << endl;
	}

	std::string sfinish = finish.SerializeAsString();
	nng::view vista_f(sfinish.c_str(), sfinish.size());
	log.information("NETWORK Size sfinish: " + std::to_string(sfinish.size()));
	nclient_sock.send( vista_f );
	//client_socket.send( finish.SerializeAsString() );

	nng::buffer finish_res_buffer = nclient_sock.recv();
	char* finish_res_char = finish_res_buffer.data<char>();
	std::string finish_res_str{finish_res_char};

	FinishCommunication finish_res;
	finish_res.ParseFromString(finish_res_str );
	int penultimate_r = finish_res.penultimate_r();

	log.information("Penultimate_r: " + std::to_string(penultimate_r));

	lpos = (lpos - penultimate_r + array_len) % array_len;
	rpos = (rpos - penultimate_r + array_len) % array_len;

	log.information("Query found: " + query_r);
	log.information("Final result ========================================================================== : " + std::to_string(lpos) + ", " + std::to_string(rpos));
}

int Client::query_pos(int pos, int query_val){

		for(int i=0; i<lg_sigma; i++, query_val >>= 1){
			int real_pos = pos;

			int bit = 1 & query_val;
			pos += (bit == 0) ? 0: (text_len);

			log.information(std::to_string(i) + " (vi) ------------------------------> pos: " + std::to_string(real_pos) + " --- query_pos: " + std::to_string(pos) + " --- " + std::to_string(bit));
			log.debug("Plain \t\tCipher");

			EncIndex enc_index;
			for(int j=0; j < array_len; j++){
				Zn zn = (j == pos)? 1: 0; 

				Elgamal::CipherText ct;
				pubt.enc( ct, zn);

				enc_index.add_query( ct.getStr() );

				Zn rzn;
				prvt.dec(rzn, ct);
				//log.debug(std::to_string(j) + " - " + "[" + rzn.getStr() + "]" + "[" + ct.getStr() + "]");
			}

			std::string senc_index = enc_index.SerializeAsString();
			nng::view vista_f(senc_index.c_str(), senc_index.size());
			log.information("NETWORK Size senc_index: " + std::to_string(senc_index.size()));
			nclient_sock.send( vista_f );
			//client_socket.send( enc_index.SerializeAsString() );

			nng::buffer query_result_buffer = nclient_sock.recv();
			char* query_result_char = query_result_buffer.data<char>();
			std::string query_result_str{query_result_char};

			QueryResult query_result;
			query_result.ParseFromString(query_result_str);
			//query_result.ParseFromString(nnxx::to_string(client_socket.recv()));

			Elgamal::CipherText cipher_res;
			cipher_res.fromStr( query_result.cipher_index() );

			Zn zres;
			prvt.dec(zres, cipher_res);
			cout << "Resultado: (ciph)  " << cipher_res << endl;
			cout << "Resultado: (plain) " << zres << endl;

			pos = zres.getUint64();
		}
	return pos;

}
