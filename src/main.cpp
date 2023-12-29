#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
/* use of image lib*/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define NUMB 3
#define MAXL 80
char images[NUMB][MAXL]={"../rough.jpg","../metalPaint.jpg", "../asphalt.jpg"};
static GLuint texName[NUMB];
/* end use of image lib*/

#define ESC 27
#define LCONTROL 6
#define WCONTROL 4
#define BLCONTROL 4
#define BWCONTROL 4
#define MAXPROP 6
#define MAXDRONE 3
#define MAXHEIGHT 10

// some random orbit data
GLfloat xORbit[MAXDRONE] = {-80.0, 80.0, 80.0};
GLfloat zORbit[MAXDRONE] = { -40.50, 42.50, -40.50};

GLdouble x_0=70.0, y_0=40.0, z_0=70.0;

GLdouble x_ref=0.0, y_ref=0.0, z_ref=0.0;
GLdouble near = 1.0, far = 1000.0;
char mode = 'o';
bool visualAids = false, ctrlPoints = false, texture = true, chromeFinish = true,
greyFinish=true, yellowFinish=false, propellersOn=false, transparent = false, floorTex = true, fog=true;
GLfloat propRotation = 0.0, height =7.0, speed = 60.0;
GLint winWidth = 1500, winHeight = 1000, propellers = 4, drones = 1; 

// colour and light definition
// GLfloat grey[] = {0.412,0.412,0.412};
GLfloat centerLight[] = {-70.0,30.0,-70.0,1.0};
GLfloat leftLight[] = {-40.0,10.0,60.0,1.0};
GLfloat rightLight[] = {40.0,60.0,-40.0,1.0};
GLfloat ligthX = 0.0;
GLfloat ligthY = 0.0;
GLfloat ligthZ = 0.0;
GLfloat droneLight[] = {ligthX , ligthY, ligthZ, 1.0};
GLfloat dir[] = {0.0,-10.0,0.0};
GLfloat ambient[] = {0.2, 0.2, 0.2,1.0};
GLfloat red[] = {0.9,0.1,0.0,1.0};
GLfloat greenBlue[] = {0.0,0.7,0.5,1.0};
GLfloat blue[] = {0.0,0.0,1.0,1.0};
GLfloat yellow[] = {1.0,1.0,0.0,1.0};
GLfloat shine[] = {10.0};
GLfloat chromeAmbient[] = {0.46,0.58,0.35,1.0};
GLfloat chromeDiffuse[] = {0.23,0.29,0.17,1.0};
GLfloat chromeSpecular[] = {0.69,0.87,0.52,1.0};
GLfloat bronzeAmbient[] = {0.21,0.13,0.10,1.0};
GLfloat bronzeDiffuse[] = {0.39,0.27,0.17,1.0};
GLfloat bronzeSpecular[] = {0.71,0.43,0.18,1.0};
GLfloat greyAmbient[] = {0.22,0.22,0.22,1.0};
GLfloat greyDiffuse[] = {0.33,0.33,0.33,1.0};
GLfloat greySpecular[] = {0.11,0.11,0.11,1.0};
GLfloat whiteAmbient[] = {0.22,0.22,0.22,1.0};
GLfloat whiteDiffuse[] = {0.33,0.33,0.33,1.0};
GLfloat whiteSpecular[] = {0.11,0.11,0.11,1.0};
GLfloat yellowAmbient[] = {0.65,0.55,0.15,0.0};
GLfloat yellowDiffuse[] = {0.75,0.45,0.15,0.0};
GLfloat yellowSpecular[] = {0.85,0.35,0.15,0.0};
GLfloat cutOff = 20.0;
GLfloat expo = 10.0;
GLfloat lilaAmbient[] = {0.45,0.15,0.75,0.0};
GLfloat lilaDiffuse[] = {0.55,0.15,0.65,0.0};
GLfloat lilaSpecular[] = {0.35,0.15,0.85,0.0};
GLfloat fogEffect[] = {0.5,0.5,0.5,1.0};

// surfaces definition
GLfloat controlPoints[LCONTROL][WCONTROL][3] = {
    {{-0.5, 0.0, 0.0}, {-0.5, 0.0, 1.0}, {0.5, 0.0, 1.0}, {0.0, 0.0, 0.0}},
    {{-1.5, 2.0, 0.0}, {-0.5, 2.0, 1.0}, {0.5, 2.0, 1.0}, {1.0, 2.0, 0.0}},
    {{-1.5, 4.0, 0.0}, {-0.5, 4.0, 1.0}, {0.5, 4.0, 1.0}, {1.0, 4.0, 0.0}},
    {{-1.5, 6.0, 0.0}, {-0.5, 6.0, 1.0}, {0.5, 6.0, 1.0}, {1.0, 6.0, 0.0}},
    {{-1.5, 8.0, 0.0}, {-0.5, 8.0, 1.0}, {0.5, 8.0, 1.0}, {1.0, 8.0, 0.0}},
    {{-1.0, 16.0, 0.0}, {-0.5, 16.0, 1.0}, {0.5, 16.0, 1.0}, {0.5, 16.0, 0.0}},
};

GLfloat controlPoints2[BLCONTROL][BWCONTROL][3] = {
    {{-1.0, 0.0, 1.0}, {-0.5, 0.0, 1.0}, {0.5, 0.0, 1.0}, {1.0, 0.0, 1.0}},
    {{-1.0, 0.2, 0.0}, {-0.5, 1.0, 0.0}, {0.5, 1.0, 0.0}, {1.0, 0.2, 0.0}},
    {{-1.0, 0.2, -1.0}, {-0.5, 1.0, -1.0}, {0.5, 1.0, -1.0}, {1.0, 0.2, -1.0}},
    {{-1.0, 0.0, -1.0}, {-0.5, 0.0, -1.0}, {0.5, 0.0, -1.0}, {1.0, 0.0, -1.0}}
};

GLfloat controlPoints3[BLCONTROL][BWCONTROL][3] = {
    {{-1.0, 0.0, 1.0}, {-0.5, 0.0, 1.0}, {0.5, 0.0, 1.0}, {1.0, 0.0, 1.0}},
    {{-1.0, 0.0, 0.0}, {-0.5, 0.5, 0.0}, {0.5, 0.5, 0.0}, {1.0, 0.0, 0.0}},
    {{-1.0, 0.0, -1.0}, {-0.5, 0.0, -1.0}, {0.5, 0.0, -1.0}, {1.0, 0.0, -1.0}},
    {{-1.0, 0.0, -1.0}, {-0.5, 0.0, -1.0}, {0.5, 0.0, -1.0}, {1.0, 0.0, -1.0}}
};

GLUnurbsObj *sideNurb;
GLUnurbsObj *topNurb;

void lightsOn(void)
{
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, greenBlue);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT2, GL_SPECULAR, red);
    glEnable(GL_LIGHT2);

    // the spot
    glLightfv(GL_LIGHT3, GL_AMBIENT, yellow);
    glLightfv(GL_LIGHT3,GL_DIFFUSE,yellow);
    glLightfv(GL_LIGHT3,GL_SPECULAR,yellow);
    // glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, cutOff);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dir);
    // glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, expo);
    glEnable(GL_LIGHT3);

    // general light mode
    // glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    
    // fog definition
    glFogfv(GL_FOG_COLOR, fogEffect);
    glFogf(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, 0.01);
    
    // enable transparency
    // glEnable( GL_BLEND ); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
}


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
        printf("%s not found\n",images[i]);
        exit(0);
      }
      printf("size X is %d and size Y is %d and n %d\n",x,y,n);
      glBindTexture (GL_TEXTURE_2D,texName[i]);
      GLint format;
      switch(n){
        case 3 : format = GL_RGB;
          break;
        case 4 : format = GL_RGBA;
          break;
        default: printf("Not enough components per pixel\n");
          exit(0);
      }
      glTexImage2D(GL_TEXTURE_2D, 0,format ,x,y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    }

    // to generate the B-spline surfaces
    sideNurb = gluNewNurbsRenderer();
    gluNurbsProperty(sideNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(sideNurb, GLU_DISPLAY_MODE, GLU_FILL);

    topNurb = gluNewNurbsRenderer();
    gluNurbsProperty(topNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(topNurb, GLU_DISPLAY_MODE, GLU_FILL);

    lightsOn();
    
}

// visual aid: xyz-axis on window
void drawAxes(void)
{   
    // glColor3fv(grey);
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(40.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 40.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0,40.0);
    glEnd();
    
}

// visual aid: showing light sources
void lightsPos(void)
{
    // glColor3fv(grey);
    glBegin(GL_LINES);
    glVertex3fv(centerLight);
    glVertex3f(0.0,0.0,0.0);
    glVertex3fv(leftLight);
    glVertex3f(0.0,0.0,0.0);
    glVertex3fv(rightLight);
    glVertex3f(0.0,0.0,0.0);
    glEnd();
}

// an asphalt floor with a grid to visualize the spot effect
void canvasFloor(void)
{
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture (GL_TEXTURE_2D,texName[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    for (float x = -80.0; x <= 80.0; x += 0.5) {
        for (float z = -80.0; z <= 80.0; z += 0.5) {
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex3f(x, 0.0, z);
            glTexCoord2f(1.0, 0.0); glVertex3f(x + 0.5, 0.0, z);
            glTexCoord2f(1.0, 1.0); glVertex3f(x + 0.5, 0.0, z + 0.5);
            glTexCoord2f(0.0, 1.0); glVertex3f(x, 0.0, z + 0.5);
            glEnd();
        }
    }

    glDisable(GL_TEXTURE_2D);
}

// define the orbit of the drone (an ellipse with origin 0.0)
void setOrbit (GLfloat orbitSpeed, GLfloat orbitRadiusX,  GLfloat orbitRadiusZ)
{

    GLfloat droneX =  0.0 + (orbitRadiusX * cos(orbitSpeed / 180.0 * M_PI));
    GLfloat droneZ = 0.0 + (orbitRadiusZ * sin(orbitSpeed / 180.0 * M_PI));
    glTranslatef(droneX, height, droneZ);

    ligthX = droneX;
    ligthY = height;
    ligthZ = droneZ;

    glLightfv(GL_LIGHT3,GL_POSITION,droneLight);

    if (visualAids)
    {
        glBegin(GL_LINES);
        glVertex3fv(droneLight);
        glVertex3fv(dir);
        glEnd();
    }
    
}

// draw a Bezier surface with possible texture (i.e. a propeller)
void drawBezierSurface(void) {

    //define normal to enable shine?
    glNormal3f(0,0,1);

    if (texture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture (GL_TEXTURE_2D,texName[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    }
    
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 6, &controlPoints[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);

    if (texture)
    {
        glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 3, 4, 0, 1, 12, 6, &controlPoints[0][0][0]);
        glEnable(GL_MAP2_TEXTURE_COORD_2);
    }

    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);

    if (texture)
    {
        glDisable(GL_MAP2_TEXTURE_COORD_2);

    }
    
    glDisable(GL_MAP2_VERTEX_3);

    if (texture)
    {
        glDisable(GL_TEXTURE_2D);
    }
    
}

// helper func: show conrol points and lines for a given surface
void showCtrlPoints(char surface)
{
    int i;
    int j;
    glLineWidth(1.2);
    glDisable(GL_LIGHTING);
    switch (surface)
    {
    case 'p':
        glPointSize(2.0);
        glBegin(GL_POINTS);
        for (i = 0; i < LCONTROL; i++)
        {
            for (j = 0; j < WCONTROL; j++)
            {
                glVertex3fv(&controlPoints[i][j][0]);
            }

        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (i = 0; i < LCONTROL; i++)
        {
            for (j = 0; j < WCONTROL; j++)
            {
                glVertex3fv(&controlPoints[i][j][0]);
            }

        }
        glEnd();
        break;
    
    case 's':
        glPointSize(4.0);
        glBegin(GL_POINTS);
        for (i = 0; i < BLCONTROL; i++) {
            for (j = 0; j < BWCONTROL; j++) {
                // glVertex3f(controlPoints2[i][j][0], controlPoints2[i][j][1], controlPoints2[i][j][2]);
                glVertex3fv(&controlPoints2[i][j][0]);
            }
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (i = 0; i < BLCONTROL; i++) {
            for (j = 0; j < BWCONTROL; j++) {
                glVertex3fv(&controlPoints2[i][j][0]);
            }
        }
        glEnd();
        break;

    case 't':
        glPointSize(4.0);
        glBegin(GL_POINTS);
        for (i = 0; i < BLCONTROL; i++) {
            for (j = 0; j < BWCONTROL; j++) {
                // glVertex3f(controlPoints2[i][j][0], controlPoints2[i][j][1], controlPoints2[i][j][2]);
                glVertex3fv(&controlPoints3[i][j][0]);
            }
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (i = 0; i < BLCONTROL; i++) {
            for (j = 0; j < BWCONTROL; j++) {
                glVertex3fv(&controlPoints3[i][j][0]);
            }
        }
        glEnd();
        break;
    }
    glEnable(GL_LIGHTING);
}

// draw a propeller at a given position (left or right)
void drawPropeller(char pos)
{
    glPushMatrix();
    if (pos == 'l')
    {
        glTranslatef(-34, 2, 0.0);
    }
    else
    {
        glTranslatef(34, 2, 0.0);
    }
    
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    GLUquadricObj * propCylinder;
    propCylinder = gluNewQuadric();
    gluQuadricDrawStyle(propCylinder, GLU_FILL );
    gluCylinder(propCylinder,1.0,1.0, 2.0,20,20);
    gluDeleteQuadric(propCylinder);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glRotatef(propRotation, 0.0, 0.0, 1.0);
    glutSolidSphere(1.0,20,20);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glRotatef(180.0, 0.0, 1.0, 0.0);
    glRotatef(propRotation, 0.0, 0.0, 1.0);
    drawBezierSurface();
    if (ctrlPoints) showCtrlPoints('p');
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glRotatef(propRotation, 0.0, 0.0, 1.0);
    drawBezierSurface();
    if (ctrlPoints) showCtrlPoints('p');
    glPopMatrix();
    
    glPopMatrix();
    
}

// draw the feet of the drone at a given position (left or right)
void drawFeet(char pos)
{
    glPushMatrix();
    if (pos == 'l')
    {
        glTranslatef(-16, 2, 0.0);
    }
    else
    {
        glTranslatef(16, 2, 0.0);
    }
    
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidSphere(1.0,20,20);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    GLUquadricObj * propCylinder;
    propCylinder = gluNewQuadric();
    gluQuadricDrawStyle(propCylinder, GLU_FILL );
    gluCylinder(propCylinder,1.0,1.0, 15.0,20,20);
    gluDeleteQuadric(propCylinder);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -15.0, 0.0);
    glScalef(5.0, 0.2, 1.0);
    glutSolidCube(2.0);
    glPopMatrix();

    glPopMatrix();

}

// draw one bar using 2 propellers
void drawPropBar(void)
{
    glPushMatrix();
    glScalef(35.0, 0.5, 1.0);
    glutSolidCube(2.0);
    glPopMatrix();

    if (chromeFinish)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, chromeAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, chromeDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, chromeSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    }
    else
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bronzeAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bronzeDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bronzeSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    }
    
    drawPropeller('l');
    drawPropeller('r');
    drawFeet('l');
    drawFeet('r');
}

// draw a single side casing using a B-spline
void drawSideCasing(GLfloat rotation, GLfloat xTrans, GLfloat yTrans, GLfloat zTrans)
{
    GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

    int i, j;

    glPushMatrix();
    glScalef (7.0, 7.0, 7.0);
    glRotatef(rotation, 1.0,0.0,0.0);
    glTranslatef(0.0, 2.0, 0.0);
    glTranslatef(xTrans, yTrans, zTrans);

    if (texture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture (GL_TEXTURE_2D,texName[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    }

    gluBeginSurface(sideNurb);
    gluNurbsSurface(sideNurb, 
                    8, knots, 8, knots,
                    4 * 3, 3, &controlPoints2[0][0][0], 
                    4, 4, GL_MAP2_VERTEX_3);
    if (texture)
    {
    gluNurbsSurface(sideNurb, 
                    8, knots, 8, knots,
                    4 * 3, 3, &controlPoints2[0][0][0], 
                    4, 4, GL_MAP2_TEXTURE_COORD_2);
    }
    gluEndSurface(sideNurb);

    if (texture)
    {
        glDisable(GL_MAP2_TEXTURE_COORD_2);
        glDisable(GL_TEXTURE_2D);

    }

    if (ctrlPoints) showCtrlPoints('s');

    glPopMatrix();


}
// draw the casing using a B-spline
void drawTopCasing(void)
{
    GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

    int i, j;

    glPushMatrix();
    glScalef (7.0, 7.0, 7.0);
    glTranslatef(0.0, 0.7, 0.0);

    if (texture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture (GL_TEXTURE_2D,texName[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    }

    gluBeginSurface(topNurb);
    gluNurbsSurface(topNurb, 
                    8, knots, 8, knots,
                    4 * 3, 3, &controlPoints3[0][0][0], 
                    4, 4, GL_MAP2_VERTEX_3);
    if (texture)
    {
    gluNurbsSurface(topNurb, 
                    8, knots, 8, knots,
                    4 * 3, 3, &controlPoints3[0][0][0], 
                    4, 4, GL_MAP2_TEXTURE_COORD_2);
    }
    gluEndSurface(topNurb);

    if (texture)
    {
        glDisable(GL_MAP2_TEXTURE_COORD_2);
        glDisable(GL_TEXTURE_2D);

    }
    
    if (ctrlPoints) showCtrlPoints('t');
    
    glPopMatrix();
}

// generate a single drone
void drawDrone(int number)
{
    //set height and orbit
    glPushMatrix();
    glTranslatef(0.0, height, 0.0);
    setOrbit(speed,xORbit[number], zORbit[number]);

    // the inner part of the spot
    glPushMatrix();
    glRotatef(180.0, 0.0, 1.0, 1.0);
    glTranslatef(0.0, 0.0, -5.0);
    GLUquadricObj * lightCyl;
    lightCyl = gluNewQuadric();
    gluQuadricDrawStyle(lightCyl, GLU_FILL );
    gluCylinder(lightCyl,2.0,2.0, 5.0,20,20);
    gluDeleteQuadric(lightCyl);
    glPopMatrix();
    
    // start with transparent shapes to have the nicest transparency effect
    if (transparent)
    {
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
    }
    
    if (yellowFinish)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, yellowAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellowDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellowSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    }
    else
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, lilaAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lilaDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lilaSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

    }
    
    drawTopCasing();
    
    //4 side casings
    GLfloat rotation = 0.0;
    for(int i=0;i<4;i++)
    {
        glPushMatrix();
        glRotatef(rotation, 0.0,1.0,0.0);
        drawSideCasing(-90.0, 0.0, -1.0, -0.3); //change order for depth sorting?
        glPopMatrix();

        rotation -= 90.0;
    }

    if (transparent)
    {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    // base material
    if (greyFinish)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, greyAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, greyDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, greySpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    }
    else
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, whiteAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
        
    }

    //torus to hold propeller bars
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidTorus(1.5,8.0,20,20);
    glPopMatrix();

    // draw propellers
    for(int i=0;i< (propellers/2);i++)
    {
        int angle;
        if (i==0) angle=0;

        glPushMatrix();
        drawPropBar();
        glRotatef(angle, 0.0, 1.0, 0.0);
        drawPropBar();
        glPopMatrix();
        angle+=360/propellers;
    }
    
    // the spot
    glPushMatrix();
    glRotatef(180.0, 0.0, 1.0, 1.0);
    glTranslatef(0.0, 0.0, -5.0);
    GLUquadricObj * cyl;
    cyl = gluNewQuadric();
    gluQuadricDrawStyle(cyl, GLU_FILL );
    gluCylinder(cyl,4.0,2.5, 5.0,20,20);
    gluDeleteQuadric(cyl);
    glPopMatrix();

    // ending with transparent shapes has poorer transparency effect?
    // if (transparent)
    // {
    //     glDepthMask(GL_FALSE);
    //     glEnable(GL_BLEND);
    // }
    
    // if (yellowFinish)
    // {
    //     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, yellowAmbient);
    //     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellowDiffuse);
    //     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellowSpecular);
    //     glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    // }
    // else
    // {
    //     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, lilaAmbient);
    //     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lilaDiffuse);
    //     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lilaSpecular);
    //     glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    // }
    
    //a B-spline top casing
    // drawTopCasing();
    
    // a B-spline side casing
    // GLfloat rotation = 0.0;
    // for(int i=0;i<4;i++)
    // {
    //     glPushMatrix();
    //     glRotatef(rotation, 0.0,1.0,0.0);
    //     drawSideCasing(-90.0, 0.0, -1.0, -0.3); //change order for depth sorting?
    //     glPopMatrix();

    //     rotation -= 90.0;
    // }

    // //a B-spline top casing
    // drawTopCasing();

    // if (transparent)
    // {
    //     glDisable(GL_BLEND);
    //     glDepthMask(GL_TRUE);
    //     // glEnable(GL_LIGHTING);
    // }

    glPopMatrix();
}



void drawCanvas(void)
{
    if (visualAids)
    {
        drawAxes();
        lightsPos();
        
    }

    if (fog)
    {
        glEnable(GL_FOG);
    }
    
    for(int i=0;i<drones;i++)
    {
        drawDrone(i);
    }
    
    if (floorTex)
    {
        canvasFloor();
    }
    if (fog)
    {
        glDisable(GL_FOG);
    }
    

}

// rotate the propellers
void rotate(int delta)
{
    if (propRotation == 360.0)
    {
        propRotation=delta;
    }
    else
    {
        propRotation+=delta;
    } 
    glutTimerFunc(10, rotate, 20);     
    glutPostRedisplay();
}

// fly a drone in orbit
// Flying requires being airborn, working propellers and max number of drones on the canvas
void fly(int delta)
{
    if (height>7 && propellersOn && drones<3) 
    {
        if (speed == 360.0)
        {
            speed=delta;
        }
        else
        {   
            speed+=delta;
        } 
    }
    glutTimerFunc(200, fly, 10);     
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
        // not required?
        // case 'a': x_ref++; break;
        // case 'A': x_ref--; break;
        // case 'b': y_ref++; break;
        // case 'B': y_ref--; break;
        // case 'c': z_ref++; break;
        // case 'C': z_ref--; break;
        // case 'r': x_0=70.0, y_0=30.0, z_0=70.0; x_ref = 0.0; y_ref = 0.0; z_ref = 0.0; break;
        // view
        // required to change mode at runtime?
        // case 'o' : mode='o'; printf("orthographic projection\n"); break;
        // case 'p' : mode='p'; printf("symmetric perspective projection\n"); break;
        // case 'i' : mode='f'; printf("general perspective projection\n"); break;
        case 'j' : visualAids=!visualAids; printf("visual assistance\n"); break;
        case 'k' : ctrlPoints=!ctrlPoints; printf("controlPoints and lines\n"); break;
        case 'n' : if (drones<3)drones++;printf("new drone - max number is 3\n"); break;
        //lighting & material choices
        case 'a' : glEnable(GL_LIGHT0); printf("light 1 on - ambient\n"); break;
        case 'A' : glDisable(GL_LIGHT0); printf("light 1 off  \n"); break;
        case 'b' : glEnable(GL_LIGHT1); printf("light 2 on  diffuse\n"); break;
        case 'B' : glDisable(GL_LIGHT1); printf("light 2 off  \n"); break;
        case 'c' : glEnable(GL_LIGHT2); printf("light 3 on  specular\n"); break;
        case 'C' : glDisable(GL_LIGHT2); printf("light 3 off  \n"); break;
        case 'd' : glEnable(GL_LIGHT3); printf("light 4 on spot\n"); break;
        case 'D' : glDisable(GL_LIGHT3); printf("light 4 off  \n"); break;
        case 't' : texture=!texture; printf("texture change\n"); break;
        case 'P' : chromeFinish=!chromeFinish; printf("propeller style\n"); break;
        case 'F' : greyFinish=!greyFinish; printf("frame style\n"); break;
        case 'Q' : yellowFinish=!yellowFinish; printf("case style\n"); break;
        case 'f' : transparent=!transparent; printf("show inside\n"); break;
        case 'u' : floorTex=!floorTex; printf("show floor\n"); break;
        case 'm' : fog=!fog; printf("fog change\n"); break;
        case 's':  glShadeModel(GL_FLAT);printf("flat\n");  break;
        case 'S':  glShadeModel(GL_SMOOTH);printf("smooth\n");break;
        case 'e': if (shine[0]<128)shine[0]=shine[0]+5; ;printf("shine: %f\n",shine[0] ); break;
        case 'E': if (shine[0]>5)shine[0]=shine[0]-5; ;printf("shine: %f\n", shine[0]); break;
        case 'v': if (cutOff<90)cutOff=cutOff+5; ;printf("cutOff: %f\n",cutOff ); break;
        case 'V': if (cutOff>5)cutOff=cutOff-5; ;printf("cutOff: %f\n", cutOff); break;
        case 'w': if (expo<128)expo=expo+5; ;printf("cutOff: %f\n",expo ); break;
        case 'W': if (expo>5)expo=expo-5; ;printf("cutOff: %f\n", expo); break;
        // move objects
        case 'g' : glutTimerFunc(10, rotate, 20); propellersOn=true; printf("activate propellers\n"); break;
        case 'G' : glutTimerFunc(200, fly, 10); printf("fly around\n"); break;
        case 'h' : if (height>7 && propellersOn) height--; printf("go down\n"); break;
        case 'H' : if (height<MAXHEIGHT && propellersOn)height++; printf("go up until max height\n"); break;

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
    glShadeModel( GL_FLAT );
    
    switch (mode)
    {
        case 'o':
            if (aspect >= 1.0)
            {
                glOrtho(-100*aspect, 100*aspect, -100, 100, near, far);
            }
            else
            {
                glOrtho(-100, 100, -100/aspect, 100/aspect, near, far);
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

    // modify spot params;
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, cutOff);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, expo);

    //optimal base cam for a given perspective
    switch (mode)
    {
        case 'o': x_0=70.0, y_0=40.0, z_0=70.0;break;
        case 'p': x_0=5.0, y_0=55.0, z_0=210.0;break;
        case 'f': x_0=0.0, y_0=55.0, z_0=120.0;break;
    }
	
    // set camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x_0,y_0,z_0,x_ref,y_ref,z_ref,    0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0,GL_POSITION,centerLight);
    glLightfv(GL_LIGHT1,GL_POSITION,leftLight);
    glLightfv(GL_LIGHT2,GL_POSITION,rightLight);
    // glLightfv(GL_LIGHT3,GL_POSITION,droneLight);

    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    drawCanvas();
    
    //repeat timerfunct and swap buffers in display?
    glutSwapBuffers();

    glFlush();
    
}

int main(int argc, char * argv[]) {
    if ( argc > 1 )
    {
        // mode argument
        mode = argv[1][0];
        // propeller argument: should be even
        if ( argc > 2 )
        {
            char *arg = argv[2];

            propellers = atoi(arg);
            if (propellers % 2 != 0)
            {
                printf("Propellers should be an even number, %d is not even...\n",propellers);
                exit(0);
            }
            else if (propellers > MAXPROP)
            {
                printf("The maximum number of propellers is 6, %d is bigger...\n",propellers);
                exit(0);
            }

        }
    }
    printf("  near %lf  far %lf mode %c propellers %d\n", near, far, mode, propellers);
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
