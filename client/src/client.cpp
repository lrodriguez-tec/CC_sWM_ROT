#include "client.hpp"

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
	log.information("--------------------------------------------------");

	/////////////////////////////////////////////////////////////////////////
	
	nnxx::socket client_socket { nnxx::SP, nnxx::REQ };
	client_socket.connect("tcp://" + server_name + ":" + std::to_string(port));

	int lg_sigma = 5; //TODO: calcular, este valor lo tiene el server
	int text_len = 13;
	int array_len = text_len * 2;

	for(auto &v: query){
		int query_val = v;
		std::cout << "Query : " << query_val << std::endl;
		int pos = 0;
		cout << "starting position: " << pos << endl;


		for(int i=0; i<lg_sigma; i++, query_val >>= 1){
			std::cout << "Postition: " << pos;
			int bit = 1 & query_val;
			pos += (bit == 0) ? 0: (text_len);

			std::cout << "\t qPosition: " << pos << "\tbit:" << bit << endl;

			////////////////////////////////////////////////////////////////////

			std::vector<Elgamal::CipherText> sec_vec(array_len);

			cout << "Plain" << "\t\tCipher" << endl;

			for(int j=0; j < array_len; j++){
				Zn zn = (j == pos)? 1: 0; 
				pubt.enc( sec_vec[j], zn);

				Zn rzn;
				prvt.dec(rzn, sec_vec[j]);
				cout << setw(3) << j << "[" << rzn << "]" << "[" << zn << "] [" << sec_vec[j] << "]" << endl;
			}

			////////////////////////////////////////////////////////////////////

			std::string pubts = pubt.toStr();
			client_socket.send( pubts );
			cout << "Received: " << client_socket.recv() << endl;

	 		cout << "Sended - Response" << endl;
			for(auto &item: sec_vec){
				std::string val = item.getStr();
				client_socket.send( val );
				std::cout << val << " --- " << client_socket.recv() << endl;

			}

			client_socket.send( "Respuesta!" );
			nnxx::message result = client_socket.recv();
			Elgamal::CipherText cipher_res;
			cipher_res.fromStr( nnxx::to_string(result) );
		

			Zn zres;
			prvt.dec(zres, cipher_res);
			cout << "Resultado: (ciph)  " << cipher_res << endl;
			cout << "Resultado: (plain) " << zres << endl;

			pos = zres.getInt64();
		}

		cout << "Resultado Final ========================================================================== : (query: " << v << ") " << pos << endl;
	}
}