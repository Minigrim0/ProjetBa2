#include "../includes/LesFenetres_QT.hpp"

#include "../UI/src/Salon_HoteQt_ui.hpp"

SalonQT::SalonQT(int id, MainWindow *parent, Client* cli):
WindowQT(id, parent, client){
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SalonQT::update_para);
    setTimerIntervalle(200);

  page = new Ui::Salon_HoteWidget;
  page->setupUi(this);

  connect(page->Leave_roomToolButton, SIGNAL(clicked()), this, SLOT(leave_room()));

  signalMapper->setMapping(page->Change_ParametreToolButton, SET_GAME_PARAM);
  connect(page->Change_ParametreToolButton, SIGNAL(clicked()), signalMapper, SLOT(map()));

  connect(page->Send_InvitationToolButton, SIGNAL(clicked()), this, SLOT(sendGameInvit()));

  connect(page->Confim_EquipetoolButton, SIGNAL(clicked()), this, SLOT(change_equipe()));

  connect(page->Lets_PlayToolButton, SIGNAL(clicked()), this, SLOT(play()));

}


void SalonQT::initWindow(){
    client->resetGameParam();
    bool room = true;

    page->Equipe_quatreplainTextEdit->setVisible(true);
    page->Equipe_troisplainTextEdit->setVisible(true);
    page->Equipe_quatreLabel->setVisible(true);
    page->Equipe_troisLabel->setVisible(true);

    //parent->setStyleSheet("background-image: url(:/wallpaper/UI/Resources/cropped-1920-1080-521477.jpg);");

    if (id == ROOM_SCREEN){
      room = client->createRoom();
    }

    if(!room){
      parent->setPage(MAIN_MENU_SCREEN);
      }
    else{
      std::cout << "Room correctement crée" << std::endl;
    }

    if (id == ROOM_INVITEE_SCREEN)
    {
      page->Lets_PlayToolButton->setVisible(false);
      page->Change_ParametreToolButton->setVisible(false);
      page->Send_InvitationToolButton->setVisible(false);
      page->InvitationLabel->setVisible(false);
      page->InvitationComboBox->setVisible(false);
    }
    else{
      friendsList = client->getFriendList();

      page->InvitationComboBox->clear();
      for(int i=0; i<friendsList.size(); ++i){
          page->InvitationComboBox->addItem(QString(friendsList.at(i).c_str()));
      }
    }
    std::string nbr_equipe_str = "";
    std::string nbr_lombric_str = "";
    std::string time_round_str = "";
    std::string map_str = "";
    std::string pseudo = "";
    std::string temps_eau_str = "";
    std::string proba_caisse_str = "";
    int current_equipe = 0;
    int nbr_equipe, nbr_lombric, time_round, map, len_tab, temps_eau, proba_caisse;
    vector<playerTeam> joueur_in_room;
    paramsPartie Para_partie;

    //on clear les plain text edit
    page->Equipe_uneplainTextEdit->clear();
    page->Equipe_deuxplainTextEdit->clear();
    page->Equipe_troisplainTextEdit->clear();
    page->Equipe_quatreplainTextEdit->clear();
    page->SpectateurplainTextEdit->clear();

    //########################################################""

    if(id == ROOM_SCREEN || id == ROOM_INVITEE_SCREEN){

      infoPartie = client->getInfoRoom();
      joueur_in_room = infoPartie.pseudos;
      map = infoPartie.map;
      nbr_equipe = infoPartie.nbr_eq;
      nbr_lombric = infoPartie.nbr_lomb;
      time_round = infoPartie.time_round;
      temps_eau = infoPartie.time;
      proba_caisse = infoPartie.probabilite;

      len_tab = static_cast<int>(joueur_in_room.size());
      for (int i = 0; i< len_tab; i++)
      {
        pseudo = joueur_in_room[static_cast<unsigned int>(i)].pseudo;
        current_equipe = joueur_in_room[static_cast<unsigned int>(i)].id_team;
        switch (current_equipe) {
          case 0:
            page->SpectateurplainTextEdit->appendPlainText(QString(pseudo.c_str()));
            break;
          case 1:
            page->Equipe_uneplainTextEdit->appendPlainText(QString(pseudo.c_str()));
            break;
          case 2:
            page->Equipe_deuxplainTextEdit->appendPlainText(QString(pseudo.c_str()));
            break;
          case 3:
            page->Equipe_troisplainTextEdit->appendPlainText(QString(pseudo.c_str()));
            break;
          case 4:
            page->Equipe_quatreplainTextEdit->appendPlainText(QString(pseudo.c_str()));
            break;
          default:
            break;
        }
      }
      switch (map) {
        case 1:
          map_str = "Hijacked";
          break;
        case 2:
          map_str = "Warzone";
          break;
        case 3:
          map_str = "No Man's Land";
          break;
        default:
          break;
      }

      nbr_equipe_str = "Nombre d'équipes : " + to_string(nbr_equipe);
      nbr_lombric_str = "Nombre max lombric : " + to_string(nbr_lombric);
      time_round_str = "Temps max/tour : " + to_string(time_round);
      temps_eau_str = "Temps avant la montée des eaux : " + to_string(temps_eau);
      proba_caisse_str = "Probabilité des caisses de vie : " + to_string(proba_caisse) + '%';
      map_str = "Carte : " + map_str;

      page->CarteLabel->setText(QString(map_str.c_str()));
      page->Nombre_LombricLabel->setText(QString(nbr_lombric_str.c_str()));
      page->Nombre_EquipeLabel->setText(QString(nbr_equipe_str.c_str()));
      page->Temps_MaxTourLabel->setText(QString(time_round_str.c_str()));
      page->Temps_EauLabel->setText(QString(temps_eau_str.c_str()));
      page->Proba_CaisseLabel->setText(QString(proba_caisse_str.c_str()));


      page->Equipe_quatreplainTextEdit->setVisible(true);
      page->Equipe_troisplainTextEdit->setVisible(true);
      page->Equipe_quatreLabel->setVisible(true);
      page->Equipe_troisLabel->setVisible(true);

      if( nbr_equipe== 2)
      {
        page->Equipe_quatreplainTextEdit->setVisible(false);
        page->Equipe_troisplainTextEdit->setVisible(false);
        page->Equipe_quatreLabel->setVisible(false);
        page->Equipe_troisLabel->setVisible(false);
      }
      if( nbr_equipe == 3)
      {
        page->Equipe_quatreplainTextEdit->setVisible(false);
        page->Equipe_quatreLabel->setVisible(false);
      }
  }
}
void SalonQT::sendGameInvit(){

  std::string choose_friend = page->InvitationComboBox->currentText().toStdString();
  client->addToGame(choose_friend);

}

void SalonQT::update_para(){
  std::string nbr_equipe_str = "";
  std::string nbr_lombric_str = "";
  std::string time_round_str = "";
  std::string map_str = "";
  std::string temps_eau_str = " ";
  std::string pseudo = "";
  std::string proba_caisse_str = "";
  int current_equipe = 0;
  int nbr_equipe, nbr_lombric, time_round, map, len_tab, temps_eau, proba_caisse;
  vector<playerTeam> joueur_in_room;
  paramsPartie Para_partie;

  //on clear les plain text edit
  //########################################################""
  joueur_in_room = client->getTeams();
  Para_partie = client->getParamsPartie();
  map = Para_partie.map;
  nbr_equipe = Para_partie.nbr_equipes;
  nbr_lombric = Para_partie.nbr_lombs;
  time_round = Para_partie.time_round;
  temps_eau = Para_partie.time;
  proba_caisse = Para_partie.probabilite;

  len_tab = static_cast<int>(joueur_in_room.size());
  page->Equipe_uneplainTextEdit->clear();
  page->Equipe_deuxplainTextEdit->clear();
  page->Equipe_troisplainTextEdit->clear();
  page->Equipe_quatreplainTextEdit->clear();
  page->SpectateurplainTextEdit->clear();

  for (int i = 0; i< len_tab; i++)
  {
    pseudo = joueur_in_room[static_cast<unsigned int>(i)].pseudo;
    current_equipe = joueur_in_room[static_cast<unsigned int>(i)].id_team;
    switch (current_equipe) {
      case 0:
        page->SpectateurplainTextEdit->appendPlainText(QString(pseudo.c_str()));
        break;
      case 1:
        page->Equipe_uneplainTextEdit->appendPlainText(QString(pseudo.c_str()));
        break;
      case 2:
        page->Equipe_deuxplainTextEdit->appendPlainText(QString(pseudo.c_str()));
        break;
      case 3:
        page->Equipe_troisplainTextEdit->appendPlainText(QString(pseudo.c_str()));
        break;
      case 4:
        page->Equipe_quatreplainTextEdit->appendPlainText(QString(pseudo.c_str()));
        break;
      default:
        break;
    }
  }

  switch (map) {
    case 1:
      map_str = "Hijacked";
      break;
    case 2:
      map_str = "Warzone";
      break;
    case 3:
      map_str = "No Man's Land";
      break;
    default:
      break;
  }

  nbr_equipe_str = "Nombre d'équipes : " + to_string(nbr_equipe);
  nbr_lombric_str = "Nombre max lombric : " + to_string(nbr_lombric);
  time_round_str = "Temps max/tour : " + to_string(time_round);
  temps_eau_str = "Temps avant la montée des eaux : " + to_string(temps_eau);
  proba_caisse_str = "Probabilité des caisses de vie : " + to_string(proba_caisse) + '%';
  map_str = "Carte : " + map_str;

  page->CarteLabel->setText(QString(map_str.c_str()));
  page->Nombre_LombricLabel->setText(QString(nbr_lombric_str.c_str()));
  page->Nombre_EquipeLabel->setText(QString(nbr_equipe_str.c_str()));
  page->Temps_MaxTourLabel->setText(QString(time_round_str.c_str()));
  page->Temps_EauLabel->setText(QString(temps_eau_str.c_str()));
  page->Proba_CaisseLabel->setText(QString(proba_caisse_str.c_str()));

  page->Equipe_quatreplainTextEdit->setVisible(true);
  page->Equipe_troisplainTextEdit->setVisible(true);
  page->Equipe_quatreLabel->setVisible(true);
  page->Equipe_troisLabel->setVisible(true);

  if( nbr_equipe== 2)
  {
    page->Equipe_quatreplainTextEdit->setVisible(false);
    page->Equipe_troisplainTextEdit->setVisible(false);
    page->Equipe_quatreLabel->setVisible(false);
    page->Equipe_troisLabel->setVisible(false);
  }
  else if( nbr_equipe == 3)
  {
    page->Equipe_quatreplainTextEdit->setVisible(false);
    page->Equipe_quatreLabel->setVisible(false);
  }
  page->Choix_EquipeSpinBox->setMaximum(nbr_equipe);
  //teste si la partie est lancéée
  if(id == ROOM_INVITEE_SCREEN && client->isStarted()){
    parent->setPage(GAME_SCREEN);
  }
}

void SalonQT::play(){
  vector<playerTeam> joueur_in_room;
  bool player = false;
  int len_tab;
  joueur_in_room = client->getTeams();
  len_tab = static_cast<int>(joueur_in_room.size());

  for (int i = 0; i< len_tab; i++){
    if (joueur_in_room[static_cast<unsigned int>(i)].id_team != 0){
      player = true;
      break;
    }
  }
  if (player){
    client->startGame();
    parent->setPage(GAME_SCREEN);
  }
}


void SalonQT::change_equipe(){
  int equipe = page->Choix_EquipeSpinBox->value();
  client->changeTeam(equipe);
}

void SalonQT::leave_room(){
  if (id == ROOM_INVITEE_SCREEN){
  client->quitRoom();
  parent->setPage(MAIN_MENU_SCREEN);
  }

}


SalonQT::~SalonQT(){
    delete page;
}
