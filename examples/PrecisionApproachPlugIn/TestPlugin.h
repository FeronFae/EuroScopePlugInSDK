#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

#ifdef BUILD_TESTPLUGIN_DLL
#define TESTPLUGIN_API __declspec(dllexport)
#else
#define TESTPLUGIN_API __declspec(dllimport)
#endif

// Includes
#include <windows.h>	// Needed for DLL API
#include "include/EuroScopePlugIn.h"


// Plugin class declaration

class CPrecisionApproachPlugIn : public EuroScopePlugIn::CPlugIn {
public: 
	explicit CPrecisionApproachPlugIn(); //Explicit constructor
	virtual ~CPrecisionApproachPlugIn(); // Inherit the class destructor from CPlugIn
};


// AVOID PRAGMA ONCE FOR COMPILER UNIVERSALITY!! MSVC COMPILER CROSS-COMPATIBILITY ISSUES MAY ARISE

#endif