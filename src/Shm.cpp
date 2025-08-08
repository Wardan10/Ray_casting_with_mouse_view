#include "Shm.h"

Shm::Shm(const char* name, size_t size, bool create) {
    this->name = name;
    this->size = size;
    if (create) {
        this->fd = shm_open(this->name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
        if (this->fd == -1) {
            throw std::runtime_error("Cannot setup shared memory");
        }
    }
    else {
        this->fd = shm_open(this->name, O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
        if (this->fd == -1) {
            throw std::runtime_error("Cannot access shared memory");
        }
    }
    if (ftruncate(this->fd, this->size) == -1) {
        throw std::runtime_error("cannot resize shared memory");
    }
    auto ptr=mmap(0, this->size, PROT_WRITE | PROT_READ, MAP_SHARED, this->fd, 0);
    this->mutex=(pthread_mutex_t*)ptr;
    this->buffer = (uint8_t*)ptr + sizeof(pthread_mutex_t);
    if (this->mutex == MAP_FAILED){
        throw std::runtime_error("Cannot setup mutex lock");
    }
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &attr);
    printf("Created Shared memory %s with size %ld\n",name,size);
}

void Shm::write(std::string msg){
    pthread_mutex_lock(mutex);
    strncpy((char *)this->buffer, msg.data(), this->size);
    pthread_mutex_unlock(mutex);    
}

const std::string Shm::read() {
    std::string msg;
    pthread_mutex_lock(mutex);
    msg=(char*)buffer;
    pthread_mutex_unlock(mutex);
    return msg;
}


Shm::~Shm() {
    int res = pthread_mutex_trylock(mutex);
    if (res == 0) {
        pthread_mutex_unlock(mutex);
    } else if (res == EBUSY) {
        std::cerr << "Mutex still in use, skipping destroy\n";
    } else {
        perror("Trylock failed");
    }
    if (this->mutex && this->mutex != MAP_FAILED) {
        pthread_mutex_destroy(this->mutex);
        munmap(this->mutex, this->size);
    }
    close(this->fd);
    shm_unlink(this->name);
}
