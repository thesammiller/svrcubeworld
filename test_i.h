
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_FORWARDING_TEST_I_H
#define TAO_FORWARDING_TEST_I_H

#include "testS.h"

/**
 * @class Simple_Server_i
 *
 * @brief Simpler Server implementation
 *
 * Implements the Simple_Server interface in test.idl
 */
class Simple_Server_i : public POA_Simple_Server
{
public:
  /// ctor
  Simple_Server_i (CORBA::ORB_ptr orb);
  float data[7];

  // = The Simple_Server methods.
  void send_data (const CORBA::Long microsecond, const Simple_Server::HeadPoseArray headpose );
  void get_data(float *in_pose);
  Simple_Server::pixels* sendImageData();
  
  void setImageData(unsigned char* iData);
  
  void shutdown (void);
  CORBA::Long sendJpegSize();
  void setJpegSize(long unsigned int js);

private:
  /// The ORB
  CORBA::ORB_var orb_;
  CORBA::Octet* imageData;
  //CORBA::Octet* oldData;
  bool dataSet = false;
  CORBA::Long jpegSize;
  
};

#if defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_FORWARDING_TEST_I_H */
