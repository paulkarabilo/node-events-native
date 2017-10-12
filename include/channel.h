#include <nan.h>



namespace addon {
    class Channel {
    public:
        Channel(char* _name);
        ~Channel();
        void Add(Nan::Callback* cb, boolean once);
        void Remove(Nan::Callback* cb);
        void Clear();
        void Exec(int l, Local<Value>[] args);
        char * GetName();
    private:
        class CallbackNode {
            public:
                Nan::Callback* cb;
                boolean once;
                CallbackNode* next;
        };
    private:
        char* name;
        CallbackNode* head;
    }
}