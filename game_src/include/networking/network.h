#include "Shm.h"
#include <thread>
# define FRAME_SIZE 4092
# define MUTEX_SIZE 40
# define TOTAL_SIZE FRAME_SIZE+MUTEX_SIZE

class Network{
    private:
    bool RUN_THREADS=true;
    Shm* get_data;
    Shm* send_data;
    std::thread* handle_get_data;
    std::thread* handle_send_data;
    public:
    void get_data_loop();
    void send_data_loop();
    void initialise_connection(std::string send_stream,std::string get_stream);
    void close_connection();
};