#include <gl/glut.h>
#include <opencv.hpp>
using namespace cv;

//OpenCV读取图像
Mat I = imread("32.bmp");
//设置长宽
int width = I.cols;
int height = I.rows;
//设置图像指针
GLubyte* pixels;
GLfloat angle = 0.0;
static GLint spinx = 0;
static GLint spiny = 0;

GLuint load_texture()
{
    //OpenGL纹理用整型数表示
    GLuint texture_ID;
    //获取图像指针
    int pixellength = width * height * 3;
    pixels = new GLubyte[pixellength];
    memcpy(pixels, I.data, pixellength * sizeof(char));
    imshow("OpenCV", I);
    //将texture_ID设置为2D纹理信息
    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    //纹理放大缩小使用线性插值
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //纹理水平竖直方向外扩使用重复贴图
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //纹理水平竖直方向外扩使用边缘像素贴图(与重复贴图二选一)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    //将图像内存用作纹理信息
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    free(pixels);
    return texture_ID;
}

void display()
{
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //获取纹理对象
    GLuint image = load_texture();
    //重新设置OpenGL窗口：原点位置为左上角，x轴从左到右，y轴从上到下，坐标值与像素坐标值相同
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 0.01, 1000.0);   //投影矩阵
    //显示纹理
    glMatrixMode(GL_MODELVIEW);  //模型视图
    glLoadIdentity();   //重置当前指定的矩阵为单位矩阵
    gluLookAt (0.0,0.0,4.0,  0.0,0.0,0.0,  0.0,1.0,0.0);

    glPushMatrix();
    glRotatef ((GLfloat) spinx, 1.0, 0.0, 0.0);//左右旋转
    glRotatef ((GLfloat) spiny, 0.0, 1.0, 0.0);//上下旋转

    glEnable(GL_TEXTURE_2D);    //允许使用纹理
    glBindTexture(GL_TEXTURE_2D, image);    //选择纹理对象
    glBegin(GL_POLYGON);    //设置为多边形纹理贴图方式并开始贴图
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 2.0f*height/width, 0.0f);    //纹理左上角对应窗口左上角
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, -2.0f*height/width, 0.0f);    //纹理左下角对应窗口左下角
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, -2.0f*height/width, 0.0f);    //纹理右下角对应窗口右下角
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, 2.0f*height/width, 0.0f);    //纹理右上角对应窗口右上角
    glEnd();    //结束贴图*/
    glDisable(GL_TEXTURE_2D);    //禁止使用纹理
    glutSwapBuffers();   //双缓存交换缓存以显示图像
    angle = angle + 0.01;
    glPopMatrix();
    glFlush();
}

void reshape (int w, int h)     //窗口改变的时候调用的函数
{    
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);  //视口变换函数
  glMatrixMode (GL_PROJECTION);  //投影
  glLoadIdentity ();   //重置当前指定的矩阵为单位矩阵
  gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 4.0, 10.0);   //投影矩阵
  glMatrixMode(GL_MODELVIEW);  //模型视图
  glLoadIdentity();   //重置当前指定的矩阵为单位矩阵
  gluLookAt (0.0,0.0,5.0,  0.0,0.0,0.0,  0.0,1.0,0.0);
}    

void mouse(int button, int state, int x, int y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      spinx = (spinx + 5) % 360; 
      glutPostRedisplay();//标记当前窗口需要重新绘制
      break;
    default:
      break;
    }
    break;

  case GLUT_MIDDLE_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      spiny = (spiny + 5) % 360; 
      glutPostRedisplay();
      break;
    default:
      break;
    }
    break;

  default:
    break;
  }
}

void main(int argc, char** argv)
{
    //初始化GL
    glutInit(&argc, argv);
    //设置显示参数(双缓存，RGB格式)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    //设置窗口尺寸：width*height
    glutInitWindowSize(width, height);
    //设置窗口位置：在屏幕左上角像素值(100,100)处
    glutInitWindowPosition(100, 100);
    //设置窗口名称
    glutCreateWindow("OpenGL");
    //显示函数，display事件需要自行编写
    glutDisplayFunc(display);
    //窗口改变的时候调用的函数
    glutReshapeFunc (reshape);
    //定义鼠标交互
    glutMouseFunc(mouse);
    //重复循环GLUT事件
    glutMainLoop();
}