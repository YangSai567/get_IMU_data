#ifndef MYOPENGL_H
#define MYOPENGL_H

#include <QWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include<QOpenGLWidget>
#include"QOpenGLFunctions"
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QGLWidget>
#include <QVector2D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <ft2build.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class myopenGL : public QGLWidget
{
    Q_OBJECT
public:
    explicit myopenGL(QWidget *parent = nullptr);

    struct Character {
        GLuint     TextureID;  // 字形纹理的ID
        glm::ivec2 Size;       // 字形大小
        glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
        GLuint     Advance;    // 原点距下一个字形原点的距离
    };

    std::map<GLchar, Character> Characters;

protected:

    void paintGL();
    void initializeGL();
    void resizeGL(int width, int height);
    void timerEvent(QTimerEvent *event);
    void print_bitmap_string(void* ,const char* );
//    void wheelEvent(QWheelEvent *event);
private:
    GLfloat mScale;
    QMatrix4x4 up4ps, down4ps;
//    QMatrix4x4 projection, up4ps, down4ps;
    QVector2D mousePos;
    QQuaternion rotation;
    QQuaternion rotationFromIMU;

signals:

private slots:
    void IMUDatRecv(float,float,float); //接收传感器数据
};

#endif // MYOPENGL_H
