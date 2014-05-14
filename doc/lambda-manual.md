# Lambda 2.1

![](https://raw.githubusercontent.com/gesellkammer/lambda/master/pics/icon2/lambdaicon128.png)

## Contents

* Installation  
	- Installing the prebuilt binaries  
	- Building from source  
    	+ Linux   
		+ Windows  
		+ OSX  
* Usage
* Simulation Format
* Using the Lambda binary

# INSTALLATION

You can either run the prebuilt binaries from this package or make your own build from the source, especially if the prebuilt executable generates problems on your machine.

## Linux

Required packages:

	libc6 version 2.4 or higher
	
If you have the Linux package (suffix "glx"), simply extract the contents of "bin" to "/usr/bin/". Extract the directories "examples", "doc" and "scripts" to your file system if you intend to use them. 

## Windows

If you have the Windows package (suffix "w32"), extract the contents of "bin" from the zipped archive to a path of choice, e.g. "C:\Lambda". Extract the directories "examples", "doc" and "scripts" to the same directory if you intend to use them. 

## OSX

* Install [X11](http://xquartz.macosforge.org/landing/)
* Install Qt4

Lambda depends on the Qt4 library being present in your system. Currently the best way to install Qt4 is via [Homebrew](http://brew.sh/).

	$ brew install qt
	
* Download the [Lambda.app](https://github.com/gesellkammer/lambda/raw/master/dist)

Copy Lambda.app to your /Applications folder

---

# Building from source

## Linux

Required packages:

	libQt4-core
    libQt4-debug
    libQt4-debug-dev
    libQt4-dev
    gcc, g++ (or any other C++ compiler)
	
Extract the archive to your file system. Go to the location you extracted it to and cd into the "src" directory. Run

	$ qmake
	
Once qmake is finished, run

	$ make
	
You should now have the lambda executable in the "src" directory. 

## Windows

Required software:

* Trolltech Qt 4
	
Extract the archive to your file system. Go to the location you extracted it to and cd into the "src" directory. Run

	$ qmake
	
Once qmake is finished, run

	$ make
	
You should now find the executable in the "release" directory. Now, copy the QT libraries `QtCore4.dll` and `QtGui4.dll` to that directory. Also copy `mingwm10.dll` to that directory (all these .dll files can are also provided in the bin directory of the lambda w32 package).

Since compiling the AVI encoder creates a lot of problems on Windows machines, the Windows version of Lambda compiles without this feature in the standard configuration. If you intend to compile with the AVI encoder, open lambda.pro in a text editor and replace `lambda_noAVI.h` in the `HEADERS` line with `lambda.h` and replace `lambda_noAVI.cpp` in the `SOURCES` line with `lambda.cpp`. Then, open main.cpp in a text editor and replace `lambda_noAVI.h` in the #include line with lambda.h and run qmake and make again.

## OSX 

Required software:

* Trolltech Qt 4
* revel (>= 1.1.0), distributed as part of this package.
* X11 (XQuartz)	

### With macports
	
	$ port install xvid

### With homebrew

	$ brew install xvid

Compile and install revel (used for video output). If you installed libxvid via macports, you need to include the path (opt/local/include by default)

	$ cd dependencies
	$ export CFLAGS="-I/opt/local/include -I/usr/include/malloc"
	$ ./configure
	$ make
	$ sudo make install
	
Modify the following lines in src/Lambda-OSX.pro to match your system. An Example:

	INCLUDEPATH += . /opt/X11/include/X11 /usr/include/malloc /opt/local/include
	LIBS += -L. -lrevel -lxvidcore -lX11 -L/opt/X11/lib -L/opt/local/lib
	
Now run:

	$ qmake Lambda-OSX.pro
	
This will generate a Makefile. Modify it to match your system if necessary. Then:

	$ make
	
In `src` you should have now see a `Lambda.app` inside. Move it to `/Applications`. Then launch `X11` (`XQuartz`) and open the app. If `X11` is not running, you will not be able to open the visualization window.

# Usage

## Open a simulation

* Start XQuartz.app (this is needed to display simulations)
* Start Lambda.app
* Click on the button `Open file`. ![](https://raw.githubusercontent.com/gesellkammer/lambda/master/doc/openfile.png)
* If you installed `Lambda` from source, you will find some examples in the `sims` folder. Otherwise look in the [github repo](https://github.com/gesellkammer/lambda/tree/master/sims). Click on one of the .sim files and then on `Raw`: this will download the file.

![](https://raw.githubusercontent.com/gesellkammer/lambda/master/doc/open-sim-from-github.png)

![](https://raw.githubusercontent.com/gesellkammer/lambda/master/doc/download-sim.png)

## Run the simulation

Once you have a simulation loaded, click the `Vis` checkbox to open the visualization window. Clicking the `Simulate/Pause` button will start the simulation.
* Check the `Walls` checkbox to visualize the walls defined in the simulation.
* `Avi` will record the simulation to an .avi video

--- 

## Defining a simulation

There are two ways to create a simulation:

* **Octave / Matlab**: 	Look at the files in the `scripts/matlab` or at [github](https://github.com/gesellkammer/lambda/tree/master/scripts/matlab)
* **Scilab**: Look at `scipts/scilab` or at [github](https://github.com/gesellkammer/lambda/tree/master/scripts/scilab)

### Python

There is a package to interact with Lambda, installable via pip:

	$ pip install lambdasim

Or, even better, clone it from git

	$ git clone https://github.com/gesellkammer/lambdasim
	$ cd lambdasim
	$ pip install -r requirements.txt
	$ python setup.py install
	
Read the [Documentation](https://github.com/gesellkammer/lambdasim/tree/master/docs)

--- 
# Simulation Format

[Simulation Format](https://github.com/gesellkammer/lambda/tree/master/doc/simformat.md)

---

# Command line options 

Lambda offers some useful command line options to enable users to automize the simulation routines. With these options it is possible to configure and run Lambda from an external script so that no clicking or manual interference is necessary.

```
-file <filename>   Loads the specified file upon startup 
                   and starts the simulation as soon as 
                   all input arguments have been processed.

-contrast <val>    Sets the visualization contrast to the 
                   provided value. 
                   Contrast ranges from 0 to 100
                   (default=50)

-zoom <val>        Sets the visualization zoom 
                   (1-999, default=1)
                   .avi video output will not be encoded zoomed. 
                  
-skip <val>        Sets the amount of images to skip in 
                   the visualizaton (0-999, default=0)
                   This reduces CPU usage

-quality <val>     Sets the graphics quality for the avi encoder. 
                   (percent, 1-100, default=100)

-iterations <val>  Sets the amount of simulation iterations 
                   or discrete time steps.
                   0=infinite (default)

-vis               Switches on Lambda's visualization mode 
                   on startup. 
                   OSX: X11 should be running.

-avi               Switches on avi encoder on startup. 
                   Output will be written to a file with 
                   the same name of the simulation file and 
                   .avi extension.

-rce               Switches on the recording of receivers.
                   Sound pressure data at the user defined 
                   receiver nodes will be saved to disk.

-rco               Switches on recording of all the 
                   simulation's pressure data.
                   .rco files can be opened and replayed 
                   in later sessions. 
						
-walls             Switches on visualization of walls
                   (this only makes sense if the -vis 
                   option is also set). 
							
-exit              exit after the final number of iterations
                   is reached. This is very useful for batch
                   processing of multiple simulations. 
                   If a simulation's final number of iterations
                   is set to 0 (infinite), the -exit option has 
                   no effect because the end of the simulation will
                   never be reached.
					
```

All arguments can be combined together, calling order is irrelevant. 

	$ ./lambda -file example.sim -quality 50 -iterations 1000 -avi   
	
### Windows

TODO

---------------

simon.ahrens@stud.fh-oldenburg.de  
matthias.blau@fh-oldenburg.de  
marco.ruhland@stud.fh-oldenburg.de  
[www.hoertechnik-audiologie.de](www.hoertechnik-audiologie.de)



[osxbin]: https://github.com/gesellkammer/lambda/raw/master/dist