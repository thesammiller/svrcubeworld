//TAO
#include "test_i.h"
#include "tao/IORTable/IORTable.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Task.h"
#include "svr/svrWorker.h"

#include "../TAO/orbsvcs/orbsvcs/CosEvent/CEC_EventChannel.h"
#include "../TAO/orbsvcs/orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"


class svrServer {


    public:
        svrServer();
        int createOrb(int argc, char *argv[]);
        int createServer();
        void startWorker(Worker *worker, int nthreads);
        int parse_args(int argc, char *argv[]);
        void setImage(unsigned char *pixels);
        void setJpegSize(long unsigned int js);
        void setHeader(unsigned char* header);
        void setHeaderSize(long unsigned int hs);
        void wait();

        CORBA::ORB_var orb;
        const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");
        PortableServer::POA_var root_poa = NULL;
        PortableServer::POAManager_var poa_manager = NULL;
        Simple_Server_i server_impl = NULL;
        
        long unsigned int jpegSize;
        long unsigned int headerSize;

        bool newFrame = false;

};