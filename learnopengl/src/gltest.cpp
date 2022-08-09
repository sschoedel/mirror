#include "shaderProgram.hpp"
#include "Model.hpp"

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <csignal>
#include <iostream>

glm::vec3 cameraPos_g = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront_g = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 up_g = glm::vec3(0.0f, 1.0f, 0.0f);

float pitch_g = 0.0f;
float yaw_g = -90.0f;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void signal_handler(int signum)
{
    exit(signum);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    // {
    //     std::cout << "Escape key pressed, closing window\n";
    //     glfwSetWindowShouldClose(window, true);
    // }


    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        std::cout << "Switching to wireframe mode\n";
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        std::cout << "Switching to fill mode\n";
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    const float baseCameraSpeed = 2.5f * deltaTime; // adjust accordingly
    float cameraSpeed = baseCameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraSpeed = baseCameraSpeed * 2;
    else
        cameraSpeed = baseCameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos_g += cameraSpeed * cameraFront_g;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos_g -= cameraSpeed * cameraFront_g;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos_g -= glm::normalize(glm::cross(cameraFront_g, up_g)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos_g += glm::normalize(glm::cross(cameraFront_g, up_g)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos_g += cameraSpeed * up_g;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos_g -= cameraSpeed * up_g;
}

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
//     std::cout << "key pressed: " << key << "\n";
//     // if (key == GLFW_KEY_ESCAPE)
//     // {
//     //     std::cout << "Escape key pressed, closing window\n";
//     //     glfwSetWindowShouldClose(window, true);
//     // }
    
//     if (key == GLFW_KEY_W)
//     {
//         std::cout << "Switching to wireframe mode\n";
//         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//     }

//     if (key == GLFW_KEY_F)
//     {
//         std::cout << "Switching to fill mode\n";
//         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//     }
//     const float cameraSpeed = 0.05f; // adjust accordingly
//     if (key == GLFW_KEY_W)
//         cameraPos_g += cameraSpeed * cameraFront_g;
//     if (key == GLFW_KEY_S)
//         cameraPos_g -= cameraSpeed * cameraFront_g;
//     if (key == GLFW_KEY_A)
//         cameraPos_g -= glm::normalize(glm::cross(cameraFront_g, up_g)) * cameraSpeed;
//     if (key == GLFW_KEY_D)
//         cameraPos_g += glm::normalize(glm::cross(cameraFront_g, up_g)) * cameraSpeed;
// }

bool first_mouse_callback_g = true;
float lastX, lastY;

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{   
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (first_mouse_callback_g) {
            first_mouse_callback_g = false;
            lastX = xpos;
            lastY = ypos;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        const float sensitivity = 0.3f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw_g += xoffset;
        pitch_g += yoffset;

        if (pitch_g > 89.0f)
            pitch_g = 89.0f;
        if (pitch_g < -89.0f)
            pitch_g = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw_g)) * cos(glm::radians(pitch_g));
        direction.y = sin(glm::radians(pitch_g));
        direction.z = sin(glm::radians(yaw_g)) * cos(glm::radians(pitch_g));
        cameraFront_g = glm::normalize(direction);
    }
    else { // GLFW_RELEASE
        first_mouse_callback_g = true;
    }
}

int main()
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFWmonitor* primary = glfwGetPrimaryMonitor();
    GLFWwindow* window = glfwCreateWindow(800, 600, "window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    // glfwSetKeyCallback(window, key_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // float hex_vertices[18];
    // float ang = M_PI;
    // float hex_scale = 0.5;
    // for (int i=0; i<sizeof(hex_vertices)/sizeof(hex_vertices[0])/3; i++) {
    //     hex_vertices[i*3] = cos(ang)*hex_scale;
    //     hex_vertices[i*3 + 1] = sin(ang)*hex_scale;
    //     hex_vertices[i*3 + 2] = 0;
    //     ang += M_PI/3;
    // }
    // unsigned int hex_indices[] = { // note that we start from 0!
    //     0, 1, 5,
    //     1, 4 ,5,
    //     1, 2, 4,
    //     2, 3, 4,
    // };
    // float cube_vertices[] = {
    //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    //     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    //     -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    // };

    // // Vertex buffer objects
    // unsigned int hex_VBO;
    // glGenBuffers(1, &hex_VBO);
    // unsigned int cube_VBO;
    // glGenBuffers(1, &cube_VBO);

    // // Element buffer objects
    // unsigned int hex_EBO;
    // glGenBuffers(1, &hex_EBO);

    // // Vertex array objects
    // unsigned int hex_VAO;
    // glGenVertexArrays(1, &hex_VAO);
    // unsigned int cube_VAO;
    // glGenVertexArrays(1, &cube_VAO);

    // glBindVertexArray(cube_VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    // glBindVertexArray(hex_VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, hex_VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(hex_vertices), hex_vertices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hex_EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hex_indices), hex_indices, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // // bind texture
    // unsigned int texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // // set the texture wrapping/filtering options (on currently bound texture)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load and generate the texture
    // int width, height, nrChannels;
    // unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // // Free image memory
    // stbi_image_free(data);

    ShaderProgram shaderProgram("shaders/vertex.txt", "shaders/fragment.txt");

    Model backpack("models/backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        ////////////
        // Rendering

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Do shady stuff
        shaderProgram.use();
        // view/projection transformations
        
        // Update projection matrix based on window size
        GLint m_viewport[4];
        glGetIntegerv(GL_VIEWPORT, m_viewport);
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)m_viewport[2] / (float)m_viewport[3], 0.1f, 100.0f);
        
        glm::mat4 view = glm::lookAt(cameraPos_g, cameraPos_g + cameraFront_g, up_g);

        shaderProgram.setMat4("projection", proj);
        shaderProgram.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
        // shaderProgram.setMat4("model", model);
        backpack.draw(shaderProgram);


        // // glBindVertexArray(hex_VAO);
        // // glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        // glm::mat4 view = glm::lookAt(cameraPos_g, cameraPos_g + cameraFront_g, up_g);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture);


        // // Update projection matrix based on window size
        // GLint m_viewport[4];
        // glGetIntegerv(GL_VIEWPORT, m_viewport);
        // glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)m_viewport[2] / (float)m_viewport[3], 0.1f, 100.0f);

        // glBindVertexArray(cube_VAO);
        // for(unsigned int i = 0; i < 10; i++)
        // {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePositions[i]);
        //     float angle = 20.0f * i;
        //     model = glm::rotate(model, glm::radians(angle + (float)glfwGetTime()*10), glm::vec3(1.0f, 0.3f, 0.5f));

        //     shaderProgram.setMat4("model", model);
        //     shaderProgram.setMat4("view", view);
        //     shaderProgram.setMat4("projection", proj);

        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }


        // glBindVertexArray(rect_VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // Rendering
        ////////////

        // Drawing buffer swap
        glfwSwapBuffers(window);

        // Events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}