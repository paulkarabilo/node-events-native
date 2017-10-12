#ifndef NODE_NATIVE_EVENTS_H
#define NODE_NATIVE_EVENTS_H

#include <nan.h>
#include <vector>
#include <map>
#include <string>
#include "channels.h"

using namespace v8;
using namespace node;
using namespace std;

namespace addon {
    class NativeEvents : public Nan::ObjectWrap {
        public:
            static NAN_MODULE_INIT(Initialize);
            //~NativeEvents();
        private:
            NativeEvents();
            Channels* m_channels;
            static NAN_METHOD(New);
            static NAN_METHOD(AddListener);
            static NAN_METHOD(RemoveListener);
            static NAN_METHOD(RemoveAllListeners);
            static NAN_METHOD(Emit);
            static NAN_METHOD(On);
            static NAN_METHOD(Once);
    };
}
#endif