#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <exception>
#include "../includes/UI.hpp"

using namespace std;


info Menu_creation_partie::run(info information)
{
  information.ishost=TRUE;
  int largeur,len_str;
  bool setting_one = TRUE;
  bool setting_two = TRUE;
  bool setting_three = TRUE;
  bool setting_four = TRUE;
  int nombre1, nombre2, nombre3, nombre4;
  int set = 0;
  int ret =0;
  initscr();
  echo(); //permet de voir ce qu'on ecrit au clavier

  //les etapes ci dessous servent a initialiser les differents messages
  char str[15];
  char str2[15];
  char str3[15];
  char str4[15];
  string msg_nombre_equipe="Veuillez entrer le nombre d'équipe (min 2/max 4) :";
  string msg_nombre_lombric="Veuillez entrer le nombre de lombric par équipe (max 8 par équipe):";
  string msg_titre= "Création de partie";
  string msg_temps="Temps maximum (en seconde)pour chaque tour (max 100)  :";
  string msg_map ="Veuillez entrer l'id de la carte (entre 1 et 3)";
  string msg_confirmation="Pour confirmer, appuyez sur ENTER";
  string msg_quitter="Pour revenir en arriere, appuyez sur RETURN";
  WINDOW *nombre_equipe,*nombre_lombric,*temps,*map,*confirmer;
  getmaxyx(stdscr,len_str,largeur);


  len_str=static_cast<int>(msg_titre.size());
  //on print le titre
  draw(1,(largeur/2)-(static_cast<int>(len_str/2)),msg_titre.c_str());

  //on crée les differentes sous-window
  confirmer=newwin(5,50,15,(largeur/2)-18);
  nombre_equipe= newwin(3,20,4,2);
  nombre_lombric= newwin(3,20,7,2);
  temps=newwin(3,20,10,2);
  map=newwin(3,20,13,2);

  //on crée des bordures
  box(nombre_lombric,0,0);
  box(temps,0,0);
  box(nombre_equipe,0,0);
  box(map,0,0);

  //on print les messages
  draw(5,23,msg_nombre_equipe);
  draw(8,23,msg_nombre_lombric);
  draw(11,23,msg_temps);
  draw(14,23,msg_map);

  //on refresh les window
  refresh();
  wrefresh(nombre_equipe);
  wrefresh(nombre_lombric);
  wrefresh(temps);
  wrefresh(map);

  //on prend les infos qu'on ecrit au clavier
  mvwgetnstr(nombre_equipe,1,1,str,14);
  //on vérifie si l'input est conforme aux attentes
  try
  {
    nombre1 = stoi(str);
  } catch (invalid_argument &){ //stoi() renvoi une exception de type std::invlid_argument si l'entré n'est pas bonne
    setting_one = FALSE;
    ret++;
  }
  if (nombre1 > 4 || nombre1 < 2)
  {
    setting_one = FALSE;
    set++;
  }


  mvwgetnstr(nombre_lombric,1,1,str2,14);
  //on vérifie si l'input est conforme aux attentes
  try
  {
    nombre2 = stoi(str2);
  } catch (invalid_argument &){
    setting_two = FALSE;
    ret++;
  }
  if (nombre2 > 8)
  {
    setting_two = FALSE;
    set++;
  }


  mvwgetnstr(temps,1,1,str3,14);
  //on vérifie si l'input est conforme aux attentes
  try
  {
    nombre3 = stoi(str3);
  } catch (invalid_argument &){
    setting_three = FALSE;
    ret++;
  }
  if (nombre3 > 100)
  {
    setting_three = FALSE;
    set++;
  }

  mvwgetnstr(map,1,1,str4,14);
  try
  {
    nombre4 = stoi(str4);
  } catch (invalid_argument &){
    setting_four = FALSE;
    ret++;
  }
  if (nombre4 < 1 || nombre4 > 3)
  {
    setting_four = FALSE;
    set++;
  }

  //on voit plus le curseur de la souris
  curs_set(FALSE);
  refresh();
  //on print le msg de confirmation et d'annulation
  print_string_window(confirmer,1,1,msg_confirmation.c_str());
  len_str=static_cast<int>(msg_quitter.size());
  draw(17,(largeur/2)-(static_cast<int>(len_str/2 -1)),msg_quitter.c_str());
  refresh();

  //on active les fleches
  keypad(confirmer, TRUE);
  int boucle=1;
  int key;
  noecho();
  while (boucle)
  {
    key=wgetch(confirmer); //on capture la touche utiliser
    switch(key)
    {

      case 10: //si c'est ENTER, on confirme
        boucle=0;
        if (set >= 2 || ret >= 2 || set+ret > 2)
        {
          if (information.id == CHANGE_GAME_PARAM)
          {
            information.id = WARNING_SECOND_COME_WRONG_PARAMETERS_SCREEN;
            break;
          }
          else
          {
            information.id = WARNING_FIRST_COME_WRONG_PARAMETERS_SCREEN;
            break;
          }
        }
        if(!setting_one)
        {
          if (information.id == CHANGE_GAME_PARAM)
          {
            information.id = WARNING_SECOND_COME_WRONG_FIRST_PARA_SCREEN;
            break;
          }
          else
          {
            information.id = WARNING_FIRST_COME_WRONG_FIRST_PARA_SCREEN;
            break;
          }
        }
        if(!setting_two)
        {
          if (information.id == CHANGE_GAME_PARAM)
          {
            information.id = WARNING_SECOND_COME_WROND_SECOND_PARA_SCREEN;
            break;
          }
          else
          {
            information.id = WARNING_FIRST_COME_WROND_SECOND_PARA_SCREEN;
            break;
          }
        }
        if(!setting_three)
        {
          if (information.id == CHANGE_GAME_PARAM)
          {
            information.id = WARNING_SECOND_COME_WRONG_THIRD_PARA_SCREEN;
            break;
          }
          else
          {
            information.id = WARNING_FIRST_COME_WRONG_THIRD_PARA_SCREEN;
            break;
          }
        }
        if(!setting_four)
        {
          if (information.id == CHANGE_GAME_PARAM)
          {
            information.id = WARNING_SECOND_COME_WRONG_FOUR_PARA_SCREEN;
            break;
          }
          else
          {
            information.id = WARNING_FIRST_COME_WRONG_FOUR_PARA_SCREEN;
            break;
          }
        }
        else
        {
          bool isOk = true;//devient false s'il y'a un problème à la création d'une room
          if (information.id != CHANGE_GAME_PARAM)
          {
            isOk = information.client->createRoom();
          }
          if(isOk){
              information.client->setTimeRound(static_cast<uint32_t>(nombre3));//durée par tours
              information.client->set_nrb_lombrics(static_cast<uint32_t>(nombre2));//nombre lombrics
              information.client->setMap(static_cast<uint32_t>(nombre4));//la map
              information.client->set_nbr_equipes(static_cast<uint32_t>(nombre1));
              information.id=ROOM_SCREEN;
              break;
          }
          else{
              information.id=MAIN_MENU_SCREEN;
              break;
          }
        }
      case 263: //si c'est DELETE, on retourne en arriere
        boucle=0;
        information.id=MAIN_MENU_SCREEN;
        break;

      default:
        break;

    }
    usleep(MENU_SLEEP_TIME);
  }
  //print_string_window(confirmer,1,1,msg_confirmation.c_str());
  clear();
  delwin(nombre_equipe);
  delwin(nombre_lombric);
  delwin(temps);
  delwin(map);
  delwin(confirmer);
  endwin(); //on free les window et on quitte
  return information;
}


int Menu_creation_partie::envoyer_info_client()
{
  return 0;
}

int Menu_creation_partie::recevoir_info_client()
{
  return 0;
}
