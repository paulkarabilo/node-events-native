#include "../include/native-events.h"

namespace nativeevents {
    NAN_MODULE_INIT(NativeEvents::Initialize) {
        Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(New);
        ctor->InstanceTemplate()->SetInternalFieldCount(1);
        Nan::SetPrototypeMethod(ctor, "addListener", AddListener);
        Nan::SetPrototypeMethod(ctor, "removeListener", RemoveListener);
        Nan::SetPrototypeMethod(ctor, "removeAllListeners", RemoveAllListeners);
        Nan::SetPrototypeMethod(ctor, "emit", Emit);
        Nan::SetPrototypeMethod(ctor, "on", On);
        Nan::SetPrototypeMethod(ctor, "once", Once);
        Nan::Set(target, Nan::New("exports").ToLocalChecked(), Nan::GetFunction(ctor).ToLocalChecked());
    }

    NAN_METHOD(NativeEvents::New) {
        NativeEvents *ne = new NativeEvents();
        ne->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }

    NAN_METHOD(NativeEvents::AddListener) {
        if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsFunction()) {
            return Nan::ThrowError("Method AddListener expects 2 arguments: event name and callback");
        }
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

    NativeEvents::NativeEvents() : Nan::ObjectWrap(), m_channels() {

    }
}

void NativeEventsInit(Handle<Object> exports, Handle<Object> module) {
    nativeevents::NativeEvents::Initialize(module);
}


NODE_MODULE(NODE_GYP_MODULE_NAME, NativeEventsInit)