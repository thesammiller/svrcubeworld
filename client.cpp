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


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::vector<CORBA::Octet*> textureBufferList;


unsigned int loadTexture(unsigned char *data);

ACE_Thread_Mutex m_mutex;

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

  
     CORBA::Object_var object =
        orb->string_to_object (ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

           
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

    unsigned int pixelTexture;
    glGenTextures(1, &pixelTexture);


    CORBA::Octet* uncompressedBuffer;
    //CORBA::Octet* jpegBuff;
    
    int frame = 0;

    

    /*
    FrameWorker* fw = (FrameWorker *) malloc (sizeof(FrameWorker) * 1);
    for (int i=0; i < 1; i++) {
      FrameWorker frameWorker(orb.in());

      if (frameWorker.activate (THR_NEW_LWP | THR_JOINABLE, nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate worker threads\n"),
                          1);
      FrameWorker * currentWorker = fw+i;
      currentWorker = &frameWorker;
      usleep(16666);

    }*/






    while (!glfwWindowShouldClose(window))
    {
     
        //Decompression handle
        //tjhandle handle = tjInitDecompress();

        //Get the size of the JPEG from the server
        
        //Allocate size for the buffer for TAO
        
        //Get the TAO data handler     
        //Create the uncompressedBuffer for JPEG Decompression
        
        //jpegBuff = (unsigned char*) malloc (_jpegSize);

        
        uncompressedBuffer = (unsigned char*) malloc (SCR_WIDTH * SCR_HEIGHT * 3);


          //decoder declaration
        ISVCDecoder *pSvcDecoder;
        //input: encoded bitstream start position; should include start code prefix
        long unsigned int _jpegSize = server->sendJpegSize();      
        long unsigned int _headerSize = server->sendHeaderSize();      
        std::cout << _jpegSize << std::endl;
        Simple_Server::pixels* taoBuff = server->sendImageData();

        Simple_Server::header* headerBuff = server->sendHeaderData();
        std::cout<< _headerSize << std::endl;

        unsigned char *pBuf = (*taoBuff).get_buffer(true);
        unsigned char *hBuf = (*headerBuff).get_buffer(true);

        FILE* file = fopen("test2.264", "a+");
        fwrite(headerBuff, _headerSize, 1, file);
        fwrite(pBuf, _jpegSize, 1, file);
        fclose(file);   
        //input: encoded bit stream length; should include the size of start code prefix
        int iSize  = _jpegSize;
        //output: [0~2] for Y,U,V buffer for Decoding only
        unsigned char *pData[3]  {0, 0, 0};
        //in-out: for Decoding only: declare and initialize the output buffer info, this should never co-exist with Parsing only
        SBufferInfo sDstBufInfo;
        memset(&sDstBufInfo, 0, sizeof(SBufferInfo));

        WelsCreateDecoder(&pSvcDecoder);

        SDecodingParam sDecParam = {0};
        sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_AVC;
        //for Parsing only, the assignment is mandatory
        sDecParam.bParseOnly = false;

        pSvcDecoder->Initialize(&sDecParam);

        DECODING_STATE iRet = pSvcDecoder->DecodeFrameNoDelay(pBuf, iSize, pData, &sDstBufInfo);
        //for Decoding only, pData can be used for render.
        if (sDstBufInfo.iBufferStatus==1){
            //output handling (pData[0], pData[1], pData[2])
        }
       
         //no-delay decoding can be realized by directly calling DecodeFrameNoDelay(), which is the recommended usage.
         //no-delay decoding can also be realized by directly calling DecodeFrame2() again with NULL input, as in the following. In this case, decoder would immediately reconstruct the input data. This can also be used similarly for Parsing only. Consequent decoding error and output indication should also be considered as above.
         //iRet = pSvcDecoder->DecodeFrame2(NULL, 0, pData, &sDstBufInfo);
      
      std::cout << "CLIENT FRAME " << ++frame << std::endl;

      unsigned int _textures[3];
      int width = 800;
      int height = 600;

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  
      glGenTextures(3, _textures);  
      const unsigned char *pixels[3] = { pData[0], pData[1], pData[2] };  
      const unsigned int widths[3]  = { width, width / 2, width / 2 };  
      const unsigned int heights[3] = { height, height / 2, height / 2 };  
      for (int i = 0; i < 3; ++i) {  
          glBindTexture(GL_TEXTURE_2D, _textures[i]);  
          glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, widths[i],heights[i],0,GL_LUMINANCE,GL_UNSIGNED_BYTE,pixels[i]);  
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      
      
      }

      static const GLfloat texCoords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };  
      static const GLfloat vertices[]= {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
      glClear(GL_COLOR_BUFFER_BIT);  
      renderBufferShader.use();

       GLint _uniformSamplers[3];
        _uniformSamplers[0] = glGetUniformLocation(renderBufferShader.ID, "s_texture_y");
      _uniformSamplers[1] = glGetUniformLocation(renderBufferShader.ID, "s_texture_u");
      _uniformSamplers[2] = glGetUniformLocation(renderBufferShader.ID, "s_texture_v");

     

      //Select the GL Shader for Framebuffer
      renderBufferShader.use();
      // Default framebuffer
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDisable(GL_DEPTH_TEST);
      //Clear screen (to white)
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      //Bind the Framebuffer Quad Vertex
      glBindVertexArray(quadVAO);

      
      //glVertexAttribPointer(GL_ATTRIBUTE_VERTEX, 2, GL_FLOAT, 0, 0, vertices);  
      //glEnableVertexAttribArray(ATTRIBUTE_VERTEX);  
      //glVertexAttribPointer(ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, 0, 0, texCoords);  
      //glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);  
      //glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer); 

        for (int i = 0; i < 3; ++i) {  
          glActiveTexture(GL_TEXTURE0 + i);  
          glBindTexture(GL_TEXTURE_2D, _textures[i]);  
          glUniform1i(_uniformSamplers[i], i);  
      }  
      

      //CORBA::Octet* uncompressedBuffer = (*textureBufferList.begin());

      //OPENGL TEXTURE LOAD AND DRAW
      //pixelTexture = loadTexture(uncompressedBuffer);
      //glBindTexture(GL_TEXTURE_2D, pixelTexture); 
      glDrawArrays(GL_TRIANGLES, 0, 6);
      
      
      //PSwap framebuffer to front buffer
      glfwSwapBuffers(window);
      glfwPollEvents();

      
      //Release GL data
      glDeleteTextures(1, &pixelTexture);
      glFinish();
      glFlush();

      delete(uncompressedBuffer);
      //textureBufferList.erase(textureBufferList.begin());

      //SLEEP
      //-----
      //This helps...
      //TODO: NOT OPTIMAL
      //But trying 120 frames per second
      //usleep -- > 1 sec = 1,000,000
      //1,000,000 / 16 = 16,666
      // 1/2 of that is 8333

     
      
  }


    worker.thr_mgr ()->wait ();

      orb->destroy ();
    

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
FrameWorker::run_test (void)
{
      CORBA::Object_var object =
        orb_->string_to_object (ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

        
      while(true) {



        std::cout << "Frame Thread" << std::endl;
        //Decompression handle
        //tjhandle handle = tjInitDecompress();

        //Get the size of the JPEG from the server
        long unsigned int _jpegSize = server->sendJpegSize();      
        //Allocate size for the buffer for TAO
        Simple_Server::pixels* taoBuff = server->sendImageData();
        //Get the TAO data handler     
        //Create the uncompressedBuffer for JPEG Decompression
        
        //jpegBuff = (unsigned char*) malloc (_jpegSize);

        
        CORBA::Octet* uncompressedBuffer = (unsigned char*) malloc (SCR_WIDTH * SCR_HEIGHT * 3);


          //decoder declaration
        ISVCDecoder *pSvcDecoder;
        //input: encoded bitstream start position; should include start code prefix
        unsigned char *pBuf = (*taoBuff).get_buffer(true);
        //input: encoded bit stream length; should include the size of start code prefix
        int iSize  = _jpegSize;
        //output: [0~2] for Y,U,V buffer for Decoding only
        unsigned char *pData[3] = {0, 0, 0};
        //in-out: for Decoding only: declare and initialize the output buffer info, this should never co-exist with Parsing only
        SBufferInfo sDstBufInfo;
        memset(&sDstBufInfo, 0, sizeof(SBufferInfo));

        WelsCreateDecoder(&pSvcDecoder);

        SDecodingParam sDecParam = {0};
        sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_AVC;
        //for Parsing only, the assignment is mandatory
        sDecParam.bParseOnly = true;

        pSvcDecoder->Initialize(&sDecParam);

        DECODING_STATE iRet = pSvcDecoder->DecodeFrameNoDelay(pBuf, iSize, pData, &sDstBufInfo);
        //for Decoding only, pData can be used for render.
        if (sDstBufInfo.iBufferStatus==1){
            //output handling (pData[0], pData[1], pData[2])
        }
         //no-delay decoding can be realized by directly calling DecodeFrameNoDelay(), which is the recommended usage.
         //no-delay decoding can also be realized by directly calling DecodeFrame2() again with NULL input, as in the following. In this case, decoder would immediately reconstruct the input data. This can also be used similarly for Parsing only. Consequent decoding error and output indication should also be considered as above.
         iRet = pSvcDecoder->DecodeFrame2(NULL, 0, pData, &sDstBufInfo);


        int COLOR_COMPONENTS = 3; //RGB
        int jpegSubsamp;
        int width = (int) SCR_WIDTH;
        int height = (int) SCR_HEIGHT;
        int pitch = width * COLOR_COMPONENTS;

        //CORBA::Octet* jpegBuff = (*taoBuff).get_buffer(true);
        //JPEG_TURBO DECOMPRESSION
        //Send the TAO Buffer in directly 
        /*tjDecompressHeader2(handle, jpegBuff, _jpegSize, &width, &height, &jpegSubsamp);
        tjDecompress2(handle, jpegBuff, _jpegSize, uncompressedBuffer, width, pitch, height, TJPF_RGB, TJFLAG_FASTDCT); 

        if (textureBufferList.size() > 16) {
          
          std::cout << "Clearing Texture Buffer List" << std::endl;
          textureBufferList.erase(textureBufferList.begin(), textureBufferList.end());
          
        }

        m_mutex.acquire();
        textureBufferList.push_back(uncompressedBuffer); 
        m_mutex.release();

        //Release TAO data
        (*taoBuff).freebuf(jpegBuff);
        delete(taoBuff);

        std::cout << "Frame stashed " << glfwGetTime() << std::endl;
        usleep(16333);*/
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

