#include <iostream>
#include "ping_pong.h"

using namespace std;

/** Constructor 
 *  @param size - Size of data that needs 
 *                to be stored. */
ping_pong::ping_pong(size_t size)
{
    if( size <= 0 )
        throw "invalid slot size";
    
    _slot_size = size;
    
    _slots[0] = new uint8_t[_slot_size];
    _slots[1] = new uint8_t[_slot_size];
    
    _slot_has_data[0] = false;
    _slot_has_data[1] = false;
    
    _read_index = 0;
    
    pthread_mutex_init(&_lock[0], nullptr);
    pthread_mutex_init(&_lock[1], nullptr);
}

/** Destructor */
ping_pong::~ping_pong()
{
    _slot_size = -1;
    
    _slot_has_data[0] = false;
    _slot_has_data[1] = false;
    
    _read_index = -1;
    
    pthread_mutex_destroy(&_lock[0]);
    pthread_mutex_destroy(&_lock[1]);
    
    delete _slots[0];
    delete _slots[1];
}

/** Function to read data from the buffer
 *  @param data - Pointer to location where
*                 data needs to be copied to.
 */
status_t ping_pong::read(uint8_t * data)
{
    pthread_mutex_lock(&_lock[_read_index]);
    
    if( !_slot_has_data[_read_index] )
    {
        pthread_mutex_unlock(&_lock[_read_index]);
        cout << "nothing to read\n";
        return ERROR_EMPTY_MEM;
    }
        
    if( data == nullptr )
    {
        pthread_mutex_unlock(&_lock[_read_index]);
        cout << "allocate memory\n";
        return ERROR_EMPTY_MEM;
    }
        
    memcpy(data, _slots[_read_index], _slot_size);
    _slot_has_data[_read_index] = false;
    
    pthread_mutex_unlock(&_lock[_read_index]);
    
    return STATUS_OK;
}

/** Function to write data to the buffer
 *  @param data - Pointer to data which needs
 *                to be written to buffer.
 */
status_t ping_pong::write(uint8_t * data)
{
    pthread_mutex_lock(&_lock[1 - _read_index]);
    
    if( data == nullptr )
    {
        pthread_mutex_unlock(&_lock[1 - _read_index]);
        cout << "nothing to write\n";
        return ERROR_NO_DATA;
    }
    
    memcpy(_slots[1 - _read_index], data, _slot_size);
    _slot_has_data[1 - _read_index] = true;
    swap();
    
    pthread_mutex_unlock(&_lock[_read_index]);
    
    return STATUS_OK;
}

/** Function to swap read and write pointers. */
void ping_pong::swap()
{
    pthread_mutex_lock(&_lock[_read_index]);
    
    _read_index = 1 - _read_index;
    
    pthread_mutex_unlock(&_lock[1 - _read_index]);
}