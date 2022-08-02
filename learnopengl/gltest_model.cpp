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
    // Commented out because escape key seems to be permanently
    // pressed between instances after pressing once
    // Only restarting has reset GLFW_KEY_ESCAPE to GLFW_RELEASE
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
    
    ShaderProgram shaderProgram("shaders/vertex.txt", "shaders/fragment.txt");

    Model backpack("models/backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);
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
        
        // Update projection matrix based on window size
        GLint m_viewport[4];
        glGetIntegerv(GL_VIEWPORT, m_viewport);
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)m_viewport[2] / (float)m_viewport[3], 0.1f, 100.0f);
        
        glm::mat4 view = glm::lookAt(cameraPos_g, cameraPos_g + cameraFront_g, up_g);

        shaderProgram.setMat4("projection", proj);
        shaderProgram.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        double modelPosX = sin(glfwGetTime()*M_PI) * 5;
        model = glm::translate(model, glm::vec3(modelPosX, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        shaderProgram.setMat4("model", model);
        backpack.Draw(shaderProgram);

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