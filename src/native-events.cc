#include "../include/native-events.h"

namespace nativeevents {
    NAN_MODULE_INIT(NativeEvents::Initialize) {
        Local<FunctionTemplate> events = Nan::New<FunctionTemplate>(New);
        events->InstanceTemplate()->SetInternalFieldCount(1);
        Nan::Set(target, Nan::New("exports").ToLocalChecked(), Nan::GetFunction(events).ToLocalChecked());
    }

    NAN_METHOD(NativeEvents::New) {
        NativeEvents *ne = new NativeEvents();
        ne->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }

    NAN_METHOD(NativeEvents::AddListener) {

    }
    NAN_METHOD(NativeEvents::RemoveListener) {

    }
    NAN_METHOD(NativeEvents::RemoveAllListeners) {

    }
    NAN_METHOD(NativeEvents::Emit) {

    }
    NAN_METHOD(NativeEvents::On) {

    }
    NAN_METHOD(NativeEvents::Once) {
        
    }

    NativeEvents::NativeEvents() : Nan::ObjectWrap() { }
}

void NativeEventsInit(Handle<Object> exports, Handle<Object> module) {
    nativeevents::NativeEvents::Initialize(module);
}


NODE_MODULE(NODE_GYP_MODULE_NAME, NativeEventsInit)