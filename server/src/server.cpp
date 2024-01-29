#include "server.hpp"
#include "El_Gammal_config.hpp"
#include "ROT.hpp"
#include "enc_index.pb.h"

#include <nnxx/reqrep.h>
#include <nnxx/socket.h>
#include <nnxx/message.h>
#include <random>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

Server::Server (int port_, std::string file_name_, int allowed_deletes_) : 
														port{port_}, 
														wm{file_name}, 
														file_name{file_name_}, 
														allowed_deletes{allowed_deletes_},
														lg_sigma{ wm.get_lg_sigma()},
														text_len{ wm.get_text_len()},
														array_len{ text_len * 2},
														rd{},
														//gen{rd()},
														gen{2},
														distrib{0, array_len - 1},
														//distrib{0,0},
														serv_socket{nnxx::SP, nnxx::REP} {

	nserv_sock = nng::rep::open();

	std::stringstream ss;
	ss << wm;
	log.information(ss.str());

	start_server();
}

void Server::start_server(){

	serv_socket.bind("tcp://127.0.0.1:" + std::to_string(port));
	nserv_sock.listen( "tcp://localhost:8000" );

	while(true){

		nng::buffer query_config_buffer = nserv_sock.recv();
		char* query_config_char = query_config_buffer.data<char>();
		std::string query_config_str{query_config_char};

		QueryConfig query_config;
		query_config.ParseFromString( query_config_str );

		Elgamal::PublicKey pubt;
		pubt.setStr(query_config.pub_key());

		Elgamal::PrivateKey prvt;
		prvt.setStr(query_config.priv_key());	//for debugging

		log.information("=============================================================================");
		log.information("============================== Attending query ==============================");
		log.information("=============================================================================");
		log.information("Publicey (pubt): " + pubt.getStr() );
		log.information("Private  (prvt): " + prvt.getStr() );
		log.information("Query size     : " + std::to_string(query_config.query_size()));

		TextConfig text_config;
		text_config.set_lg_sigma( lg_sigma );
		text_config.set_text_len( text_len );
		
		std::string stext_config = text_config.SerializeAsString();
		nng::view vista(stext_config.c_str(), stext_config.size());
		nserv_sock.send( vista );
		//serv_socket.send( text_config.SerializeAsString() );

		attend_query( query_config, pubt, prvt);
	}
}

void Server::attend_query(QueryConfig &query_config, Elgamal::PublicKey &pubt, Elgamal::PrivateKey &prvt){

	int prev_r=0;
	int current_r= distrib(gen);

	int cur_char_r = 0;
	int prev_char_r[] = {0,0,0};
	int prev_char_r_index = 0;

	int current_deletes{0};

	ROT rot;

	for(int i=0; i< query_config.query_size(); i++){

		log.information(std::to_string(i) + " ================================================== NEXT QUERY CHAR ==================================================" );

		for(int vi=0; vi < lg_sigma; vi++){
			log.information(std::to_string(vi) + " (vi) ---> Query" );

			if(vi == (lg_sigma-1) && i == (query_config.query_size() - 1) ) //use 0 as r for the last iteration (do not use a random r).
				current_r = 0;

			for(int left_right=0; left_right < 2; left_right++){
				log.information(std::string{(left_right? "** left " : "** right")} + " : -------------------- Current r: " + std::to_string(current_r) + " ---- Prev r: " + std::to_string(prev_r));
				
				nng::buffer enc_index_buffer = nserv_sock.recv();
				char* enc_index_char = enc_index_buffer.data<char>();
				std::string enc_index_str{enc_index_char};

				EncIndex enc_index;
				enc_index.ParseFromString(enc_index_str);

				QueryResult query_res;

				if(enc_index.mismatch()){
					current_deletes++;
					prev_r = prev_char_r[(prev_char_r_index + 1) % 3];
					prev_char_r_index = (prev_char_r_index + 2) % 3;

					log.information("===============***** MISMATCH!!! : " + std::to_string(current_deletes),__FILE__,__LINE__);
					log.information("===============***** using: " + std::to_string(prev_r),__FILE__,__LINE__);

					log.information(" -> corrigiendo: ------------ Current r: " + std::to_string(current_r) + " ---- Prev r: " + std::to_string(prev_r));
					log.information("Allowed deletes: " + std::to_string(allowed_deletes) + " --- Current deletes: " +  std::to_string(current_deletes));

					if(allowed_deletes == (current_deletes-1)){
						log.information("limit readed on allowed deletes");

						QueryResult final_result;
						final_result.set_reached_deletes(true);
						final_result.set_last_r(prev_r);

						std::string sfinal_result = final_result.SerializeAsString();
						nng::view vista(sfinal_result.c_str(), sfinal_result.size());
						nserv_sock.send( vista );
						//serv_socket.send( final_result.SerializeAsString() );
						return;
					}
				}

				std::vector<Elgamal::CipherText> enc_pos = rot.req_query(enc_index, prev_r, prvt); //remove r
				Elgamal::CipherText res = rot.RanOT(enc_pos, vi, current_r, wm);				   //new result with new random_r

				query_res.set_cipher_index( res.getStr() );
				log.information( "Sending ciph pos: " + query_res.cipher_index() );

				std::string squery_result = query_res.SerializeAsString();
				nng::view vista(squery_result.c_str(), squery_result.size());
				nserv_sock.send( vista );
				//serv_socket.send( query_res.SerializeAsString() );
			}

			prev_r = current_r;
			current_r = distrib(gen);
		}

		prev_char_r[ prev_char_r_index] = prev_r;
		prev_char_r_index = (prev_char_r_index + 1) % 3;

		std::stringstream ss;
		ss << "Current prev_char_r: ";
		for(auto &v: prev_char_r)
			ss << v << ", ";
		log.information(ss.str(),__FILE__,__LINE__);

		std::cout << "llega AQUI->>>" << endl;


		if(i == (query_config.query_size() - 1) ){ //finish message, from client. Client should ask for penultimate r
			nng::buffer finish_buffer = nserv_sock.recv();
			char* finish_char = finish_buffer.data<char>();
			std::string finish_str{finish_char};

			FinishCommunication finish;
			finish.ParseFromString( finish_str );
			finish.set_testing(100);

			FinishCommunication finish_res;
			finish_res.set_testing(777);;

			if( finish.request_r() ){
				prev_char_r_index = (prev_char_r_index + 1) % 3;
				finish_res.set_penultimate_r( prev_char_r[prev_char_r_index] );
				log.information("Client solicitate penultimate_r");
			}

			std::string sfinish_res = finish_res.SerializeAsString();
			nng::view vista(sfinish_res.c_str(), sfinish_res.size());
			nserv_sock.send( vista );
			//serv_socket.send( finish_res.SerializeAsString() );
		}
	}
}
