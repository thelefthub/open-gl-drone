#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
// #include <string.h>
#include <stdlib.h>
/* use of image lib*/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define NUMB 2
#define MAXL 80
char images[NUMB][MAXL]={"../sun.jpg","../space.jpeg"};
static GLuint texName[NUMB];
/* end use of image lib*/

#define ESC 27
#define PLANETS 8
#define LCONTROL 6
#define WCONTROL 4


GLfloat xORbit[PLANETS] = {4.06, 5.68, 7.01, 8.42, 10.66, 12.26, 13.96, 15.41};
GLfloat zORbit[PLANETS] = {3.06, 5.00, 7.71, 9.42, 10.00, 11.26, 14.46, 16.41};
GLfloat radius[PLANETS] = {0.3, 0.50,  0.55, 0.4, 1.05, 1.03, 0.85, 0.825};
GLfloat angle[PLANETS] = {0.0, 60.0,  120.0, 340.0, 270.0, 30.0, 180.0, 90.0};
GLfloat speed[PLANETS] = {1.0, 1.5,  3.0, 2.5, 1.2, 2.3, 0.7, 1.7};

// GLdouble x_0=20.0, y_0=20.0, z_0=20.0; 
// GLdouble x_0=1.0, y_0=70.0, z_0=1.0; //cf. top view!
GLdouble x_0=0.0, y_0=10.0, z_0=20.0; //cf. side view!

// GLdouble x_0=0.0, y_0=20.0, z_0=20.0; 
GLdouble x_ref=0.0, y_ref=0.0, z_ref=0.0;
GLdouble near = 1.0, far = 100.0;
char mode = 'o';
bool visualAids = true;
GLint winWidth = 1000, winHeight = 1000, propellers = 4; 
GLfloat sunLight[] = {0.0 ,0.0, 0.0, 1.0};
GLfloat leftLight[] = {-6.0 ,3.0, 6.0, 0.0};
GLfloat rightLight[] = {6.0 , 3.0, -6.0, 1.0};

// GLfloat sunLight[] = {0.1 ,0.0, 0.0, 0.0};


// some colours
GLfloat yellow[4] = {1.0, 1.0, 0.0, 1.0};
GLfloat red[4] = {0.9,0.1,0.0, 1.0};
GLfloat green[4] = {0.0,1.0,0.0, 1.0};
GLfloat purple[4] = {0.9,0.0,0.9, 1.0};
GLfloat black[4] = {0.0,0.0,0.0};
GLfloat shine = 60.0;

GLfloat redTrans[] = {0.9,0.1,0.0,0.5};
GLfloat greenTrans[] = {0.0,1.0,0.0,0.5};
GLfloat yellowTrans[] = {1.0, 1.0, 0.0, 0.5};

// GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat myRed[3] = {0.9,0.1,0.0};
// GLfloat green[3] = {0.0,1.0,0.0};
// GLfloat purple[3] = {0.9,0.0,0.9};
// GLfloat black[3] = {0.0,0.0,0.0};
GLfloat grey[3] = {0.412, 0.412, 0.412};

// GLfloat controlPoints[6][4][3] = {
//     {{-1.0, 0.0, 0.0}, {-0.5, 0.0, 1.0}, {0.5, 0.0, 1.0}, {1.0, 0.0, 0.0}},
//     {{-1.0, 1.0, 0.0}, {-0.5, 1.0, 1.0}, {0.5, 1.0, 1.0}, {1.0, 1.0, 0.0}},
//     {{-1.0, 2.0, 0.0}, {-0.5, 2.0, 1.0}, {0.5, 2.0, 1.0}, {1.0, 2.0, 0.0}},
//     {{-1.0, 3.0, 0.0}, {-0.5, 3.0, 1.0}, {0.5, 3.0, 1.0}, {1.0, 3.0, 0.0}},
//     {{-1.0, 4.0, 0.0}, {-0.5, 4.0, 1.0}, {0.5, 4.0, 1.0}, {1.0, 4.0, 0.0}},
//     {{-1.0, 5.0, 0.0}, {-0.5, 5.0, 1.0}, {0.5, 5.0, 1.0}, {1.0, 5.0, 0.0}},
// };


// propeller control points
// GLfloat controlPoints[6][4][3] = {
//     {{-1.0, 0.0, 0.0}, {-0.5, 0.0, 1.0}, {0.5, 0.0, 1.0}, {0.0, 0.0, 0.0}},
//     {{-2.0, 1.0, 0.0}, {-0.5, 1.0, 1.0}, {0.5, 1.0, 1.0}, {1.0, 1.0, 0.0}},
//     {{-2.0, 2.0, 0.0}, {-0.5, 2.0, 1.0}, {0.5, 2.0, 1.0}, {1.0, 2.0, 0.0}},
//     {{-2.0, 3.0, 0.0}, {-0.5, 3.0, 1.0}, {0.5, 3.0, 1.0}, {1.0, 3.0, 0.0}},
//     {{-2.0, 4.0, 0.0}, {-0.5, 4.0, 1.0}, {0.5, 4.0, 1.0}, {1.0, 4.0, 0.0}},
//     {{-1.5, 8.0, 0.0}, {-0.5, 8.0, 1.0}, {0.5, 8.0, 1.0}, {0.5, 8.0, 0.0}},
// };

GLfloat controlPoints[6][4][3] = {
    {{-0.5, 0.0, 0.0}, {-0.5, 0.0, 1.0}, {0.5, 0.0, 1.0}, {0.0, 0.0, 0.0}},
    {{-1.5, 2.0, 0.0}, {-0.5, 2.0, 1.0}, {0.5, 2.0, 1.0}, {1.0, 2.0, 0.0}},
    {{-1.5, 4.0, 0.0}, {-0.5, 4.0, 1.0}, {0.5, 4.0, 1.0}, {1.0, 4.0, 0.0}},
    {{-1.5, 6.0, 0.0}, {-0.5, 6.0, 1.0}, {0.5, 6.0, 1.0}, {1.0, 6.0, 0.0}},
    {{-1.5, 8.0, 0.0}, {-0.5, 8.0, 1.0}, {0.5, 8.0, 1.0}, {1.0, 8.0, 0.0}},
    {{-1.0, 16.0, 0.0}, {-0.5, 16.0, 1.0}, {0.5, 16.0, 1.0}, {0.5, 16.0, 0.0}},
};



void init(void)
{

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    // image lib
    glGenTextures(NUMB, texName);
    for(int i=0;i< NUMB;i++)
    {
      int x,y,n;
      stbi_uc *data = stbi_load(images[i],&x,&y,&n,0);
      if( data == NULL)
      {
        printf("%s niet gevonden\n",images[i]);
        exit(0);
      }
      printf("size X is %d en size Y is %d en n %d\n",x,y,n);
      glBindTexture (GL_TEXTURE_2D,texName[i]);
      GLint format;
      switch(n){
        case 3 : format = GL_RGB;
          break;
        case 4 : format = GL_RGBA;
          break;
        default: printf("Afbeelding niet genoeg componenten per pixel\n");
          exit(0);
      }
      glTexImage2D(GL_TEXTURE_2D, 0,format ,x,y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    }

    // Als je met glColor3 wilt blijven werken heb
    // je glEnable(GL_COLOR_MATERIAL) en glColorMaterial(GL_FRONT_AND_BACK, 
    // GL_AMBIENT_AND_DIFFUSE)

    //general light mode
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

    //light source 1
    glLightfv(GL_LIGHT0,GL_AMBIENT,black);
    glLightfv(GL_LIGHT0,GL_SPECULAR,yellow);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,yellow);
    glEnable(GL_LIGHT0);

    //enable 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable( GL_BLEND );
    
}

// visual aid: xyz-axis on window
void drawAxes(void)
{    
    glColor3fv(grey);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(4.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 4.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0,4.0);
    glEnd();
}

// visual aid: showing light sources
void lightsPos(void)
{
    glColor3fv(grey);
    // glColor3f(0.412, 0.412, 0.412);
    glBegin(GL_LINES);
    glVertex3fv(leftLight);
    glVertex3f(0.0,0.0,0.0);
    glVertex3fv(rightLight);
    glVertex3f(0.0,0.0,0.0);
    glEnd();
}

void imageBackground(void)
{
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture (GL_TEXTURE_2D,texName[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    
    glPushMatrix();

    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0, 0.0);   glVertex3f(20.0, 20.0, -15.0);
    glTexCoord2f(1.0, 0.0);   glVertex3f(-20, 20.0, -15.0);
    glTexCoord2f(1.0, 1.0);   glVertex3f(-20.0, -50.0, -15.0);
    glTexCoord2f(0.0, 1.0);   glVertex3f(20.0, -50.0, -15.0);

    glEnd();
    
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}

// define the orbit of the drone (an ellipse with origin 0.0)
void setOrbit (GLfloat orbitSpeed, GLfloat orbitRadiusX,  GLfloat orbitRadiusZ)
{
    GLfloat planetX =  0.0 + (orbitRadiusX * cos(orbitSpeed / 180.0 * M_PI));
    GLfloat planetZ = 0.0 + (orbitRadiusZ * sin(orbitSpeed / 180.0 * M_PI));

    glTranslatef(planetX, 0.0, planetZ);

}

void drawPlanet (GLint planet, GLfloat orbitSpeed)
{
    glPushMatrix();
    setOrbit(orbitSpeed,xORbit[planet], zORbit[planet]); 
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,yellow);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,yellow);
    glMaterialf(GL_FRONT,GL_SHININESS,shine);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,yellow);
    glutSolidSphere(radius[planet],20,20);
    glPopMatrix();

}

void drawBezierSurface() {
    int i, j;
    GLfloat u, v;
    // glPointSize(5.0);

    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 6, &controlPoints[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);

    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

    glEvalMesh2(GL_FILL, 0, 20, 0, 20);

    glDisable(GL_MAP2_VERTEX_3);
}

// show conrol points
void showCtrlPoints()
{
    int i;
    int j;
    glLineWidth(1.2);
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    for (i = 0; i < LCONTROL; i++)
    {
         for (j = 0; j < WCONTROL; j++)
         {
            glVertex3fv(&controlPoints[i][j][0]);
            // printf("%f \n", controlPoints[i][j][0]);

         }

    }
    glEnd();
    // glEnable(GL_LINE_STIPPLE);
    // glLineStipple(2, 0x00ff);
    // glBegin(GL_LINE_STRIP);
    // for (i = 0; i < AANTAL; i++)
    //     glVertex3fv(&ctrlpoints[i][0]);
    // glEnd();
    // glDisable(GL_LINE_STIPPLE);
}






// generate a single drone
void drawDrone(void)
{
    if (visualAids)
    {
        drawAxes();
        // lightsPos();
    }

    
    glPushMatrix();
    // glTranslatef(-5.0, 0.0, 0.0);
    glRotatef(90.0, 0.0, 0.0, 1.0);
    // glRotatef(-20.0, 1.0, 1.0, 0.0);
    drawBezierSurface();
    showCtrlPoints();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(-30.0, 1.0, 0.0, 0.0);
    glRotatef(-90.0, 0.0, 0.0, 1.0);
    // glRotatef(180.0, 0.0, 1.0, 0.0);
    // glRotatef(-20.0, 1.0, 1.0, 0.0);
    drawBezierSurface();
    showCtrlPoints();
    glPopMatrix();
    
    

    
    // use material info

   



    

    

    

}



void drawCanvas(void)
{
    drawDrone();
    
    // drawPlanet(0, angle[0]);

    // glEnable(GL_TEXTURE_2D);
    
    // // the sun in the center
    // glBindTexture (GL_TEXTURE_2D,texName[0]);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    
    // glPushMatrix();
    // GLUquadricObj * imageSphere ;
    // imageSphere = gluNewQuadric();
    // gluQuadricTexture(imageSphere, GL_TRUE );
    // gluQuadricDrawStyle(imageSphere, GLU_FILL );
    // gluSphere(imageSphere,1,50,50);
    // gluDeleteQuadric(imageSphere);
    // glPopMatrix();

    // glDisable(GL_TEXTURE_2D);
    

    // glDepthMask(GL_TRUE);
    // glDisable(GL_BLEND);
}

void move(int delta)
{
    printf("moving...  \n");
    // for(int i=0;i< PLANETS;i++)
    // {
    //     // limit needed (cf. sin/cos(angle))? 
    //     if (angle[i] > 360.0 )
    //     {
    //          angle[i]=0;
    //     } 
    //     else
    //     {
    //         angle[i]+=speed[i];
    //     } 
    // }
    
    glutTimerFunc(500, move, 1);     
    // glutSwapBuffers();
    glutPostRedisplay();
}

void keys(unsigned char key, int x, int y)
{
    switch (key)
    {
        //move camera
        case 'x': x_0++; break;
        case 'X': x_0--; break;
        case 'y': y_0++; break;
        case 'Y': y_0--; break;
        case 'z': z_0++; break;
        case 'Z': z_0--; break;
        case 'a': x_ref++; break;
        case 'A': x_ref--; break;
        case 'b': y_ref++; break;
        case 'B': y_ref--; break;
        case 'c': z_ref++; break;
        case 'C': z_ref--; break;
        case 'r': x_0=20.0, y_0=20.0, z_0=20.0; x_ref = 0.0; y_ref = 0.0; z_ref = 0.0; break;
        // view
        case 'o' : mode='o'; printf("orthographic projection\n"); break;
        case 'p' : mode='p'; printf("symmetric perspective projection\n"); break;
        case 'f' : mode='f'; printf("general perspective projection\n"); break;
        case 'h' : visualAids=!visualAids; printf("visual assistance\n"); break;

        //lighting
        // case 'o' : glEnable(GL_LIGHT0); printf("light 1 on  diffuse\n"); break;
        // case 'O' : glDisable(GL_LIGHT0); printf("light 1 off  \n"); break;
        // moving planets
        case 'm' : glutTimerFunc(500, move, 1); printf("move around...  \n"); break;
        case ESC: exit(0); break;
    }
    printf("Camera x%5.1f y%5.1f z%5.1f ",x_0,y_0,z_0);
    printf("Ref x%5.1f y%5.1f z%5.1f \n",x_ref,y_ref,z_ref);
    glutPostRedisplay();
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
    GLfloat aspect = (GLfloat)newWidth / (GLfloat)newHeight;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glShadeModel( GL_FLAT ); // GL_FLAT or GL_SMOOTH shading
    switch (mode)
    {
        case 'o':
            if (aspect >= 1.0)
            {
                glOrtho(-20*aspect, 20*aspect, -20, 20, near, far);
            }
            else
            {
                glOrtho(-20, 20, -20/aspect, 20/aspect, near, far);
            }
            break;
        case 'p':
            gluPerspective(45.0,  aspect, near, far);
            break;
        case 'f':
            glFrustum(-1.0*aspect, 1.0*aspect, -1.0, 1.0, near, far);
            break;
    }
    glViewport (0,0,newWidth,newHeight);
	winWidth = newWidth;
	winHeight = newHeight;
    fprintf(stderr, "window size %dx%d\n", newWidth, newHeight);
}

void displayFcn(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
    // set camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT0,GL_POSITION,sunLight);
    gluLookAt(x_0,y_0,z_0,    x_ref,y_ref,z_ref,    0.0, 1.0, 0.0);
    // glLightfv(GL_LIGHT0,GL_POSITION,sunLight);

    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    
    drawCanvas();
    // imageBackground();

    //repeat timerfunct and swap buffers in display?
    glutSwapBuffers();


        
    glFlush();
    // glDisable(GL_LIGHTING);//show initial color
    
}

int main(int argc, char * argv[]) {
    if ( argc > 1 )
    {
        // use first param
        char *arg = argv[1];
        propellers = atoi(arg);
        if ( argc > 2 )
                // use second param
                mode = argv[2][0];
    }
    printf("  near %lf  far %lf mode %c\n", near, far, mode);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_DEPTH|GLUT_RGBA);
    glutInitWindowSize(winWidth,winHeight);
    glutInitWindowPosition(300, 100);
    glutCreateWindow("The drone project");
    
    glViewport(0,0,winWidth,winHeight);
    
    init();
    glutDisplayFunc(displayFcn);	
	glutReshapeFunc(winReshapeFcn);
    glutKeyboardFunc(keys);
    glutMainLoop();
}
