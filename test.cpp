#include <iostream>
#include <unistd.h>
#include "ping_pong.h"

using namespace std;

ping_pong * pp;
bool done = false;

void * reader(void * data)
{
    int * i = new int;
    while( *i != 9 )
    {
        pp->read(reinterpret_cast<uint8_t **>(&i));
        cout << *i << endl;
        usleep(1000);
    }
    
    pthread_exit(nullptr);
}

void * writer(void * data)
{
    for(int i = 0; i < 10; i++)
    {
        pp->write(reinterpret_cast<uint8_t *>(&i));
        usleep(10000);
    }
    
    done = true;
    
    pthread_exit(nullptr);
}

int main(int argc, char * argv[])
{
    pp = new ping_pong(sizeof(int));
 
    pthread_t r_id;
    pthread_t w_id;
    
    pthread_create(&w_id, nullptr, writer, nullptr);
    pthread_create(&r_id, nullptr, reader, nullptr);
    
    pthread_join(r_id, nullptr);
    pthread_join(w_id, nullptr);
    
    delete pp;
    
    return 0;
}