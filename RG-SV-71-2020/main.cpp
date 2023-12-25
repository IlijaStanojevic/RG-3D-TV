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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);

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






    float indexVertices[] =
    {   //X      Y      S    T 
        0.5,    1.0,   0.0, 1.0, // Top-left
        0.5,    0.9,   0.0, 0.0, // Bottom-left
        1,    0.9,   1.0, 0.0, // Bottom-right

        0.5,    1.0,   0.0, 1.0, // Top-left
        1,    0.9,   1.0, 0.0, // Bottom-right
        1,    1.0,   1.0, 1.0  // Top-right
    };





    float markaVertices[] =
    {   //X      Y      S    T 
        -0.5,    -1.0,   0.0, -1.0, // Top-left
        -0.5,    -0.7,   0.0, 0.0, // Bottom-left
        0.5,    -0.7,   1.0, 0.0, // Bottom-right

        -0.5,    -1.0,   0.0, -1.0, // Top-left
        0.5,    -0.7,   1.0, 0.0, // Bottom-right
        0.5,    -1.0,   1.0, -1.0  // Top-right
    };
    unsigned int indexStride = (2 + 2) * sizeof(float);


    //okvir TV-a
    float stripVertices[] = {
        1.0, 1.0,     0.5, 0.5, 0.5,
        0.9, 0.9,     0.5, 0.5, 0.5,
        1.0, -1.0,    0.5, 0.5, 0.5,
        0.9, -0.7,    0.5, 0.5, 0.5,
        -1.0, -1.0,   0.5, 0.5, 0.5,
        -0.9, -0.7,   0.5, 0.5, 0.5,
        -1.0, 1.0,    0.5, 0.5, 0.5,
        -0.9, 0.9,    0.5, 0.5, 0.5,
        1.0, 1.0,     0.5, 0.5, 0.5,
        0.9, 0.9,     0.5, 0.5, 0.5,
    };
    unsigned int stripStride = 5 * sizeof(float);

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


    float dotCircle[CRES * 2 + 4]; // +4 je za x i y koordinate centra kruga, i za x i y od nultog ugla
    const float dotCenterX = 0; // Centar X0
    const float dotCenterY = 0; // Centar Y0
    const float dotCircleRadius = 0.4;    // poluprecnik
    for (int i = 0; i <= CRES; i++) {
        dotCircle[2 + 2 * i] = (dotCircleRadius * cos((3.141592 / 180) * (i * 360 / CRES))) / aspectRatio; //Xi
        dotCircle[2 + 2 * i + 1] = dotCircleRadius * sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
    }

    float clockCircle[CRES * 2 + 4]; // +4 je za x i y koordinate centra kruga, i za x i y od nultog ugla
    const float clockCenterX = 0; // Centar X0
    const float clockCenterY = 0; // Centar Y0
    const float clockRadius = 0.5;    // poluprecnik
    clockCircle[0] = clockCenterX; // Centar X0
    clockCircle[1] = clockCenterY; // Centar Y0

    for (int i = 0; i <= CRES; i++) {
        clockCircle[2 + 2 * i] = (clockRadius * cos((3.141592 / 180) * (i * 360 / CRES))) / aspectRatio; //Xi
        clockCircle[2 + 2 * i + 1] = clockRadius * sin((3.141592 / 180) * (i * 360 / CRES)); //Yi
    }

    float luigiVertices[] =
    {   //X      Y      S    T 
        -0.3,    0.3,   0.0, 1.0, // Top-left
        -0.3,    -0.2,   0.0, 0.0, // Bottom-left
        -0.4,    -0.2,   1.0, 0.0, // Bottom-right

        -0.3,    0.3,   0.0, 1.0, // Top-left
        -0.4,    -0.2,   1.0, 0.0, // Bottom-right
        -0.4,    0.3,   1.0, 1.0  // Top-right
    };


    float marioVertices[] =
    {   //X      Y      S    T 
        0.3,    0.3,   0.0, 1.0, // Top-left
        0.3,    -0.2,   0.0, 0.0, // Bottom-left
        0.4,    -0.2,   1.0, 0.0, // Bottom-right

        0.3,    0.3,   0.0, 1.0, // Top-left
        0.4,    -0.2,   1.0, 0.0, // Bottom-right
        0.4,    0.3,   1.0, 1.0  // Top-right
    };







    unsigned VAO[9]; //0 = TV border, 1= button, 2= dot circle, 3= clock circle,4= clock needle, 5= marka texture, 6= luigi(p2), 7= mario(p1), 8= index gore levo
    glGenVertexArrays(9, VAO);
    unsigned VBO[9];
    glGenBuffers(9, VBO);


    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stripVertices), stripVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stripStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stripStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(button), button, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dotCircle), dotCircle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(clockCircle), clockCircle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, indexStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, indexStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(luigiVertices), luigiVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, indexStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, indexStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indexVertices), indexVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, indexStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, indexStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




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




    float rgb = 0;
    bool isTVOn = false;
    int currentChannel = 3;
    float p1x = 0;
    float p2x = 0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

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
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            if (isTVOn) {
                currentChannel = 1;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            if (isTVOn) {
                currentChannel = 2;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            if (isTVOn) {
                currentChannel = 3;
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
                }

            }
        }
        if (isTVOn) {
            glClearColor(1.0, 1.0, 1.0, 1.0); // White background
        }
        else {
            glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
        }

        glUseProgram(playerShader);
        glUniform1f(p1xLoc, p2x);
        glUniform1f(p2xLoc, p1x);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(basicShader);
        glBindVertexArray(VAO[0]); // Border TV-a
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(stripVertices) / stripStride);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUseProgram(textureShader);
        glBindVertexArray(VAO[5]);
        glActiveTexture(GL_TEXTURE0); // Ime prezime index marka textura
        glBindTexture(GL_TEXTURE_2D, indexTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUseProgram(textureShader);
        glBindVertexArray(VAO[8]);
        glActiveTexture(GL_TEXTURE0); // Ime prezime index gore desno textura
        glBindTexture(GL_TEXTURE_2D, indexTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);

        if (isTVOn)
        {
            glUseProgram(buttonShader);
            glBindVertexArray(VAO[1]);
            glUniform3f(glGetUniformLocation(buttonShader, "staticColor"), 1.0f, 0.0f, 1.0f); // static pink dugme
            glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(button) / (2 * sizeof(float)));
            if (currentChannel == 1) {
                glUseProgram(basicShader);
                glBindVertexArray(VAO[3]);
                glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(dotCircle) / (2 * sizeof(float)));

                // Draw the clock line
                glUseProgram(basicShader);
                glBindVertexArray(VAO[4]);

                float clockLineVertices[] = {
                    clockCenterX, clockCenterY, 1.0f, 0.0f, 0.0f, // Center of the clock
                    clockCenterX + (clockRadius * cos(-glfwGetTime())) / aspectRatio, clockCenterY + clockRadius * sin(-glfwGetTime()), 1.0f, 0.0f, 0.0f // End point of the clock line
                };

                glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(clockLineVertices), clockLineVertices, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
                glEnableVertexAttribArray(1);

                glEnable(GL_LINE_WIDTH);
                glLineWidth(5.0f);
                glDrawArrays(GL_LINES, 0, 2);
                glDisable(GL_LINE_WIDTH);
            }

            else if (currentChannel == 2)
            {

                glUseProgram(channel2Shader);
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                glEnable(GL_PROGRAM_POINT_SIZE);
                glBindVertexArray(VAO[2]);
                glPointSize(7.0f); // triangle point size forming a circle
                glDrawArrays(GL_TRIANGLES, 0, sizeof(dotCircle) / (2 * sizeof(float)));
                glDisable(GL_PROGRAM_POINT_SIZE);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else if (currentChannel == 3) {

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

            glUseProgram(buttonShader);
            glUniform3f(glGetUniformLocation(buttonShader, "staticColor"), 0.0f, 0.0f, 0.0f); // reset static color
            glBindVertexArray(VAO[1]);
            glUniform1f(glGetUniformLocation(buttonShader, "time"), glfwGetTime());
            glUniform3f(glGetUniformLocation(buttonShader, "pulsatingColor"), rgb, rgb, rgb);
            glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(button) / (2 * sizeof(float))); // pulsirajuce crno belo dugme
        }






        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);


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
