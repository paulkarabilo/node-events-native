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
        Nan::Set(target, Nan::New("exports").ToLocalChecked(), Nan::GetFunction(ctor).ToLocalChecked());
    }

    NativeEvents::NativeEvents() : Nan::ObjectWrap(), m_channels(), m_once_channels() {
        
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

        String::Utf8Value val(info[0]->ToString());
        string key (*val);

        Nan::Callback *cb = new Nan::Callback(Local<Function>::Cast(info[1]));

        if (ne->m_channels.find(key) == ne->m_channels.end()) {
            vector<Nan::Callback*> vec;
            vec.push_back(cb);
            ne->m_channels[key] = vec;
        } else {
            ne->m_channels[key].push_back(cb);
        }
    }

    NAN_METHOD(NativeEvents::RemoveListener) {
        if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsFunction()) {
            return Nan::ThrowError("Method removeListener expects 2 arguments: event name and callback");
        }
        String::Utf8Value val(info[0]->ToString());
        string key (*val);

        NativeEvents* ne = Nan::ObjectWrap::Unwrap<NativeEvents>(info.Holder());

        Local<Function> listener = Local<Function>::Cast(info[1]);
        map<string, vector<Nan::Callback*> >::iterator map_it = ne->m_channels.find(key);
        if (map_it != ne->m_channels.end()) {
            for (vector<Nan::Callback*>::iterator it = (*map_it).second.begin();
                    it != (*map_it).second.end();) {
                Nan::Callback* cb = *it;
                if (cb->GetFunction() == listener) {
                    delete cb;
                    it = (*map_it).second.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    NAN_METHOD(NativeEvents::RemoveAllListeners) {
        
    }

    NAN_METHOD(NativeEvents::Emit) {
        if (info.Length() < 1 || !info[0]->IsString()) {
            return Nan::ThrowError("Method emit expects at least 1 argument: event name");
        }
        NativeEvents* ne = Nan::ObjectWrap::Unwrap<NativeEvents>(info.Holder());

        String::Utf8Value val(info[0]->ToString());
        string key (*val);

        int l = info.Length() - 1;
        Local<Value> argv[l];
        for (int i = 1; i < info.Length(); i++) {
            argv[i - 1] = info[i];
        }
        map<string, vector<Nan::Callback*> >::iterator map_it = ne->m_channels.find(key);
        map_it = ne->m_once_channels.find(key);
        if (map_it != ne->m_once_channels.end()) {
            vector<Nan::Callback*>::iterator it = (*map_it).second.begin();
            while (it != (*map_it).second.end()) {
                Nan::Callback* cb = *it;
                cb->Call(l, argv);
                ++it;
            }
            map_it = ne->m_once_channels.erase(map_it);
        }
        map_it = ne->m_channels.find(key);
        if (map_it != ne->m_channels.end()) {
            vector<Nan::Callback*> vec = (*map_it).second;
            for (vector<Nan::Callback*>::iterator it = vec.begin(); it != vec.end(); ++it) {
                Nan::Callback* cb = *it;
                cb->Call(l, argv);
            }
        }
    }

    NAN_METHOD(NativeEvents::Once) {
        if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsFunction()) {
            return Nan::ThrowError("Method once expects 2 arguments: event name and callback");
        }

        NativeEvents* ne = Nan::ObjectWrap::Unwrap<NativeEvents>(info.Holder());

        String::Utf8Value val(info[0]->ToString());
        string key (*val);

        Nan::Callback *cb = new Nan::Callback(Local<Function>::Cast(info[1]));
        
        if (ne->m_once_channels.find(key) == ne->m_once_channels.end()) {
            vector<Nan::Callback*> vec;
            vec.push_back(cb);
            ne->m_once_channels[key] = vec;
        } else {
            ne->m_once_channels[key].push_back(cb);
        }
    }

}

void NativeEventsInit(Handle<Object> exports, Handle<Object> module) {
    addon::NativeEvents::Initialize(module);
}


NODE_MODULE(NODE_GYP_MODULE_NAME, NativeEventsInit)