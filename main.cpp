//
//  main.cpp
//  Cubenado
//
//  Created by 戴一通 on 7/7/16.
//  Copyright © 2016 ___FULLUSERNAME__. All rights reserved.
//

#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <thread>
#include <chrono>
#include <math.h>
#include <time.h>
#define PI 3.14

static int num_of_cube = 10;
static int random_control = 0;
//static int

class tornado{
private:
    GLfloat velocity;
    GLfloat theta;
    GLfloat shape;
public:
    tornado(GLfloat i_velocity, GLfloat i_theta, GLfloat i_shape){
        velocity = i_velocity;
        theta = i_theta;
        shape = i_shape;
    }
    GLfloat get_velocity(){
        GLfloat randomizer;
        if(random_control == 0) randomizer = 1;
        else  randomizer = (float)(rand() % (random_control * 2) + 11 - random_control)/10;
        return velocity * randomizer;
        //return velocity;
    }
    GLfloat get_theta(){
        GLfloat randomizer = (float)(rand() % 20 + 1)/10;
        if(random_control == 0) randomizer = 1;
        else  randomizer = (float)(rand() % (random_control * 2) + 11 - random_control)/10;
        return theta * randomizer;
        //return theta;
    }
    GLfloat get_shape(){
        GLfloat randomizer = (float)(rand() % 20 + 1)/10;
        if(random_control == 0) randomizer = 1;
        else  randomizer = (float)(rand() % (random_control * 2) + 11 - random_control)/10;
        return shape * randomizer;
    }
    
} tor(0.04, 0.1256, 0.25);

class cube{
public:
    GLfloat x;//position
    GLfloat y;//position
    GLfloat z;//position
    GLfloat w;//rotation angle
    cube(GLfloat i_x, GLfloat i_y, GLfloat i_z, GLfloat i_w){
        x = i_x;
        y = i_y;
        z = i_z;
        w = i_w;
    }
    cube(){}
    void update_position(tornado i_t){
        if(y >= 45){
            reset_cube();
        }
        else{
            y+= i_t.get_velocity();
            w+= i_t.get_theta();
            x = y * tor.get_shape() * cos(w); //+ tor.get_shape_random();
            z = y * tor.get_shape() * sin(w); //+ tor.get_shape_random();
        }
    }
    void reset_cube(){
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }
} c[10000];
class slider{
public:
    int x;
    int y;
    int switcher;
    slider(int i_x, int i_y){
        x = i_x;
        y = i_y;
        switcher = 0;
    }
} s_num(200, 150), s_random(200, 100);
/**********************************************************************************************/
void prepare_cubes(){// set the positions of all cubes in the system
    for(int i = 0; i < 10000; i++){
        cube t(0.004*i, 0.004*i, 0, 0);
        c[i] = t;
    }
}
void update_objects() {// a seperated thread that is used to update virtual 3D world
    while (1) {
        for(int i = 0; i < num_of_cube; i++){
            c[i].update_position(tor);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void update_slider(int x, int y){
    if(s_num.switcher == 1 && x >= 200 && x <= 600){
        s_num.x = x;
        num_of_cube = (int)(9990 * (x - 200)/ 400) + 10;
    }
    if(s_random.switcher == 1 && x >= 200 && x <= 600){
        s_random.x = x;
        random_control = (int)((x - 200)/40);
    }
}
void mouse(int btn, int state, int x, int y){
     y = 800 - y;
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN && x > s_num.x - 20 && x < s_num.x + 20 && y > s_num.y - 10 && y < s_num.y + 10){
        s_num.switcher = 1;
    }
    else{
        s_num.switcher = 0;
    }
    
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN && x > s_random.x - 20 && x < s_random.x + 20 && y > s_random.y - 10 && y < s_random.y + 10){
        s_random.switcher = 1;
    }
    else{
        s_random.switcher = 0;
    }
}
void myinit(void)
{	/* attributes */
    glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
    prepare_cubes();
    srand(time(0));
}
void viewMatrix() {
    gluLookAt(0, 20, 90, 0, 20, 0, 0, 1, 0);
}
void draw_silder(){
    glRectf(-20, 10, 20, -10);
}
void draw_bar(){
    glBegin(GL_POLYGON);
     glVertex3f(-2, 1, 0);
     glVertex3f(2, 1, 0);
     glVertex3f(2, -1, 0);
     glVertex3f(-2, -1, 0);
     glEnd();
}
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, 1, 0.5, 10000);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);//enable depth test
    glColor3f(0, 0, 1);
    for(int i = 0; i < num_of_cube; i++){
        glLoadIdentity();
        viewMatrix();
        glTranslatef(c[i].x, c[i].y, c[i].z);
        glutWireCube(1);
    }
 ////////////draw 2d////////////////////////////
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 800, 0.0, 800, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_DEPTH_TEST);
    glColor3f(0, 1, 0);
    glLoadIdentity();
    glTranslatef(s_num.x, s_num.y, 0);
    glScalef(1, 1, 1);
    draw_silder();//button
    
    glLoadIdentity();
    glColor3f(0, 0, 0);
    glTranslatef(400, 150, 0);
    glScalef(100, 2, 1);
    draw_bar();//bar
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 800, 0.0, 800, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_DEPTH_TEST);
    glColor3f(0, 1, 0);
    glLoadIdentity();
    glTranslatef(s_random.x, s_random.y, 0);
    glScalef(1, 1, 1);
    draw_silder();//button
    
    glLoadIdentity();
    glColor3f(0, 0, 0);
    glTranslatef(400, 100, 0);
    glScalef(100, 2, 1);
    draw_bar();//bar

    
    glFlush();
    glutSwapBuffers();
}
void refresh() {
    glutPostRedisplay();
}
int main(int argc, char** argv) {
    // insert code here...
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Cubenado");
    myinit();
    glutDisplayFunc(display);
    
    glutIdleFunc(refresh);//every frame will be rendered in main thread
    glutMotionFunc(update_slider);
    glutMouseFunc (mouse);
    
    std:: thread launcher(update_objects);
    glutMainLoop();
    return 0;
}
