#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "../includes/UI.hpp"

using namespace std;

info Menu_log_in::run(info information)
{
  int hauteur,largeur,len_str;
  string quitter = "quitter";
  initscr();
  echo();
  int taille_max_user_name=18;
  char str[19];
  bzero(str, 19);
  char str2[19];
  bzero(str2, 19);
  string msg_id="veuillez entrez votre pseudo :";
  string msg_mdp="veuillez entrez votre mot de passe :";
  string msg_titre= get_message_Window(information.id);
  string msg_confirmation="Pour confirmer, appuyez sur ENTER";
  string msg_quitter="Pour revenir en arriere, appuyez sur RETURN";
  WINDOW *user_name,*mot_de_passe,*titre,*fin,*confirmer;
  getmaxyx(stdscr,hauteur,largeur);

  len_str=static_cast<int>(msg_titre.size());
  //on cree la sous window pour le titre
  titre=newwin(2,20,1,(largeur/2)-(static_cast<int>(len_str/2)+1));
  mvwprintw(titre,1,1,msg_titre.c_str());

  //on print les msg
  len_str=static_cast<int>(msg_id.size());
  draw((hauteur/2)-7,(largeur/2)-(len_str/2),msg_id.c_str());

  len_str=static_cast<int>(msg_mdp.size());
  draw((hauteur/2)-2,(largeur/2)-(len_str/2),msg_mdp.c_str());



  //on cree les autres sous-window
  confirmer=newwin(5,50,(hauteur/2)+2,(largeur/2)-18);
  fin=newwin(5,50,(hauteur/2)+4,(largeur/2)-23);
  user_name=newwin(3,20,(hauteur/2)-6,(largeur/2)-10);
  mot_de_passe= newwin(3,20,(hauteur/2)-1,(largeur/2)-10);

  //on crée des bordures
  box(mot_de_passe,0,0);
  box(user_name,0,0);

  refresh();

  //on refresh les window
  wrefresh(user_name);
  wrefresh(mot_de_passe);
  wrefresh(titre);

  //on capture ce qu'on ecrit au clavier
  mvwgetnstr(user_name,1,1,str,taille_max_user_name);

  int taille_mdp_max=18;
  int chara;
  int decalage=0;
  noecho();
  keypad(confirmer, TRUE);
  keypad(mot_de_passe,TRUE);
  while (taille_mdp_max>0)
  {
    chara=wgetch(mot_de_passe);
    if (chara==10)
    {
      break;
    }
    if (chara==263)
    {
      if (decalage!=0)
      {
        decalage--;
        str2[decalage]= ' ';
        effacer_caractere_window(mot_de_passe,1,1+decalage,1);
        taille_mdp_max+=1;
      }
    }
    else
    {
      str2[decalage]= static_cast<char>(chara);
      print_string_window(mot_de_passe,1,1+decalage,"*");
      decalage++;
      taille_mdp_max-=1;
    }
    usleep(MENU_SLEEP_TIME);
  }

  char *ok[2]={str,str2};
  int len_str_pseudo = strlen(str);
  int len_str_password = strlen(str2);
  curs_set(FALSE);
  refresh();

  //on print les msg de confirmation et d'annulation
  print_string_window(confirmer,1,1,msg_confirmation.c_str());
  print_string_window(fin,1,1,msg_quitter.c_str());

  keypad(confirmer, TRUE);
  int boucle=1;
  int key;
  while (boucle)
  {
    /*
    fonctionnement de la boucle:
    on appuye sur Enter pour confirmer et sur DELETE pour revenir au MENU
    precedent*/
    key=wgetch(confirmer);
    switch(key)
    {

      case 10:
        boucle=0;
        if (information.id==LOGIN_SCREEN)
        {
          bool connected;
          if (len_str_pseudo != 0 || len_str_password != 0)
          {
            connected=information.client->connection(ok[0],ok[1],true);
          }
          else
          {
            connected = false;
          }

          if (connected)
          {
            string pseudo= string(ok[0]);
            information.username=pseudo;
            information.id=MAIN_MENU_SCREEN;
            break;
          }
          else
          {
            information.id=WARNING_WRONG_DATA_IN_LOGIN_SCREEN;
            break;
          }
        }
        if (information.id==REGISTER_SCREEN)
        {
          bool isenregistrer;
          if (len_str_pseudo != 0 || len_str_password != 0)
          {
            isenregistrer=information.client->connection(ok[0],ok[1],false);

          }
          else
          {
            isenregistrer = false;
          }
          if (isenregistrer)
          {
            string pseudo= string(ok[0]);
            information.username=pseudo;
            information.id=MAIN_MENU_SCREEN;
            break;
          }
          else
          {
            information.id=WARNING_PSEUDO_ALREADY_EXIST_SCREEN;
            break;
          }
        }
        information.id=MAIN_MENU_SCREEN;

        break;
      case 263:
        boucle=0;
        information.id=INIT_SCREEN;
        break;

      default:
        break;

    }
    usleep(MENU_SLEEP_TIME);
  }
  clear();
  delwin(user_name);
  delwin(mot_de_passe);
  delwin(fin);
  delwin(titre);
  delwin(confirmer);
  endwin();
  return information;
}

int Menu_log_in::envoyer_info_client()
{
  return 0;
}

int Menu_log_in::recevoir_info_client()
{
  return 0;
}
