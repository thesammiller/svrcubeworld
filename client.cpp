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


const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 1024;

std::vector<CORBA::Octet*> textureBufferList;


unsigned int loadTexture(unsigned char *data);

typedef enum
{
	YCBCR_JPEG,
	YCBCR_601,
	YCBCR_709
} YCbCrType;

void yuv420_rgb24_std(
	uint32_t width, uint32_t height, 
	const uint8_t *y, const uint8_t *u, const uint8_t *v, uint32_t y_stride, uint32_t uv_stride, 
	uint8_t *rgb, uint32_t rgb_stride, 
	YCbCrType yuv_type);

  typedef struct
{
	uint8_t cb_factor;   // [(255*CbNorm)/CbRange]
	uint8_t cr_factor;   // [(255*CrNorm)/CrRange]
	uint8_t g_cb_factor; // [Bf/Gf*(255*CbNorm)/CbRange]
	uint8_t g_cr_factor; // [Rf/Gf*(255*CrNorm)/CrRange]
	uint8_t y_factor;    // [(YMax-YMin)/255]
	uint8_t y_offset;    // YMin
} YUV2RGBParam;

#define FIXED_POINT_VALUE(value, precision) ((int)(((value)*(1<<precision))+0.5))

#define YUV2RGB_PARAM(Rf, Bf, YMin, YMax, CbCrRange) \
{.cb_factor=FIXED_POINT_VALUE(255.0*(2.0*(1-Bf))/CbCrRange, 6), \
.cr_factor=FIXED_POINT_VALUE(255.0*(2.0*(1-Rf))/CbCrRange, 6), \
.g_cb_factor=FIXED_POINT_VALUE(Bf/(1.0-Bf-Rf)*255.0*(2.0*(1-Bf))/CbCrRange, 7), \
.g_cr_factor=FIXED_POINT_VALUE(Rf/(1.0-Bf-Rf)*255.0*(2.0*(1-Rf))/CbCrRange, 7), \
.y_factor=FIXED_POINT_VALUE(255.0/(YMax-YMin), 7), \
.y_offset=YMin}

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

    //unsigned int pixelTexture;
    //glGenTextures(1, &pixelTexture);

    
    int frame = 0;

  
    FrameWorker* fw = (FrameWorker *) malloc (sizeof(FrameWorker) * 1);
    for (int i=0; i < 1; i++) {
      FrameWorker frameWorker(orb.in());

      if (frameWorker.activate (THR_NEW_LWP | THR_JOINABLE, nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate worker threads\n"),
                          1);
      FrameWorker * currentWorker = fw+i;
      currentWorker = &frameWorker;
      usleep(8666);
      

    }

     float startTime = glfwGetTime();

    float old_time = 0;

    sleep(1);
  
    
    while (!glfwWindowShouldClose(window))
    {
      if (textureBufferList.size() < 3) {
        continue;
      }
       
      float dataTime = glfwGetTime();

      //Select the GL Shader for Framebuffer
      renderBufferShader.use();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDisable(GL_DEPTH_TEST);
      //Clear screen (to white)
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      //Bind the Framebuffer Quad Vertex
      glBindVertexArray(quadVAO);
      glEnable(GL_TEXTURE_2D);

      
      unsigned int _uniformSamplers[3];
      _uniformSamplers[0] = glGetUniformLocation(renderBufferShader.ID, "YTex");
      _uniformSamplers[1] = glGetUniformLocation(renderBufferShader.ID, "UTex");
      _uniformSamplers[2] = glGetUniformLocation(renderBufferShader.ID, "VTex");

      unsigned int _textures[3];
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  
      glGenTextures(3, _textures);  
      //const unsigned char *pixels[3] = { pData0, pData1, pData2 };  
      const unsigned int widths[3]  = { SCR_WIDTH, SCR_WIDTH / 2, SCR_WIDTH / 2 };  
      const unsigned int heights[3] = { SCR_HEIGHT , SCR_HEIGHT / 2, SCR_HEIGHT / 2 };  
      for (int i = 0; i < 3; ++i) {  
          m_mutex.acquire();
          glBindTexture(GL_TEXTURE_RECTANGLE_NV, _textures[i]);  
          glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_LUMINANCE, widths[i],heights[i],0,GL_LUMINANCE,GL_UNSIGNED_BYTE,*textureBufferList.begin());  
          //The data is there... 
          //std::cout << *textureBufferList.begin() << std::endl;
          glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
          glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
          glTexParameterf(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
          glTexParameterf(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
          glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL); 
          glActiveTexture(GL_TEXTURE0 + i);  
          glBindTexture(GL_TEXTURE_RECTANGLE_NV, _textures[i]);  
          glUniform1i(_uniformSamplers[i], i);  
          textureBufferList.erase(textureBufferList.begin());
          m_mutex.release();
      }

      glDrawArrays(GL_TRIANGLES, 0, 6);
      
      //PSwap framebuffer to front buffer
      glfwSwapBuffers(window);
      glfwPollEvents();

      glFinish();
      glFlush();


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

        int success = 0;
        int total = 0;

      while(true) {
        try {
          float startTime = glfwGetTime();
          CORBA::Octet* uncompressedBuffer = (unsigned char*) malloc (1024 * 1024 * 3);

          long unsigned int _headerSize = server->sendHeaderSize();      
          Simple_Server::header* headerBuff = server->sendHeaderData();
          unsigned char *hBuf = (*headerBuff).get_buffer(true);

          long unsigned int _pixelSize = server->sendJpegSize();      
          Simple_Server::pixels* pixelBuff = server->sendImageData();
          unsigned char *pBuf = (*pixelBuff).get_buffer(true);

          usleep(8333);

          float dataTime = glfwGetTime();

          std::cout << "TAO DATA TIME: \t" << dataTime - startTime << "\t";

          //input: encoded bit stream length; should include the size of start code prefix
          int iSize  = _pixelSize;
          
          //memcpy(&sDstBufInfo.UsrData, hBuf, _headerSize);

          uint8_t* newBuf = new uint8_t[4 + _headerSize + iSize];
          uint8_t uiStartCode[4] = {0, 0, 0, 1};

          memcpy(newBuf, hBuf, _headerSize);
          memcpy(newBuf+_headerSize, pBuf, iSize);
          memcpy(newBuf + _headerSize + iSize, &uiStartCode[0], 4);


          DECODING_STATE iRet = pSvcDecoder->DecodeFrameNoDelay(newBuf, iSize+_headerSize+4, pData, &sDstBufInfo);

          if (iRet != 0) {
            std::cout << iRet << std::endl;
            //return -1;
          }
          
          if (iRet == 0) {
            //std::cout << "SUCCESS" << std::endl;
          }

          float decodeTime = glfwGetTime();

          std::cout << "DECODE TIME: \t" << decodeTime - dataTime << "\t";


          if (sDstBufInfo.iBufferStatus==1){
              //output handling (pData[0], pData[1], pData[2])

              int stride0 = sDstBufInfo.UsrData.sSystemBuffer.iStride[0];
              int stride1 = sDstBufInfo.UsrData.sSystemBuffer.iStride[1];
              //the third stride is width * 3
             //yuv420_rgb24_std(SCR_WIDTH, SCR_HEIGHT, pData[0], pData[1], pData[2], (uint32_t) stride0, (uint32_t) stride1, uncompressedBuffer, (uint32_t) (1024 * 3), YCBCR_709);

              m_mutex.acquire();
              textureBufferList.push_back(pData[0]); 
              textureBufferList.push_back(pData[1]);
              textureBufferList.push_back(pData[2]);
              m_mutex.release();

              std::cout << "YUV2RGB TIME:\t" << 0 << std::endl;
              success++;
          }
          else { 
            std::cout << "FAILURE STATUS " << sDstBufInfo.iBufferStatus << std::endl; 
            std::cout << "DIDN'T COMPLETE " << std::endl;
            }

            

            total++;

            std::cout << "SUCCESS\t" << success << "\tTOTAL\t" << total << std::endl;
        }
         catch (const CORBA::Exception& ex)
          {
            ex._tao_print_exception ("FrameException caught in thread (%t)\n");


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

uint8_t clamp(int16_t value)
{
	return value<0 ? 0 : (value>255 ? 255 : value);
}



static const YUV2RGBParam YUV2RGB[3] = {
	// ITU-T T.871 (JPEG)
	YUV2RGB_PARAM(0.299, 0.114, 0.0, 255.0, 255.0),
	// ITU-R BT.601-7
	YUV2RGB_PARAM(0.299, 0.114, 16.0, 235.0, 224.0),
	// ITU-R BT.709-6
	YUV2RGB_PARAM(0.2126, 0.0722, 16.0, 235.0, 224.0)
};



//From https://raw.githubusercontent.com/descampsa/yuv2rgb/master/yuv_rgb.c
//Stolen function directly from code
//Thank you to the developer for making it freely available.
//This is not an optimal implementation.
void yuv420_rgb24_std(
	uint32_t width, uint32_t height, 
	const uint8_t *Y, const uint8_t *U, const uint8_t *V, uint32_t Y_stride, uint32_t UV_stride, 
	uint8_t *RGB, uint32_t RGB_stride, 
	YCbCrType yuv_type)
{
	const YUV2RGBParam *const param = &(YUV2RGB[yuv_type]);
	uint32_t x, y;
	for(y=0; y<(height-1); y+=2)
	{
		const uint8_t *y_ptr1=Y+y*Y_stride,
			*y_ptr2=Y+(y+1)*Y_stride,
			*u_ptr=U+(y/2)*UV_stride,
			*v_ptr=V+(y/2)*UV_stride;
		
		uint8_t *rgb_ptr1=RGB+y*RGB_stride,
			*rgb_ptr2=RGB+(y+1)*RGB_stride;
		
		for(x=0; x<(width-1); x+=2)
		{
			int8_t u_tmp, v_tmp;
			u_tmp = u_ptr[0]-128;
			v_tmp = v_ptr[0]-128;
			
			//compute Cb Cr color offsets, common to four pixels
			int16_t b_cb_offset, r_cr_offset, g_cbcr_offset;
			b_cb_offset = (param->cb_factor*u_tmp)>>6;
			r_cr_offset = (param->cr_factor*v_tmp)>>6;
			g_cbcr_offset = (param->g_cb_factor*u_tmp + param->g_cr_factor*v_tmp)>>7;
			
			int16_t y_tmp;
			y_tmp = (param->y_factor*(y_ptr1[0]-param->y_offset))>>7;
			rgb_ptr1[0] = clamp(y_tmp + r_cr_offset);
			rgb_ptr1[1] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr1[2] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr1[1]-param->y_offset))>>7;
			rgb_ptr1[3] = clamp(y_tmp + r_cr_offset);
			rgb_ptr1[4] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr1[5] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr2[0]-param->y_offset))>>7;
			rgb_ptr2[0] = clamp(y_tmp + r_cr_offset);
			rgb_ptr2[1] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr2[2] = clamp(y_tmp + b_cb_offset);
			
			y_tmp = (param->y_factor*(y_ptr2[1]-param->y_offset))>>7;
			rgb_ptr2[3] = clamp(y_tmp + r_cr_offset);
			rgb_ptr2[4] = clamp(y_tmp - g_cbcr_offset);
			rgb_ptr2[5] = clamp(y_tmp + b_cb_offset);
			
			rgb_ptr1 += 6;
			rgb_ptr2 += 6;
			y_ptr1 += 2;
			y_ptr2 += 2;
			u_ptr += 1;
			v_ptr += 1;
		}
	}
}