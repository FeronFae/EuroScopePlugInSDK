#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

#ifdef BUILD_TESTPLUGIN_DLL
#define TESTPLUGIN_API __declspec(dllexport)
#else
#define TESTPLUGIN_API __declspec(dllimport)
#endif

#define CString char*

// Includes
#include <windows.h>	// Needed for DLL API
#include "include/EuroScopePlugIn.h"


// Plugin class declaration
class CRadarPlugIn : public EuroScopePlugIn::CPlugIn {
public:
	explicit CRadarPlugIn(); //Explicit constructor
	virtual ~CRadarPlugIn(); // Inherit the class destructor from CPlugIn
};


// Global variables from default example
CString     m_Description = "LHBP 31R PAR";
CString     m_Latitude = "N047.25.24.615";
CString m_Longitude = "E019.17.35.260";
int         m_Altitude = 550;
int         m_Heading = 310;
double      m_Slope = 2.5;





// AVOID PRAGMA ONCE FOR COMPILER UNIVERSALITY!! MSVC COMPILER CROSS-COMPATIBILITY ISSUES MAY ARISE

#endif