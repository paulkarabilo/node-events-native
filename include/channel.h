#ifndef NODE_NATIVE_EVENTS_CHANNEL_H
#define NODE_NATIVE_EVENTS_CHANNEL_H

#include <nan.h>
using namespace v8;


namespace addon {
    class Channel {
    public:
        Channel(char* _name);
        ~Channel();
        void Add(Nan::Callback* cb, bool once);
        void Remove(Nan::Callback* cb);
        void Clear();
        void Exec(int l, Local<Value> args[]);
        char * GetName();
    private:
        class CallbackNode {
            public:
                Nan::Callback* cb;
                bool once;
                CallbackNode* next;
        };
    private:
        char* name;
        CallbackNode* head;
    };
}

#endif