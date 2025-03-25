<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Red Boxed Text</title>
    <style>
        .red-box {
            border: 2px solid red;
            padding: 10px;
        }
    </style>
</head>


# The plug-in development
The plug-in development offers the possibility to add your own code to EuroScope. In a plug-in you will be able to:
- be informed about all the actions in the session (even in a playback one),
- draw anything into the radar screen,
- define your own unique radar display,
- store and recall additional data in the ASR files,
- add new TAG items (not yet implemented),
- manipulate data available for the controller (not yet implemented).

# Plug-in development environment
EuroScope is programmed in C++ using MFC (Microsoft Foundation Classes). The actual version is created by Microsoft Visual Studio 2005 using MFC and CRT 8.0.50727.762. All the necessary DLLs are included in the installer. For plug-in development I do suggest using the very same versions as in this case you only need to publish your final DLL and rest is available in the EuroScope environmet.

But this is not restricted at all. You may use different CRT and MFC. But in this case you have to publish all necessary files that are required by your DLL to be loaded successfully.
All the interfaces in the plug-in development environment are built on C++ and some Windows GDI, but never used any MFC. It is up to you if you are using MFC in the plug-in or not. I use MFC just for causal programming.

All classes are designed to store only on or two pointers, but no more member variables. This allows me to change the underlying structure hidden from the plug-ins and still possible to load the old plug-ins.

# What is the development environment?
The development environment is actually no more than two files:
- `EuroScopePlugIn.h` – This file contains all the class definitions you need for the plug-in. Just include it into your sources and implement some functions and subclasses.
- `EuroScopePlugInDll.lib` – This is auto loading DLL library. It contains all the exported features from EuroScope.exe. Just link this library to your DLL.

<div class="red-box"> These files can be found in your <code>%appdata$/EuroScope/PlugIn/</code> directory for the latest running version of EuroScope. </div>

# Quick look to the structure
Let us take a quick look to the `EuroScopePlugIn.h` and to the classes and methods there. I will not describe all the methods here as the header file itself contains probably enough comments on them, but some descriptions on the classes and the relationships.

## EuroScopePlugIn namespace
All the items defined in the header file are in the EuroScopePlugIn namespace. In your code you need the `EuroScopePlugIn::` prefix for all references or use the using EuroScopePlugIn command to allow direct access.

## Constants definitions
Some constants are defined at the beginning of the header. They are declaring the followings:
- flight plan states,
- route extractions classifications,
- controller assigned value types and
- refresh phases,
- TAG color definitions,
- TAG item code definitions,
- TAG item function definitions,
- button definition (mouse handling),
- popup element types (popup lists),
- connection and coordination types.

They will be references later in the functions.

## CPosition class
The `CPosition` class is designed to store latitude/longitude coordinates. For easy programming there are functions to calculate the distance and direction between two points. These functions are very new in EuroScope and based on the formulas I received from Juha Holopainen. So far I added to this environment only. I use a far less precise formulas inside EuroScope, but I am planning to change it. Therefore expect some differences between the values here and the values inside EuroScope.

The direction returns the magnetic heading. The value calculated by the original function is corrected by the magnetic deviation defined in the main sectorfile.

## CFlightPlanExtractedRoute class
This class stores the extracted route of one flight plan. It is actually an array of structures that describes the route of the route point by point. All segments are classified by the AIRAC data if available. Sids and stars are also extracted.

You can access to this data via a `CFlightPlan` class instance.

## CFlightPlanPositionPredictions class
This class contains an array of point predictions. Every point is one minute time from the previous and describes where the plane is expected (position and altitude) and what controller is online there.

You can access to this data via a `CFlightPlan` class instance.

## CRadarTargetPositionData class
This class stores one position updates of the plane. The latest is of course the actual position of the aircraft while the previous ones can be used for history analysis. Only last 20 points are available.

You can access to this data via a `CRadarTarget` or the `CFlightPlan` class instance.

## CFlightPlanData class
This class stores all the information about the flight plan.

You can access to this data via a `CFlightPlan` class instance.

## CFlightPlanControllerAssignedData class
It is just a collection class. It allows you to access all the data that can be assigned to an aircraft by the controller.

You can access to this data via a `CFlightPlan` class instance.

## CFlightPlan class
This is the main class to store information about a flight plans. You can select a flight plan or iterate all them via the `CPlugin` class instance. And you will be noticed whenever a flight plan update is received or when a controller assigned any kind of data.

This class holds all the functions from the original `CAircraft` class that query or change the flight plan part.

## CController class
This class is used to access to the online controllers. Like the aircrafts you can select a controller or iterate all them via the `CPlugin` class instance. And you will be noticed whenever a position update is received.

## CRadarScreen class
This class is designed to hold information about a radar screen. When a new screen is created all plug-ins are offered to allocate a radar screen instance.

This class has some virtual functions that might be overridden by the plug-in. These functions will be called for screen updates, data load and save.

There is also a special function that can compile command line commands. The unrecognized command lines are first sent to the radar screens then to the plug-ins.

## CFlightPlanList
This class references to a flight plan list on the screen. You do not have access to the `SIL`, `SEL` and `DEP` lists, but you can make your own one. You can define the columns to be displayed and feed the list with aircrafts. The new example program creates a holding list.

## CPlugIn class
This is the main plug-in class. All plug-ins have to subclass this one and allocate an instance in the initialization call. All subsequent interaction between EuroScope and the plug-in will use this instance.

This function has many virtual functions that might be overridden by the plug-in.

## Global exported functions
There are two global functions that you **MUST** export from your DLL. When EuroScope loads the DLL it looks for the `EuroScopePlugInInit` and `EuroScopePlugInExit` functions. They must be implemented and declared as exported (as you see them in the header file).

Just after the DLL loading EuroScope calls the `EuroScopePlugInInit` function. There you must allocate and return the instance of you `CPlugIn` class. Just before unload EuroScope calls the `EuroScopePlugInExit` function to clean up all local variables.

# Example plug-in step-by-step
Here I describe all the steps you need to build your own plug-in. In the example I will use the “Precision Approach Plug-in” that is also available as source code for further exploration.

<div class="red-box"> There will be thoroughly commented examples compiled using <b>CMake</b> under the <code>examples</code> subdirectory for you to find, including the following presented!</div>

## New DLL project
<div class="red-box"> The following instructions are meant for a Microsoft Visual Stuio project, I will append the <b>Cmake</b> under each step, when necessary.</div>

The first step is to create a new project that creates the DLL:

![](/media/img1.png)
Here I started with an MFC DLL that is the convenient for me, but you can start with a DLL without MFC support.

![](/media/img2.png)
I added MFC with shared DLLs. That creates a smaller plug-in and uses the DLLs installed by EuroScope.

Just after the project wizard some minor adjustments have to be made in the project settings. Change the followings in both debug and release versions:
![](/media/img3.png)
In the General tab change the Character set from the default *Unicode* to Use *Multi-Byte Character Set*. Nor the VATSIM protocol, nor EuroScope supports Unicode characters at the moment.
<div class="red-box"> After creating a new empty <b>CMake</b> project in VisualStudio (or VSCode), you will have to add the following argument to your <code>CMakeLists.txt</code>: <pre><code>target_compile_definitions(${PROJECT_NAME} PRIVATE _MBCS)</code></pre>
to add the <i>Multi-Byte Character Set</i> to your project DLL, which should automatically overwrite the default <code>_UNICODE</code> character set.
</div><p></p>

![](/media/img4.png)
In the C++/General tab add the folder name where `EuroScopePlugIn.h` is located to the *Additional Include Directories*. That will enable to include it to your sources easily.
<div class="red-box">
As for <b>CMake</b>, just make sure you drag-and-drop the files from <code>%appdata$/EuroScope/PlugIn/</code> (as was previously stated) into your working directory. I like making my own separate <code>incl</code> and <code>lib</code> directories when managing multiple external header files and libraries. Having done this, just make sure your header file is imported in your main source file <code>main.cpp</code> and that the library is then linked with your DLL by importing it through your <code>CMakeLists.txt</code> as follows:
<pre><code>add_library (EUROSCOPE_SDK STATIC IMPORTED)
set_target_properties (
	EUROSCOPE_SDK PROPERTIES 
	IMPORTED_LOCATION 
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/EuroScopePlugInDll.lib"
	)
</code></pre>
before your <code>add_library</code> DLL declaration, and then linking it with
<pre><code>target_link_libraries (${PROJECT_NAME} PRIVATE EUROSCOPE_SDK)</code></pre>
after your DLL declaration.
</div><p></p>

![](/media/img5.png)
I found that disabling the *Treat wchar_t as Built-in Type* avoids some compatibility problems.
<div class="red-box">
Using <b>CMake</b> with the MSVC compiler, we have to declare this compilation setting in our <code>CMakeLists.txt</code> with:
<pre><code>set(CMAKE_CXX_FLAGS "/Zc:wchar_t-")</code></pre>
</div><p></p>

![](/media/img6.png)
Add the `EuroScopePlugInDll.lib` to the Linker/Input *Additional Dependencies*. That will link the EuroScope library to the plug-in.

To enable intelli-sense working better, I suggest adding the `EuroScopePlugIn.h` to the project. Click on the `Header Files` with a right click and choose `Add/Existing Item`.

That is enough for the base project. You can start and build the empty DLL files.

<div class="red-box">
Refer to the previous step where we added the header file and library to the project. Including the header file in our main source code and importing + linking the library into the <b>CMake</b> 
</div>

## An empty CPlugIn sub class
<div class="red-box">This example can be thoroughly found using <b>CMake</b> under <code>/examples/PrecisionApproachPlugin/</code>.</div>

To start creating the plug-in we first need to subclass the `CPlugIn` class. To do that start with the `Project/Add Class` menu:

![](/media/img8.png)
Click *Add*.

![](/media/img9.png)
Fill in the class name (here `CPrecisionApproachPlugIn`). I also changed the default file names adding a “2” postfix as the same file name is already used by the main code. My plug-in class has to be inherited from `EuroScopePlugIn::CPlugIn`.

![](/media/img10.png)
You might receive a warning message indicating that the base class is not yet found in the project. Just accept it, and continue with the class creation. We will add the necessary includes later.

Open the `PrecisionApproachPlugIn2.h` and add the include statement just after the `#pragma once` statement:
<pre><code>#include "EuroScopePlugIn.h"</code></pre>

You may compile and build the solution again.

## Adding the public exported functions

Open the original `PrecisionApproachPlugIn.cpp` file. Include `PrecisionApproachPlugIn2.h` to be able to see your class:
<pre><code>#include "PrecisionApproachPlugIn2.h"</code></pre>

Copy the `EuroScopePlugInInit` and `EuroScopePlugInExit` declarations and extend it with a body. In the init just allocate a new instance while in the exit clean it up:
<pre><code>void __declspec ( dllexport )
    EuroScopePlugInInit ( EuroScopePlugIn :: CPlugIn ** ppPlugInInstance )
{
    // allocate
    * ppPlugInInstance = pMyPlugIn =
        new CPrecisionApproachPlugIn ;
}

void __declspec ( dllexport ) EuroScopePlugInExit ( void )
{
	delete pMyPlugIn ;
}
As there is no default constructor (without parameters) for CPlugIn you must specify them at the constructor of your class:

CPrecisionApproachPlugIn::CPrecisionApproachPlugIn( void )
   : CPlugIn ( EuroScopePlugIn::COMPATIBILITY_CODE, 
                                   "Precision Approach Radar",
               "1.0.0",
               "Gergely Csernak",
               "Free to be distributed as source code" )
{
	// add your initialization here
}
</code></pre>

This is actually enough. The plug-is is ready, you can load it via the *Other Settings/Plug-ins* … menu. But practically it will not do anything.