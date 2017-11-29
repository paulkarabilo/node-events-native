#include "../include/channel.h"
#include <iostream>

namespace addon {
    Channel::Channel(char* _name) : head(NULL) {
        if (_name) {
            int l = strlen(_name);
            name = new char[l + 1];
            strcpy(name, _name);
            name[l] = '\0';
        }
    }

    Channel::~Channel() {
        Clear();
        if (name != NULL) {
            delete[] name;
        }
    }

    void Channel::Add(Nan::Callback* cb, bool once) {
        CallbackNode* n = new CallbackNode();
        n->cb = cb;
        n->once = once;
        if (head == NULL) {
            head = tail = n;
        } else {
            tail->next = n;
            n->prev = tail;
            tail = n;
        }
    }

    bool Channel::IsEmpty () {
        return head == NULL;
    }

    void Channel::Remove(Nan::Callback* cb) {
        CallbackNode* h = head;
        if (head->cb == cb) {
            if (head == tail) {
                tail = NULL;
            }
            head = head->next;
            delete h->cb;
            delete h;
        } else {
            while (h->next != NULL) {
                if (h->cb == cb) {
                    CallbackNode* hh = h->next;
                    if (hh == tail) {
                        tail = h;
                    }
                    h->next = h->next->next;
                    delete hh->cb;
                    delete hh;
                }
                h = h->next;
            }
        }
    }

    bool Channel::Exec(int n, Local<Value>* args) {
        CallbackNode* h = head;
        while (h != NULL) {
            if (h->once) {
                if (h == head) {
                    head = h->next;
                }
                if (h == tail) {
                    tail = h->prev;
                }
                if (h->prev != NULL) {
                    h->prev->next = h->next;
                }
                if (h->next != NULL) {
                    h->next->prev = h->prev;
                }
                h->cb->GetFunction()->Call(Isolate::GetCurrent()->GetCurrentContext()->Global(), n, args);
                CallbackNode* next = h->next;
                delete h->cb;
                delete h;
                h = next;
            } else {
                h->cb->GetFunction()->Call(Isolate::GetCurrent()->GetCurrentContext()->Global(), n, args);
                h = h->next;
            }
        }
        return false;
    }

    void Channel::Clear() {
        while (head != NULL) {
            CallbackNode* h = head;
            head = head->next;
            if (h->cb != NULL) {
                delete h->cb;
            }
            if (h != NULL) {
                delete h;
            }
        }
    }

    char* Channel::GetName() {
        return name;
    }

    Local<Array> Channel::ListenersToArray() {
        Nan::EscapableHandleScope scope;
        Local<Array> arr = Nan::New<Array>();
        CallbackNode* n = head;
        unsigned int i = 0;
        while (n != NULL) {
            arr->Set(i, n->cb->GetFunction());
            n = n->next;
            i++;
        }
        return scope.Escape(arr);
    }
}