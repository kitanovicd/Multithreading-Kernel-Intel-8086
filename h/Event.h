#ifndef _event_h_
#define _event_h_

typedef unsigned char IVTNo;

class KEvent;

class Event
{
public:
   Event(IVTNo ivtNo);
   ~Event();
   void wait();

protected:
   friend class KEvent;
   void signal();

private:
   KEvent *myImpl;
};

#endif
