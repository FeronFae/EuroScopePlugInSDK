#include "TestPlugin.h"

CPrecisionApproachPlugIn::CPrecisionApproachPlugIn()
    : CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
        "Precision Approach Radar",
        "1.0.0",
        "Feron",
        "Free to be distributed as source code")
{
    // Constructor code here
};

CPrecisionApproachPlugIn::~CPrecisionApproachPlugIn() {};



// Prime class pointer for initialization
CPrecisionApproachPlugIn* myPluginInstance = NULL;



void __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn** ppPlugInInstance)
{
    // create the instance
    *ppPlugInInstance = myPluginInstance = new CPrecisionApproachPlugIn();
}


void __declspec (dllexport) EuroScopePlugInExit()
{
    // delete the instance
    delete myPluginInstance;
}