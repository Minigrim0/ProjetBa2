#ifndef game
    #define game

#include <vector>
#include <ctime>
#include "../proto/src/user.pb.h"
#include "partie_serv.hpp"
#include "in_game_player.hpp"

#define INIT_MAP_ID 1
#define INIT_NB_LOMBS 8
#define INIT_NB_TEAMS 4
#define INIT_ROUND_TIME 20
#define INIT_GLOBAL_TIME 30
#define INIT_PROBA 30
#define INIT_BOX_PV 25
#define INIT_LOMB_PV_INIT 100
#define INIT_LOMB_PV_MAX 150

//Tout ce qui est en rapport avec la fin de la partie(timer) sera geré en partie 3(eau qui monte)
class Game{
    public:
        Game(uint32_t owner, uint32_t room_id);
        virtual ~Game();

        // Game parameters
        void set_lomb(uint8_t nb_lomb);
        void set_nb_lombs(uint8_t nbr_lombs);
        void set_map(uint8_t id_map);
        void set_nb_teams(uint8_t nbr_teams);
        void set_round_time(int round_time);
        void set_global_time(int global_time);
        void set_users_team(ZMQ_msg* zmq_msg);
        void set_box_pv(int box_pv);
        void set_prob_health_box(int prob_health_box);
        void set_lomb_pv_init(int lomb_pv_init);
        void set_lomb_pv_max(int lomb_pv_max);

        void change_map(ZMQ_msg* zmq_msg);
        void change_nb_teams(ZMQ_msg* zmq_msg);
        void change_time(ZMQ_msg* zmq_msg);
        void change_round_time(ZMQ_msg* zmq_msg);
        void change_nb_lomb(ZMQ_msg* zmq_msg);
        void change_box_pv(ZMQ_msg* zmq_msg);
        void change_prob_health_box(ZMQ_msg* zmq_msg);
        void change_lomb_pv_init(ZMQ_msg* zmq_msg);
        void change_lomb_pv_max(ZMQ_msg* zmq_msg);

        void add_user(ZMQ_msg *zmq_msg);

        //Verification methods
        bool check_round_time();
        bool check_time();
        void nb_alive_teams(size_t *nbr_teams, size_t *last_team);

        uint32_t get_next_lombric_id();
        void end_round(int* current_step);
        void end_game(int* current_step, size_t last_team);
        void spawn_lombric();

        void handle_room(ZMQ_msg zmq_msg, int* current_step);
        void handle_game(ZMQ_msg zmq_msg, int* current_step);
        void handle_quit(ZMQ_msg zmq_msg, int* current_step); // Handles the event where a user quits


    private:
        Partie m_game_object; //faut set les attributs de la partie au debut du jeu

        uint8_t m_map_id;

        time_t m_begin_time_round; // Temps de début du round
        int m_max_time_round; // Durée max d'un round

        time_t m_begin_time_game; // Temps de début de partie
        int m_max_time_game; // Durée totale d'une partie en sec

        uint8_t m_lomb_pv_init;
        uint8_t m_lomb_pv_max;
        uint8_t m_box_pv;
        uint8_t m_prob_health_box;
        uint8_t m_team_nb; // Nbr equipes
        uint8_t m_lomb_nb; // nombre lombric par joueur
        uint32_t m_owner_id;
        uint32_t m_game_id;
        uint8_t m_current_player_id;
        std::vector<Joueur> m_players;
        std::vector<Sprite*> m_lombs;
        Map* m_map;

        bool m_alive_team[4];
};

#endif
