#include "test_i.h"
#include "time.h"
#include "ace/ACE.h"
#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

#include <iostream>
#include <unistd.h>

static ACE_Thread_Mutex m_mutex;
static ACE_Thread_Mutex m_mutex_pose;

Simple_Server::pixels* Simple_Server_i::sendImageData() {
  m_mutex.acquire();
  Simple_Server::pixels* value = 0;

  //Write frame to file -- confirm JPEG compression is working
  FILE *file = fopen("out1.jpg", "wb");
  fwrite(this->imageData, jpegSize, 1, file);
  fclose(file);

  ACE_NEW_THROW_EX(value, Simple_Server::pixels(jpegSize, this->imageData), CORBA::NO_MEMORY());
  
  //memcpy(value, this->imageData, jpegSize);
  m_mutex.release();
  return value;
}

void Simple_Server_i::setImageData(unsigned char* iData) {
  m_mutex.acquire();
  this->imageData = (CORBA::Octet*) malloc (jpegSize);
  memcpy(this->imageData, iData, jpegSize); 
  m_mutex.release();
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
  

  m_mutex_pose.acquire();
  memcpy(data, headpose, sizeof(float) * 7);
  m_mutex_pose.release();


  
}

void Simple_Server_i::get_data(float* in_data) {
  m_mutex_pose.acquire();
  memcpy(in_data, data, sizeof(float) * 7);
  m_mutex_pose.release();
  
}

CORBA::Long Simple_Server_i::sendJpegSize() {
  return jpegSize;
}

void Simple_Server_i::setJpegSize(long unsigned int js) {
  jpegSize = js;
}

void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (false);
}

