#include "../include/channels.h"

namespace addon {
    Channels::Channels() {
        channels = new ChannelBucket*[CHANNELS_SIZE];
        memset(channels, 0, sizeof(*channels) * CHANNELS_SIZE);
    }

    Channels::~Channels() {

    }

    void Channels::Add(char* name, Nan::Callback* cb, bool once) {
        Channel* c = Get(name);
        if (c == NULL) {
            c = new Channel(name);
            c->Add(cb, once);
            ChannelBucket* b = new ChannelBucket;
            b->channel = c;
            int hash = Hash(name);
            b->next = channels[hash];
            channels[hash] = b;
        } else {
            c->Add(cb, once);
        }
    }

    void Channels::Remove(char* name, Nan::Callback* cb) {
        Channel* c = Get(name);
        if (c != NULL) {
            c->Remove(cb);
        }
    }

    Channel* Channels::Get(char* name) {
        int hash = Hash(name);
        ChannelBucket* bucket = channels[hash];
        if (!bucket) return NULL;
        while (bucket != NULL) {
            if (strcmp(bucket->channel->GetName(), name) == 0) {
                return bucket->channel;
            }
            bucket = bucket->next;
        }
        return NULL;
    }

    void Channels::Exec(char* name, int n, Local<Value>* args) {
        Channel* c = Get(name);
        if (c != NULL) {
            c->Exec(n, args);
        }
    }

    int Channels::Hash(char* s) {
        unsigned long hash = 5381;
        int c;
    
        while ((c = *s++))
            hash = ((hash << 5) + hash) + c;
    
        return hash % CHANNELS_SIZE;
    }

}