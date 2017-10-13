#include "../include/channel.h"

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
        delete[] name;
    }

    void Channel::Add(Nan::Callback* cb, bool once) {
        CallbackNode* n = new CallbackNode();
        n->cb = cb;
        n->once = once;
        if (head == NULL) {
            head = tail = n;
        } else {
            tail->next = n;
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
                if (h->next->cb == cb) {
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
        CallbackNode* prev = h;
        while (h != NULL) {
            h->cb->Call(n, args);
            if (h->once) {
                prev->next = h->next;
                if (h == head) {
                    head = h->next;
                }
                if (h == tail) {
                    tail = prev;
                }
                delete h->cb;
                delete h;
                h = prev->next;
            } else {
                prev = h;
                h = h->next;
            }
        }
        return head == NULL;
    }

    void Channel::Clear() {
        while (head != NULL) {
            CallbackNode* h = head;
            head = head->next;
            delete h->cb;
            delete h;
        }
    }

    char* Channel::GetName() {
        return name;
    }
}