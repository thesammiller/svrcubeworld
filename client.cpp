#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "unistd.h"





const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int niterations = 10;
int nthreads = 1;
int do_shutdown = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class Worker : public ACE_Task_Base
{
public:
  Worker (CORBA::ORB_ptr orb);
  // Constructor

  virtual void run_test (void);
  // The actual implementation of the test

  // = The Task_Base methods
  virtual int svc ();

private:
  CORBA::ORB_var orb_;
  // The ORB reference
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      Worker worker (orb.in ());

      if (worker.activate (THR_NEW_LWP | THR_JOINABLE, nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate worker threads\n"),
                          1);

      ACE_Time_Value tv (5, 0);

      orb->run (tv);
      //orb->run ();
      
      //worker.run_test ();

      



    worker.thr_mgr ()->wait ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in main:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc (void)
{
  try
    {
      this->run_test ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in thread (%t)\n");
    }

  return 0;
}

void
Worker::run_test (void)
{



      CORBA::Object_var object =
        orb_->string_to_object (ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

	  std::cout<< "Running" <<std::endl;

	  for (;;) {
	  std::fstream in("sample_controller.txt");
	  std::string line;
	  std::vector<std::vector<float>> v;
	  int i=0;
	  while(std::getline(in, line)) {
	      float value;
	      std::stringstream ss(line);
	      float values[9];
	      // -1161455114	-1161488748		-0.101638 -0.008850 0.042846 0.993859	-0.004179 1.231212 -0.337064
	      
	      for (int j=0; j < 9; j++) {
		ss >> values[j];
	      }
	      
	      std::cout << values[0]  << std::endl;
	      float outArray[7] = {values[2], values[3], values[4], values[5], values[6], values[7], values[8]};
	      server->send_data(1234567, outArray);
	      ++i;
	      usleep(100000);
	  }
	  }
    

}
