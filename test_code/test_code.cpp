#include <iostream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include <cmath>

int count = 0;

inline float grad2rad(int &g)
{
    g = g % 360;
    return (3.14 * g) / 180;
}

void RenderSceneCB()
{
    float red_value = std::abs(std::sin(grad2rad(++count)));

    glClearColor(red_value, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();

    glutPostRedisplay();
}

void init(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Test Code");

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        std::cerr << "Error : " << glewGetErrorString(res) << std::endl;
        exit(1);
    }

    glutDisplayFunc(RenderSceneCB);

    // Dummy code to test the glm library
    glm::vec3 Vertices[3] = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)};
}

int main(int argc, char *argv[])
{
    init(argc, argv);

    glutMainLoop();

    return 0;
}