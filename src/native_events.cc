#include "../include/native_events.h"

namespace addon {
    NAN_MODULE_INIT(NativeEvents::Initialize) {
        Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(New);
        ctor->InstanceTemplate()->SetInternalFieldCount(1);
        Nan::SetPrototypeMethod(ctor, "addListener", AddListener);
        Nan::SetPrototypeMethod(ctor, "removeListener", RemoveListener);
        Nan::SetPrototypeMethod(ctor, "removeAllListeners", RemoveAllListeners);
        Nan::SetPrototypeMethod(ctor, "emit", Emit);
        Nan::SetPrototypeMethod(ctor, "on", AddListener);
        Nan::SetPrototypeMethod(ctor, "once", Once);
        Nan::SetPrototypeMethod(ctor, "listeners", GetListeners);
        Nan::Set(target, Nan::New("exports").ToLocalChecked(), Nan::GetFunction(ctor).ToLocalChecked());
    }

    NativeEvents::NativeEvents() : Nan::ObjectWrap() {
        channels = new Channels();
    }

    NAN_METHOD(NativeEvents::New) {
        NativeEvents *ne = new NativeEvents();
        ne->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }

    NAN_METHOD(NativeEvents::AddListener) {
        if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsFunction()) {
            return Nan::ThrowError("Method on/addListener expects 2 arguments: event name and callback");
        }

        NativeEvents* ne = Nan::ObjectWrap::Unwrap<NativeEvents>(info.Holder());       
        
        Local<Value> argv[2];
        argv[0] = info[0];
        argv[1] = info[1];
        char newKey[12] = "newListener";
        ne->channels->Exec(newKey, 2, argv);

        char* key = *(Nan::Utf8String)(info[0]);
        Nan::Callback *cb = new Nan::Callback(Local<Function>::Cast(info[1]));
        ne->channels->Add(key, cb, false);
    }

    NAN_METHOD(NativeEvents::RemoveListener) {
        if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsFunction()) {
            return Nan::ThrowError("Method removeListener expects 2 arguments: event name and callback");
        }
        
        char* key = *(Nan::Utf8String)(info[0]);
        NativeEvents* ne = Nan::ObjectWrap::Unwrap<NativeEvents>(info.Holder());
        Nan::Callback *cb = new Nan::Callback(Local<Function>::Cast(info[1]));
        ne->channels->Remove(key, cb);
        delete cb;
    }

    NAN_METHOD(NativeEvents::RemoveAllListeners) {
        if (info.Length() == 1 && info[0]->IsString()) {
            char* key = *(Nan::Utf8String)(info[0]);
            NativeEvents* ne = Nan::ObjectWrap::Unwrap<NativeEvents>(info.Holder());
            ne->channels->RemoveAll(key);
        } else if (info.Length() == 0) {
            NativeEvents* ne = Nan::ObjectWrap::Unwrap<NativeEvents>(info.Holder());
            ne->channels->RemoveAll();
        } else {
            return Nan::ThrowError("Method removeAllListeners expects at least 1 argument: event name");
        }
    }

    NAN_METHOD(NativeEvents::GetListeners) {
        if (info.Length() == 1 && info[0]->IsString()) {
            char* key = *(Nan::Utf8String)(info[0]);
            NativeEvents* ne = Nan::ObjectWrap::Unwrap<NativeEvents>(info.Holder());
            Channel* chan = ne->channels->Get(key);
            if (chan != NULL) {
                info.GetReturnValue().Set(chan->ListenersToArray());
            } else {
                Local<Array> listeners = Nan::New<Array>();
                info.GetReturnValue().Set(listeners);
            }
        } else {
            return Nan::ThrowError("Method lisyeners expects 1 argument: event name");
        }
    }

    NAN_METHOD(NativeEvents::Emit) {
        if (info.Length() < 1 || !info[0]->IsString()) {
            return Nan::ThrowError("Method emit expects at least 1 argument: event name");
        }
        NativeEvents* ne = Nan::ObjectWrap::Unwrap<NativeEvents>(info.Holder());
        char* key = *(Nan::Utf8String)(info[0]);

        int l = info.Length() - 1;
        Local<Value> argv[l];
        for (int i = 1; i < info.Length(); i++) {
            argv[i - 1] = info[i];
        }
        ne->channels->Exec(key, l, argv);
    }

    NAN_METHOD(NativeEvents::Once) {
        if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsFunction()) {
            return Nan::ThrowError("Method once expects 2 arguments: event name and callback");
        }

        NativeEvents* ne = Nan::ObjectWrap::Unwrap<NativeEvents>(info.Holder());

        Local<Value> argv[2];
        argv[0] = info[0];
        argv[1] = info[1];
        char newKey[12] = "newListener";
        ne->channels->Exec(newKey, 2, argv);

        char* key = *(Nan::Utf8String)(info[0]);
        Nan::Callback *cb = new Nan::Callback(Local<Function>::Cast(info[1]));
        ne->channels->Add(key, cb, true);
    }
}

void NativeEventsInit(Handle<Object> exports, Handle<Object> module) {
    addon::NativeEvents::Initialize(module);
}


NODE_MODULE(NODE_GYP_MODULE_NAME, NativeEventsInit)