#include "svr/svrAppl.h"
#include <iostream>
#include <unistd.h>




//Square (two triangles) for framebuffer to hold texture
float quadVertices[] = {
        // positions  //texcoords
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f     };


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


//OVR Helper Application for converting OVR Type to Vertex Array for GL
template <typename _attrib_type_>
void PackVertexAttribute(
    std::vector<uint8_t>& packed,
    const std::vector<_attrib_type_>& attrib,
    const int glLocation,
    const int glType,
    const int glComponents) {
    if (attrib.size() > 0) {
        const size_t offset = packed.size();
        const size_t size = attrib.size() * sizeof(attrib[0]);

        packed.resize(offset + size);
        memcpy(&packed[offset], attrib.data(), size);

        glEnableVertexAttribArray(glLocation);
        glVertexAttribPointer(
            glLocation, glComponents, glType, false, sizeof(attrib[0]), (void*)(offset));
    } else {
        glDisableVertexAttribArray(glLocation);
    }
}

//Used by OpenGL for Vertex information
enum VertexAttributeLocation {
    VERTEX_ATTRIBUTE_LOCATION_POSITION = 0,
    VERTEX_ATTRIBUTE_LOCATION_COLOR = 1,
    VERTEX_ATTRIBUTE_LOCATION_ROTATION = 2
};


svrAppl::svrAppl() {

    int rv = WelsCreateSVCEncoder (&encoder_);
    assert (rv == 0);
    assert (encoder_ != NULL);

    memset (&param, 0, sizeof (SEncParamBase));
    param.iUsageType = CAMERA_VIDEO_REAL_TIME; //from EUsageType enum
    param.fMaxFrameRate = 60.0f;
    param.iPicWidth = 1024;
    param.iPicHeight = 1024;
    param.iTargetBitrate = 8500000;
    

    
}

/*
svrAppl::createImage()
- Reads pixels from OpenGL 
- Compressed pixels into JPEG
- Stores Pixels and Size in svrAppl object
*/
void svrAppl::createImage() {
    //OPEN GL
    //Read uncompressed image buffer as source

    float read_buffer_time = glfwGetTime();
    unsigned char* srcBuf = (unsigned char*) malloc (m_width * m_height * 3);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    //Read pixels from the GL Draw
    glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, srcBuf);
    float read_buffer_end_time = glfwGetTime();

    std::cout << "GLREADBUFFER TIME:\t" << read_buffer_end_time - read_buffer_time << "\t";

    float jpeg_time = glfwGetTime();

    int width = 1024;
    int height = 1024;

    
    cv::Mat image (1024, 1024, CV_8UC3, (void *) srcBuf); //cv::imdecode(rawData, cv::IMREAD_COLOR);

    //U800stackoverflow.com/questions/51287413/how-do-i-properly-use-the-openh264-usage-code-example-for-encoding
    //Thanks to TimSC on StackOverflow
    cv::Mat imageResized, imageYuv, imageYuvMini; 
    resize(image, imageResized, cv::Size(width, height));
    cv::Mat imageYuvCh[3], imageYuvMiniCh[3];
    cvtColor(imageResized, imageYuv, cv::COLOR_BGR2YUV);
    split(imageYuv, imageYuvCh);
    resize(imageYuv, imageYuvMini, cv::Size(width/2, height/2));
    split(imageYuvMini, imageYuvMiniCh);
    float opencv_end_time = glfwGetTime();

    std::cout << "OPENCV TIME:\t" << opencv_end_time - read_buffer_end_time << "\t";

    //step 4: encode and store output bitstream
    SFrameBSInfo info;
    memset (&info, 0, sizeof (SFrameBSInfo));
    SSourcePicture pic;
    memset (&pic, 0, sizeof (SSourcePicture));
    pic.iPicWidth = width;
    pic.iPicHeight = height;
    pic.iColorFormat = videoFormatI420;
    pic.iStride[0] = imageYuvCh[0].step;
    pic.iStride[1] = imageYuvMiniCh[1].step;
    pic.iStride[2] = imageYuvMiniCh[2].step;
    pic.pData[0] = imageYuvCh[0].data;
    pic.pData[1] = imageYuvMiniCh[1].data;
    pic.pData[2] = imageYuvMiniCh[2].data;

    //initialize encoder
    //Tried to refactor to constructor but ran into errors
    encoder_->Initialize (&param);

    //step 3: set option, during encoding process
    static int     g_LevelSetting = WELS_LOG_ERROR;
    encoder_->SetOption (ENCODER_OPTION_TRACE_LEVEL, &g_LevelSetting);
    int videoFormat = videoFormatI420;
    encoder_->SetOption (ENCODER_OPTION_DATAFORMAT, &videoFormat);


    int rve = encoder_->EncodeFrame (&pic, &info);
    assert (rve == cmResultSuccess);
    
    if (info.eFrameType != videoFrameTypeSkip) {
      
        SLayerBSInfo* pLayerBsInfo = &info.sLayerInfo[0];
        int iLayerSize = 0;
        int iNalIdx = pLayerBsInfo->iNalCount - 1;
        //std::cout << "iNalIdx\t" << iNalIdx << std::endl;
        do {
            iLayerSize += pLayerBsInfo->pNalLengthInByte[iNalIdx];
            --iNalIdx;
        } while (iNalIdx >= 0);
        //Allocate the header
        header = (unsigned char *) malloc (iLayerSize * sizeof(unsigned char));
        headerSize = iLayerSize;
        memcpy(header, pLayerBsInfo->pBsBuf, headerSize);

        pLayerBsInfo = &info.sLayerInfo[1];
        iLayerSize = 0;
        iNalIdx = pLayerBsInfo->iNalCount - 1;
        do {
            iLayerSize += pLayerBsInfo->pNalLengthInByte[iNalIdx];
            --iNalIdx;
        } while (iNalIdx >= 0);
        pixels = (unsigned char *) malloc (iLayerSize * sizeof(unsigned char));
        jpegSize = iLayerSize;
        memcpy(pixels, pLayerBsInfo->pBsBuf, jpegSize);

    }
    float encode_end = glfwGetTime();
    std::cout << "ENCODE END TIME:\t" << encode_end - opencv_end_time << std::endl;


    //to free the memory allocated by TurboJPEG (either by tjAlloc(), 
    //or by the Compress/Decompress) after you are done working on it:
    //tjFree(pixels);

    delete(srcBuf);

}


/*
Create the OpenGL Window
*/
int svrAppl::createWindow(unsigned int width, unsigned int height, char *name) {
    m_height = height;
    m_width = width;
    // glfw: initialize and configure

    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glEnable(GL_DEPTH_TEST);

    return 1;

}

/*
Create OpenGL Shader
*/
void svrAppl::createShader() {
     

    //build and compile shader
    program = Shader("./shaders/vertexShader.vs", "./shaders/fragmentShader.fs");


}

/*
Create OpenGL Framebuffer
*/
void svrAppl::createFramebuffer() {

    renderBufferShader = Shader("./shaders/renderBuffer.vs", "./shaders/renderBuffer.fs");

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

    renderBufferShader.use();

    // OVR RENDER BUFFER LOGIC 
    renderBufferShader.setInt("screenTexture", 0);

    glGenTextures(1, &textureColorbuffer);
    GLenum colorTextureTarget = GL_TEXTURE_2D;
    GL(glBindTexture(colorTextureTarget, textureColorbuffer));
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
    GLfloat borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f};
    GL(glTexParameterfv(colorTextureTarget, GL_TEXTURE_BORDER_COLOR, borderColor));
    GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL(glTexParameteri(colorTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL(glBindTexture(colorTextureTarget, 0));

    // Create depth buffer.
    GL(glGenRenderbuffers(1, &rbo));
    GL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
    GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height));
    GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    // Create the frame buffer.
    GL(glGenFramebuffers(1, &framebuffer));
    GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer));
    GL(glFramebufferRenderbuffer(
        GL_DRAW_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER,
        rbo));
    GL(glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0));
        
    GL(GLenum renderFramebufferStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER));
    GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
    if (renderFramebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Incomplete frame buffer object: " << std::endl;
    }

    GL(glGenBuffers(1, &InstanceTransformBuffer));

}

/*
Render Vertex Data to OpenGL Framebuffer
- Will also CreateImage, right before drawing to screen

*/
void svrAppl::render() {

    // Bind to our new buffer
    // Set the framebuffer as the current
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST);

    // OVR CubeWorld Game Logic Updates -- update instance transforms
    // ---------------------------------

    program.use();
    float time = glfwGetTime();
    
    //Make sure we bind to the VertexArrayObject
    glBindVertexArray(vertexArrayObject);

    GL(glBindBuffer(GL_ARRAY_BUFFER, InstanceTransformBuffer));
    GL(glBufferData(
        GL_ARRAY_BUFFER, NUM_INSTANCES * 4 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW));
    
    for (int i = 0; i < 4; i++) {
        GL(glEnableVertexAttribArray(VertexTransformAttribute + i));
        GL(glVertexAttribPointer(
            VertexTransformAttribute + i,
            4,
            GL_FLOAT,
            false,
            4 * 4 * sizeof(float),
            (void*)(i * 4 * sizeof(float))));
        GL(glVertexAttribDivisor(VertexTransformAttribute + i, 1));
    }
        
    GL(glBindBuffer(GL_ARRAY_BUFFER, InstanceTransformBuffer));

    GL(ovrMatrix4f* cubeTransforms = (ovrMatrix4f*)glMapBufferRange(
    GL_ARRAY_BUFFER,
    0,
    NUM_INSTANCES * sizeof(ovrMatrix4f),
    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

    update_cube_rotations(startTime, time, Rotations, NUM_ROTATIONS, cubeTransforms, NUM_INSTANCES, CubeRotations, CubePositions); 
    
    GL(glUnmapBuffer(GL_ARRAY_BUFFER));

    // GL Clear Screen and Prepare Draw
    // --------------------------------
    GL(glEnable(GL_SCISSOR_TEST));
    GL(glDepthMask(GL_TRUE));
    GL(glEnable(GL_DEPTH_TEST));
    GL(glDepthFunc(GL_LEQUAL));
    GL(glEnable(GL_CULL_FACE));
    GL(glViewport(0, 0, m_width, m_height));
    GL(glScissor(0, 0, m_width, m_height));
    GL(glClearColor(0.016f, 0.0f, 0.016f, 1.0f));
    GL(glEnable(GL_FRAMEBUFFER_SRGB_EXT));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GL(glDisable(GL_FRAMEBUFFER_SRGB_EXT));

    // GLM Update Camera Postion
    // -------------------------
    updateView();
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); 
    
    unsigned int viewLoc = glGetUniformLocation(program.ID,"view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    unsigned int projectionLoc = glGetUniformLocation(program.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //BIND BIND BIND TO THE TEXTURE
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // GL DRAW CALL
    // ------------------
    GL(glBindVertexArray(vertexArrayObject));
    GL(glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, NULL, NUM_INSTANCES));

    //OVR LOGIC
    GL(glActiveTexture(GL_TEXTURE0));
    GL(glBindTexture(GL_TEXTURE_2D, 0));

    GL(glBindVertexArray(0));
    GL(glUseProgram(0));

    // FRAMEBUFFER LOGIC
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    renderBufferShader.use();
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer); 
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //****************************************************************************
    //Create the image to send before we swap the buffers
    //This improves performance in reading the pixels with the off-screen buffer
    //****************************************************************************

    //TODO: This should be spun off as its own thread... 
    createImage();

    //TODO: CREATE HEADLESS OPENGL MODE
    //Commenting out the below improves the performance.
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
}



int svrAppl::createWorld() {

    //Reset back to default, regular shader 
    program.use();

    //CubeWorld Setup
    //Convert Vertex Struct to Vector of Vertexes
    int VERTICES_PER_OBJECT = 8; //8
    
    attribs.position.resize(VERTICES_PER_OBJECT);
    attribs.color.resize(VERTICES_PER_OBJECT);
    for (int i = 0; i < VERTICES_PER_OBJECT; i++) {
        attribs.position[i] = cubeVertices.positions[i];
        attribs.color[i] = cubeVertices.colors[i];
    }

    //Convert Index Struct to Vector if Indices
    
    int NUMBER_OF_INDICES = 36; //36
    indices.resize(NUMBER_OF_INDICES);
    for (int i = 0; i < NUMBER_OF_INDICES; i++) {
        indices[i] = cubeIndices[i];
    }

    //Get size of new data structure
    int vertexCount = attribs.position.size();
    indices.size();

    std::vector<OVR::Vector3f> position;
    std::vector<OVR::Vector3f> normal;
    std::vector<OVR::Vector3f> tangent;
    std::vector<OVR::Vector3f> binormal;

    //GL Create the Vertex Buffer, IndexBuffer and VertexArrayObject
    

    //GL Generate Buffer Pointers
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &indexBuffer);
    glGenVertexArrays(1, &vertexArrayObject);
    
    //GL Bind to Buffers and Attribute Location
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindVertexArray(vertexArrayObject);
    glBindAttribLocation(program.ID, VERTEX_ATTRIBUTE_LOCATION_COLOR, "VertexColor");    
    
    //OVR Use the PackVertexAttribute function
    std::vector<uint8_t> packed;
    PackVertexAttribute(
        packed, attribs.position, VERTEX_ATTRIBUTE_LOCATION_POSITION, GL_FLOAT, 3);

    PackVertexAttribute(packed, attribs.color, VERTEX_ATTRIBUTE_LOCATION_COLOR, GL_FLOAT, 4);

    //GL Add Data to the Buffers
    glBufferData(GL_ARRAY_BUFFER, packed.size() * sizeof(packed[0]), packed.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(indices[0]),
        indices.data(),
        GL_STATIC_DRAW);

    //Unbind the Vertex Array
    glBindVertexArray(0);

      // OVR Game Logic Setup
    // ------------------------
    
    generate_random_rotations(Rotations, NUM_ROTATIONS);
    generate_random_locations(CubePositions, CubeRotations, NUM_INSTANCES, Rotations, NUM_ROTATIONS);
    
    // Get the attribute for the instance transforms
    
    VertexTransformAttribute = glGetAttribLocation(program.ID, "VertexTransform");

    startTime = glfwGetTime();

    return 1;
}

void svrAppl::createCamera() {
    projection = glm::perspective(glm::radians(90.0f), (float) m_width / (float) m_height, 0.1f, 10000.0f);

    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


    deltaTime = 0.0f;
    lastFrame = 0.0f;

    firstMouse = true;
    yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    pitch =  0.0f;
    lastX =  1024.0f / 2.0;
    lastY =  1024.0 / 2.0;
    fov   =  45.0f;


}

void svrAppl::updateView() {

    cameraPos = glm::vec3(headpose_data[0], headpose_data[1], headpose_data[3]);
    //std::cout << headpose_data[0] << std::endl;

    // Position.x
    // Postion.y
    // Position.z
    //Orientation.x
    //Orientation.y
    //Orientation.z
    //Orientation.w

    /*
    ovrPosef *headpose = (ovrPosef *) malloc (sizeof(ovrPosef));
    headpose->Position.x = headpose_data[0];
    headpose->Position.y = headpose_data[1];
    headpose->Position.z = headpose_data[2];
    headpose->Orientation.x = headpose_data[3];
    headpose->Orientation.y = headpose_data[4];
    headpose->Orientation.z = headpose_data[5];
    headpose->Orientation.w = 1.0f;

    ovrMatrix4f rotation = ovrMatrix4f_CreateFromQuaternion(&headpose->Orientation);
    ovrMatrix4f translation = ovrMatrix4f_CreateTranslation(headpose_data[0], headpose_data[1], headpose_data[2]);
    ovrMatrix4f transform = ovrMatrix4f_Multiply(&translation, &rotation);
    ovrMatrix4f result = ovrMatrix4f_Inverse(&transform);

    glm::mat4 glm_result = glm::mat4(1.0f);
    glm_result[0] = glm::vec4(result.M[0][0], result.M[0][1], result.M[0][2], result.M[0][3]);
    glm_result[1] = glm::vec4(result.M[1][0], result.M[1][1], result.M[1][2], result.M[1][3]);
    glm_result[2] = glm::vec4(result.M[2][0], result.M[2][1], result.M[2][2], result.M[2][3]);
    glm_result[3] = glm::vec4(result.M[3][0], result.M[3][1], result.M[3][2], result.M[3][3]);

    delete(headpose);
    
    return glm_result;
    */
    
    float xpos = headpose_data[3];
    float ypos = headpose_data[4] * -1.0f;

    
    //std::cout << "Moving mouse" << std::endl;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    //std::cout << "XPOS:/t" << xpos << "\tYPOS:\t" << ypos << std::endl;
        
    float xoffset = xoffset = lastX-xpos;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 500.0f; //0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if(pitch > 89.0f)
    pitch = 89.0f;
    if(pitch < -89.0f)
    pitch = -89.0f;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);

    float value = headpose_data[3] < 0 ? -1.0f : 1.0f;
    cameraPos -= value * glm::normalize(glm::cross(cameraFront, cameraUp));  


}
