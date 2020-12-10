#include <iostream>
#include <locale>
#include <windows.h>
#include <gl/glut.h>
#include <cmath>
#include <vector>
#include <SOIL.h>

#define PI 3.142315665

bool cam_f = 1,     // ���� ��� �����������
wires_f = 0,   // ���� ��� ����������� �������
textur_f = 0;  // ���� ��� ����������� ��������

int left_click = GLUT_UP,   // ��������� ����� �������
right_click = GLUT_UP,  // ��������� ������ �������
xold,                   // ���������� � �� ��������� (��� ������� ����)
yold,                   // ���������� y �� ��������� (��� ������� ����)
cameraDistance = 0,     // ��������� �� ������
width,                  // ������ ����
height;                 // ������ ����

float rotate_x = 0, // ��� ��������
rotate_y = 0;

GLuint texture; // ��� ��������

GLfloat LightPosition[4] = { 32, 32, 32, 1 }; // ������� ��������� ����� GL_LIGHT1

// ������ � �������
const GLfloat colors[3][4] = {
    { 1.0, 0.0, 0.0, 1.0 }, // �������
    { 0.0, 1.0, 0.0, 1.0 }, // �����
    { 0.0, 0.0, 1.0, 1.0 }, // �������
 
};

//������ � RGB
const GLfloat rgb[3][4] = {
    { 1.0, 0.0, 0.0, 1.0 },
    { 0.0, 1.0, 0.0, 1.0 },
    { 0.0, 0.0, 1.0, 1.0 }
};

/*  ��������� ���������� �����    */
GLfloat LightAmbient[4] = { 0.1f, 0.1f, 0.05f, 1.0f }; // ���� �����
void setlight()
{
    glDisable(GL_LIGHT1);
    GLfloat LightDiffuse[] = { (1.3f + LightAmbient[0]) / 2, (1.3f + LightAmbient[1]) / 2, (1.3f + LightAmbient[2]) / 2, 1.0f }; // ���� ����������� ������� �� ����� �������
    GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 0.7f }; // ���� ��������� ������ �����
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     // �������� ����
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     // ��������������� ����
    glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);   // ���������� ����
    glEnable(GL_LIGHT1); // ��������
}

/*   ��������� ���������� ���������   */
void setmaterial()
{
    GLfloat MatAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // ��� ������ ���
    GLfloat MatDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat MatShininess = 128; // ������� ������
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);
}

/*   ���������  */

void draw(GLfloat R, GLfloat height)
{
   
    double B, L;
    int dB = 10, dL = 10;
    double x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3 ;
    int color = 0;

    for (B = -90; B < 90; B += dB)
    {
        for (L = 0; L <= 360; L += dL)
        {

            x0 = R * cos(B * 3.14 / 180) * sin(L * 3.14 / 180);
            y0 = R * cos(B * 3.14 / 180) * cos(L * 3.14 / 180);
            z0 = R * sin(B * 3.14 / 180);
                z0 = 0.5 * z0;

            x1 = R * cos((B + dB) * 3.14 / 180) * sin(L * 3.14 / 180);
            y1 = R * cos((B + dB) * 3.14 / 180) * cos(L * 3.14 / 180);
            z1 = R * sin((B + dB) * 3.14 / 180);
                z1 = 0.5 * z1;

            x2 = R * cos((B + dB) * 3.14 / 180) * sin((L + dL) * 3.14 / 180);
            y2 = R * cos((B + dB) * 3.14 / 180) * cos((L + dL) * 3.14 / 180);
            z2 = R * sin((B + dB) * 3.14 / 180);
                z2 = 0.5 * z2;

            x3 = R * cos(B * 3.14 / 180) * sin((L + dL) * 3.14 / 180);
            y3 = R * cos(B * 3.14 / 180) * cos((L + dL) * 3.14 / 180);
            z3 = R * sin(B * 3.14 / 180);
                z3 = 0.5 * z3;
           
                if (!textur_f)
                { // �������
                    if (color == 3)
                        color = 0;
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colors[color]);
                    color++;
                }
              glBegin(GL_POLYGON);
              glVertex3f(x0, y0, z0);
              glVertex3f(x1, y1, z1);
              glVertex3f(x2, y2, z2);
              glVertex3f(x3, y3, z3);
              glEnd();   
                           
        }      
    }        

}

/*   ����������� ��������� � ����������� �� WIRE_F   */
void figure()
{
    setmaterial();
    setlight();

    if (wires_f) // ��� ������� ��� ����� ������� ������
    {
        glDepthFunc(GL_LEQUAL); // ��� z-buffer
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // ���������� ���� �������� �������
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // ��������� ������ ���������
        draw(10, 40);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // ����� ���� ��� ���������� ��������� �������� ��� z-buffer �������� �������
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // ������ ������ ������
    }
    draw(10, 40);
    glFlush();
}

void install_light()
{
    glPointSize(10);
    glBegin(GL_POINTS);
    glColor3d(1, 1, 1);
    glVertex3d(LightPosition[0], LightPosition[1], LightPosition[2]);
    glEnd();
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ��������
    glLoadIdentity();

    gluLookAt(0, 50, -10, 0, 0, 0, 0, 1, 0); // ��������� ����

    glPushMatrix();
    {
        // ��������� ��������� ������ � ������������
        glTranslatef(0, cameraDistance, 0);
        glRotatef(-rotate_y, 1, 0, 0);
        glRotatef(-rotate_x, 0, 1, 0);

        glPushMatrix();
        {
            install_light();
            figure();
        }
        glPopMatrix();

        glRotatef(-rotate_y, 1, 0, 0);
        glRotatef(-rotate_x, 0, 1, 0);
    }
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void keyboardFunc(unsigned char key, int x, int y)
{
    std::string a, choose;
    double r, g, b;
    while (true)
    {

        std::cout << "������� ��������:\n1. ����� ����� �����\n2. ����� ����� ������� �����(����)\n3. ������������ ����� ��������� � ����������\n4. ��������� ��������� ��������� �����\n5. ��������� �������� ���������(��������� ��� ���)\n6. ������������ ����� �������� � �������� �������\n";
        std::cin >> a;
        if (a == "1")
        {
            std::cout << "������� r, g, b:\n";
            std::cin >> r >> g >> b;
            LightAmbient[0] = r;
            LightAmbient[1] = g;
            LightAmbient[2] = b;
            break;
        }
        if (a == "2")
        {
            std::cout << "������� r, g, b:\n";
            std::cin >> r >> g >> b;
            glClearColor(r, g, b, 1);
            break;
        }
        if (a == "3")
        {
            if (textur_f)
            {
                textur_f = false;
                glDisable(GL_TEXTURE_2D);
                break;
            }
            else
            {
                textur_f = true;
                glEnable(GL_TEXTURE_2D);
                break;
            }
        }

        if (a == "4")
        {
            std::cout << "������� x, y, z:\n";
            int x, y, z;
            std::cin >> x >> y >> z;
            float LightDirection[4] = { x, y, z, 1.0f };
            glPointSize(10);
            glBegin(GL_POINTS);
            glColor3d(1, 1, 1);
            glVertex3d(x, y, z); // ������ �����
            glEnd();
            glLightfv(GL_LIGHT1, GL_POSITION, LightDirection);
            break;
        }

        if (a == "5")
        {
            while (1)
            {
                std::cout << "1. ���������\n2. �� ���������\n";
                std::cin >> choose;
                if (choose == "1")
                {
                    float MatSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
                    break;
                }
                if (choose == "2")
                {
                    float MatSpecular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
                    break;
                }
                else
                {
                    std::cout << "�������� ��������� �����!" << std::endl;
                }
            }
        }
        if (a == "6")
        {
            if (wires_f)
            {
                draw(10,40);
                wires_f = false;
                break;
            }
            else
            { 
                figure();
                wires_f = true;
                break;
            }
        }
        else
        {
            std::cout << "�������� ���� �� ��������� �������" << std::endl;
            break;
        }
    }
    system("cls");
}

void mouseFunc(int button, int state, int x, int y) { //��������� ��������� ������ � ������������
    if (button == GLUT_LEFT_BUTTON)                  
        left_click = state;                           
    if (button == GLUT_RIGHT_BUTTON)                 
        right_click = state;                          
                                                      
    xold = x;                                         
    yold = y;                                         
}                                                     
void motionFunc(int x, int y) {                       
    if (left_click == GLUT_DOWN) {                    
        rotate_y = rotate_y + (y - yold) / 5.f;       
        rotate_x = rotate_x + (x - xold) / 5.f;       
        glutPostRedisplay();                          
    }                                                 
    if (right_click == GLUT_DOWN)                     
    {                                                 
        cameraDistance -= (y - yold) * 0.2f;          
        yold = y;                                    
        glutPostRedisplay();                          
    }                                                 
                                                      
    xold = x;                                         
    yold = y;                                         
}                                                     


void reshapeFunc(int new_width, int new_height) {
    width = new_width;
    height = new_height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);

    //�������� ��������
    texture = SOIL_load_OGL_texture
    (
        "Image.bmp",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    
   
    glBindTexture(GL_TEXTURE_2D, texture); // �������� ��������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (cam_f)
        gluPerspective(90, width / height, 1, 200);
    else  glOrtho(-45, 45, -45, 45, 1, 200);

    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Ru");
    // ������� ����
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // ���������� 4 ������, � ����� ��������, � ������� �������
    glutInitWindowSize(900, 600);

    glutCreateWindow("���������");

    std::string choose;
    while (1)
    {
        std::cout << "1. ������������� ��������\n2. �����������������\n";
        std::cin >> choose;
        if (choose == "1")
        {
            cam_f = 1;
            break;
        }
        if (choose == "2")
        {
            cam_f = 0;
            break;
        }
        else std::cout << "����������, �������� ��������� �����!" << std::endl;

    }

    glEnable(GL_DEPTH_TEST); // ��� z-buffer

    glEnable(GL_POLYGON_SMOOTH); // ��� ����������� ����������� ���������
    glEnable(GL_LINE_SMOOTH); //  � �����

    glEnable(GL_TEXTURE_2D); // ������������� 2� �������

    glEnable(GL_LIGHTING); // ������������� �����
    glShadeModel(GL_SMOOTH); // �������  ����

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(&display);
    glutReshapeFunc(&reshapeFunc);
    glutMouseFunc(&mouseFunc);
    glutKeyboardFunc(&keyboardFunc);
    glutMotionFunc(&motionFunc);

    glutMainLoop();

    return 0;
}