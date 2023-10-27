# include "server.hpp"
#include "El_Gammal_config.hpp"
/*#include "nngpp/aio.h"*/
/*#include "nngpp/msg.h"*/
/*#include "nngpp/view.h"*/

#include <nnxx/reqrep.h>
#include <nnxx/socket.h>
#include <nnxx/message.h>
#include <sstream>
#include <string>
#include <iomanip>
//#include <nngpp/nngpp.h>
//#include <nngpp/protocol/rep0.h>

using namespace std;

Server::Server (int port_, std::string file_name_) : file_name{file_name_}, port{port_}, wm{file_name}{

	std::cout << wm << std::endl;

	nnxx::socket serv_socket{ nnxx::SP, nnxx::REP };
	serv_socket.bind("tcp://127.0.0.1:" + std::to_string(port));

	int vi = 0;
	nnxx::message msg;

	int lg_sigma = wm.get_lg_sigma();
	int text_len = wm.get_text_len();
	int array_len = text_len * 2;

	while(true){
		std::string pubk = nnxx::to_string( serv_socket.recv() );
		serv_socket.send("PublicKey received");

		Elgamal::PublicKey pubt;
		pubt.setStr(pubk);

		log.information("Publicey (pubt): " + pubt.getStr());

		std::vector<Elgamal::CipherText> sec_vec(array_len);

		for(int i=0; i<array_len; i++){
			nnxx::message msg = serv_socket.recv();
			std::string rec_msg = nnxx::to_string(msg);
			sec_vec[i].fromStr(rec_msg);

			std::cout << std::setw(3) << i << " --- " << sec_vec[i] << std::endl;
			serv_socket.send( "server_recibido" );
		}

		auto msg = serv_socket.recv();
		std::cout << "Recibido: " << msg << std::endl;
		Elgamal::CipherText res = wm.query_rankCF_pos( sec_vec,  vi);
		std::string response = res.getStr();
		std::cout << "Sending ciph pos: " << response << std::endl;
		serv_socket.send( response );

		vi = (vi + 1)% lg_sigma;
	}
}
