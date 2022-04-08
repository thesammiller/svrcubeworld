#include "Consumer.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://ec.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Consumer consumer;

  return consumer.run (argc, argv);
}

// ****************************************************************

Consumer::Consumer (void)
  : event_count_ (0)
{
}

int
Consumer::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // ORB initialization boiler plate...
      // Pure CORBA: "initialize the ORB" p. 536
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // Do *NOT* make a copy because we don't want the ORB to outlive
      // the Consumer object.
      this->orb_ = orb.in ();


      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA"); // Pure CORBA uses "EventService"
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();

      // Obtain the event channel, we could use a naming service, a
      // command line argument or resolve_initial_references(), but
      // this is simpler...


      // Pure CORBA: 
      // 1. The client (Supplier or COnsumer) first binds to the Event Channel
      // (which must be created by someone already, perhaps the client itself)

      object =
        orb->string_to_object (ior);

      CosEventChannelAdmin::EventChannel_var event_channel =
        CosEventChannelAdmin::EventChannel::_narrow (object.in ());

      if (event_channel == NULL) {
        ACE_DEBUG ((LM_DEBUG,
                  "Could not find EventChannel"));
        return 2;
      }
      ACE_DEBUG((LM_DEBUG, "Event Channel found"));

      // The canonical protocol to connect to the EC
      // Pure CORBA
      // 2. The client obtains an Admin object from the Event Channel
      // A consumer needs a consumerAdmin object and a Supplier needs a SupplierAdmin object
      CosEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers ();

      // Pure CORBA (using a proxy pull)
      // 3. The client obtains a proxy object from the Admin object
      // (a Consumer Proxy for a Supplier client,
      // and a Supplier Proxy for a Consumer cliente)
      CosEventChannelAdmin::ProxyPushSupplier_var supplier =
        consumer_admin->obtain_push_supplier ();
      

      CosEventComm::PushConsumer_var consumer =
        this->_this ();

      supplier->connect_push_consumer (consumer.in ());

      // Wait for events, using work_pending()/perform_work() may help
      // or using another thread, this example is too simple for that.
      orb->run ();

      // We don't do any cleanup, it is hard to do it after shutdown,
      // and would complicate the example; plus it is almost
      // impossible to do cleanup after ORB->run() because the POA is
      // in the holding state.  Applications should use
      // work_pending()/perform_work() to do more interesting stuff.
      // Check the supplier for the proper way to do cleanup.
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Consumer::run");
      return 1;
    }
  return 0;
}

void
Consumer::push (const CORBA::Any &)
{
  this->event_count_ ++;
  if (this->event_count_ % 100 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t): %d events received\n",
                  this->event_count_));
    }
}

void
Consumer::disconnect_push_consumer (void)
{
  // In this example we shutdown the ORB when we disconnect from the
  // EC (or rather the EC disconnects from us), but this doesn't have
  // to be the case....
  this->orb_->shutdown (false);
}

