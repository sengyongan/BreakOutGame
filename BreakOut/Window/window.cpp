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
    //��������
    glfwInit();
    if (!glfwInit())
    {
        // ��ʼ��ʧ�ܴ����������������Ϣ���˳�����
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
        // ��ʼ��ʧ�ܴ�������ر�GLFW���˳�����
        glfwTerminate();
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    glCheckError();
    // Call it once to catch glewInit() bug, all other errors are now from our application.

    // OpenGL configuration
    glViewport(0, 0, width, height);//�����ӿڴ�С
    //glEnable(GL_CULL_FACE);//���޳�
    glEnable(GL_BLEND);//���
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCheckError();
}

void Window::Clear()
{
    // clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//��Ⱦǰ������ɫ������
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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)//���¿ո��˳�
        glfwSetWindowShouldClose(window, GL_TRUE);

    Game& Breakout = GetGameRef();
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)//���°�����Ч
            Breakout.Keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)//�����ͷŰ�����Ч
            Breakout.Keys[key] = GL_FALSE;
            Breakout.KeysProcessed[key] = GL_FALSE;
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //
    Game& Breakout = GetGameRef();
    Breakout.GetPostProcessor().CreateFBs(width, height );
    Breakout.GetPostProcessor().Width = width;//blitλ�鴫�䲿��
    Breakout.GetPostProcessor().Height = height;
    //Breakout.Width = width;
    //Breakout.Height = height;

    glViewport(0, 0, width, height);
    Width = width;
    Height = height;
}