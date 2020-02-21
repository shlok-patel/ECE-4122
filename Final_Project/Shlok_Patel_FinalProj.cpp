#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "iomanip"
#include <cmath>
#include <math.h>
#include <cstdlib>
#include <GL/glut.h>
#include <chrono>
#include <thread>

#include "Shlok_Patel_FinalProj.h"

GLuint texture[2];


struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};

typedef struct Image Image;

BMP inBitmap;

#define checkImageWidth 1200
#define checkImageHeight 579

GLubyte checkImage[checkImageWidth][checkImageHeight][3];

void makeCheckImage(void) {
    int i, j, c;
    for (i = 0; i < checkImageWidth; i++) {
        for (j = 0; j < checkImageHeight; j++) {
            c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
            checkImage[i][j][0] = (GLubyte)c;
            checkImage[i][j][1] = (GLubyte)c;
            checkImage[i][j][2] = (GLubyte)c;
        }
    }
}

void textureInit(void)
{
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    inBitmap.read("AmFBfield.bmp");
    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Create Textures
    glGenTextures(2, texture);
    // Setup first texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, inBitmap.bmp_info_header.width, inBitmap.bmp_info_header.height, 0,
                 GL_BGR_EXT, GL_UNSIGNED_BYTE, &inBitmap.data[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Do the second texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth, checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &checkImage[0][0][0]);
    glEnable(GL_TEXTURE_2D);
}

bool ifCircling = false;

// Send location and velocity vector in each direction
const int numElements = 6; // x, y, z, vx, vy, vz
const int rcvSize = 16 * 6; // (Main task + 15 UAVs) * numElements
double* rcvbuffer {new double[rcvSize]{0, 0, 0, 0, 0, 0,
                                       12, 3, 1.4, 0, 0, 0,
                                       12, 24.75, 1.4, 0, 0, 0,
                                       12, 46.5, 1.4, 0, 0, 0,
                                       33.5, 3, 1.4, 0, 0, 0,
                                       33.5, 24.75, 1.4, 0, 0, 0,
                                       33.5, 46.5, 1.4, 0, 0, 0,
                                       55, 3, 1.4, 0, 0, 0,
                                       55, 24.75, 1.4, 0, 0, 0,
                                       55, 46.5, 1.4, 0, 0, 0,
                                       76.5, 3, 1.4, 0, 0, 0,
                                       76.5, 24.75, 1.4, 0, 0, 0,
                                       76.5, 46.5, 1.4, 0, 0, 0,
                                       98, 3, 1.4, 0, 0, 0,
                                       98, 24.75, 1.4, 0, 0, 0,
                                       98, 46.5, 1.4, 0, 0, 0}};

double sendBuffer[numElements];

void timerFunction(int id);

double yardsToMeters(double yards)
{
    return yards/1.0936;
}

void displayFootballField()         //// PIAZZA @817 for better dimensions
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);   // normalize tiles for lighting
    glTexCoord2f(0, 0);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0, 1);
    glVertex3f(0.0f, yardsToMeters(54.0), 1.0f);          // use for loop counters for picking vertices
    glTexCoord2f(1, 1);
    glVertex3f(yardsToMeters(120.0), yardsToMeters(54.0), 1.0f);
    glTexCoord2f(1, 0);
    glVertex3f(yardsToMeters(120.0), 0.0f, 1.0f);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D,0);

    glPushMatrix();
    glTranslatef(yardsToMeters(120/2), yardsToMeters(54/2), 50);
    glutWireSphere(10, 10, 10);
    glPopMatrix();
}

void drawUAVs()
{
    glColor3ub(255, 255, 0);
    for(int i = 1; i < 16; i++)
    {
        glPushMatrix();
        glTranslatef(rcvbuffer[i*numElements], rcvbuffer[i*numElements+1], rcvbuffer[i*numElements+2]);
        glScalef(0.4, 0.4, 0.5);
        glRotatef(90, 1.0, 0.0, 0.0);
        glutSolidTeapot(1.0);
        glPopMatrix();
    }
}

//----------------------------------------------------------------------
// Reshape callback
//
// Window size has been set/changed to w by h pixels. Set the camera
// perspective to 45 degree vertical field of view, a window aspect
// ratio of w/h, a near clipping plane at depth 1, and a far clipping
// plane at depth 100. The viewport is the entire window.
//
//----------------------------------------------------------------------
void changeSize(int w, int h)
{
    float ratio = ((float)w) / ((float)h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(60.0, ratio, 0.1, 1000.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW); // return to modelview mode
    glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}
//----------------------------------------------------------------------
// Draw the entire scene
//
// We first update the camera location based on its distance from the
// origin and its direction.
//----------------------------------------------------------------------
void renderScene()
{
    // Clear color and depth buffers
    glClearColor(0.0, 0.0, 0.0, 1.0); // background color to green??
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Reset transformations
    glLoadIdentity();
    gluLookAt(yardsToMeters(60.0), -55, 80,
              yardsToMeters(60.0), yardsToMeters(54.0/2), 30,
              0.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    displayFootballField();
    drawUAVs();
    glutSwapBuffers(); // Make it all visible
    MPI_Allgather(sendBuffer, numElements, MPI_DOUBLE, rcvbuffer, numElements,
                  MPI_DOUBLE, MPI_COMM_WORLD);
}
//----------------------------------------------------------------------
// mainOpenGL  - standard GLUT initializations and callbacks
//----------------------------------------------------------------------
void timer(int id)
{
    glutPostRedisplay();
    glutTimerFunc(100, timer, 0);
}

//----------------------------------------------------------------------
// mainOpenGL  - standard GLUT initializations and callbacks
//----------------------------------------------------------------------
void mainOpenGL(int argc, char**argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow(argv[0]);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // Setup lights as needed
    // ...
    textureInit();

    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutTimerFunc(100, timerFunction, 0);
    glutMainLoop();
    ////  TODO: implement exit condition here and put MPI_Finalize(); in it
}
//----------------------------------------------------------------------
// timerFunction  - called whenever the timer fires
//----------------------------------------------------------------------
void timerFunction(int id)
{
    glutPostRedisplay();
    glutTimerFunc(100, timerFunction, 0);
}

void CalculateUAVsLocation(int rank)
{
    for(int i = 0; i < numElements; i++){
        sendBuffer[i] = rcvbuffer[rank*numElements + i];
    }

    if(!ifCircling) {
        double direction[3] = {yardsToMeters(120 / 2) - sendBuffer[0], yardsToMeters(54 / 2) - sendBuffer[1],
                               50 - sendBuffer[2]};

        double directionMag = sqrt(pow(direction[0], 2) + pow(direction[1], 2) + pow(direction[2], 2));

        double dirUnitVec[3] = {direction[0] / directionMag, direction[1] / directionMag, direction[2] / directionMag};

        if(directionMag < 10)
        {
            ifCircling = true;

            double randX = rand() % 10;
            double randY = rand() % 10;

            direction[0]+=randX;
            direction[1]+=randY;

            directionMag = sqrt(pow(direction[0], 2) + pow(direction[1], 2) + pow(direction[2], 2));

            dirUnitVec[0] = direction[0] / directionMag;
            dirUnitVec[1] = direction[1] / directionMag;
            dirUnitVec[2] = direction[2] / directionMag;
        }

        if(directionMag < 18)
        {
            dirUnitVec[0] = -(direction[0] / directionMag);
            dirUnitVec[1] = -(direction[1] / directionMag);
            dirUnitVec[2] = -(direction[2] / directionMag);
        }

        double forceApplied[3] = {(dirUnitVec[0] / directionMag) * 10, (dirUnitVec[1] / directionMag) * 10,
                                  (dirUnitVec[2] / directionMag) * 10 + 10};

        double forceGrav[3] = {0, 0, -10};

        double accel[3] = {forceApplied[0] + forceGrav[0], forceApplied[1] + forceGrav[1],
                           forceApplied[2] + forceGrav[2]};

        sendBuffer[3] = sendBuffer[3] + accel[0] * 0.1;
        sendBuffer[4] = sendBuffer[4] + accel[1] * 0.1;
        sendBuffer[5] = sendBuffer[5] + accel[2] * 0.1;

        double velMag = sqrt(pow(sendBuffer[3], 2) + pow(sendBuffer[4], 2) + pow(sendBuffer[5], 2));

        if (velMag > 2.0)
        {
            forceApplied[0] = 0.0;
            forceApplied[1] = 0.0;
            forceApplied[2] = 10.0;

            accel[0] = forceApplied[0] + forceGrav[0];
            accel[1] = forceApplied[1] + forceGrav[1];
            accel[2] = forceApplied[2] + forceGrav[2];

            sendBuffer[3] = sendBuffer[3] + accel[0] * 0.1;
            sendBuffer[4] = sendBuffer[4] + accel[1] * 0.1;
            sendBuffer[5] = sendBuffer[5] + accel[2] * 0.1;
        }

        sendBuffer[0] = sendBuffer[0] + sendBuffer[3] * 0.1 + 0.5 * accel[0] * 0.1 * 0.1;
        sendBuffer[1] = sendBuffer[1] + sendBuffer[4] * 0.1 + 0.5 * accel[1] * 0.1 * 0.1;
        sendBuffer[2] = sendBuffer[2] + sendBuffer[5] * 0.1 + 0.5 * accel[2] * 0.1 * 0.1;
    }
    else
    {
        double direction[3] = {yardsToMeters(120 / 2) - sendBuffer[0], yardsToMeters(54 / 2) - sendBuffer[1],
                               50 - sendBuffer[2]};

        double directionMag = sqrt(pow(direction[0], 2) + pow(direction[1], 2) + pow(direction[2], 2));

        double dirUnitVec[3] = {direction[0] / directionMag, direction[1] / directionMag, direction[2] / directionMag};

        double forceApplied[3] = {(dirUnitVec[0] / directionMag) * 10, (dirUnitVec[1] / directionMag) * 10,
                                  (dirUnitVec[2] / directionMag) * 10 + 10};

        double forceGrav[3] = {0, 0, -10};

        double accel[3] = {forceApplied[0] + forceGrav[0], forceApplied[1] + forceGrav[1],
                           forceApplied[2] + forceGrav[2]};

        double k = 10000;

        double forceHooke = -k * (10 - directionMag);

        double velMag = sqrt(pow(sendBuffer[3], 2) + pow(sendBuffer[4], 2) + pow(sendBuffer[5], 2));

        if (velMag < 3.0)
        {
            double boostedX = (sendBuffer[3] / velMag) * 20;
            double boostedY = (sendBuffer[4] / velMag) * 20;
            double boostedZ = (sendBuffer[5] / velMag) * 20;

            accel[0] = boostedX + forceGrav[0];
            accel[1] = boostedY + forceGrav[1];
            accel[2] = boostedZ + forceGrav[2];

            sendBuffer[3] = sendBuffer[3] + accel[0] * 0.1;
            sendBuffer[4] = sendBuffer[4] + accel[1] * 0.1;
            sendBuffer[5] = sendBuffer[5] + accel[2] * 0.1;
        }
        else if(velMag > 6.0)
        {
            double boostedX = (sendBuffer[3] / velMag) * 20;
            double boostedY = (sendBuffer[4] / velMag) * 20;
            double boostedZ = (sendBuffer[5] / velMag) * 20;

            accel[0] = -boostedX + forceGrav[0];
            accel[1] = -boostedY + forceGrav[1];
            accel[2] = -boostedZ + forceGrav[2];

            sendBuffer[3] = sendBuffer[3] + accel[0] * 0.1;
            sendBuffer[4] = sendBuffer[4] + accel[1] * 0.1;
            sendBuffer[5] = sendBuffer[5] + accel[2] * 0.1;
        }
        else
        {
            double newForceX = dirUnitVec[0] * 1.73 * forceHooke;
            double newForceY = dirUnitVec[1] * 1.73 * forceHooke;
            double newForceZ = dirUnitVec[2] * 1.73 * forceHooke;

            double newMag = sqrt(pow(newForceX, 2) + pow(newForceY, 2) + pow(newForceZ, 2));
            double newUnitVec[3] = {newForceX/newMag, newForceY/newMag, newForceZ/newMag};

            accel[0] = newUnitVec[0]*10 + forceGrav[0];
            accel[1] = newUnitVec[1]*10 + forceGrav[1];
            accel[2] = newUnitVec[2]*10 + forceGrav[2] + 10;

            sendBuffer[3] = sendBuffer[3] + accel[0] * 0.1;
            sendBuffer[4] = sendBuffer[4] + accel[1] * 0.1;
            sendBuffer[5] = sendBuffer[5] + accel[2] * 0.1;
        }

        sendBuffer[0] = sendBuffer[0] + sendBuffer[3] * 0.1 + 0.5 * accel[0] * 0.1 * 0.1;
        sendBuffer[1] = sendBuffer[1] + sendBuffer[4] * 0.1 + 0.5 * accel[1] * 0.1 * 0.1;
        sendBuffer[2] = sendBuffer[2] + sendBuffer[5] * 0.1 + 0.5 * accel[2] * 0.1 * 0.1;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Main entry point determines rank of the process and follows the
// correct program path
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int main(int argc, char**argv)
{
    int numTasks, rank;
    int rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS)
    {
        printf("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int gsize = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &gsize);

    srand(rank);
    if (rank == 0) {
        mainOpenGL(argc, argv);
    }
    else
    {
        // Sleep for 5 seconds
        std::this_thread::sleep_for(std::chrono::seconds(5));
        for (int ii = 0; ii < 850 ; ii++)
        {
            CalculateUAVsLocation(rank);
            MPI_Allgather(sendBuffer, numElements, MPI_DOUBLE, rcvbuffer,
                          numElements, MPI_DOUBLE, MPI_COMM_WORLD);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    exit(0);
}