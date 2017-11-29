#ifndef NODE_NATIVE_EVENTS_CHANNEL_BUCKET_H
#define NODE_NATIVE_EVENTS_CHANNEL_BUCKET_H

#ifndef NODE_NATIVE_EVENTS_CHANNEL_BUCKET_H_DEFINED
    #define NODE_NATIVE_EVENTS_CHANNEL_BUCKET_H_DEFINED
    namespace addon {
        class ChannelBucket;
    }
    #include "channel.h"
#endif

namespace addon {

    class ChannelBucket {
    public:
        Channel* channel;
        ChannelBucket* prev;
        ChannelBucket* next;
    };
}
#endif