#include "../include/channels.h"

namespace addon {
    Channels::Channels() {
        channels = new ChannelBucket*[CHANNELS_SIZE];
        memset(channels, 0, sizeof(*channels) * CHANNELS_SIZE);
    }

    Channels::~Channels() {
        RemoveAll();
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

    void Channels::RemoveAll(char* name) {
        int hash = Hash(name);
        ChannelBucket* bucket = channels[hash];
        ChannelBucket* prev = bucket;
        while (bucket != NULL) {
            Channel* chan = bucket->channel;
            if (strcmp(chan->GetName(), name) == 0) {
                prev->next = bucket->next;
                if (bucket == channels[hash]) {
                    channels[hash] = bucket->next;
                }
                delete bucket->channel;
                delete bucket;
                bucket = prev->next;
            } else {
                bucket = bucket->next;
            }
        }
    }

    void Channels::RemoveAll() {
        for (int i = 0; i < CHANNELS_SIZE; i++) {
            ChannelBucket* bucket = channels[i];
            while (bucket != NULL) {
                delete bucket->channel;
                delete bucket;
                bucket = bucket->next;
            }
            channels[i] = NULL;
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
        int hash = Hash(name);
        ChannelBucket* bucket = channels[hash];
        ChannelBucket* prev = bucket;
        while (bucket != NULL) {
            Channel* chan = bucket->channel;
            if (strcmp(bucket->channel->GetName(), name) == 0 && !chan->IsEmpty()) {
                if (chan->Exec(n, args)) {
                    prev->next = bucket->next;
                    if (bucket == channels[hash]) {
                        channels[hash] = bucket->next;
                    }
                    delete bucket->channel;
                    delete bucket;
                    bucket = prev->next;
                } else {
                    prev = bucket;
                    bucket = bucket->next;
                }
            } else {
                bucket = bucket->next;
            }
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