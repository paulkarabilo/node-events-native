#include "../include/channels.h"

namespace addon {
    Channels::Channels() {
        channels = new Channels*[CHANNELS_SIZE];
        memset(channels, 0, sizeof(*channels) * CHANNELS_SIZE);
    }

    Channels::~Channels() {

    }

    void Channels::Add(char* name, Nan::Callback* cb) {

    }

    void Channels::Remove(char* name, Nan::Callback* cb) {

    }

    Channel* Channels::Get(char* name) {

    }

    void Channels::Exec(char* name, int n, Local<Value>[] args) {

    }

    int Channels::Hash(char* s) {
        unsigned long hash = 5381;
        int c;
    
        while (c = *s++)
            hash = ((hash << 5) + hash) + c;
    
        return hash % CHANNELS_SIZE;
    }

}