#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <string>
#include <vector>

#include <sys/types.h>
#include <dirent.h>

#include "../includes/UI.hpp"

info Choose_replay::run(info information)
{
    initscr();
    nodelay(stdscr, TRUE);//pour que les getch ne soient bloquant
    noecho();//empêche d'écrire dans la console
    curs_set(FALSE);//affiche pas le curseur

    std::vector<std::string> vectorReplays;
    DIR* dirp = opendir(DEFAULT_REPLAY_PATH);
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        if(dp->d_name[0] != '.')vectorReplays.push_back(dp->d_name);
    }
    closedir(dirp);

    int focus = 0;//indice selectionné
    int decalage = 0;

    WINDOW* win = nullptr;
    int width, height;
    bool must_draw = true;
    bool running = true;
    int touch;

    keypad(win, true);


    while(running){
        if(must_draw){
            must_draw = false;
            if(win)delwin(win);
            getmaxyx(stdscr, height, width);
            win = newwin(height, width, 0, 0);
            werase(win);
            box(win,0,0);
            print_string_window(win, 1, width/2, "Choisissez un replay");

            height -= 3;//hauteur disponible pour afficher les infos
            height /= 2;//ça n'a aucun sens

            std::string text = "echo Height: " + std::to_string(height) + " >> out.txt";
            system(text.c_str());

            int taille = vectorReplays.size()-decalage>height?height:vectorReplays.size()-decalage;

            for(int i=decalage; i<taille+decalage; ++i){
                print_string_window(win, 2*(i-decalage)+3, width/2, vectorReplays[i]);
            }

            print_string_window(win, 2*(focus-decalage)+3, width/2 - 3, "->");
        }
        touch = wgetch(win);

        switch(touch){
            case NAVIGATE_UP:
                if(focus > 0){
                    --focus;
                    must_draw = true;
                    if(focus < decalage)--decalage;
                }
                break;
            case NAVIGATE_DOWN:
                if(focus < vectorReplays.size()-1){
                    ++focus;
                    must_draw = true;
                    if(focus+decalage>=height)++decalage;
                }
                break;
            case ENTER_KEY:
                if (vectorReplays.size()){
                    if(information.client->beginReplay(DEFAULT_REPLAY_PATH + vectorReplays.at(focus))){
                        information.id = GAME_SCREEN;
                        running = false;
                    }
                }
                break;
            case 127:
                std::cout<<"loooooooooooool"<<std::endl;
                information.id = MAIN_MENU_SCREEN;
                running = false;
                break;

        }
    }
    wclear(win);
    werase(win);
    delwin(win);
    //echo();

    return information;
}
