#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
float sizeFactor(float centerX, float centerY, float cursorX, float cursorY);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("shader.vs", "shader.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // float vertices[] = {
    //     // positions         // colors
    //      0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
    //      0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    // };

    double mouseX;
    double mouseY;
    double center[70] = {0};
    double size[35] = {1};
    const double SQUARE_SIZE = 100;
    double vertices[420];

    glfwGetCursorPos(window, &mouseX, &mouseY);

    //plot all center of squares
    int counter = 0;
    for(int i = -(SCR_WIDTH/2) + 50; i < SCR_WIDTH/2; i += 100) {
        for(int j = -(SCR_HEIGHT/2) + 50; j < SCR_HEIGHT/2; j += 100) {
            center[counter] = double(i);
            center[counter + 1] = double(j);
            counter += 2;
        }
    }

    for(int i = 0; i < 70; i += 2) {
        double cen_x = center[i]; 
        double cen_y = center[i+1]; 
        double factor = sizeFactor(cen_x, cen_y, mouseX, mouseY);
        double vertices[] = {
            cen_x - SQUARE_SIZE*factor/2, cen_y - SQUARE_SIZE*factor/2, 1.0f, 1.0f, 0.0f,
            cen_x - SQUARE_SIZE*factor/2, cen_y - SQUARE_SIZE*factor/2, 1.0f, 1.0f, 0.0f,
            cen_x - SQUARE_SIZE*factor/2, cen_y - SQUARE_SIZE*factor/2, 1.0f, 1.0f, 0.0f,
            cen_x - SQUARE_SIZE*factor/2, cen_y - SQUARE_SIZE*factor/2, 1.0f, 1.0f, 0.0f,
        };
        glDrawArrays(GL_TRIANGLES, 0, 3);
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render square of each center
        ourShader.use();
        glBindVertexArray(VAO);
        

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

double sizeFactor(float centerX, float centerY, float cursorX, float cursorY) {
    float maxDist = 150;
    float minDist = 25;
    float dist = sqrt(pow((cursorX - centerX), 2) + pow((cursorY - centerY), 2));
    if(dist > maxDist) return 0;
    else if(dist < minDist) return 1;
    else return (dist/(maxDist-minDist));
}