#include "server.hpp"
#include "El_Gammal_config.hpp"
#include "enc_index.pb.h"

#include <nnxx/reqrep.h>
#include <nnxx/socket.h>
#include <nnxx/message.h>
#include <random>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

Server::Server (int port_, std::string file_name_) : 
														port{port_}, 
														wm{file_name}, 
														file_name{file_name_}, 
														lg_sigma{ wm.get_lg_sigma()},
														text_len{ wm.get_text_len()},
														array_len{ text_len * 2},
														rd{},
														//gen{rd()},
														gen{2},
														distrib{0, array_len - 1},
														//distrib{0,0},
														serv_socket{nnxx::SP, nnxx::REP} {
	std::cout << wm << std::endl;
	start_server();
}

void Server::start_server(){

	serv_socket.bind("tcp://127.0.0.1:" + std::to_string(port));

	while(true){

		QueryConfig query_config;
		query_config.ParseFromString( nnxx::to_string(serv_socket.recv()) );

		Elgamal::PublicKey pubt;
		pubt.setStr(query_config.pub_key());

		Elgamal::PrivateKey prvt;
		prvt.setStr(query_config.priv_key());

		log.information("================================================== Attending query");
		log.information("Publicey (pubt): " + pubt.getStr() );
		log.information("Private  (prvt): " + prvt.getStr() );
		log.information("Query size     : " + std::to_string(query_config.query_size()));

		TextConfig text_config;
		text_config.set_lg_sigma( lg_sigma );
		text_config.set_text_len( text_len );
		serv_socket.send( text_config.SerializeAsString() );

		attend_query( query_config, pubt, prvt);
	}
}

void Server::attend_query(QueryConfig &query_config, Elgamal::PublicKey &pubt, Elgamal::PrivateKey &prvt){

	int prev_r=0;
	int current_r= distrib(gen);

	int cur_char_r = 0;
	int prev_char_r[] = {0,0,0};
	int prev_char_r_index = 0;

	for(int i=0; i< query_config.query_size(); i++){

		log.information(std::to_string(i) + " ================================================== NEXT QUERY CHAR ==================================================" );

		for(int vi=0; vi < lg_sigma; vi++){

			if(vi == (lg_sigma-1) && i == (query_config.query_size() - 1) ) //remove r for the last iteration.
				current_r = 0;

			for(int left_right=0; left_right < 2; left_right++){
				log.information(std::to_string(vi) + " (vi) ---> Query" );

				log.information("-------------------------------------------------- Current r: " + std::to_string(current_r) + " ---- Prev r: " + std::to_string(prev_r));
				EncIndex enc_index;
				enc_index.ParseFromString( nnxx::to_string( serv_socket.recv() ) );

				QueryResult query_res;

				if(enc_index.mismatch()){
					prev_char_r_index = (prev_char_r_index + 1) % 3;

					cout << "===============***** MISMATCH!!!" <<  endl;
					cout << "===============***** using: " << prev_char_r[prev_char_r_index] <<  endl;
					prev_r = prev_char_r[prev_char_r_index];
					cout << " -> corrigiendo: ----------------------------------------------- Current r: " + std::to_string(current_r) + " ---- Prev r: " + std::to_string(prev_r) << endl;
				}

				Elgamal::CipherText res = wm.query_rankCF_pos( enc_index,  vi, prev_r, current_r, prvt);

				query_res.set_cipher_index( res.getStr() );
				log.information( "Sending ciph pos: " + query_res.cipher_index() );
				serv_socket.send( query_res.SerializeAsString() );
			}

			prev_r = current_r;
			current_r = distrib(gen);
		}

		prev_char_r[ prev_char_r_index] = prev_r;
		prev_char_r_index = (prev_char_r_index + 1) % 3;

		cout << "Current prev_char_r: ";
		for(auto &v: prev_char_r){
			cout << v << ", ";
		}
		cout << endl;

		if(i == (query_config.query_size() - 1) ){ //finish message, from client. Client should ask for penultimate r
			FinishCommunication finish;
			finish.ParseFromString( nnxx::to_string(serv_socket.recv()) );

			FinishCommunication finish_res;

			if( finish.request_r() ){
				prev_char_r_index = (prev_char_r_index + 1) % 3;
				finish_res.set_penultimate_r( prev_char_r[prev_char_r_index] );
				log.information("Client solicitate penultimate_r");
			}

			serv_socket.send( finish_res.SerializeAsString() );
		}
	}
}
