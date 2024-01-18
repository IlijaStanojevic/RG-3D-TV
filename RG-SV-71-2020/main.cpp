// Autor: Ilija Stanojevic, SV71/2020
// Opis: 2D projekat - 5. TV

#define _CRT_SECURE_NO_WARNINGS

#define CRES 50
#define HEIGHT 1280.0f
#define WIDTH 720.0f
#define FULLSCREEN false

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "shader.hpp"
#include "model.hpp"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);
bool isAngleLessThan60Degrees(float remoteX, float remoteY, float remoteZ,
    float remoteRotateX, float remoteRotateY,
    float signalX, float signalY, float signalZ);
float channelLight(int channel, float p1x, float p2x);

int main(void)
{


    if (!glfwInit())
    {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = HEIGHT;
    unsigned int wHeight = WIDTH;
    const char wTitle[] = "TV";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, FULLSCREEN ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }
    unsigned int basicShader = createShader("basic.vert", "basic.frag");
    unsigned int buttonShader = createShader("button.vert", "button.frag");
    unsigned int textureShader = createShader("texture.vert", "texture.frag");
    unsigned int channel2Shader = createShader("channel2.vert", "channel2.frag");
    unsigned int playerShader = createShader("player.vert", "player.frag");
    unsigned int unifiedShader = createShader("unified.vert", "unified.frag");
    unsigned int screenShader = createShader("screen.vert", "screen.frag");
    unsigned int clockShader = createShader("clock.vert", "unified.frag");
    unsigned int lightShader = createShader("unified.vert", "light.frag");






    float indexVertices[] =
    {   //X      Y      S    T 
        0.5,    1.0, 0.0,   0.0, 1.0, // Top-left
        0.5,    0.9,0.0,   0.0, 0.0, // Bottom-left
        1,    0.9,  0.0, 1.0, 0.0, // Bottom-right

        0.5,    1.0,0.0,   0.0, 1.0, // Top-left
        1,    0.9,0.0,   1.0, 0.0, // Bottom-right
        1,    1.0,0.0,   1.0, 1.0  // Top-right
    };





    float markaVertices[] =
    {   //X      Y      S    T 
        -0.5,    -1.0,0.0,   0.0, -1.0, // Top-left
        -0.5,    -0.7,0.0,   0.0, 0.0, // Bottom-left
        0.5,    -0.7, 0.0,  1.0, 0.0, // Bottom-right

        -0.5,    -1.0,0.0,   0.0, -1.0, // Top-left
        0.5,    -0.7,0.0,   1.0, 0.0, // Bottom-right
        0.5,    -1.0,0.0,   1.0, -1.0  // Top-right
    };
    unsigned int indexStride = (3 + 2) * sizeof(float);


    //okvir TV-a
    float stripVertices[] = {
        // Front face
        1.0, 1.0, 0.5,  0.5, 0.5, 0.5, 0.0,
        -1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.0,
        1.0, -1.0, 0.5, 0.5, 0.5, 0.5, 0.0,
        -1.0, -1.0, 0.5, 0.5, 0.5, 0.5, 0.0,

        // Left side
        -1.0, 1.0, 0.0, 0.5, 0.5, 0.5, 0.0,
        -1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.0,
        -1.0, -1.0, 0.0, 0.5, 0.5, 0.5, 0.0,
        -1.0, -1.0, 0.5, 0.5, 0.5, 0.5, 0.0,

        // Right side
        1.0, 1.0, 0.0,  0.5, 0.5, 0.5, 0.0,
        1.0, 1.0, 0.5,  0.5, 0.5, 0.5, 0.0,
        1.0, -1.0, 0.0, 0.5, 0.5, 0.5, 0.0,
        1.0, -1.0, 0.5, 0.5, 0.5, 0.5, 0.0,

        // Back face
        -1.0, 1.0, 0.0,  0.5, 0.5, 0.5, 0.0,
        1.0, 1.0, 0.0,   0.5, 0.5, 0.5, 0.0,
        -1.0, -1.0, 0.0, 0.5, 0.5, 0.5, 0.0,
        1.0, -1.0, 0.0,  0.5, 0.5, 0.5, 0.0,

        // Bottom face
        -1.0, -1.0, 0.0, 0.5, 0.5, 0.5, 0.0,
        1.0, -1.0, 0.0,  0.5, 0.5, 0.5, 0.0,
        -1.0, -1.0, 0.5, 0.5, 0.5, 0.5, 0.0,
        1.0, -1.0, 0.5,  0.5, 0.5, 0.5, 0.0,

        // Top face
        1.0, 1.0, 0.0,  0.5, 0.5, 0.5, 0.0,
        -1.0, 1.0, 0.0, 0.5, 0.5, 0.5, 0.0,
        1.0, 1.0, 0.5,  0.5, 0.5, 0.5, 0.0,
        -1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.0,
    };


    float screenVertices[] = {
        0.9, 0.9, 0.5,  1.0, 1.0, 1.0, 0.0,
        -0.9, 0.9, 0.5,  1.0, 1.0, 1.0, 0.0,
        -0.9, -0.7, 0.5,  1.0, 1.0, 1.0, 0.0,

        -0.9, -0.7, 0.5,  1.0, 1.0, 1.0, 0.0,
        0.9, -0.7, 0.5,  1.0, 1.0, 1.0, 0.0,
        0.9, 0.9, 0.5,  1.0, 1.0, 1.0, 0.0,
    };


    unsigned int stripStride = 7 * sizeof(float);


    float button[CRES * 2 + 4]; // +4 je za x i y koordinate centra kruga, i za x i y od nultog ugla
    const float buttonCenterX = 0.75; // Centar X0
    const float buttonCenterY = -0.85; // Centar Y0
    const float buttonRadius = 0.1;    // poluprecnik
    button[0] = buttonCenterX; // Centar X0 in normalized coordinates
    button[1] = buttonCenterY; // Centar Y0 in normalized coordinates
    float aspectRatio = HEIGHT / WIDTH;
    for (int i = 0; i <= CRES; i++) {
        button[2 + 2 * i] = buttonCenterX + (buttonRadius * cos((3.141592 / 180) * (i * 360 / CRES))) / (aspectRatio); //Xi
        button[2 + 2 * i + 1] = buttonCenterY + buttonRadius * sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
    }


    float dotCircle[CRES * 3 + 6]; // +6 for x, y, and z coordinates of the center, and for x, y of the starting point
    const float dotCenterX = 0; // Center X
    const float dotCenterY = 0; // Center Y
    const float dotCenterZ = 0.5; // Center Z
    const float dotCircleRadius = 0.3; // Radius
    const int startingIndex = 3; 

    // Set center coordinates
    //dotCircle[0] = dotCenterX;
    //dotCircle[1] = dotCenterY;
    //dotCircle[2] = dotCenterZ;

    // Set starting point coordinates
    dotCircle[startingIndex] = dotCircleRadius; // X coordinate
    dotCircle[startingIndex + 1] = 0; // Y coordinate
    dotCircle[startingIndex + 2] = dotCenterZ; // Z coordinate

    for (int i = 0; i <= CRES; i++) {
        // Calculate coordinates in 3D
        dotCircle[startingIndex + 3 * i] = dotCenterX + dotCircleRadius * cos((3.141592 / 180) * (i * 360 / CRES)); // Xi
        dotCircle[startingIndex + 3 * i + 1] = dotCenterY + dotCircleRadius * sin((3.141592 / 180) * (i * 360 / CRES)); // Yi
        dotCircle[startingIndex + 3 * i + 2] = dotCenterZ; 
    }


    float clockCircle[CRES * 3 + 6]; // +4 je za x i y koordinate centra kruga, i za x i y od nultog ugla
    const float clockCenterX = 0; // Centar X0
    const float clockCenterY = 0; // Centar Y0
    const float clockCenterZ = 0.5; // Centar Y0
    const float clockRadius = 0.5;    // poluprecnik
    clockCircle[0] = clockCenterX; // Centar X0
    clockCircle[1] = clockCenterY; // Centar Y0
    clockCircle[2] = clockCenterZ; // Centar Y0

    for (int i = 0; i <= CRES; i++) {
        clockCircle[startingIndex + 3 * i] = clockCenterX + clockRadius * cos((3.141592 / 180) * (i * 360 / CRES)); // Xi
        clockCircle[startingIndex + 3 * i + 1] = clockCenterY + clockRadius * sin((3.141592 / 180) * (i * 360 / CRES)); // Yi
        clockCircle[startingIndex + 3 * i + 2] = clockCenterZ;
    }
    float clockLine[] = {
        clockCenterX, clockCenterY, clockCenterZ, 1.0, 0.0, 0.0, 0.0,
        0.32, 0.32, clockCenterZ, 1.0, 0.0, 0.0, 0.0,
    };
    float signalCircle[CRES * 3 + 6]; // +4 je za x i y koordinate centra kruga, i za x i y od nultog ugla
    const float signalCenterX = 0.2; // Centar X0
    const float signalCenterY = -0.8; // Centar Y0
    const float signalCenterZ = 0.5; // Centar Y0
    const float signalRadius = 0.05;    // poluprecnik
    signalCircle[0] = signalCenterX; // Centar X0
    signalCircle[1] = signalCenterY; // Centar Y0
    signalCircle[2] = signalCenterZ; // Centar Y0

    for (int i = 0; i <= CRES; i++) {
        signalCircle[startingIndex + 3 * i] = signalCenterX + signalRadius * cos((3.141592 / 180) * (i * 360 / CRES)); // Xi
        signalCircle[startingIndex + 3 * i + 1] = signalCenterY + signalRadius * sin((3.141592 / 180) * (i * 360 / CRES)); // Yi
        signalCircle[startingIndex + 3 * i + 2] = signalCenterZ;
    }
    
    float luigiVertices[] =
    {   //X      Y      S    T 
        -0.3,    0.3,0.5,   0.0, 1.0, // Top-left
        -0.3,    -0.2,0.5,   0.0, 0.0, // Bottom-left
        -0.4,    -0.2,0.5,   1.0, 0.0, // Bottom-right

        -0.3,    0.3,0.5,   0.0, 1.0, // Top-left
        -0.4,    -0.2,0.5,   1.0, 0.0, // Bottom-right
        -0.4,    0.3,0.5,   1.0, 1.0  // Top-right
    };


    float marioVertices[] =
    {   //X      Y      S    T 
        0.3,    0.3,0.5,   0.0, 1.0, // Top-left
        0.3,    -0.2,0.5,   0.0, 0.0, // Bottom-left
        0.4,    -0.2,0.5,   1.0, 0.0, // Bottom-right

        0.3,    0.3,0.5,   0.0, 1.0, // Top-left
        0.4,    -0.2,0.5,   1.0, 0.0, // Bottom-right
        0.4,    0.3,0.5,   1.0, 1.0  // Top-right
    };







    unsigned VAO[9]; //0 = TV border, 1= button, 2= dot circle, 3= clock circle,4= clock needle, 5= marka texture, 6= luigi(p2), 7= mario(p1), 8= index gore levo
    glGenVertexArrays(9, VAO);
    unsigned VBO[9];
    glGenBuffers(9, VBO);


    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stripVertices), stripVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stripStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stripStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stripStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stripStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //glBindVertexArray(VAO[1]);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(button), button, GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);



    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dotCircle), dotCircle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(clockCircle), clockCircle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(clockLine), clockLine, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stripStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stripStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(markaVertices), markaVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, indexStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, indexStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(marioVertices), marioVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, indexStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, indexStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(luigiVertices), luigiVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, indexStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, indexStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(signalCircle), signalCircle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);




    unsigned indexTexture = loadImageToTexture("index.png"); //Ucitavamo teksturu
    unsigned int marioTexture = loadImageToTexture("mario.png");
    unsigned int luigiTexture = loadImageToTexture("luigi.png");
    glBindTexture(GL_TEXTURE_2D, indexTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    unsigned uTexLoc = glGetUniformLocation(textureShader, "uTex");
    glUniform1i(uTexLoc, 0); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)
    //Odnosi se na glActiveTexture(GL_TEXTURE0) u render petlji
    //Moguce je sabirati indekse, tj GL_TEXTURE5 se moze dobiti sa GL_TEXTURE0 + 5 , sto je korisno za iteriranje kroz petlje

    glBindTexture(GL_TEXTURE_2D, marioTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(uTexLoc, 0);
    glBindTexture(GL_TEXTURE_2D, luigiTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(uTexLoc, 0);

    unsigned p1xLoc = glGetUniformLocation(playerShader, "p1x");
    unsigned p2xLoc = glGetUniformLocation(playerShader, "p2x");

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++            UNIFORME            +++++++++++++++++++++++++++++++++++++++++++++++++

    glm::mat4 model = glm::mat4(1.0f); //Matrica transformacija - mat4(1.0f) generise jedinicnu matricu
    unsigned int modelLoc = glGetUniformLocation(unifiedShader, "uM");
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // lookAt(Gdje je kamera, u sta kamera gleda, jedinicni vektor pozitivne Y ose svijeta  - ovo rotira kameru)
    unsigned int viewLoc = glGetUniformLocation(unifiedShader, "uV");
    glm::mat4 projectionP = glm::perspective(glm::radians(90.0f), (float)wWidth / (float)wHeight, 0.0001f, 100.0f); //Matrica perspektivne projekcije
    glm::mat4 projectionO = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0001f, 100.0f); //Matrica ortogonalne projekcije
    unsigned int projectionLoc = glGetUniformLocation(unifiedShader, "uP");


    glUseProgram(unifiedShader); // Set the unifiedShader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP)); // Use perspective projection matrix
    glBindVertexArray(VAO[0]);


    glUseProgram(screenShader); 
    GLint isTvOnLoc = glGetUniformLocation(screenShader, "isTvOn");
    glUniform1i(glGetUniformLocation(screenShader, "isTvOn"), 1);
    glUniformMatrix4fv(glGetUniformLocation(screenShader, "uM"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(screenShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(screenShader, "uP"), 1, GL_FALSE, glm::value_ptr(projectionP));
    

    glUseProgram(channel2Shader); // Set the channel2Shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP)); // Use perspective projection matrix



    glUseProgram(playerShader);
    glUniformMatrix4fv(glGetUniformLocation(playerShader, "uM"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(playerShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(playerShader, "uP"), 1, GL_FALSE, glm::value_ptr(projectionP)); 


    glUseProgram(clockShader);
    glUniformMatrix4fv(glGetUniformLocation(clockShader, "uM"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(clockShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(clockShader, "uP"), 1, GL_FALSE, glm::value_ptr(projectionP));



    glUseProgram(lightShader);
    glUniform1i(glGetUniformLocation(lightShader, "state"), 2);
    glUniformMatrix4fv(glGetUniformLocation(lightShader, "uM"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(lightShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(lightShader, "uP"), 1, GL_FALSE, glm::value_ptr(projectionP));



    Model daljinski("daljinski.obj");
    Shader modelShader("model.vert", "model.frag");
    modelShader.use();
    modelShader.setVec3("uLightPos", 0, 1, 0.5);
    modelShader.setVec3("uViewPos", 0, 0, 0.5);
    modelShader.setVec3("uLightColor", 1, 1, 1);
    modelShader.setMat4("uP", projectionP);
    modelShader.setMat4("uV", view);






    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << error << std::endl;
    }
    

    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();




    glDepthFunc(GL_LEQUAL);
    float rgb = 0;
    bool isTVOn = false;
    int currentChannel = 3;
    float p1x = 0;
    float p2x = 0;
    float daljinskiX = 2;
    float daljinskiY = -9;
    float daljinskiZ = -12.0;
    float daljinskiRotateX = -93;
    float daljisnkiRotateY = 0;
    bool changeChannel = false;
    double channelSwitchTime = 0.0;
    const double delayDuration = 0.5;
    float lightValue = 0.1;

    const float maxX = 3.0f;
    const float minX = -3.0f;
    const float maxY = 2.0f;
    const float minY = -2.0f;

    while (!glfwWindowShouldClose(window))
    {
        //std::cout << "daljinskiX: " << daljinskiX << std::endl;
        //std::cout << "daljinskiY: " << daljinskiY << std::endl;
        //std::cout << "daljinskiRotateX: " << daljinskiRotateX << std::endl;
        //std::cout << "daljisnkiRotateY: " << daljisnkiRotateY << std::endl;
        //std::cout << "lightValue: " << lightValue << std::endl;
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        {
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionO));
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        {
            // Move along the up vector
            view = glm::translate(view, -0.01f * glm::vec3(1.0f, 0.0f, 0.0f));

            // Check if within bounds
            if (glm::translate(view, glm::vec3(-0.01f, 0.0f, 0.0f))[3][0] > minX)
            {
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            }
            else
            {
                // Undo the translation if out of bounds
                view = glm::translate(view, 0.01f * glm::vec3(1.0f, 0.0f, 0.0f));
            }
        }

        // Move the camera right (J key)
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            // Move along the up vector
            view = glm::translate(view, 0.01f * glm::vec3(1.0f, 0.0f, 0.0f));

            // Check if within bounds
            if (glm::translate(view, glm::vec3(0.01f, 0.0f, 0.0f))[3][0] < maxX)
            {
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            }
            else
            {
                // Undo the translation if out of bounds
                view = glm::translate(view, -0.01f * glm::vec3(1.0f, 0.0f, 0.0f));
            }
        }

        // Move the camera up (Y key)
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        {
            // Move along the up vector
            view = glm::translate(view, 0.01f * glm::vec3(0.0f, 1.0f, 0.0f));

            // Check if within bounds
            if (glm::translate(view, glm::vec3(0.0f, 0.01f, 0.0f))[3][1] < maxY)
            {
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            }
            else
            {
                // Undo the translation if out of bounds
                view = glm::translate(view, -0.01f * glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }

        // Move the camera down (H key)
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            // Move along the down vector
            view = glm::translate(view, -0.01f * glm::vec3(0.0f, 1.0f, 0.0f));

            // Check if within bounds
            if (glm::translate(view, glm::vec3(0.0f, -0.01f, 0.0f))[3][1] > minY)
            {
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            }
            else
            {
                // Undo the translation if out of bounds
                view = glm::translate(view, 0.01f * glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }

        // Zoom in (Z key)
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            view = glm::translate(view, -0.01f * glm::vec3(view[2])); // Move along the forward vector (zoom in)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        }

        // Zoom out (X key)
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            view = glm::translate(view, 0.01f * glm::vec3(view[2])); // Move along the backward vector (zoom out)
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }


        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        {
            isTVOn = true;

        }
        else
        {
            rgb = abs(sin(glfwGetTime()));
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        {
            isTVOn = false;
        }
        //isAngleLessThan60Degrees(daljinskiX, daljinskiY, daljinskiZ,
         //   daljinskiRotateX, daljisnkiRotateY, 0.0, -0.9, 0.5)
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            if (isTVOn) {
                currentChannel = 1;
                changeChannel = true;
                channelSwitchTime = glfwGetTime();
                lightValue = channelLight(1, p1x, p2x);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            if (isTVOn) {
                currentChannel = 2;
                changeChannel = true;
                channelSwitchTime = glfwGetTime();
                lightValue = channelLight(2, p1x, p2x);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            if (isTVOn) {
                currentChannel = 3;
                changeChannel = true;
                channelSwitchTime = glfwGetTime();
                lightValue = channelLight(3, p1x, p2x);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            if (isTVOn) {
                if (currentChannel == 3) {
                    p1x -= 0.001;
                    if (p1x <= -0.5) {
                        p1x = -0.5;
                    }
                    lightValue = channelLight(3, p1x, p2x);
                }

            }
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            if (isTVOn) {
                if (currentChannel == 3) {
                    p1x += 0.001;
                    if (p1x >= 0.3) {
                        p1x = 0.3;
                    }
                    lightValue = channelLight(3, p1x, p2x);
                }

            }
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {

            if (isTVOn) {
                if (currentChannel == 3) {
                    p2x += 0.001;
                    if (p2x >= 0.5) {
                        p2x = 0.5;
                    }
                    lightValue = channelLight(3, p1x, p2x);
                }

            }
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            if (isTVOn) {
                if (currentChannel == 3) {
                    p2x -= 0.001;
                    if (p2x <= -0.3) {
                        p2x = -0.3;
                    }
                    lightValue = channelLight(3, p1x, p2x);
                }

            }
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            daljinskiX += 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            daljinskiX -= 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            daljinskiY += 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            daljinskiY -= 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        {
            daljinskiRotateX += 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        {
            daljinskiRotateX -= 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        {
            daljisnkiRotateY += 0.1;
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
        {
            daljisnkiRotateY -= 0.1;
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Osvjezavamo i Z bafer i bafer boje
        glDepthMask(GL_TRUE);
        glUseProgram(unifiedShader);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glBindVertexArray(VAO[0]); // Border TV-a
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(stripVertices) / stripStride);



        if (changeChannel) {
            if (glfwGetTime() - channelSwitchTime < delayDuration) {
                glUseProgram(screenShader);
                glBindVertexArray(VAO[1]);
                glUniformMatrix4fv(glGetUniformLocation(screenShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
                glUniform1i(glGetUniformLocation(screenShader, "isTvOn"), 1);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(screenVertices) / stripStride);
                lightValue = 0.1;
                glUseProgram(lightShader);
                glUniform1i(glGetUniformLocation(lightShader, "state"), 2);
                glUniformMatrix4fv(glGetUniformLocation(lightShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
                glBindVertexArray(VAO[8]);
                glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(dotCircle) / (2 * sizeof(float)));
            }else {
                glUseProgram(screenShader);
                glBindVertexArray(VAO[1]);
                glUniformMatrix4fv(glGetUniformLocation(screenShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
                glUniform1i(glGetUniformLocation(screenShader, "isTvOn"), 0);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(screenVertices) / stripStride);
                lightValue = channelLight(currentChannel, p1x, p2x);
                changeChannel = false;
            }
        }
        else {
            if (isTVOn) {
                glUseProgram(screenShader);
                glBindVertexArray(VAO[1]);
                glUniformMatrix4fv(glGetUniformLocation(screenShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
                glUniform1i(glGetUniformLocation(screenShader, "isTvOn"), 0);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(screenVertices) / stripStride);
                if (currentChannel == 3) {
                    lightValue = channelLight(currentChannel, p1x, p2x);
                }
                glUseProgram(lightShader);
                glUniform1i(glGetUniformLocation(lightShader, "state"), 1);
                glUniformMatrix4fv(glGetUniformLocation(lightShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
                glBindVertexArray(VAO[8]);
                glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(dotCircle) / (2 * sizeof(float)));
                
            }
            else {
                glUseProgram(screenShader);
                glBindVertexArray(VAO[1]);
                glUniformMatrix4fv(glGetUniformLocation(screenShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
                glUniform1i(glGetUniformLocation(screenShader, "isTvOn"), 1);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(screenVertices) / stripStride);
                glUseProgram(lightShader);
                glUniform1i(glGetUniformLocation(lightShader, "state"), 0);
                glUniformMatrix4fv(glGetUniformLocation(lightShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
                glBindVertexArray(VAO[8]);
                glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(dotCircle) / (2 * sizeof(float)));
            }

        }


        if (isTVOn && !changeChannel)
        {

            if (currentChannel == 1) {
                glUseProgram(unifiedShader);
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glBindVertexArray(VAO[3]);
                glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(dotCircle) / (2 * sizeof(float)));


                glUseProgram(clockShader);
                glUniform2f(glGetUniformLocation(clockShader, "rotationCenter"), clockCenterX, clockCenterY);
                glUniform1f(glGetUniformLocation(clockShader, "rotationAngle"), glfwGetTime());
                glUniformMatrix4fv(glGetUniformLocation(clockShader, "uV"), 1, GL_FALSE, glm::value_ptr(view));
                glBindVertexArray(VAO[4]);
                glEnable(GL_LINE_WIDTH);
                glLineWidth(5.0f);
                glDrawArrays(GL_LINES, 0, 2);
                glDisable(GL_LINE_WIDTH);

            }
            if (currentChannel == 2) {
                glUseProgram(channel2Shader);
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                glEnable(GL_PROGRAM_POINT_SIZE);
                glBindVertexArray(VAO[2]);
                glPointSize(2.0f); // triangle point size forming a circle
                glDrawArrays(GL_TRIANGLES, 0, sizeof(dotCircle) / (2 * sizeof(float)));
                glDisable(GL_PROGRAM_POINT_SIZE);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            }
            if (currentChannel == 3) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glUseProgram(playerShader);
                glBindVertexArray(VAO[7]);
                glActiveTexture(GL_TEXTURE0); // mario textura
                glBindTexture(GL_TEXTURE_2D, marioTexture);
                glUniform1i(uTexLoc, 0);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_BLEND);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glUseProgram(playerShader);
                glBindVertexArray(VAO[6]);
                glActiveTexture(GL_TEXTURE0); // luigi textura
                glBindTexture(GL_TEXTURE_2D, luigiTexture);
                glUniform1i(uTexLoc, 0);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_BLEND);
            }
        }
        else
        {
            glClearColor(0.1f, 0.0f, 0.8f, 1.0f);
        }
        



        
        modelShader.use();
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::vec3 translationFactors(daljinskiX, daljinskiZ, daljinskiY);
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translationFactors);
        modelMatrix = translationMatrix * modelMatrix;

        float angleInRadiansX = glm::radians(daljinskiRotateX);
        float angleInRadiansY = glm::radians(daljisnkiRotateY);
        glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), angleInRadiansX, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), angleInRadiansY, glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = rotationMatrixX * rotationMatrixY * modelMatrix;

        glm::vec3 scaleFactors(0.1f, 0.1f, 0.1f);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scaleFactors);
        modelMatrix = scaleMatrix * modelMatrix;

        modelShader.setMat4("uM", modelMatrix);
        modelShader.setMat4("uV", view);
        modelShader.setMat4("uP", projectionP);
        if (isTVOn) {
            modelShader.setVec3("uLightPos", 0, 1, 0.5);
            modelShader.setVec3("uViewPos", 0, 0, 5);
            modelShader.setVec3("uLightColor", lightValue,lightValue, lightValue);
        }
        else {
            modelShader.setVec3("uLightPos", 0, 1, 0.5);
            modelShader.setVec3("uViewPos", 0, 0, 5);
            modelShader.setVec3("uLightColor", 0.1, 0.1, 0.1);
        }
        daljinski.Draw(modelShader);
        

       


        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    glDeleteBuffers(2, VBO);
    glDeleteVertexArrays(2, VAO);
    glDeleteProgram(basicShader);
    glDeleteProgram(channel2Shader);
    glDeleteProgram(buttonShader);
    glDeleteProgram(textureShader);
    glDeleteProgram(playerShader);

    glfwTerminate();
    return 0;
}
unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
    std::string temp = ss.str();
    const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);

    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
float channelLight(int channel, float p1x, float p2x) {
    if (channel == 1) {
        return 0.4;
    }
    else if (channel == 2) {
        return 1.0;
    }
    else {
        p1x = -0.4 + p1x;
        p2x = 0.4 + p2x;
        float distance = std::abs(p2x) + std::abs(p1x);
        float minDistance = 0.2;
        float maxDistance = 1.8;
        float maxScale = 1.0;
        float minScale = 0.1;
        // Scale the distance to the range [minScale, maxScale]
        float scaledDistance = minScale + (maxScale - minScale) * (1.0 - (distance - minDistance) / (maxDistance - minDistance));

        return scaledDistance;
    }
}
bool isAngleLessThan60Degrees(float remoteX, float remoteY, float remoteZ,
    float remoteRotateX, float remoteRotateY,
    float signalX, float signalY, float signalZ) {
    // Remote position
    glm::vec3 remotePosition(remoteX, remoteY, remoteZ);

    float angleInRadiansX = glm::radians(remoteRotateX);
    float angleInRadiansY = glm::radians(remoteRotateY);

    glm::vec3 remoteDirection(
        glm::cos(angleInRadiansY) * glm::cos(angleInRadiansX),
        glm::sin(angleInRadiansY) * glm::cos(angleInRadiansX),
        glm::sin(angleInRadiansX)
    );

    glm::vec3 signalDirection = glm::normalize(glm::vec3(signalX, signalY, signalZ) - remotePosition);

    float dotProduct = glm::dot(remoteDirection, signalDirection);

    float angleInDegrees = glm::degrees(glm::acos(dotProduct));

    return angleInDegrees < 60.0f;
}

unsigned int createShader(const char* vsSource, const char* fsSource)
{

    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);


    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}
