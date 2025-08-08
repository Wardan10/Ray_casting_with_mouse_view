#include "network.h"
#include "config.h"

bool player_moved_global;
std::pair<float,float> player_pos_global;
bool player1_moved_global;
std::pair<float,float> player1_pos_global;
bool STOP_GAME=false;

void Network::get_data_loop(){
    std::string previous;
    while(RUN_THREADS){
        std::string testing=get_data->read();
        if(testing==previous)continue;
        if(testing=="terminate"){
            RUN_THREADS=false;
        }
        else{
            player1_moved_global=true;
            std::string x;
            int len=testing.size();
            int i=0;
            while(i<len){
                x.push_back(testing[i]);
                i++;
                if(testing[i]==' ')break;
            }
            i++;
            std::string y;
            while(i<len){
                y.push_back(testing[i]);
                i++;
            }
            try{
                float X=std::stof(x);
                float Y=std::stof(y);
                player1_pos_global.first=X;
                player1_pos_global.second=Y;
                previous=testing;
            }
            catch(const std::exception& e){
                std::cout<<"error converting sent data\n"<<std::endl;
            }
        }
    }
    printf("exiting get_data_loop..\n");
}

void Network::send_data_loop(){
    while (RUN_THREADS) {
        if(player_moved_global){
            float x=player_pos_global.first;
            float y=player_pos_global.second;
            std::string textx=std::to_string(x);
            std::string texty=std::to_string(y);
            textx.push_back(' ');
            for(auto i:texty)textx.push_back(i);
            send_data->write(textx);
            player_moved_global=false;
        }
        if(STOP_GAME){
            send_data->write("terminate");
            RUN_THREADS=false;
        }
    }
    printf("exiting send_data_loop..\n");
}

void Network::initialise_connection(){
    try{
        send_data= new Shm("send_data", TOTAL_SIZE, true);
        get_data= new Shm("get_data", TOTAL_SIZE, true);
    }
    catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }
    handle_get_data=new std::thread(&Network::get_data_loop,this);
    handle_send_data=new std::thread(&Network::send_data_loop,this);
}

void Network::close_connection(){
    handle_get_data->join();
    handle_send_data->join();
    std::cout<<"Clearing shared memory resources...\n"<<std::endl;
    sleep(1);
    delete handle_get_data,handle_send_data;
    delete send_data,get_data;
}