#ifndef IEVENT3DVIEWERRECEIVER_H
#define IEVENT3DVIEWERRECEIVER_H

#include <string>

class IEvent3DViewerReceiver
{
    public: 
        virtual void onViewerDestroyed(const char* name) = 0;
        
};


#endif