<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Red Boxed Text</title>
    <style>
        .red-box {
            border: 2px solid red; /* Red border */
            padding: 10px;          /* Space inside the box */
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