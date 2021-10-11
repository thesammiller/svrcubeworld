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

#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/mat.hpp>


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

//https://stackoverflow.com/questions/57212966/how-to-convert-openh264-decodeframenodelay-output-format-to-opencv-matrix
inline void copyWithStride(void* dst, const void* src, size_t width, size_t height, size_t stride
) {
    for (size_t row = 0; row < height; ++row) {
        uint8_t* posFrom = (uint8_t*)src + row * stride;
        uint8_t* posTo = (uint8_t*)dst + row * width;
        memcpy(posTo, posFrom, width);
    }
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

        Shader renderBufferShader("shaders/renderBuffer.vs", "shaders/renderBuffer.fs");

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

     float startTime = glfwGetTime();

    float old_time = 0;

    //decoder declaration
        ISVCDecoder *pSvcDecoder;

    int rv = WelsCreateDecoder(&pSvcDecoder);
    assert (rv == 0);
    ISVCDecoder* decoder_;
    //assert (decoder_ != NULL);


    SDecodingParam sDecParam = {0};
    sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_AVC;
    sDecParam.bParseOnly = false;
    //sDecParam.eEcActiveIdc = ERROR_CON_SLICE_COPY;
    pSvcDecoder->Initialize(&sDecParam);

    //output: [0~2] for Y,U,V buffer for Decoding only
        unsigned char *pData[3]  = {nullptr, nullptr, nullptr};
        //in-out: for Decoding only: declare and initialize the output buffer info, this should never co-exist with Parsing only
        SBufferInfo sDstBufInfo;
        memset(&sDstBufInfo, 0, sizeof(SBufferInfo));
        


    while (!glfwWindowShouldClose(window))
    {
        float dataTime = glfwGetTime();
        //uncompressedBuffer = (unsigned char*) malloc (SCR_WIDTH * SCR_HEIGHT * 3);

        //https://github.com/cisco/openh264/wiki/UsageExampleForDecoder

        

        //input: encoded bitstream start position; should include start code prefix
        
        long unsigned int _headerSize = server->sendHeaderSize();      
        Simple_Server::header* headerBuff = server->sendHeaderData();
        unsigned char *hBuf = (*headerBuff).get_buffer(true);

        long unsigned int _pixelSize = server->sendJpegSize();      
        Simple_Server::pixels* pixelBuff = server->sendImageData();
        unsigned char *pBuf = (*pixelBuff).get_buffer(true);

        //FILE* vout = fopen("client_in.264", "a+");
        //fwrite(hBuf, _headerSize, 1, vout);
        //fwrite(pBuf, _pixelSize, 1, vout);
        //fclose(vout);

       //float timeDiff = glfwGetTime() - dataTime;
       //std::cout << "DATA TIME \t" << timeDiff << std::endl;
        
        //input: encoded bit stream length; should include the size of start code prefix
        int iSize  = _pixelSize;
        
        //memcpy(&sDstBufInfo.UsrData, hBuf, _headerSize);

        uint8_t* newBuf = new uint8_t[4 + _headerSize + iSize];
        uint8_t uiStartCode[4] = {0, 0, 0, 1};

        memcpy(newBuf, hBuf, _headerSize);
        memcpy(newBuf+_headerSize, pBuf, iSize);
        memcpy (newBuf + _headerSize + iSize, &uiStartCode[0], 4);

        

        //WE"RE FAILING HERE
        //I NEED TO READ UP ON THIS PART OF THE API
        DECODING_STATE iRet = pSvcDecoder->DecodeFrameNoDelay(newBuf, iSize+_headerSize+4, pData, &sDstBufInfo);

        


        if (iRet != 0) {
          std::cout << iRet << std::endl;
          //return -1;
        }
        
        if (iRet == 0) {
          std::cout << "SUCCESS" << std::endl;
        }

        
        if (sDstBufInfo.iBufferStatus==1){
            //output handling (pData[0], pData[1], pData[2])
            std::cout << "SUCCESS" << std::endl;
        }
        else { 
          std::cout << "FAILURE STATUS " << sDstBufInfo.iBufferStatus << std::endl; 
          }

        //https://stackoverflow.com/questions/53742981/converting-yuv-420-file-to-rgb-not-getting-expected-output
        //yuv420 to rgb
        
        int l_nSize = 800 * 600 * 1.5;
        unsigned char *g_pcRGBbuffer;
        unsigned char *g_pcYUVBuffer;
        g_pcYUVBuffer = new unsigned char[l_nSize];
        g_pcRGBbuffer = new unsigned char[800 * 600 * 3];
      
        int l_ny, l_nu, l_nv, l_ni, RGBval;
        int l_dr, l_dg, l_db;
        int l_nj; 

        unsigned char *rgbBuffer = new unsigned char[800 * 600 *3];

        if(iRet == 0) {
          unsigned char *l_pcRGBbuffer = g_pcRGBbuffer;
          for (int j = 0; j < 600; j++) {
              for (int i = 0; i < 1024; i++) {
                // position for yuv components for yuv planar
                //wikipedia
                      int Y = g_pcYUVBuffer[j * 800 + i];
                      int U = g_pcYUVBuffer[((j / 2) * 400) + (i / 2) + (800 * 600)];
                      int V = g_pcYUVBuffer[((j / 2) * 400) + (i / 2) + (800 * 600) + ((800 * 600) / 4)];

                       int R = 1.164*(Y - 16) + 1.596*(V - 128);
                       int G = 1.164*(Y - 16) - 0.813*(V - 128) - 0.391*(U - 128);
                       int B = 1.164*(Y - 16) + 2.018*(U - 128);



                       if (R>255)R = 255;
                       if (R<0)R = 0;
                       if (G>255)G = 255;
                       if (G<0)G = 0;
                       if (B>255)B = 255;
                       if (B<0)B = 0;


                       l_pcRGBbuffer[0] = R;
                       l_pcRGBbuffer[1] = G;
                       l_pcRGBbuffer[2] = B;
                       l_pcRGBbuffer += 3;
              }
          }
          
        }
        
        
         //no-delay decoding can be realized by directly calling DecodeFrameNoDelay(), which is the recommended usage.
         //no-delay decoding can also be realized by directly calling DecodeFrame2() again with NULL input, as in the following. In this case, decoder would immediately reconstruct the input data. This can also be used similarly for Parsing only. Consequent decoding error and output indication should also be considered as above.
         //iRet = pSvcDecoder->DecodeFrame2(NULL, 0, pData, &sDstBufInfo);


        //https://stackoverflow.com/questions/57212966/how-to-convert-openh264-decodeframenodelay-output-format-to-opencv-matrix

        int width = 800;
        int height = 600;
        
        /*
        int stride0 = sDstBufInfo.UsrData.sSystemBuffer.iStride[0];
                int stride1 = sDstBufInfo.UsrData.sSystemBuffer.iStride[1];
        
        std::cout << stride0 << std::endl;


        cv::Mat imageYuvCh[3] {
           cv::Mat(width, height, CV_8UC1, pData[0]), 
          cv::Mat(width/2, height/2, CV_8UC1, pData[1]), 
          cv::Mat(width/2, height/2, CV_8UC1, pData[2])
        };
        cv::Mat imageYuvMiniCh[3] = {
          cv::Mat(width, height, CV_8UC1, pData[0]), 
          cv::Mat(width/2, height/2, CV_8UC1, pData[1]), 
          cv::Mat(width/2, height/2, CV_8UC1, pData[2])
        };

        
        copyWithStride(imageYuvCh[0].data, pData[0], width, height, stride0);
        copyWithStride(imageYuvMiniCh[1].data, pData[1], width/2, height/2, stride1);
        copyWithStride(imageYuvMiniCh[2].data, pData[2], width/2, height/2, stride1);
        

        //imageYuvCh[0].data = (unsigned char*) malloc (width * height * sizeof(unsigned char));
        //imageYuvMiniCh[1].data = (unsigned char*) malloc (width/2 * height/2 * sizeof(unsigned char));
        //imageYuvMiniCh[2].data = (unsigned char*) malloc (width/2 * height/2 * sizeof(unsigned char));

        //memcpy(imageYuvCh[0].data, pData[0], width * height);
        //memcpy(imageYuvMiniCh[1].data, pData[1], width/2 * height/2);
        //memcpy(imageYuvMiniCh[2].data, pData[2], width/2 * height/2);


        cv::resize(imageYuvMiniCh[1], imageYuvCh[1], cv::Size(width, height));
        cv::resize(imageYuvMiniCh[2], imageYuvCh[2], cv::Size(width, height));

        cv::Mat *mv = imageYuvCh; 
        size_t n = 3;
        cv::Mat resultYuv;
        int i;
        int depth = mv[0].depth();

        for( i = 0; i < n; i++ )
        {
            std::cout << "mv " << i << " size \t" << mv[i].size << "\t0\t" << mv[0].size;
            std::cout << "mv " << i << "depth \t" << mv[i].depth() << "\t0\t" <<  depth;
            
        }
            
        //cv::Mat resultYuv;
        cv::merge(imageYuvCh, 3, resultYuv);

        cv::Mat image = cv::Mat();
        
        //cv::Mat result;
        cvtColor(resultYuv, image, cv::COLOR_YUV2BGR);

        cv::imwrite("file.tga", image);

        */


      //https://stackoverflow.com/questions/16809833/opencv-image-loading-for-opengl-texture

      /*
      
      unsigned int textureTrash;
      glGenTextures(1, &textureTrash);
      glBindTexture(GL_TEXTURE_2D, textureTrash);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set texture clamping method
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


      glTexImage2D(GL_TEXTURE_2D,     // Type of texture
                     0,                 // Pyramid level (for mip-mapping) - 0 is the top level
                     GL_RGB,            // Internal colour format to convert to
                     width,          // Image width  i.e. 640 for Kinect in standard mode
                     height,          // Image height i.e. 480 for Kinect in standard mode
                     0,                 // Border width in pixels (can either be 1 or 0)
                     GL_BGR, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                     GL_UNSIGNED_BYTE,  // Image data type
                     pData[0]);        // The actual image data itself

      glGenerateMipmap(GL_TEXTURE_2D);

      unsigned int _textures[3];


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


      

      

       GLint _uniformSamplers[3];
      _uniformSamplers[0] = glGetUniformLocation(renderBufferShader.ID, "s_texture_y");
      _uniformSamplers[1] = glGetUniformLocation(renderBufferShader.ID, "s_texture_u");
      _uniformSamplers[2] = glGetUniformLocation(renderBufferShader.ID, "s_texture_v");

      
     */

      //Select the GL Shader for Framebuffer
      renderBufferShader.use();
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

      /*
        for (int i = 0; i < 3; ++i) {  
          glActiveTexture(GL_TEXTURE0 + i);  
          glBindTexture(GL_TEXTURE_2D, _textures[i]);  
          glUniform1i(_uniformSamplers[i], i);  
          glDrawArrays(GL_TRIANGLES, 0, 6);
          
      }*/  
      
      

      //CORBA::Octet* uncompressedBuffer = (*textureBufferList.begin());

      //OPENGL TEXTURE LOAD AND DRAW
      pixelTexture = loadTexture(g_pcRGBbuffer);
      glBindTexture(GL_TEXTURE_2D, pixelTexture); 

      glDrawArrays(GL_TRIANGLES, 0, 6);
      
      
      
      //PSwap framebuffer to front buffer
      glfwSwapBuffers(window);
      glfwPollEvents();

      
      //Release GL data
      //glDeleteTextures(1, &pixelTexture);
      glFinish();
      glFlush();

      //delete(uncompressedBuffer);
      //textureBufferList.erase(textureBufferList.begin());

      //SLEEP
      //-----
      //This helps...
      //TODO: NOT OPTIMAL
      //But trying 120 frames per second
      //usleep -- > 1 sec = 1,000,000
      //1,000,000 / 16 = 16,666
      // 1/2 of that is 8333

      if ( (old_time + 1) < glfwGetTime() ) {
            std::cout << "FPS " << frame << std::endl;
            frame = 0;
            old_time = glfwGetTime();
        }
        ++frame;

    

     
      
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

