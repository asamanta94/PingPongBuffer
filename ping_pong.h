#ifndef PING_PONG_H
#define PING_PONG_H

#include <string.h>
#include <stdint.h>
#include <pthread.h>

enum status_t
{
    STATUS_OK = 0,
    ERROR_NO_DATA,
    ERROR_EMPTY_MEM
};

class ping_pong
{
    uint8_t * _slots[2];
    bool _slot_has_data[2];
    
    size_t _slot_size;
    
    int _read_index;
    
    pthread_mutex_t _lock[2];
    
    public:
    
        ping_pong(size_t size);
        ~ping_pong();

        status_t read(uint8_t * data);
        status_t write(uint8_t * data);
        void swap();
};

#endif