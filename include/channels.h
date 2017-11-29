#ifndef NODE_NATIVE_EVENTS_CHANNELS_H
#define NODE_NATIVE_EVENTS_CHANNELS_H

#include <nan.h>
#include "channel.h"

#define CHANNELS_SIZE 1024
using namespace v8;

namespace addon {
    class Channels {
    public:
        Channels();
        ~Channels();
        void Add(char* name, Nan::Callback* cb, bool once);
        void Remove(char* name, Nan::Callback* cb);
        void RemoveAll(char* name);
        void RemoveAll();
        Channel* Get(char* name);
        void Exec(char* name, int n, Local<Value>* args);
        void Clear();

    private:
        class ChannelNode {
        public:
            Channel* channel;
            ChannelNode* next;
        };
    private:
        ChannelNode** channels;
        int Hash(char* s);
    };
}

#endif