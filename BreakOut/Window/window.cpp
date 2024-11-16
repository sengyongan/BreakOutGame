#include"window.h"
#include"../tool/debug.h"

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
glm::mat4 Window::projection;
int Width;
int Height;

int Window::Init(const GLuint width, const GLuint height, const char* windowName)
{
    //创建窗口
    glfwInit();
    if (!glfwInit())
    {
        // 初始化失败处理，比如输出错误信息并退出程序
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);

    glfwSetKeyCallback(window, key_callback);

    //init glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        // 初始化失败处理，比如关闭GLFW并退出程序
        glfwTerminate();
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    glCheckError();
    // Call it once to catch glewInit() bug, all other errors are now from our application.

    // OpenGL configuration
    glViewport(0, 0, width, height);//调整视口大小
    //glEnable(GL_CULL_FACE);//面剔除
    glEnable(GL_BLEND);//混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCheckError();
}

void Window::Clear()
{
    // clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//渲染前清理颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Update()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    glCheckError();
}

void Window::Terminate()
{
    glfwTerminate();
}

void Window::SetWindowSize()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)//按下空格退出
        glfwSetWindowShouldClose(window, GL_TRUE);

    Game& Breakout = GetGameRef();
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)//按下按键有效
            Breakout.Keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)//控制释放按键无效
            Breakout.Keys[key] = GL_FALSE;
            Breakout.KeysProcessed[key] = GL_FALSE;
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //
    Game& Breakout = GetGameRef();
    Breakout.GetPostProcessor().CreateFBs(width, height );
    Breakout.GetPostProcessor().Width = width;//blit位块传输部分
    Breakout.GetPostProcessor().Height = height;
    //Breakout.Width = width;
    //Breakout.Height = height;

    glViewport(0, 0, width, height);
    Width = width;
    Height = height;
}