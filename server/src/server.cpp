#include "server.hpp"
#include "El_Gammal_config.hpp"
#include "enc_index.pb.h"

#include <nnxx/reqrep.h>
#include <nnxx/socket.h>
#include <nnxx/message.h>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

Server::Server (int port_, std::string file_name_) : file_name{file_name_}, port{port_}, wm{file_name}, serv_socket{nnxx::SP, nnxx::REP}{

	std::cout << wm << std::endl;

	lg_sigma = wm.get_lg_sigma();
	text_len = wm.get_text_len();
	array_len = text_len * 2;

	start_server();

}

void Server::start_server(){

	serv_socket.bind("tcp://127.0.0.1:" + std::to_string(port));

	while(true){
	
		QueryConfig query_config;
		query_config.ParseFromString( nnxx::to_string(serv_socket.recv()) );

		Elgamal::PublicKey pubt;
		pubt.setStr(query_config.pub_key());

		log.information("================================================== Attending query");
		log.information("Publicey (pubt): " + pubt.getStr() );
		log.information("Query size     : " + std::to_string(query_config.query_size()));

		TextConfig text_config;
		text_config.set_lg_sigma( lg_sigma );
		text_config.set_text_len( text_len );
		serv_socket.send( text_config.SerializeAsString() );

		attend_query( query_config);
		attend_query( query_config);
	}
}

void Server::attend_query(QueryConfig &query_config){

	for(int i=0; i< query_config.query_size(); i++){
		log.information(std::to_string(i) + " ================================================== Query" );

		for(int vi=0; vi < lg_sigma; vi++){
			log.information(std::to_string(vi) + " (vi) ---> Query" );
			EncIndex enc_index;
			enc_index.ParseFromString( nnxx::to_string( serv_socket.recv() ) );

			for (int j = 0; j < enc_index.query_size(); ++j)
				log.information(std::to_string(j) + " - " + "[" + enc_index.query(j) + "]");

			QueryResult query_res;
			Elgamal::CipherText res = wm.query_rankCF_pos( enc_index,  vi);

			query_res.set_cipher_index( res.getStr() );
			log.information( "Sending ciph pos: " + query_res.cipher_index() );
			serv_socket.send( query_res.SerializeAsString() );
		}
	}
}
