#include "client.hpp"
#include "enc_index.pb.h"

#include <fstream>
#include <iomanip>
#include <cstdio>
#include <iostream>
#include <nnxx/message.h>
#include <nnxx/socket>
#include <nnxx/message>
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
	
	log.information("--------------------------------------------------");
	log.information("Query size: " + std::to_string(query_size));
	log.information("Alph  size: " + std::to_string(alph_size));

	std::string query_vals;
	for(auto &val: query)
		query_vals += std::to_string(val) + ", ";
	log.information("Query: " + query_vals);


	nnxx::socket client_socket { nnxx::SP, nnxx::REQ };
	client_socket.connect("tcp://" + server_name + ":" + std::to_string(port));

	QueryConfig query_config;
	query_config.set_pub_key( pubt.getStr() );
	query_config.set_query_size(query_size);
	client_socket.send(query_config.SerializeAsString());
	
	TextConfig text_config; 
	text_config.ParseFromString( nnxx::to_string(client_socket.recv()) );

	log.information("Received:");
	log.information("lg_sigma: " + std::to_string(text_config.lg_sigma() ));
	log.information("text_len: " + std::to_string(text_config.text_len() ));

	int lg_sigma = text_config.lg_sigma();
	int text_len = text_config.text_len();
	int array_len = text_len * 2;

	for(auto &v: query){
		log.information("------------------------------ Query: " + std::to_string(v));

		int query_val = v;
		int pos = 0;

		for(int i=0; i<lg_sigma; i++, query_val >>= 1){
			int real_pos = pos;

			int bit = 1 & query_val;
			pos += (bit == 0) ? 0: (text_len);

			log.information(std::to_string(i) + " (vi) ------------------------------> pos: " + std::to_string(real_pos) + " --- query_pos: " + std::to_string(pos) + " --- " + std::to_string(bit));
			log.information("Plain \t\tCipher");

			EncIndex enc_index;

			for(int j=0; j < array_len; j++){
				Zn zn = (j == pos)? 1: 0; 

				Elgamal::CipherText ct;
				pubt.enc( ct, zn);

				enc_index.add_query( ct.getStr() );

				Zn rzn;
				prvt.dec(rzn, ct);
				log.information(std::to_string(j) + " - " + "[" + rzn.getStr() + "]" + "[" + ct.getStr() + "]");
			}

			client_socket.send( enc_index.SerializeAsString() );
			
			QueryResult query_result;
			query_result.ParseFromString(nnxx::to_string(client_socket.recv()));

			Elgamal::CipherText cipher_res;
			cipher_res.fromStr( query_result.cipher_index() );

			Zn zres;
			prvt.dec(zres, cipher_res);
			cout << "Resultado: (ciph)  " << cipher_res << endl;
			cout << "Resultado: (plain) " << zres << endl;

			pos = zres.getUint64();
		}

		cout << "Resultado Final ========================================================================== : (query: " << v << ") " << pos << endl;
	}
}
