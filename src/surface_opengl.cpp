#include "window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad.h>
#include <stdexcept>

using namespace BBUI;

static bool glad_inited = false;

Surface_OpenGL::Surface_OpenGL(Window window) : Surface(window)
{
    glfwMakeContextCurrent(window->getWindow());
    if (!glad_inited)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw std::runtime_error("failed to initialize GLAD");
        glad_inited = true;
    }
}

Surface_OpenGL::~Surface_OpenGL() {}

void Surface_OpenGL::clear()
{
    auto w = owner.lock();
    glfwMakeContextCurrent(w->getWindow());
    glViewport(0, 0, w->getSize().x, w->getSize().y);
    glClearColor(w->clear_colour.x, w->clear_colour.y, w->clear_colour.z, 1.0f);
    glClearDepth(-1000.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Surface_OpenGL::present() { glfwSwapBuffers(owner.lock()->getWindow()); }
