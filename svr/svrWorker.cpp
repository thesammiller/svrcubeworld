#include "svr/svrWorker.h"

Worker::Worker(CORBA::ORB_ptr orb) : orb_(CORBA::ORB::_duplicate(orb)) {

}

int Worker::svc(void) {
    try {
        this->orb_->run();
    }
    catch (const CORBA::Exception&) {}
    return 0;
}