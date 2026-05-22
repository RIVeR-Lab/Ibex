#ifndef IEVENTRANGEMAPRECEIVER_H
#define IEVENTRANGEMAPRECEIVER_H

#include <string>

class IEventRangeMapReceiver
{
    public: 
        virtual void onRangeMapViewDestroyed(const char* name) = 0;
};


#endif