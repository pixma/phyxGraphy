#include <windows.h>
#include <commctrl.h>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "resource.h"

#define OBJECTLISTBOX 2001
#define CONSOLEAREA   2002

#define EDITBOX_X1    2010
#define EDITBOX_Y1    2011
#define EDITBOX_X2    2012
#define EDITBOX_Y2    2013
#define EDITBOX_X3    2014
#define EDITBOX_Y3    2015
#define EDITBOX_H    2016
#define EDITBOX_R    2017
#define EDITBOX_X    2018
#define EDITBOX_Y    2019

#define EDITBOX_X1UD    2050
#define EDITBOX_Y1UD    2051
#define EDITBOX_X2UD    2052
#define EDITBOX_Y2UD    2053
#define EDITBOX_X3UD    2054
#define EDITBOX_Y3UD    2055
#define EDITBOX_HUD     2056
#define EDITBOX_RUD     2057
#define EDITBOX_XUD     2058
#define EDITBOX_YUD     2059

#define TRANSLATEBUTTON 5001
#define CLEARBUTTON 5002



LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK OpenglWindowProcedure (HWND, UINT, WPARAM, LPARAM);
void __stdcall InitAppControls(HWND, HINSTANCE);
void __stdcall InitOpenGLEnvironment(HDC);
void __stdcall RenderScene();

void __stdcall TriangleRender();
void __stdcall RectRender();
void __stdcall TrasnlateObjectPredict();
void __stdcall TrasnlateObject();

