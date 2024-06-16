#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <iostream>

// Класс для представления вектора
class Vector {
public:
    float x, y;

    Vector(float x = 0, float y = 0) : x(x), y(y) {}

    Vector operator+(const Vector& v) const {
        return Vector(x + v.x, y + v.y);
    }

    Vector operator-(const Vector& v) const {
        return Vector(x - v.x, y - v.y);
    }

    Vector operator*(float scalar) const {
        return Vector(x * scalar, y * scalar);
    }
};

// Класс для представления точки с следом
class Point {
private:
    void drawRect(float size, Vector point) {
        glVertex2f(point.x - size / 2, point.y - size / 2);
        glVertex2f(point.x + size / 2, point.y - size / 2);
        glVertex2f(point.x + size / 2, point.y + size / 2);
        glVertex2f(point.x - size / 2, point.y + size / 2);
        glEnd();
    }
public:
    Vector position;
    Vector velocity;
    std::vector<Vector> trail;
    int maxTrailLength;
    float size;

    Point(float x = 0, float y = 0, int maxTrailLength = 50, float size = 1)
        : position(x, y), velocity(0, 0), maxTrailLength(maxTrailLength), size(size) {}

    void update() {
        position = position + velocity;
        trail.push_back(position);

        if (trail.size() > maxTrailLength) {
            trail.erase(trail.begin());
        }
    }
    void draw() {
        // Рисуем след
        for (size_t i = 0; i < trail.size(); ++i) {
            float alpha = float(i) / trail.size();
            float newSize = size + ((trail.size() - i) * 0.01f);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glColor4f(0.0, 0.0, 1.0, alpha*alpha);
            glBegin(GL_QUADS);
            drawRect(newSize, trail[i]);
        }
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_QUADS);
        drawRect(size, position);
    }
};

Point point(0, 0);

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    point.draw();

    glutSwapBuffers();
}

void timer(int = 0) {
    point.update();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 60 FPS
}

void keyboard(int key, int, int) {
    const float speed = 0.1;
    switch (key) {
    case GLUT_KEY_LEFT:
        point.velocity = Vector(-speed, 0);
        break;
    case GLUT_KEY_RIGHT:
        point.velocity = Vector(speed, 0);
        break;
    case GLUT_KEY_UP:
        point.velocity = Vector(0, speed);
        break;
    case GLUT_KEY_DOWN:
        point.velocity = Vector(0, -speed);
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Moving Point with Trail");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10, 10, -10, 10);

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}