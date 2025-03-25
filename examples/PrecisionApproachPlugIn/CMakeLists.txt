cmake_minimum_required (VERSION 3.10)

# Visual Studio compatibility
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set (CMAKE_VERBOSE_MAKEFILE ON)

# Basic DLL export compatibility setting
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Disable potential compatibility issues caused by wchar_t being treated as Built-in Type
set(CMAKE_CXX_FLAGS "/Zc:wchar_t-")

#Project declaration
project ("TestPlugin")


# Add the project to be compiled from the source as a DLL 
add_library (${PROJECT_NAME} SHARED "TestPlugin.cpp" "TestPlugin.h")

# Link the library with the DLL during compilation
target_link_libraries (${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/EuroScopePlugInDll.lib")


# Use Multi-Byte Character Set instead of Unicode
target_compile_definitions(${PROJECT_NAME} PRIVATE _MBCS)



