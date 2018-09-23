//  COPYRIGHT NOTES
// -----------------
//  Copyright (C) 2006 Simon Ahrens, Matthias Blau, IHA Oldenburg
//  Copyright (C) 2009 Marco Ruhland, Matthias Blau, IHA Oldenburg
//  All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  or visit http://www.gnu.org/licenses/gpl.html

#ifndef LAMBDA_H
#define LAMBDA_H

#include <QApplication>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QString>
#include <QVBoxLayout>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QDialog>
#include <QLabel>
#include <QQueue>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include "revel.h"
#include "CImg.h"
#include <time.h>
#include <ctime>

#if  cimg_OS==1
#define MAKEDIR mkdir((char*)dirFile.c_str(),0777);
#else
#define MAKEDIR
#endif

using namespace std;
using namespace cimg_library;

class QPushButton;
class QLabel;
class QSpinBox;
class QCheckBox;
class QGroupBox; 
class QLineEdit;
class QImage;
class QTimer;

// This struct provides 3 colors for writing text / painting.
struct simColors{
	float white[3];
	float grey[3];
	float black[3];
};

// This struct contains all the simulation config data.
struct simConfig{
	int n;     // actual iteration #
	int nX;    // number of nodes in X-direction
	int nY;    // number of nodes in Y-direction
	int nN;    // total number of iterations
	int nRec;  // number of receivers
	int nSrc;  // number of sources
	int nNodes;		// total number of nodes
	float cTube;	// sound speed in the tubes in meters/sec.
	float lTube;	// tube length in meters
	float rho;      // air density in kg/m^3, needed for velocity sources
	float tSample;	// tsample=1/fsample, sampling period
	float fSample;	// sampling frequency in Hz
	int nSamples;  // number of embedded samples for sources
	double t0;     // last time, used to check render speed
	int colormap;
};

// Sample Data
struct simSample {
	int id;
	int sr;
	int nsamples;
	float *data;
};

// This struct contains specific data about the simulation environment.
struct simData{
	float *envi;    // simulation environment as loaded from the .sim-file
	float *angle;   // angle matrix as loaded from the .sim-file
	float *srcs;    // array containing the sources
	float *pres;    // array containing the actual node pressure distribution
	float *inci;    // array containing the incident node pressures
	float *recs;    // array containing the receivers
	bool  *boundary;    // array indicating boundary nodes
	bool  *deadnode;    // array indicating "dead" nodes
	bool  *filt_left;   // array indicating left filters at the nodes
	bool  *filt_top;    // array indicating top filters at the nodes
	bool  *filt_right;  // array indicating right filters at the nodes
	bool  *filt_bottom; // array indicating bottom filters at the nodes
	int   *recIdx;  // array containing the receiver positions
	float *record;  // in case of recording into rco file, this array contains the recorded data
	float **oldx_left; 		// filter non-recursive memory for left filters
	float **oldx_top;  	 	// filter non-recursive memory for top filters
	float **oldx_right; 	// filter non-recursive memory for right filters
	float **oldx_bottom;    // filter non-recursive memory for bottom filters
	float **oldy_left;      // filter recursive memory for left filters
	float **oldy_top;       // filter recursive memory for top filters
	float **oldy_right;     // filter recursive memory for right filters
	float **oldy_bottom;    // filter recursive memory for bottom filters
	int *filtnumcoeffs_left;	// number of filter coeffs for left filters
	int *filtnumcoeffs_top;		// number of filter coeffs for top filters
	int *filtnumcoeffs_right;   // number of filter coeffs for right filters
	int *filtnumcoeffs_bottom;	// number of filter coeffs for bottom filters
	float **filtcoeffsA_left;	// recursive filter coeffs for left filters
	float **filtcoeffsB_left;	// non-recursive filter coeffs for left filters
	float **filtcoeffsA_top;	// recursive filter coeffs for top filters
	float **filtcoeffsB_top;	// non-recursive filter coeffs for top filters
	float **filtcoeffsA_right;  // recursive filter coeffs for right filters
	float **filtcoeffsB_right;  // non-recursive filter coeffs for right filters
	float **filtcoeffsA_bottom; // recursive filter coeffs for bottom filters
	float **filtcoeffsB_bottom; // non-recursive filter coeffs for bottom filters
	float *velo_left;	// array containing the actual velocity of velo sources from left dir.
	float *velo_top;    // array containing the actual velocity of velo sources from top dir.
	float *velo_right;  // array containing the actual velocity of velo sources from right dir.
	float *velo_bottom; // array containing the actual velocity of velo sources from bottom dir.
	float *mem;         // array for sources to use for storing information between samples.
	simSample **samples; // contains sample data from soundfiles embedded in the simulation
};

// Indices used during simulation process.
struct simIndex{
	float *presPast, *presPres, *presFutu;
	float *inciPastTop, *inciPastRight, *inciPastBottom, *inciPastLeft;
	float *inciPresTop, *inciPresRight, *inciPresBottom, *inciPresLeft;
	float *inciFutuTop, *inciFutuRight, *inciFutuBottom, *inciFutuLeft;
	float *idxP[3], *idxITop[3], *idxILeft[3], *idxIRight[3], *idxIBottom[3];
};

// Variables and config data needed for the graphics.
struct simGraphics{
	int contrast;
	int zoom;
	int skip;
	int dispSizeX;
	int dispSizeY;
	int quality;
	simColors colors;
	CImg<float> *frame;
    CImg<float> *vidframe;
	CImgDisplay *screen;
    bool frame_ready;
};

// Gui elements.
struct simGui{
	QPushButton *openFileButton;

	QCheckBox *visBox;
	QCheckBox *rceBox;
	QCheckBox *rcoBox;
	QCheckBox *aviBox;
	QCheckBox *showboundsBox;

	QPushButton *startButton;
	QPushButton *stopButton;
	QPushButton *snapButton;
	QPushButton *quitButton;

	QSpinBox *contrastBox;
	QLabel *contrastLabel;
	
	QSpinBox *framerateBox;
	QLabel *framerateLabel;
	
	QSpinBox *zoomBox;
	QLabel *zoomLabel;
	QSpinBox *skipBox;
	QLabel *skipLabel;
	QSpinBox *qualityBox;
	QLabel *qualityLabel;
	QSpinBox *samplesBox;
	QLabel *samplesLabel;
	QSpinBox *colormap;
	QLabel *colormapLabel;

	QLabel *statusLine;
	QLabel *copyright1;

	QGroupBox *inputBox;
	QGroupBox *configBox;
	QGroupBox *outputBox;
	QGroupBox *controlBox;
	QGroupBox *statusBox;
	QGroupBox *displayBox;
};

// Files used in the program.
struct simFiles{
	string lastFileName;
	ofstream aviFile;
	ofstream rceFile;
	ofstream rcoFile;
	int videoStream;
	Revel_VideoFrame videoFrame;
};


// Source data.
struct simSource{
	float y;		// y-position of source
	float x;		// x-position of source
	float type;		// type of source
	float amp;		// amplitude of source in Pascal or in m/s for velo sources
	float freq;		// frequency of source
	float phase;	// phase angle of source
};

// Error identifiers.
typedef enum{
	NONE=0,
	FILE_BAD,
	FILE_SIZE_BAD,
	FILE_HEADER_BAD_OR_WRONG_VERSION,
	FILE_DEF_BLOCK_BAD,
	FILE_ENV_BLOCK_BAD,
	FILE_ANG_BLOCK_BAD,
	FILE_FLT_BLOCK_BAD,
	FILE_SRC_BLOCK_BAD,
	TUBE_SPEED_BAD,
	TUBE_LENGTH_BAD,
	RHO_BAD,
	SRC_COORDS_BAD,
	SRC_FREQ_BAD,
	SRC_TYPE_BAD,
	NO_SAMPLES,
	NO_NODES,
	NO_SOURCES,
	CONTRAST_OUT_OF_RANGE,
	ZOOM_OUT_OF_RANGE,
	SKIP_OUT_OF_RANGE,
	QUALITY_OUT_OF_RANGE,
	ENCODING_ERROR,
	ENCODER_ERROR
} simError;

// Angular preemphasis identifiers.
typedef enum{
	kHorizontal=0,
	kVertical,
	kNone
} simAngularType;

// Status identifiers.
typedef enum{
	RUNNING=0,
	PLAYER,
	SIMULATOR,
	PAUSED,
	MISMATCH,
} simStatus;

// Main class
class lambda: public QWidget
{
	Q_OBJECT
public:
///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::lambda()
//
// PURPOSE
//   Constructor for the program's main class, initializes program and builds up GUI.
//
// INPUT
//   QWidget *parent :  Parent QT widget
//   const char *name : Window title
//   int argc         : Number of elements in input vector argv
//   char *argv[]     : Array of variable input parameters
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	lambda(QWidget *parent=0, const char *name=0, int argc=0, char *argv[]=NULL);

private slots:
///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::open()
//
// PURPOSE
//   QT Slot connected to the open file button. This is actually just a redirection. loadFile must
//   return a value for other reasons, but QT slots must always be void. So this is just a function
//   call to loadFile, dumping its return value.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void open();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::vis()
//
// PURPOSE
//   QT Slot connected to the Vis checkbox, starts or quits visualization
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland	Modified to show simfield immediatedly if   05/09   2.0
//				visbox is checked during paused simulation;
//              needed for walls-visualization
//
	void vis();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::rce()
//
// PURPOSE
//   QT Slot connected to the Rce checkbox, starts or quits receiver. The receiver stores sound
//   pressure data at user specified receiver pixels to a file.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void rce();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::rco()
//
// PURPOSE
//   QT Slot connected to the Rco checkbox, starts or quits recorder. The recorder saves all
//   simulation data into a file so that it can be replayed later.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void rco();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::avi()
//
// PURPOSE
//   QT Slot connected to the Avi checkbox, starts or quits avi encoder.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void avi();
	
///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::showbounds()
//
// PURPOSE
//   QT Slot connected to the Walls/Showbounds checkbox. Updates the visualization
//   window when checkbox is clicked.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	M. Ruhland 	new function								05/09	2.0
//
	void showbounds();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::start()
//
// PURPOSE
//   QT Slot connected to the Simulate/Pause button. Effects depend on current status.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void start();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::stop()
//
// PURPOSE
//   QT Slot connected to the Reset button. Stops simulation or replay and resets simulation data.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void stop();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::snap()
//
// PURPOSE
//   QT Slot connected to the Screenshot button. Saves a .jpg copy of the current frame to disk.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void snap();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::processSim
//
// PURPOSE
//   Processes the next simulation iteration.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	mainly rewritten. Completely new			05/09	2.0
//              simulation code for calculating digital
//              filters and velocity sources at a time.
//              Former pointer optimization not needed
//              any longer, GDB does this for us now.
//              Added new source types. Added progress
//              indicator. Former walls removed. All
//              walls are now treated as filters. Added
//              AUTOEXIT-functionality.
//
	void processSim();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::processRep
//
// PURPOSE
//   Processes the next replay frame. QT-connected to repTimer.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	irst build									05/06	1.0
//  M. Ruhland  changed copyright line                      04/08
//	M. Ruhland 	no changes									05/09	2.0
//
	void processRep();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::setContrast()
//
// PURPOSE
//   QT Slot connected to the contrast spinbox.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void setContrast();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::setZoom()
//
// PURPOSE
//   QT Slot connected to the zoom spinbox.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void setZoom();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::setSkip()
//
// PURPOSE
//   QT Slot connected to the skip spinbox.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void setSkip();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::setQuality()
//
// PURPOSE
//   QT Slot connected to the quality spinbox.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void setQuality();
	
	void setFramerate();

	void setColormap();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::setSamples()
//
// PURPOSE
//   QT Slot connected to the iterations spinbox.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void setSamples();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::checkScreen()
//
// PURPOSE
//   QT Slot connected to the visTimer. This makes sure that visBox gets unchecked if the user
//   closes the visualization window by checking 10 times a second.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	void checkScreen();

private:

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::initGui()
//
// PURPOSE
//   Creates the programs GUI by initializing QT Widgets and designing the layout.
//   QT Widgets become connected to provide the GUI's functionality.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens	First build									05/06	1.0
//  M. Ruhland  changed copyright line                      04/08
//	M. Ruhland	added "Walls"-Checkbox                      05/09   2.0
//
	void initGui(const char *name);

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::initVariables()
//
// PURPOSE
//   This function intializes all the important variables, arrays and matrices. Sets pointers
//   to NULL. Called only one single time at startup.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens	First build									05/06	1.0
//	M. Ruhland	partly rewritten; added pointers for		05/09   2.0
//              filter handling, velocity sources
//              and angular matrix
//
	void initVariables();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::handleParameters()
//
// PURPOSE
//   Processes input parameters and sets internal variables accordingly.
//
// INPUT
//   int argc     : Number of elements in input vector argv
//   char *argv[] : Array of variable input parameters
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens	First build									05/06	1.0
//	M. Ruhland	added "-walls" parameter and				05/09   2.0
//              "-exit" parameter
//
	void handleParameters(int argc, char *argv[]);

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::defineSource()
//
// PURPOSE
//   Adds a source to the array of sources (data.srcs) after performing a few checks on the data.
//
// INPUT
//   const unsigned int idx   : An integer defining the source's index in the array.
//   const simSource *srcData : Pointer to the source to be added.
//
// OUTPUT
//   None
//
// RETURN VALUE
//   simError: NONE if source was added successfully, error identfier otherwise.
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	added velocity sources handling				05/09	2.0
//
	virtual simError defineSource(const int idx, const simSource *srcData);

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::set()
//
// PURPOSE
//   Function template. This function should be used whenever one of the key variables of the
//   lambda class is changed. It performs the necessary checks before changing one of those
//   variables and makes necessary additional changes after changing.
//   For example, the number of elements in X-direction should not just be changed. Instead, you
//   should use set("nX",41);. This will check wether the second argument is a valid value for nX
//   (>0) and will update nNodes (which should always be nX*nY at any time) and dispSizeX automatically.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//  M. Ruhland  fixed some warnings                         04/08
//  M. Ruhland  added "rho"-parameter for velocity sources	05/09   2.0
//              and removed config.FourFields, which is
//              no longer needed
//
	template<class T> simError set(const string what, const T value);

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::loadFile
//
// PURPOSE
//   Loads a simulation or replay file into the program. If no filename is provided, opens a
//   dialog for file selection. loadFile tries to load the file as a replay file first and then
//   opens it as a simulation definition file, if that fails.
//
// INPUT
//   string fileName : name of the file to be opened. Default value is "NONE", which will open
//                     the file selection dialog.
//
// OUTPUT
//   None
//
// RETURN VALUE
//   simError: NONE if file was opened successfully, error identfier otherwise.
//
// AUTHOR		CHANGES						DATE		VERSION
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	S. Ahrens	Fixed bug preventing opening dialog         10/07
//				in Win32
//	M. Ruhland 	no changes									05/09	2.0
//
	virtual simError loadFile(string fileName="NONE");

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::loadRecord
//
// PURPOSE
//   Tries to load a recorded playback file.
//
// INPUT
//   const string fileName : name of the file to be opened.
//
// OUTPUT
//   None
//
// RETURN VALUE
//   simError: NONE if file was opened successfully, error identfier otherwise.
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	no changes									05/09	2.0
//
	virtual simError loadRecord(const string fileName);

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::loadSimulation
//
// PURPOSE
//   Tries to load a simulation file.
//
// INPUT
//   const string fileName : name of the file to be opened.
//
// OUTPUT
//   None
//
// RETURN VALUE
//   simError: NONE if file was opened successfully, error identfier otherwise.
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland	implemented sqrt(2) reflection factor		04/08	1.1
//				preemphasis, and extended reflection
//				factor value range from -1 to 1.
//				Simulation field borders initialized
//				for zero reflection.
//  M. Ruhland  Completely rewritten for new 2.0 file       05/09   2.0
//              structure and computation model.
//
	virtual simError loadSimulation(const string fileName);

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::initSimulation
//
// PURPOSE
//   Prepares variables needed for simulation.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   simError: NONE if no error occured, error identfier otherwise.
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//	M. Ruhland 	mainly rewritten; added code for 			05/09	2.0
//              allocating and resetting the filter's
//              memories and the velocity source arrays
//
	virtual simError initSimulation();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::resetAll()
//
// PURPOSE
//   Resets important variables, arrays and matrices to zero, e.g. before starting new simulation
//   runs.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens	First build									05/06	1.0
//	M. Ruhland	mainly rewritten; added code for resetting	05/09   2.0
//              the filter data and the angular matrix
//
	virtual void resetAll();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::resetSimulation()
//
// PURPOSE
//   Resets variables and arrays used directly for simulation purposes.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens	First build									05/06	1.0
//	M. Ruhland	mainly rewritten; added resetting of the 	05/09   2.0
//              filter's memories and the velocity sources;
//				removed some old optimization pointers
//              that are not needed any more
//
	virtual void resetSimulation();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::processVis()
//
// PURPOSE
//   Updates the visualization screen after each simulation iteration, if vis is on.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//  M. Ruhland  changed copyright line                      04/08
//	M. Ruhland 	Added drawing of the walls and receivers	05/09	2.0
//              if the "walls"-checkbox is checked
//
	virtual void processVis();

	virtual void processFrame(CImg<float> *frame, float* pressure);



///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::processRce()
//
// PURPOSE
//   Processes the receiver output after each calculated sim iteration if Rce is switched on.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//  M. Ruhland  no changes                                  05/09   2.0
//
	virtual void processRce();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::processRco()
//
// PURPOSE
//   Processes the recorder output after each calculated sim iteration if Rco is switched on.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//  M. Ruhland  no changes                                  05/09   2.0
//
	virtual void processRco();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::processAvi()
//
// PURPOSE
//   Processes the avi encoder output after each calculated sim iteration if Avi is switched on.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									05/06	1.0
//  M. Ruhland  changed copyright line                      04/08
//	M. Ruhland 	Added drawing of the walls and receivers	05/09	2.0
//              if the "walls"-checkbox is checked
//
	virtual void processAvi();

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::drawLambda
//
// PURPOSE
//   Draws the lambda logo on the visualization screen.
//
// INPUT
//   None
//
// OUTPUT
//   None
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	S. Ahrens 	First build									08/06	1.0
//  M. Ruhland  fixed some warnings                         04/08   1.1
//  M. Ruhland  changed copyright line                      04/08
//	M. Ruhland 	no changes									05/09	2.0
//
	virtual void drawLambda();
	
///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::adaptreflexionfactor
//
// PURPOSE
//   Creates a new digital filter for a given real-valued reflexion factor and
//   preemphases the filter coefficients due to a given sonic incidence angle
//   and horizontal/vertical alignment of the filter.
//
// INPUT
//   float r    	: desired real-valued reflexion factor
//   float alpha	: angle of incidence for the desired filter, needed for preemphasis
//   simAngularType direction : sets whether the filter is used in horizontal
//                              or vertical tubes, needed for preemphasis
//
// OUTPUT
//   int& dest_numcoeffs    : number of filter coefficients for the calculated filter
//   float*& dest_coeffsA   : array containig the new computed a-Filter-coefficients
//   float*& dest_coeffsB   : array containig the new computed b-Filter-coefficients
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	M. Ruhland 	new function								05/09	2.0
//
	virtual void adaptreflexionfactor(int& dest_numcoeffs,float*& dest_coeffsA,float*& dest_coeffsB,
							 		  float r,float alpha,simAngularType direction);

///////////////////////////////////////////////////////////////////////////////////////////////////
// lambda::adaptfilter
//
// PURPOSE
//   Creates a new digital filter from a given digital filter ID and
//   preemphases the filter coefficients due to a given sonic incidence angle
//	 and horizontal/vertical alignment of the filter.
//
// INPUT
//	 int* src_id    		: pointer on source filter ID array
//   int* src_numcoeffs 	: pointer on source filter numcoeffs array
//   float** src_coeffsA	: pointer on 2-Dim filter a-coeff Matrix
//   float** src_coeffsB	: pointer on 2-Dim filter b-coeff Matrix
//   int src_numfilters     : number of filters in the srcfilter-Arrays
//   int id    				: desired filter ID
//   float alpha			: angle of incidence for the desired filter, needed for preemphasis
//   simAngularType direction : sets whether the filter is used in horizontal
//                              or vertical tubes, needed for preemphasis
//
// OUTPUT
//   int& dest_numcoeffs    : number of filter coefficients for the calculated filter
//   float*& dest_coeffsA   : array containig the new computed a-Filter-coefficients
//   float*& dest_coeffsB   : array containig the new computed b-Filter-coefficients
//
// RETURN VALUE
//   None
//
//	AUTHOR		CHANGES										DATE	VERSION
//	M. Ruhland 	new function								05/09	2.0
//
	virtual void adaptfilter(int& dest_numcoeffs,float*& dest_coeffsA,float*& dest_coeffsB,
							 int* src_id,int* src_numcoeffs,
							 float** src_coeffsA,float** src_coeffsB,
							 int src_numfilters,
							 int id,float alpha,simAngularType direction);

	simConfig   config;
	simData     data;
	simIndex    index;
	simGraphics graphics;
	simGui      gui;
	simStatus   status;
	simFiles    files;

	QTimer *simTimer;
	QTimer *repTimer;
	QTimer *timer;
	QTimer *visTimer;
};

#endif
