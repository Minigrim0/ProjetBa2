#include <iostream>
#include <thread>
#include <unistd.h>

#include "../includes/game_thread.hpp"
#include "../includes/game.hpp"
#include "../includes/listener.hpp"
#include "../includes/utils.hpp"
#include "../includes/broker.hpp"
#include "../proto/src/user.pb.h"

int game_thread(std::string chan_sub, uint32_t room_id, uint32_t owner){

    bool game_running = true;
    int current_step = STEP_ROOM;
    Game current_game(owner, room_id);

    ZMQ_msg zmqmsg;

    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5563");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, chan_sub.c_str(), strlen(chan_sub.c_str()));

    std::cout << "room started on " << chan_sub << std::endl;

    while(game_running){
        switch(current_step){
            case STEP_ROOM:{
                while (current_step == STEP_ROOM){
                    std::string address = s_recv(subscriber);
                    std::string contents = s_recv(subscriber);

                    zmqmsg.ParseFromString(contents);
                    current_game.handle_quit(zmqmsg, &current_step);
                    current_game.handle_room(zmqmsg, &current_step);
                }

                break;
            }
            case STEP_GAME:{
                std::cout << "You're in the game" << std::endl;
                {
                    size_t opt_value = 500;
                    subscriber.setsockopt(ZMQ_RCVTIMEO, &opt_value, sizeof(int));
                }

                while(current_step == STEP_GAME){
                    std::string address = s_recv(subscriber);
                    if(address.size()){ // The receive just timed out
                        std::string contents = s_recv(subscriber);
                        zmqmsg.ParseFromString(contents);
                        current_game.handle_quit(zmqmsg, &current_step);
                        current_game.handle_game(zmqmsg, &current_step);
                    }
                    if(current_game.check_round_time()){ //Checking the time
                        std::cout << "changing round" << std::endl;
                        current_game.end_round(&current_step); // If the time's up, ending the round
                    }
                }
                break;
            }
            case STEP_ENDSCREEN:
                std::cout << "Closing room " << room_id << " in DataBase." << std::endl;
                DataBase_mutex.lock();
                db.close_room(room_id);
                DataBase_mutex.unlock();
                game_running = false;
                break;
            default:
                std::cout << "An error occured" << std::endl;
                game_running = false;
        }
    }

    std::cout << "Closing room " << room_id << std::endl;

    return EXIT_SUCCESS;
}
