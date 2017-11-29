#include "../include/channels.h"
#include <iostream>
namespace addon {
    Channels::Channels() {
        channels = new ChannelNode*[CHANNELS_SIZE];
        memset(channels, 0, sizeof(*channels) * CHANNELS_SIZE);
    }

    Channels::~Channels() {
        RemoveAll();
    }

    void Channels::Add(char* name, Nan::Callback* cb, bool once) {
        Channel* chan = Get(name);
        if (chan == NULL) {
            chan = new Channel(name);
            chan->Add(cb, once);
            ChannelNode* node = new ChannelNode;
            node->channel = chan;
            int hash = Hash(name);
            node->next = channels[hash];
            channels[hash] = node;
        } else {
            chan->Add(cb, once);
        }
    }

    void Channels::Remove(char* name, Nan::Callback* cb) {
        Channel* chan = Get(name);
        if (chan != NULL) {
            chan->Remove(cb);
        }
    }

    void Channels::RemoveAll(char* name) {
        int hash = Hash(name);
        ChannelNode* node = channels[hash];
        ChannelNode* prev = node;
        while (node != NULL) {
            Channel* chan = node->channel;
            if (strcmp(chan->GetName(), name) == 0) {
                prev->next = node->next;
                if (node == channels[hash]) {
                    channels[hash] = node->next;
                }
                delete node->channel;
                delete node;
                node = prev->next;
            } else {
                node = node->next;
            }
        }
    }

    void Channels::RemoveAll() {
        for (int i = 0; i < CHANNELS_SIZE; i++) {
            ChannelNode* node = channels[i];
            while (node != NULL) {
                delete node->channel;
                delete node;
                node = node->next;
            }
            channels[i] = NULL;
        }
    }

    Channel* Channels::Get(char* name) {
        int hash = Hash(name);
        ChannelNode* node = channels[hash];
        if (!node) return NULL;
        while (node != NULL) {
            if (strcmp(node->channel->GetName(), name) == 0) {
                return node->channel;
            }
            node = node->next;
        }
        return NULL;
    }

    void Channels::Exec(char* name, int n, Local<Value>* args) {
        int hash = Hash(name);
        ChannelNode* node = channels[hash];
        ChannelNode* prev = node;
        while (node != NULL) {
            Channel* chan = node->channel;
            if (strcmp(node->channel->GetName(), name) == 0 && !chan->IsEmpty()) {
                if (chan->Exec(n, args)) {
                    prev->next = node->next;
                    if (node == channels[hash]) {
                        channels[hash] = node->next;
                    }
                    if (node->channel != NULL) {
                        delete node->channel;
                    }
                    if (node != NULL) {
                        delete node;
                    }
                    node = prev->next;
                } else {
                    prev = node;
                    node = node->next;
                }
            } else {
                node = node->next;
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