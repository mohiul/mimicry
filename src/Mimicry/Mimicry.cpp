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

/** ID of statistics window. */
int statsWindow;

/** Horizontal space between windows and screen boundary (pixels). */
const int X_GAP = 50;

/** Vertical space between windows and screen boundary (pixels). */
const int Y_GAP = 50;

/** Initial width of model window (pixels). */
const int W_MODEL = 500;
//const int W_MODEL = 700;

/** Initial height of model window (pixels). */
const int H_MODEL = 500;
//const int H_MODEL = 700;

/** Initial position of left edge of window model (pixels). */
const int X_MODEL = X_GAP;

/** Initial position of top edge of model window (pixels). */
const int Y_MODEL = Y_GAP;

/** Initial width of statistics window (pixels). */
const int W_STATS = 600;

/** Initial height of statistics window (pixels). */
const int H_STATS =400;

/** Initial position of left edge of statistics window (pixels). */
const int X_STATS = X_MODEL + X_GAP + W_MODEL;

/** Initial position of top edge of statistics window (pixels). */
const int Y_STATS = Y_GAP;

/** Initial position of left edge of control window (pixels). */
const int X_GLUI = X_STATS;

/** Initial position of top edge of control window (pixels). */
//const int Y_GLUI = Y_STATS + H_STATS + 50;
const int Y_GLUI = Y_STATS;

/** Maximum X coordinates for statistics window. */
const GLdouble X_MAX_STATS = 10;

/** Current width of model window (pixels). */
int wModel = W_MODEL;

/** Current height of model window (pixels). */
int hModel = H_MODEL;

/** Current width of statistics window (pixels). */
int wStats = W_STATS;

/** Current height of statistics window (pixels). */
int hStats = H_STATS;

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
int updateModel = false;

/**
* The statistics view is being updated.
*/
int updateStats = true;

/** The 'Start' control is enabled. */
bool startEnabled = true;

enum
{
	START,
	PREY_SIZE,
	PREY_REPRODUCTION_AGE_LIMIT,
	PREY_REPRODUCTION_INTERVAL,
	PREY_DEMISE_AGE,
	PATTERN_MUTATION_RATE,
	PREY_GENOME_MUTATION_RATE,
	PREDATOR_REPRODUCTION_AGE_LIMIT,
	PREDATOR_REPRODUCTION_INTERVAL,
	PREDATOR_DEMISE_AGE,
	PREDATOR_GENOME_MUTATION_RATE,
	MIN_MEMORY_SIZE,
	MAX_MEMORY_SIZE,
	ZOOM,
    RESET,
    PAUSE,
    STOP,
	RING_REPORT
};

GLUI_EditText *ctlGeneBankName;
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

// Statistics Window

/**
* Display the statistics window.
*/
/*
void displayStats (void)
{
    if (updateStats)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_DEPTH_TEST);
        model.stats();
        glutSwapBuffers();
    }
}
*/

/**
* Reshape the statistics window in response to a user action.
*/
/*
void reshapeStats (int w, int h) {
    glViewport(0, 0, w, h);
    wStats = w;
    hStats = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLdouble yMax = (X_MAX_STATS * h) / w;
    gluOrtho2D(-X_MAX_STATS, X_MAX_STATS, -yMax, yMax);
    model.setStatsWindow(w, h, X_MAX_STATS, yMax);
    glutPostRedisplay();
}
*/

void idle()
{
	glutSetWindow(modelWindow);
	model.step();
	glutPostRedisplay();
    //glutSetWindow(statsWindow);
    //glutPostRedisplay();

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
            glutSetWindow(modelWindow);
			if (model.init(ctlGeneBankName->get_text()))
            {
                glutIdleFunc(idle);
                startEnabled = false;
                ctlStart->disable();
                ctlStop->enable();
                ctlPause->enable();
				std::cout << "Model Initialized!" << std::endl;
            }
            else
				std::cout << "Cannot initialize!" << std::endl;
        }
        break;

	case PREY_SIZE:
		std::cout << "Sim Time = " << model.getSimTime() << ". Prey size changed to " << System::PREY_SIZE << std::endl;
        break;

	case PREY_REPRODUCTION_AGE_LIMIT:
		std::cout << "Sim Time = " << model.getSimTime() << ". Prey reproduction age limit changed to " << System::PREY_REPRODUCTION_AGE_LIMIT << std::endl;
        break;

	case PREY_REPRODUCTION_INTERVAL:
		std::cout << "Sim Time = " << model.getSimTime() << ". Prey reproduction interval changed to " << System::PREY_REPRODUCTION_INTERVAL << std::endl;
        break;

	case PREY_DEMISE_AGE:
		std::cout << "Sim Time = " << model.getSimTime() << ". Prey demise age changed to " << System::PREY_REPRODUCTION_INTERVAL << std::endl;
        break;

	case PATTERN_MUTATION_RATE:
		std::cout << "Sim Time = " << model.getSimTime() << ". Pattern mutation rate changed to " << System::PATTERN_MUTATION_RATE << std::endl;
        break;

	case PREY_GENOME_MUTATION_RATE:
		std::cout << "Sim Time = " << model.getSimTime() << ". Prey genome mutation rate changed to " << System::PREY_GENOME_MUTATION_RATE << std::endl;
        break;

	case PREDATOR_REPRODUCTION_AGE_LIMIT:
		std::cout << "Sim Time = " << model.getSimTime() << ". Predator reproduction age limit changed to " << System::PREDATOR_REPRODUCTION_AGE_LIMIT << std::endl;
        break;

	case PREDATOR_REPRODUCTION_INTERVAL:
		std::cout << "Sim Time = " << model.getSimTime() << ". Predator reproduction interval changed to " << System::PREDATOR_REPRODUCTION_INTERVAL << std::endl;
        break;

	case PREDATOR_DEMISE_AGE:
		std::cout << "Sim Time = " << model.getSimTime() << ". Predator demise age changed to " << System::PREDATOR_REPRODUCTION_INTERVAL << std::endl;
        break;

	case PREDATOR_GENOME_MUTATION_RATE:
		std::cout << "Sim Time = " << model.getSimTime() << ". Predator genome mutation rate changed to " << System::PREDATOR_GENOME_MUTATION_RATE << std::endl;
        break;

	case MIN_MEMORY_SIZE:
		std::cout << "Sim Time = " << model.getSimTime() << ". Predator minimum memory changed to " << System::MIN_MEMORY_SIZE << std::endl;
        break;

	case MAX_MEMORY_SIZE:
		std::cout << "Sim Time = " << model.getSimTime() << ". Predator maximum memory changed to " << System::MAX_MEMORY_SIZE << std::endl;
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

    case RING_REPORT:
        model.ringReport();
        break;

   }
}

int main(int argc, char** argv)
{
	/* standard GLUT initialization */

	glutInit(&argc,argv);

    // Set up model Window
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); /* default, not needed */
    glutInitWindowSize(W_MODEL, H_MODEL);
    glutInitWindowPosition(X_MODEL, Y_MODEL);
	modelWindow = glutCreateWindow("Mimicry"); /* window title */
	glClearColor(0.5, 0.5, 0.5, 1);
	glutDisplayFunc(displayModel);
	glutReshapeFunc(reshapeModel);
	glutIdleFunc(0);

    // Set up statistics Window
	/*
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(W_STATS, H_STATS);
    glutInitWindowPosition(X_STATS, Y_STATS);
    statsWindow = glutCreateWindow("Statistics");
    glClearColor(0, 0, 0, 1);
    glutDisplayFunc(displayStats);
    glutReshapeFunc(reshapeStats);
	*/

	glutKeyboardFunc(keyboard);

    // GLUI initialization
    glui = GLUI_Master.create_glui("Controls", 0, X_GLUI, Y_GLUI);

    // Initialization panel
    GLUI_Panel *initializationPanel = glui->add_panel("Initialization");

    ctlGeneBankName = glui->add_edittext_to_panel(initializationPanel, "File   ", GLUI_EDITTEXT_TEXT);
    ctlGeneBankName->set_w(150);
    ctlGeneBankName->enable();
    ctlGeneBankName->set_text("initialconfig.xml");

	glui->add_column_to_panel(initializationPanel, true);

    ctlStart = glui->add_button_to_panel(initializationPanel, "Start", START, control);
    ctlStop = glui->add_button_to_panel(initializationPanel, "Stop", STOP, control);
    ctlStop->disable();
    ctlPause = glui->add_button_to_panel(initializationPanel, "Pause/Continue", PAUSE, control);
    ctlPause->disable();

    // Run-time controls panel
    GLUI_Panel *runtimePanel = glui->add_panel("Runtime Parameters");

    // Prey Runtime configuration panel
	GLUI_Panel *preyConfigPanel = glui->add_panel_to_panel(runtimePanel, "Prey Configuration");

    GLUI_Spinner *preySize = glui->add_spinner_to_panel(
        preyConfigPanel,
        "Prey Size",
        GLUI_SPINNER_INT,
        &System::PREY_SIZE,
        PREY_SIZE,
        control);
    preySize->set_int_limits(1, 5);

	GLUI_Panel *preyReprodConfigPanel = glui->add_panel_to_panel(preyConfigPanel, "Reproduction");

    GLUI_Spinner *preyReproductionAgeLimit = glui->add_spinner_to_panel(
        preyReprodConfigPanel,
        "Age Limit",
        GLUI_SPINNER_INT,
		&System::PREY_REPRODUCTION_AGE_LIMIT,
        PREY_REPRODUCTION_AGE_LIMIT,
        control);
    preyReproductionAgeLimit->set_int_limits(100, 10000);

    GLUI_Spinner *preyReproductionInterval = glui->add_spinner_to_panel(
        preyReprodConfigPanel,
        "Interval",
        GLUI_SPINNER_INT,
		&System::PREY_REPRODUCTION_INTERVAL,
        PREY_REPRODUCTION_INTERVAL,
        control);
    preyReproductionInterval->set_int_limits(100, 10000);

	GLUI_Panel *preyMutationRatePanel = glui->add_panel_to_panel(preyConfigPanel, "Mutation Rate");
	
	GLUI_Spinner *patternMutationRate = glui->add_spinner_to_panel(
        preyMutationRatePanel,
        "Pattern",
        GLUI_SPINNER_FLOAT,
		&System::PATTERN_MUTATION_RATE,
        PATTERN_MUTATION_RATE,
        control);
    patternMutationRate->set_int_limits(0.01, 1.0);

    GLUI_Spinner *preyGenomeMutationRate = glui->add_spinner_to_panel(
        preyMutationRatePanel,
        "Genome",
        GLUI_SPINNER_FLOAT,
		&System::PREY_GENOME_MUTATION_RATE,
        PREY_GENOME_MUTATION_RATE,
        control);
    preyGenomeMutationRate->set_int_limits(0.01, 1.0);

    GLUI_Spinner *preyDemiseAge = glui->add_spinner_to_panel(
        preyConfigPanel,
        "Demise Age",
        GLUI_SPINNER_INT,
		&System::PREY_DEMISE_AGE,
        PREY_DEMISE_AGE,
        control);
    preyDemiseAge->set_int_limits(100, 10000);

	glui->add_column_to_panel(runtimePanel, true);

	// Predator Runtime configuration panel
	GLUI_Panel *predatorConfigPanel = glui->add_panel_to_panel(runtimePanel, "Predator Configuration");

	GLUI_Panel *predatorReprodConfigPanel = glui->add_panel_to_panel(predatorConfigPanel, "Reproduction");

    GLUI_Spinner *predatorReproductionAgeLimit = glui->add_spinner_to_panel(
        predatorReprodConfigPanel,
        "Age Limit",
        GLUI_SPINNER_INT,
		&System::PREDATOR_REPRODUCTION_AGE_LIMIT,
        PREDATOR_REPRODUCTION_AGE_LIMIT,
        control);
    predatorReproductionAgeLimit->set_int_limits(100, 10000);

    GLUI_Spinner *predatorReproductionInterval = glui->add_spinner_to_panel(
        predatorReprodConfigPanel,
        "Interval",
        GLUI_SPINNER_INT,
		&System::PREDATOR_REPRODUCTION_INTERVAL,
        PREDATOR_REPRODUCTION_INTERVAL,
        control);
    predatorReproductionInterval->set_int_limits(100, 10000);

	GLUI_Panel *hopfieldNetworkConfg = glui->add_panel_to_panel(predatorConfigPanel, "Memory Configurations");

    GLUI_Spinner *predatorMinMemorySize = glui->add_spinner_to_panel(
        hopfieldNetworkConfg,
        "Minimum",
        GLUI_SPINNER_INT,
		&System::MIN_MEMORY_SIZE,
        MIN_MEMORY_SIZE,
        control);
    predatorMinMemorySize->set_int_limits(1, 20);

    GLUI_Spinner *predatorMaxMemorySize = glui->add_spinner_to_panel(
        hopfieldNetworkConfg,
        "Maximum",
        GLUI_SPINNER_INT,
		&System::MAX_MEMORY_SIZE,
        MAX_MEMORY_SIZE,
        control);
    predatorMaxMemorySize->set_int_limits(2, 20);

    GLUI_Spinner *predatorGenomeMutationRate = glui->add_spinner_to_panel(
        predatorConfigPanel,
        "Mutation Rate",
        GLUI_SPINNER_FLOAT,
		&System::PREDATOR_GENOME_MUTATION_RATE,
        PREDATOR_GENOME_MUTATION_RATE,
        control);
    predatorGenomeMutationRate->set_int_limits(0.01, 1.0);
	
	GLUI_Spinner *predatorDemiseAge = glui->add_spinner_to_panel(
        predatorConfigPanel,
        "Demise Age",
        GLUI_SPINNER_INT,
		&System::PREDATOR_DEMISE_AGE,
        PREDATOR_DEMISE_AGE,
        control);
    predatorDemiseAge->set_int_limits(100, 10000);

	glui->add_column(true);

    // Model view panel
    GLUI_Panel *modelViewPanel = glui->add_panel("Model view");

    ctlRot = glui->add_rotation_to_panel(modelViewPanel, "Orientation", gluiRotation);
    //glui->add_column_to_panel(modelViewPanel, true);
    GLUI_Translation *ctlTranslateXY = glui->add_translation_to_panel(
        modelViewPanel,
        "XY position",
        GLUI_TRANSLATION_XY,
        gluiPosition);
    //glui->add_column_to_panel(modelViewPanel, true);
    GLUI_Translation *ctlTranslateZ = glui->add_translation_to_panel(
        modelViewPanel,
        "Zoom",
        GLUI_TRANSLATION_Z,
        &gluiPosition[2],
        ZOOM, control);

    glui->add_separator_to_panel(modelViewPanel);

	glui->add_button_to_panel(modelViewPanel, "Reset view", RESET, control);
	glui->add_checkbox_to_panel(modelViewPanel, "Update model", &updateModel);
    glui->add_checkbox_to_panel(modelViewPanel, "Update statistics", &updateStats);

    // Run-time controls panel
    GLUI_Panel *controlsPanel = glui->add_panel("Runtime Controls");

    // View Selection panel
	GLUI_Panel *viewSelectionPanel = glui->add_panel_to_panel(controlsPanel, "View Selection");

	glui->add_checkbox_to_panel(viewSelectionPanel, "Outline", &System::showOutline);
	glui->add_checkbox_to_panel(viewSelectionPanel, "View Cells", &System::showCells);

    // Reports panel
    GLUI_Panel *reportPanel = glui->add_panel_to_panel(controlsPanel, "Reports");
    glui->add_button_to_panel(reportPanel, "Rings", RING_REPORT, control);

    ctlTranslateXY->set_speed(0.1);
    ctlTranslateZ->set_speed(0.1);

	/* display callback invoked when window opened */
	glutMainLoop(); /* enter event loop */
	return 0;
}
