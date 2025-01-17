#include "../includes/client.hpp"

bool Client::isStarted(){
	return started;
}

void Client::shoot(uint32_t id_arme, uint32_t force, double angle){//joueur tir
	message m{};

	//constuction de l'objet à envoyer
	force = force==0?1:force;
	Tir obj;
	obj.set_id_arme(id_arme);
	obj.set_force(force);
	obj.set_angle(angle);

	obj.SerializeToString(&m.text); //convertis en string pour l'envoyer au serveur pour l'envoyer au serveur
	m.type = SHOOT;

	sendMutex.lock();
	sendMessage(m, true);
	sendMutex.unlock();
}

std::vector<std::string> Client::getTableUpdate(){
	std::vector<std::string> res; //vecteur vide
	if (mutexPartie.try_lock()){//toutes ou aucunes des informations on été recues
		if(tableUpdate.size() == 3){
			res = tableUpdate;
			tableUpdate.clear(); //vide tableau
		}
		mutexPartie.unlock();
	}
	return res;
}

void Client::updatePos(uint32_t id, uint32_t x, uint32_t y){
	message m{};

	//constuction de l'objet à envoyer
	Lomb_pos obj;
	obj.set_id_lomb(id);
	obj.set_pos_x(x);
	obj.set_pos_y(y);

	obj.SerializeToString(&m.text);//convertis en string pour l'envoyer au serveur pour l'envoyer au serveur
	m.type = POS_LOMB_S;

	sendMutex.lock();
	sendMessage(m);
	sendMutex.unlock();
}

infoPartie_s* Client::getGameInfo(){
  /*
	m.type = GET_GAME_INFO;
	m.text = "";

	std::string* reponse = waitAnswers(GAME_INFO_R,m);
  */

	while(!thisGame){
		usleep(20);
	};

  	return thisGame;
}

void Client::endTour(std::vector<uint32_t> deadLombrics){
  message m{};

  End_tour obj;
	//remplis le proto-buff
  for(auto i=deadLombrics.begin();i!=deadLombrics.end();i++){
    obj.add_id_lomb_mort(*i);
  }
  obj.SerializeToString(&m.text);//convertis en string pour l'envoyer au serveur pour l'envoyer au serveur
	m.type = END_TOUR;

	sendMutex.lock();
	sendMessage(m);
	sendMutex.unlock();
}

void Client::endGame(){
	message m{};
	m.type = END_GAME;
	m.text = ""; //serveur n'a besoin d'aucunes infos

	sendMutex.lock();
	sendMessage(m);
	sendMutex.unlock();
}

bool Client::getIsEnded(){
	return end;
}

void Client::resetGameParam(){
	if(thisGame)deleteinfoPartie_s(thisGame);
	thisGame = nullptr;
	end = false;
}

int Client::getCurrentWeapon(){
	return currentWeapon;
}

void Client::changeWeapon(uint32_t id){
	message m{};
	m.type = CLIENT_CHANGE_WEAPON;
	Change_weapon obj;
	obj.set_id_weapon(id);

	obj.SerializeToString(&m.text);//convertis en string pour l'envoyer au serveur pour l'envoyer au serveur


	sendMutex.lock();
	sendMessage(m, true);
	sendMutex.unlock();
}

void Client::sendInGameMessage(std::string text){
	message m{};
	m.type = CLIENT_SEND_IN_GAME_MESSAGE;
	if(text.size()){
		Chat obj;
		obj.set_pseudo("");
		obj.set_msg(text);

		obj.SerializeToString(&m.text);//convertis en string pour l'envoyer au serveur pour l'envoyer au serveur


		sendMutex.lock();
		sendMessage(m, true);
		sendMutex.unlock();
	}
}

std::vector<chat_r> Client::getInGameMessage(){
	std::vector<chat_r> res = inGameMessage;
	inGameMessage.clear();
	return res;
}
