#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <pthread.h>
#include <cstdint>

class Shm {
    public:
        Shm(const char* name="shm", size_t size=0, bool create=true);

        void write(std::string msg);
        const std::string read();
        // bool fail();
        // bool lock();
        // bool unlock();
        ~Shm();
    private:
        size_t size;
        size_t buffer_size;
        const char* name;  
        int fd;
        pthread_mutex_t* mutex;     //lock address
        u_int8_t* buffer;           // memory address
};