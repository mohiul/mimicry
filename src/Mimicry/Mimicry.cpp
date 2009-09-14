/**
 * \file Mimicry.cpp
 */

#include "windows.h"
#include "Agent.h"
#include "Model.h"
#include "System.h"

#include <iostream>
#include "GL/glui.h"
#include "GL/glut.h"

/** A well-known mathematical constant. */
const double PI = 4 * atan((float)1);

/** Factor for converting degrees to radians. */
const double DEGTORAD = PI / 180;

/** ID of model window. */
int modelWindow;

/** Horizontal space between windows and screen boundary (pixels). */
const int X_GAP = 50;

/** Vertical space between windows and screen boundary (pixels). */
const int Y_GAP = 50;

/** Initial width of model window (pixels). */
const int W_MODEL = 500;

/** Initial height of model window (pixels). */
const int H_MODEL = 500;

/** Initial position of left edge of window model (pixels). */
const int X_MODEL = X_GAP;

/** Initial position of top edge of model window (pixels). */
const int Y_MODEL = Y_GAP;

/** Initial position of left edge of control window (pixels). */
const int X_GLUI = X_MODEL + W_MODEL + 50;

/** Initial position of top edge of control window (pixels). */
const int Y_GLUI = Y_MODEL;

/** Current width of model window (pixels). */
int wModel = W_MODEL;

/** Current height of model window (pixels). */
int hModel = H_MODEL;

/** Range of real coordinates (positive and negative) of the universe. */
const GLfloat MODEL_SIZE = 10;

/** Make the window slightly larger than the universe. */
const GLfloat WIN_SIZE = 1.05 * MODEL_SIZE;

/** Closest distance for visible objects.*/
const GLfloat NEAR_PLANE = 4 * MODEL_SIZE;;

/** Furthest distance for visible objects.*/
const GLfloat FAR_PLANE = 8 * MODEL_SIZE;

/** Use this distance to position the model so we can see all of it.*/
const double DIST = 0.5 * (NEAR_PLANE + FAR_PLANE);

Model model;

//GLUI

GLUI *glui;

/** Initial \a X translation for model. */
const float initX = 0;

/** Initial \a Y translation for model. */
const float initY = 0;

/** Initial field of view (Y) setting. */
const float initialViewingAngle = 25;

/** Current field of view (Y) setting. */
float viewingAngle = initialViewingAngle;

/** Current translations. */
float gluiPosition[3] = { initX, initY, tan(DEGTORAD * (initialViewingAngle - 90)) };

/** Current rotation matrix. */
float gluiRotation[16] =
{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

/**
* The model view is being updated.
*/
int updateModel = true;

/** The 'Start' control is enabled. */
bool startEnabled = true;

enum
{
    ZOOM,
	START,
    RESET,
    PAUSE,
    STOP,
};

GLUI_Button *ctlStart;
GLUI_Button *ctlStop;
GLUI_Button *ctlPause;
GLUI_Rotation *ctlRot;

/**
* Display the model in the model window.
* \a gluiRotation and \a gluiPosition are set by GLUI controls.
*/
void displayModel ()
{
    if (updateModel)
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, 0, - DIST);
		glMultMatrixf(gluiRotation);
		glTranslatef(gluiPosition[0], gluiPosition[1], 0);
		glEnable(GL_DEPTH_TEST);
		model.draw();
		glutSwapBuffers();
	}
}

/**
* Adjust the projection matrix.
* \a viewingAngle is set by a GLUI control.
*/
void setProjection()
{
	glutSetWindow(modelWindow);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewingAngle, GLdouble(hModel)/GLdouble(wModel), NEAR_PLANE, FAR_PLANE);
    glutPostRedisplay();
}

/**
* Reshape the model window.
* This is a response to a user action.
*/
void reshapeModel (int w, int h) {
    glViewport(0, 0, w, h);
    wModel = w;
    hModel = h;
    setProjection();
}

void idle()
{
	model.step();
	glutSetWindow(modelWindow);
	glutPostRedisplay();
}
/** Termination */
void stopSim()
{
	exit(0);
}

void keyboard(unsigned char key, int x, int y)
{

}

/**
 * Callback function for all GLUI controls.
 */
void control(int key)
{
    static bool paused = false;
    switch (key)
    {
    case START:
        if (startEnabled)
        {
            //if (model.init())
            //{
                glutSetWindow(modelWindow);
                glutIdleFunc(idle);
                startEnabled = false;
                ctlStart->disable();
                ctlStop->enable();
                ctlPause->enable();
				//std::cout << "Model Initialized!" << std::endl;
    //        }
    //        else
				//std::cout << "Cannot initialize!" << std::endl;
        }
        break;

    case ZOOM:
        viewingAngle = (180 / PI) * atan(gluiPosition[2]) + 90;
        setProjection();
        break;

	case RESET:
        ctlRot->reset();
        gluiPosition[0] = initX;
        gluiPosition[1] = initY;
        viewingAngle = initialViewingAngle;
        gluiPosition[2] = tan((PI/180) * (viewingAngle - 90));
        setProjection();
        break;

    case STOP:
      stopSim();
	  break;

    case PAUSE:
        if (paused)
        {
            glutSetWindow(modelWindow);
            glutIdleFunc(idle);
        }
        else
        {
            glutSetWindow(modelWindow);
            glutIdleFunc(0);
        }
        paused = !paused;
        break;
   }
}

int main(int argc, char** argv)
{

	/* standard GLUT initialization */

	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); /* default, not needed */
    glutInitWindowSize(W_MODEL, H_MODEL);
    glutInitWindowPosition(X_MODEL, Y_MODEL);
	modelWindow = glutCreateWindow("Mimicry"); /* window title */
	glClearColor(0.5, 0.5, 0.5, 1);
	glutDisplayFunc(displayModel);
	glutReshapeFunc(reshapeModel);
	glutSetWindow(modelWindow);
	glutIdleFunc(0);
	glutKeyboardFunc(keyboard);

    // GLUI initialization
    glui = GLUI_Master.create_glui("Controls", 0, X_GLUI, Y_GLUI);

    // Initialization panel
    GLUI_Panel *initializationPanel = glui->add_panel("Initialization");

    ctlStart = glui->add_button_to_panel(initializationPanel, "Start", START, control);

    ctlStop = glui->add_button_to_panel(initializationPanel, "Stop", STOP, control);
    ctlStop->disable();
    ctlPause = glui->add_button_to_panel(initializationPanel, "Pause/Continue", PAUSE, control);
    ctlPause->disable();

    // Model view panel
    GLUI_Panel *modelViewPanel = glui->add_panel("Model view");

    ctlRot = glui->add_rotation_to_panel(modelViewPanel, "Orientation", gluiRotation);
    glui->add_column_to_panel(modelViewPanel, true);
    GLUI_Translation *ctlTranslateXY = glui->add_translation_to_panel(
        modelViewPanel,
        "XY position",
        GLUI_TRANSLATION_XY,
        gluiPosition);
    glui->add_column_to_panel(modelViewPanel, true);
    GLUI_Translation *ctlTranslateZ = glui->add_translation_to_panel(
        modelViewPanel,
        "Zoom",
        GLUI_TRANSLATION_Z,
        &gluiPosition[2],
        ZOOM, control);

    glui->add_column_to_panel(modelViewPanel, true);

	glui->add_button_to_panel(modelViewPanel, "Reset view", RESET, control);
	glui->add_checkbox_to_panel(modelViewPanel, "Update model", &updateModel);

    ctlTranslateXY->set_speed(0.1);
    ctlTranslateZ->set_speed(0.1);
	
	/**	\todo
	  *	Initialize model. It should be at the event of START button. 
	  *	But currently facing bug due to textured image generation for Prey species. 
	  *	Which will be fixed later.
	  */
	model.init();

	/* display callback invoked when window opened */
	glutMainLoop(); /* enter event loop */
	return 0;
}
