#include "test_i.h"
#include "ace/Task.h"

#ifndef WORKER_H
#define WORKER_H


class Worker : public ACE_Task_Base
{
    public:
        Worker(CORBA::ORB_ptr orb);
        // Thread entry pool
        virtual int svc();
    private:
        //Corba Orb
        CORBA::ORB_var orb_;
};

#endif /* WORKER_H */