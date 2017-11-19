#ifndef PING_PONG_H
#define PING_PONG_H

#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <iostream>

using namespace std;

template<typename T>
class ping_pong
{
    T _slots[2];
    bool _slot_has_data[2];
    
    size_t _slot_size;
    
    int _read_index;
    
    pthread_mutex_t _lock[2];
    
    public:
    
        ping_pong();
        ~ping_pong();

        void read(T ** data);
        void write(T * data);
        void swap();
};

template<typename T>
ping_pong<T>::ping_pong()
{
    _slot_size = sizeof(T);
    
    _slot_has_data[0] = false;
    _slot_has_data[1] = false;
    
    _read_index = 0;
    
    pthread_mutex_init(&_lock[0], nullptr);
    pthread_mutex_init(&_lock[1], nullptr);
}

template<typename T>
ping_pong<T>::~ping_pong()
{
    _slot_size = -1;
    
    _slot_has_data[0] = false;
    _slot_has_data[1] = false;
    
    _read_index = -1;
    
    pthread_mutex_destroy(&_lock[0]);
    pthread_mutex_destroy(&_lock[1]);
}

template<typename T>
void ping_pong<T>::read(T ** data)
{
    pthread_mutex_lock(&_lock[_read_index]);
    
    if( !_slot_has_data[_read_index] )
    {
        pthread_mutex_unlock(&_lock[_read_index]);
        cout << "nothing to read\n";
        return;
    }
    
    if( *data == nullptr )
        *data = new T;   
        
    memcpy(*data, &_slots[_read_index], _slot_size);
    _slot_has_data[_read_index] = false;
    
    pthread_mutex_unlock(&_lock[_read_index]);
}

template<typename T>
void ping_pong<T>::write(T * data)
{
    pthread_mutex_lock(&_lock[1 - _read_index]);
    
    if( data == nullptr )
    {
        pthread_mutex_unlock(&_lock[1 - _read_index]);
        cout << "nothing to write\n";
        return;
    }
    
    memcpy(&_slots[1 - _read_index], data, _slot_size);
    _slot_has_data[1 - _read_index] = true;
    swap();
    
    pthread_mutex_unlock(&_lock[_read_index]);
}

template<typename T>
void ping_pong<T>::swap()
{
    pthread_mutex_lock(&_lock[_read_index]);
    
    _read_index = 1 - _read_index;
    
    pthread_mutex_unlock(&_lock[1 - _read_index]);
}

#endif