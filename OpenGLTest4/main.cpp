#include <gl/glut.h>
#include <opencv.hpp>
using namespace cv;

//OpenCV��ȡͼ��
Mat I = imread("32.bmp");
//���ó���
int width = I.cols;
int height = I.rows;
//����ͼ��ָ��
GLubyte* pixels;
GLfloat angle = 0.0;
static GLint spinx = 0;
static GLint spiny = 0;

GLuint load_texture()
{
    //OpenGL��������������ʾ
    GLuint texture_ID;
    //��ȡͼ��ָ��
    int pixellength = width * height * 3;
    pixels = new GLubyte[pixellength];
    memcpy(pixels, I.data, pixellength * sizeof(char));
    imshow("OpenCV", I);
    //��texture_ID����Ϊ2D������Ϣ
    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    //����Ŵ���Сʹ�����Բ�ֵ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //����ˮƽ��ֱ��������ʹ���ظ���ͼ
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //����ˮƽ��ֱ��������ʹ�ñ�Ե������ͼ(���ظ���ͼ��ѡһ)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    //��ͼ���ڴ�����������Ϣ
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    free(pixels);
    return texture_ID;
}

void display()
{
    // �����Ļ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //��ȡ�������
    GLuint image = load_texture();
    //��������OpenGL���ڣ�ԭ��λ��Ϊ���Ͻǣ�x������ң�y����ϵ��£�����ֵ����������ֵ��ͬ
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 0.01, 1000.0);   //ͶӰ����
    //��ʾ����
    glMatrixMode(GL_MODELVIEW);  //ģ����ͼ
    glLoadIdentity();   //���õ�ǰָ���ľ���Ϊ��λ����
    gluLookAt (0.0,0.0,4.0,  0.0,0.0,0.0,  0.0,1.0,0.0);

    glPushMatrix();
    glRotatef ((GLfloat) spinx, 1.0, 0.0, 0.0);//������ת
    glRotatef ((GLfloat) spiny, 0.0, 1.0, 0.0);//������ת

    glEnable(GL_TEXTURE_2D);    //����ʹ������
    glBindTexture(GL_TEXTURE_2D, image);    //ѡ���������
    glBegin(GL_POLYGON);    //����Ϊ�����������ͼ��ʽ����ʼ��ͼ
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 2.0f*height/width, 0.0f);    //�������ϽǶ�Ӧ�������Ͻ�
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, -2.0f*height/width, 0.0f);    //�������½Ƕ�Ӧ�������½�
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, -2.0f*height/width, 0.0f);    //�������½Ƕ�Ӧ�������½�
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, 2.0f*height/width, 0.0f);    //�������ϽǶ�Ӧ�������Ͻ�
    glEnd();    //������ͼ*/
    glDisable(GL_TEXTURE_2D);    //��ֹʹ������
    glutSwapBuffers();   //˫���潻����������ʾͼ��
    angle = angle + 0.01;
    glPopMatrix();
    glFlush();
}

void reshape (int w, int h)     //���ڸı��ʱ����õĺ���
{    
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);  //�ӿڱ任����
  glMatrixMode (GL_PROJECTION);  //ͶӰ
  glLoadIdentity ();   //���õ�ǰָ���ľ���Ϊ��λ����
  gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 4.0, 10.0);   //ͶӰ����
  glMatrixMode(GL_MODELVIEW);  //ģ����ͼ
  glLoadIdentity();   //���õ�ǰָ���ľ���Ϊ��λ����
  gluLookAt (0.0,0.0,5.0,  0.0,0.0,0.0,  0.0,1.0,0.0);
}    

void mouse(int button, int state, int x, int y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      spinx = (spinx + 5) % 360; 
      glutPostRedisplay();//��ǵ�ǰ������Ҫ���»���
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
    //��ʼ��GL
    glutInit(&argc, argv);
    //������ʾ����(˫���棬RGB��ʽ)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    //���ô��ڳߴ磺width*height
    glutInitWindowSize(width, height);
    //���ô���λ�ã�����Ļ���Ͻ�����ֵ(100,100)��
    glutInitWindowPosition(100, 100);
    //���ô�������
    glutCreateWindow("OpenGL");
    //��ʾ������display�¼���Ҫ���б�д
    glutDisplayFunc(display);
    //���ڸı��ʱ����õĺ���
    glutReshapeFunc (reshape);
    //������꽻��
    glutMouseFunc(mouse);
    //�ظ�ѭ��GLUT�¼�
    glutMainLoop();
}