#define _USE_MATH_DEFINES
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("shader.vs", "shader.fs");

    // size constant
    const float PENTAGON_SIZE = 0.15;

    const float PETAL_SOURCE = 0.1;
    const float PETAL_WIDTH = 0.3;
    const float PETAL_BODY = 0.6;
    const float PETAL_TIP = 0.9;

    const float STAMEN_SOURCE = 0.1;
    const float STAMEN_WIDTH = 0.1;
    const float STAMEN_BODY = 0.35;
    const float STAMEN_TIP = 0.5;


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // colors
         0.0f, 0.0f, 0.0f,  0.9f, 0.7f, 0.22f, 1.0f, // center
         0.0f, PENTAGON_SIZE, 0.0f,  0.9f, 0.77f, 0.3f, 1.0f, // top
         PENTAGON_SIZE*sin((0.4)*M_PI), PENTAGON_SIZE*cos((0.4)*M_PI), 0.0f,  0.9f, 0.77f, 0.3f, 1.0f,  // mid-right
         PENTAGON_SIZE*sin((0.8)*M_PI), PENTAGON_SIZE*cos((0.8)*M_PI), 0.0f,  0.9f, 0.77f, 0.3f, 1.0f,  // down-right
         -(PENTAGON_SIZE)*sin((0.8)*M_PI), PENTAGON_SIZE*cos((0.8)*M_PI), 0.0f,  0.9f, 0.77f, 0.3f, 1.0f,  // down-left
         -(PENTAGON_SIZE)*sin((0.4)*M_PI), PENTAGON_SIZE*cos((0.4)*M_PI), 0.0f,  0.9f, 0.77f, 0.3f, 1.0f,  // mid-left
         //main petal
         0.0f, PETAL_SOURCE, 0.0f,  1.0f, 0.9f, 0.95f, 1.0f, //petal source
         0.0f, PETAL_TIP, 0.0f,  0.9f, 0.65f, 0.75f, 1.0f, //petal tip
         PETAL_WIDTH, PETAL_BODY, 0.0f,  1.0f, 0.8f, 0.9f, 1.0f, //petal right
         -(PETAL_WIDTH), PETAL_BODY, 0.0f,  1.0f, 0.8f, 0.9f, 1.0f, //petal left
         //small petal
         0.0f, STAMEN_SOURCE, 0.0f,  0.8f, 0.35f, 0.42f, 1.0f, //petal source
         0.0f, STAMEN_TIP, 0.0f,  0.8f, 0.45f, 0.55f, 1.0f, //petal tip
         STAMEN_WIDTH, STAMEN_BODY, 0.0f,  0.85f, 0.5f, 0.5f, 1.0f, //petal right
         -(STAMEN_WIDTH), STAMEN_BODY, 0.0f,  0.85f, 0.5f, 0.5f, 1.0f //petal left
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 1,
        6, 7, 8,
        6, 9, 7,
        10, 11, 12,
        10, 13, 11
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);
    
    ourShader.use();
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        glm::mat4 transform = glm::mat4(1.0f);
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        // render
        glClearColor(0.3f, 0.65f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        glBindVertexArray(VAO);
        
        // ------ flower petals
        
        // transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        double sec = glfwGetTime();
        
        for(int i = 0; i < 5; i++) {
            transform = glm::rotate(transform, (float)(sec + (0.4*M_PI*i)), glm::vec3(0.0f, 0.0f, 1.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(15 * sizeof(float)));
            transform = glm::mat4(1.0f);
        }
        
        for(int i = 0; i < 10; i++) {
            transform = glm::rotate(transform, (float)(-sec + (0.2*M_PI)*i), glm::vec3(0.0f, 0.0f, 1.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(21 * sizeof(float)));
            transform = glm::mat4(1.0f);
        }
        
        // ------ flower core
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}