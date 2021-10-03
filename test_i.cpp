#include "test_i.h"
#include "time.h"
#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#include <iostream>

static ACE_Thread_Mutex m_mutex;

Simple_Server::pixels_slice* Simple_Server_i::sendImageData() {
  return this->imageData;
}

void Simple_Server_i::setImageData(unsigned char* iData) {
  this->imageData = iData;
}

void
Simple_Server_i::send_data (const CORBA::Long microsecond, const Simple_Server::HeadPoseArray headpose)

{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;

  
  ACE_DEBUG ((LM_DEBUG, "%d\t%d\t\t%f %f %f %f\t%f %f %f\n",
	      microsecond, time_in_micros,
	      headpose[0], headpose[1], headpose[2], headpose[3],
	      headpose[4], headpose[5], headpose[6]));
  
  memcpy(data, headpose, sizeof(float) * 7);


  
}

void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (false);
}

