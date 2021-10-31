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


///IMAGE DATA

Simple_Server::frameData* Simple_Server_i::sendFrameData() {

  while (!newFrame) {}

  Simple_Server::frameData fd;
  fd.m_header =  Simple_Server::header(this->headerSize, this->headerData);
  fd.m_headerSize = this->headerSize;
  fd.m_pixels = Simple_Server::pixels(this->jpegSize, this->imageData);
  fd.m_pixelSize = this->jpegSize;
  
  Simple_Server::frameData* value = 0;

  ACE_NEW_THROW_EX(value, Simple_Server::frameData(fd), CORBA::NO_MEMORY());

  newFrame = false;
  
  return value;
}

void Simple_Server_i::setImageData(unsigned char* iData) {
  
  //free(this->imageData);
  this->imageData = (CORBA::Octet*) malloc (jpegSize);
  memcpy(this->imageData, iData, jpegSize); 
  
}


void Simple_Server_i::setHeaderData(unsigned char* iData) {
  
  //free(this->headerData);
  this->headerData = (CORBA::Octet*) malloc (headerSize);
  memcpy(this->headerData, iData, headerSize); 
  
}

///POSE DATA

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

void Simple_Server_i::get_data(float* in_data) {
  
  memcpy(in_data, data, sizeof(float) * 7);
  
  
}

void Simple_Server_i::setJpegSize(long unsigned int js) {
  jpegSize = js;
}

void Simple_Server_i::setHeaderSize(long unsigned int hs) {
  headerSize = hs;
}

void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (false);
}

