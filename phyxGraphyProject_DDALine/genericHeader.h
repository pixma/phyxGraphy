
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <commctrl.h>
#include <gl/gl.h>
#include <gl/glu.h>

#define STATUSCONTROL 50001
//#define EDITCONSOLE 11001
#define LB_AXIX_X 11002
#define LB_AXIX_Y 11003
#define UPDOWNCONTROLCOORDINATEAX    8501
#define UPDOWNCONTROLCOORDINATEAY    8502
#define UPDOWNCONTROLCOORDINATEBX    8503
#define UPDOWNCONTROLCOORDINATEBY    8504

int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int );
LRESULT CALLBACK WindowProcedure(HWND , UINT, WPARAM, LPARAM);
LRESULT CALLBACK LeftWindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK RightWindowProc(HWND, UINT, WPARAM, LPARAM);
void __stdcall InitializeWindowComponents(HWND);
void __stdcall InitOpenGLEnvironment(HDC);
void __stdcall RenderScene();
void __stdcall ddaLineDraw(int, int, int, int);
void __stdcall insertAtListBox(int , int);
