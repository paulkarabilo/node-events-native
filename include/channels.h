#include <nan.h>
#include "channel.h"

#define CHANNELS_SIZE 1024

namespace addon {
    class Channels {
    public:
        Channels();
        ~Channels();
        void Add(char* name, Nan::Callback* cb);
        void Remove(char* name, Nan::Callback* cb);
        Channel* Get(char* name);
        void Exec(char* name, int n, Local<Value>[] args);

    private:
        Channel** channels;
        int Hash(char* s);
    }
}