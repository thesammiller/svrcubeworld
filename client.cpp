#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "unistd.h"

//Needed for extensions
#define GL_GLEXT_PROTOTYPES

#include <GLFW/glfw3.h>


#include "shader_s.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "extern/libjpeg-turbo/turbojpeg.h"

#include "extern/openh264/codec/api/svc/codec_api.h"

#include "../TAO/orbsvcs/orbsvcs/CosEventCommS.h"
#include "../TAO/orbsvcs/orbsvcs/CosEventChannelAdminS.h"



const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 1024;
std::vector<CORBA::Octet*> textureBufferList;
std::vector<CORBA::Octet*> frameBufferList;
std::vector<CORBA::Long> sizeList;





unsigned int loadTexture(unsigned char *data);

ACE_Thread_Mutex m_mutex;
ACE_Thread_Mutex f_mutex;

const ACE_TCHAR *ior = ACE_TEXT("file://ec.ior");
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

class VideoWorker : public ACE_Task_Base
{
public:
  VideoWorker ();
  // Constructor

  virtual void run_test (void);
  // The actual implementation of the test

  // = The Task_Base methods
  virtual int svc ();

private:
  CORBA::ORB_var orb_;
  long unsigned int hSize, pSize;
  ISVCDecoder *decoder;
  // The ORB reference
};

VideoWorker videoWorker;


class FrameWorker : public ACE_Task_Base
{
public:
  FrameWorker (CORBA::ORB_ptr orb);
  // Constructor

  virtual void run_test (void);
  // The actual implementation of the test

  // = The Task_Base methods
  virtual int svc ();

private:
  CORBA::ORB_var orb_;
  // The ORB reference
};

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

class Consumer : public POA_CosEventComm::PushConsumer
{
public:
  /// Constructor
  Consumer (void);

  /// Run the test
  int run (int argc, ACE_TCHAR* argv[]);

  // = The CosEventComm::PushConsumer methods

  /// The skeleton methods.
  virtual void push (const CORBA::Any &value);
  virtual void disconnect_push_consumer (void);

  CosEventChannelAdmin::ProxyPushSupplier_var supplier;
   ISVCDecoder *decoder;
  

private:
  /// Keep track of the number of events received.
  CORBA::ULong event_count_;

  /// The orb, just a pointer because the ORB does not outlive the
  /// run() method...
  CORBA::ORB_ptr orb_;
};






int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Consumer consumer;
  consumer.run(argc, argv);

  return 0;
}

int Consumer::run(int argc, char** argv) {
  
  try
    {


      orb_ =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      //Worker worker (orb_->in ());

      
      //VideoWorker videoWorker();

      //if (worker.activate (THR_NEW_LWP | THR_JOINABLE, nthreads) != 0)
      //  ACE_ERROR_RETURN ((LM_ERROR,
      //                     "(%P|%t) Cannot activate worker threads\n"),
      //                    1);

      

  
     CORBA::Object_var object =
        orb_->string_to_object (ior);

      //Simple_Server_var server =
    //Simple_Server::_narrow (object.in ());

 CosEventChannelAdmin::EventChannel_var event_channel =
        CosEventChannelAdmin::EventChannel::_narrow (object.in ());

      if (event_channel == NULL) {
        ACE_DEBUG ((LM_DEBUG,
                  "Could not find EventChannel"));
        return 2;
      }
      ACE_DEBUG((LM_DEBUG, "Event Channel found\n"));
      //but not a line past this!

      // we get this far

      // The canonical protocol to connect to the EC
      // Pure CORBA
      // 2. The client obtains an Admin object from the Event Channel
      // A consumer needs a consumerAdmin object and a Supplier needs a SupplierAdmin object
      CosEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        event_channel->for_consumers ();

       //ACE_DEBUG((LM_DEBUG, "For Consumers\n"));

      // Pure CORBA (using a proxy pull)
      // 3. The client obtains a proxy object from the Admin object
      // (a Consumer Proxy for a Supplier client,
      // and a Supplier Proxy for a Consumer client)
      supplier =
        consumer_admin->obtain_push_supplier ();

         //      ACE_DEBUG((LM_DEBUG, "Obtain Push Supplier\n"));


      // TODO:
      // I DON"T REALLY KNOW WHAT THIS DOES
      //it makes this object the consumer object -- there's magic here
      CosEventComm::PushConsumer_var consumer =
        this->_this ();

           //    ACE_DEBUG((LM_DEBUG, "FPush Consumer assignment\n"));


      //Pure CORBA p. 538
      // 4. The client adds the Supplier or COnsumer to the Event Channel
      // via a connect() call
      supplier->connect_push_consumer (consumer.in ());

    // didn't hit this yet
             //ACE_DEBUG((LM_DEBUG, "After Connect Push Consumer\n"));

           
       // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ClientCubeWorld", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    Shader renderBufferShader("shaders/renderBuffer.vs", "shaders/yuvShader.fs");

      float quadVertices[] = {
        // positions  //texcoords
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f     };

    unsigned int quadVBO, quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // texture coord attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    //unsigned int pixelTexture;
    //glGenTextures(1, &pixelTexture);

    
    int frame = 0;



     float startTime = glfwGetTime();

    float old_time = 0;


    //videoWorker.activate (THR_NEW_LWP | THR_JOINABLE, nthreads);
      
    
    while (!glfwWindowShouldClose(window))
    {
      orb_->work_pending();

      //orb_->run();

      if (textureBufferList.size() < 3) {
        continue;
      }
      if (textureBufferList.size() > 6) {
        m_mutex.acquire();
        f_mutex.acquire();
        for (int i=0; i < textureBufferList.size(); i ++) {
          textureBufferList.erase(textureBufferList.begin());
        }
        for (int i=0; i < frameBufferList.size(); i ++) {
          frameBufferList.erase(frameBufferList.begin());
        }
        for (int i = 0; i < sizeList.size(); i++) {
          sizeList.erase(sizeList.begin());
        }
        f_mutex.release();
        m_mutex.release();
        continue;
      }
      
      //float dataTime = glfwGetTime();

      //Select the GL Shader for Framebuffer
      renderBufferShader.use();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDisable(GL_DEPTH_TEST);
      //Clear screen (to white)

      //ACQUIRE TEXTUREBUFFERLIST MUTEX
      m_mutex.acquire();


      //THANK YOU TO
      //https://github.com/moonyl/yuv-player/blob/master/player/yuv-player.cpp
      //FOR THE YUV LOGIC HERE

      unsigned int textureY, textureU, textureV;
      glGenTextures(1, &textureY);
      
      glBindTexture(GL_TEXTURE_2D, textureY);
      // set the texture wrapping parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);	// set texture wrapping to GL_REPEAT (default wrapping method)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
      // set texture filtering parameters
      glPixelStorei(GL_UNPACK_ROW_LENGTH, 1088);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_HEIGHT, SCR_WIDTH, 0, GL_RED, GL_UNSIGNED_BYTE, *textureBufferList.begin());
      glGenerateMipmap(GL_TEXTURE_2D);
      glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
      //free(*textureBufferList.begin());
      textureBufferList.erase(textureBufferList.begin());
      

      glGenTextures(1, &textureU);
      
      glBindTexture(GL_TEXTURE_2D, textureU);
      
      // set the texture wrapping parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);	// set texture wrapping to GL_REPEAT (default wrapping method)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
      
      // set texture filtering parameters 
      glPixelStorei(GL_UNPACK_ROW_LENGTH,544);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_HEIGHT/2, SCR_WIDTH/2, 0, GL_RED, GL_UNSIGNED_BYTE, *textureBufferList.begin());
      glGenerateMipmap(GL_TEXTURE_2D);
      glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
      //free(*textureBufferList.begin());
      textureBufferList.erase(textureBufferList.begin());

      glGenTextures(1, &textureV);

      glBindTexture(GL_TEXTURE_2D, textureV);
      
      // set the texture wrapping parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);	// set texture wrapping to GL_REPEAT (default wrapping method)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
      // set texture filtering parameters
      glPixelStorei(GL_UNPACK_ROW_LENGTH,544);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_HEIGHT/2, SCR_WIDTH/2, 0, GL_RED, GL_UNSIGNED_BYTE, *textureBufferList.begin());
      glGenerateMipmap(GL_TEXTURE_2D);
      //free(*textureBufferList.begin());
      textureBufferList.erase(textureBufferList.begin());
      glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
      
      // RELEASE MUTEX
      m_mutex.release();

      glUseProgram(renderBufferShader.ID);

      glUniform1i(glGetUniformLocation(renderBufferShader.ID, "textureY"), 0);
      glUniform1i(glGetUniformLocation(renderBufferShader.ID, "textureU"), 1);
      glUniform1i(glGetUniformLocation(renderBufferShader.ID, "textureV"), 2);

      
      

      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textureY);

      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, textureU);

      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, textureV);

      glUseProgram(renderBufferShader.ID);
      
      glBindVertexArray(quadVAO);

      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei (GL_PACK_ALIGNMENT, 1);
      glPixelStorei (GL_UNPACK_SKIP_ROWS, 0);
      glPixelStorei (GL_UNPACK_SKIP_PIXELS, 0);

      

      glDrawArrays(GL_TRIANGLES, 0, 6);
      
      //PSwap framebuffer to front buffer
      glfwSwapBuffers(window);
      glfwPollEvents();

      glFinish();
      glFlush();


      if ( (old_time + 1) < glfwGetTime() ) {
            std::cout << "CLIENT FPS " << frame << std::endl;
            frame = 0;
            old_time = glfwGetTime();
            
        }
        ++frame;

        //Quick hack to cut down framerate
        //usleep(16333);
    
  }


    //worker.thr_mgr ()->wait ();

      orb_->destroy ();
    

  return 0;

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in main:");
      return 1;
    }
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

      
      for (;;) {
        std::fstream in("sample_controller.txt");
        std::string line;
        std::vector<std::vector<float>> v;
        int i=0;
        while(std::getline(in, line)) {
            std::stringstream ss(line);
            float values[9];
            // -1161455114	-1161488748		-0.101638 -0.008850 0.042846 0.993859	-0.004179 1.231212 -0.337064
            
            for (int j=0; j < 9; j++) {
                ss >> values[j];
            }
            
            float outArray[7] = {values[2], values[3], values[4], values[5], values[6], values[7], values[8]};
            server->send_data(1234567, outArray);
            ++i;
            //Sleep is not a best practice.
            usleep(100000);

	      }
        in.close();
        
	  }
    

}

FrameWorker::FrameWorker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
FrameWorker::svc (void)
{
  try
    {
      this->run_test ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("FrameException caught in thread (%t)\n");
    }

  return 0;
}

void
FrameWorker::run_test() {

}

Consumer::Consumer() {
   SDecodingParam sDecParam;

    int rv = WelsCreateDecoder(&decoder);

    sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_AVC;
    sDecParam.bParseOnly = false;

    decoder->Initialize(&sDecParam);
}  

void
Consumer::disconnect_push_consumer (void)
{
  // In this example we shutdown the ORB when we disconnect from the
  // EC (or rather the EC disconnects from us), but this doesn't have
  // to be the case....
  //this->orb_->shutdown (false);
}


void
Consumer::push(const CORBA::Any& value )
{
     
     ACE_DEBUG((LM_DEBUG, "PUSH\n"));


    const Simple_Server::frameData* fd;
    value >>= fd;
    Simple_Server::frameData m_fd = (*fd);


    long unsigned int _headerSize = m_fd.m_headerSize;
    Simple_Server::header *hb = &m_fd.m_header;
    unsigned char *hBuf = hb->get_buffer(true);

    long unsigned int _pixelSize = m_fd.m_pixelSize;      
    Simple_Server::pixels* pixelBuff = &m_fd.m_pixels;
    unsigned char *pBuf = pixelBuff->get_buffer(true);


    //output: [0~2] for Y,U,V buffer for Decoding only
    unsigned char *pData[3]  = {nullptr, nullptr, nullptr};
    //in-out: for Decoding only: declare and initialize the output buffer info, this should never co-exist with Parsing only
    SBufferInfo sDstBufInfo;
    memset(&sDstBufInfo, 0, sizeof(SBufferInfo));

    //input: encoded bit stream length; should include the size of start code prefix

    //Copy Header from buffer


    //int pixelSize = *sizeList.begin();
    //sizeList.erase(sizeList.begin());
    int pixelSize = _pixelSize;

    //unsigned int hSize = *sizeList.begin();
    //sizeList.erase(sizeList.begin());
      unsigned int hSize = _headerSize;

      //Allocate new buffer and set magic start code
    uint8_t* newBuf = new uint8_t[4 + hSize + pixelSize];
    uint8_t uiStartCode[4] = {0, 0, 0, 1};

    //HEADER BUFFER
    memcpy(newBuf, hBuf, hSize);

    
    //Copy Pixels from Buffer
    memcpy(newBuf+hSize,  pBuf, pixelSize);
    
    memcpy(newBuf + hSize + pixelSize, &uiStartCode[0], 4);

    DECODING_STATE iRet = decoder->DecodeFrameNoDelay(newBuf, pixelSize+hSize+4, pData, &sDstBufInfo);
            
      //int iRet = 0;
      if (iRet != 0) {
        std::cout << iRet << std::endl;
        //return -1;
      }

    
      if (sDstBufInfo.iBufferStatus==1){
          //output handling (pData[0], pData[1], pData[2])

          //int stride0 = sDstBufInfo.UsrData.sSystemBuffer.iStride[0];
          //int stride1 = sDstBufInfo.UsrData.sSystemBuffer.iStride[1];
          //the third stride is width * 3

          m_mutex.acquire();
          textureBufferList.push_back(pData[0]); 
          textureBufferList.push_back(pData[1]);
          textureBufferList.push_back(pData[2]);
          m_mutex.release();

      }
      else { 
        std::cout << "FAILURE STATUS " << sDstBufInfo.iBufferStatus << std::endl; 
        std::cout << "DIDN'T COMPLETE " << std::endl;
        }
      
      float decodeTime = glfwGetTime();

      //std::cout << "DECODE TIME: \t" << decodeTime - dataTime << "\t";

      //usleep(16333);

          
}
 


VideoWorker::VideoWorker () 
{


 
}

int
VideoWorker::svc (void)
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
VideoWorker::run_test (void)
{
  while (true) {
    try {
      orb_->run();
    }
     
        catch (const CORBA::Exception& ex)
            {
              ex._tao_print_exception ("VideoException caught in thread (%t)\n");

            }
    }
}





unsigned int loadTexture(unsigned char* data)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    //int width, height, nrComponents;
    //unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //delete(data); 
    }
    else
    {
        std::cout << "Texture failed to load" << std::endl;
    }

    return textureID;
}
