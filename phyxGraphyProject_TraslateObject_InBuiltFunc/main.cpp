#include "genricHeader.h"

/*  Declare Windows procedure  */


/*  Make the class name into a global variable  */
char szClassName[ ] = "phyxGraphyWindow";

HINSTANCE ghInstance;
HWND ghMainWindow, ghGlWindow, ghConsoleArea;
HWND ghListBoxObject;
HWND ghUpDownX1, ghUpDownY1, ghUpDownX2, ghUpDownY2, ghUpDownX3, ghUpDownY3, ghUpDownHt, ghUpDownRs, ghUpDownX, ghUpDownY;
HWND ghTranslateButton, ghClearButton;
HWND ghUpDownXUD, ghUpDownYUD;

bool transFlag;
bool translateFlag;

HGLRC ghHGLRC;
HDC ghHDC;

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    
    transFlag = false;
    translateFlag = false;
    ghInstance = hThisInstance;
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (hThisInstance, MAKEINTRESOURCE(APPICON));
    wincl.hIconSm = LoadIcon (hThisInstance, MAKEINTRESOURCE(APPICON));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_WINDOW;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           WS_EX_TOPMOST ,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "phyxGraphy : Controls ",       /* Title Text */
           WS_MINIMIZEBOX | WS_VISIBLE | WS_SYSMENU, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           250,                 /* The programs width */
           500,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    ghMainWindow = hwnd;
    /* Make the window visible on the screen */
    InitAppControls(ghMainWindow, hThisInstance);
    
    ShowWindow (hwnd, nFunsterStil);
    UpdateWindow( hwnd);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
        int indexSel;
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_COMMAND:
             if(wParam == TRANSLATEBUTTON){
             //MessageBox(NULL,"Button Clicked!", "Error", MB_OK);     
             TrasnlateObject();     
             }
             else if( wParam == CLEARBUTTON){
                  transFlag = false;
                  translateFlag = false;    
                  EnableWindow(ghTranslateButton, FALSE);              
                  RenderScene();
                  SwapBuffers(ghHDC);
                  ValidateRect(hwnd, NULL);
             }
             switch(HIWORD(wParam)){
             case CBN_SELCHANGE:
                  if(OBJECTLISTBOX == (int) LOWORD(wParam)){
                  indexSel = SendMessage(ghListBoxObject, CB_GETCURSEL, 0, 0);
                           RenderScene(); 
                           if( indexSel == 0 ){
                              TriangleRender();                                                  
                           }
                           else if( indexSel == 1){
                                RectRender();                                
                           }                           
                  }
                  break;
             }
             break;
        case WM_NOTIFY:
             switch(((NMHDR FAR *) lParam)->code){
                           case UDN_DELTAPOS:                                                                                  
                           indexSel = SendMessage(ghListBoxObject, CB_GETCURSEL, 0, 0);
                           RenderScene(); 
                           if( indexSel == 0 ){
                              TriangleRender();                                                  
                           }
                           else if( indexSel == 1){
                                RectRender();                                
                           }
                           if(((NMHDR FAR *) lParam)->hwndFrom == ghUpDownXUD || ((NMHDR FAR *) lParam)->hwndFrom ==  ghUpDownYUD)                           {                           
                           //Translation Prediction Procedure.  
                           EnableWindow(ghTranslateButton, TRUE);                                                                                                          
                           TrasnlateObjectPredict();
                           }
                           break;                          
                  }//ending of switch control.
             break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
LRESULT CALLBACK OpenglWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
        RECT rect;
        int indexSel;
        GetClientRect( hwnd, &rect);
        switch(message){
        case WM_CREATE:
             ghHDC = GetDC(hwnd);
             break;
        case WM_PAINT:
             glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);
             indexSel = SendMessage(ghListBoxObject, CB_GETCURSEL, 0, 0); 
             RenderScene();
                           if( indexSel == 0 ){
                               TriangleRender();                                                             
                           }
                           else if( indexSel == 1){
                                RectRender();                                 
                           }     
             if(transFlag){
             TrasnlateObjectPredict();                           
             }         
             if(translateFlag){
             TrasnlateObject();
             }                   
             break;
        case WM_SIZE:
             glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);
             indexSel = SendMessage(ghListBoxObject, CB_GETCURSEL, 0, 0); 
             RenderScene();
                           if( indexSel == 0 ){
                               TriangleRender();                                                              
                           }
                           else if( indexSel == 1){
                                RectRender();                           
                           }    
             if(transFlag){
             TrasnlateObjectPredict();              
             }      
             if(translateFlag){
             TrasnlateObject();
             }                       
             break;
        case WM_DESTROY:
             
             wglMakeCurrent(ghHDC, NULL);
             wglDeleteContext(ghHGLRC);
             
             DestroyWindow(hwnd);
             PostQuitMessage(0);
             break;
             default:
             return DefWindowProc (hwnd, message, wParam, lParam);
        }
        return 0;
}



void __stdcall InitAppControls(HWND hWnd, HINSTANCE hInst){
     
     RECT rect;
     GetClientRect(hWnd, &rect);
     HWND hGroupBoxA, hGroupBoxB;
     HWND hStaticA,hStaticB, hStaticC,hStaticD, hStaticE, hStaticF, hStaticG, hStaticH, hStaticI, hStaticJ, hStaticK;
     
     //Loading CommonControl DLLs.
     InitCommonControls();
     //Drawing Controls over Tool Window.
     hGroupBoxA = CreateWindow("button","Coordinates", BS_GROUPBOX | WS_VISIBLE | WS_CHILD , 2, 2, rect.right-rect.left - 2, (rect.bottom - rect.top )/ 2 - 100, hWnd, (HMENU)1001, hInst, NULL);
     hGroupBoxB = CreateWindow("button","Translation Coordinates", BS_GROUPBOX | WS_VISIBLE | WS_CHILD , 2, (rect.bottom - rect.top )/ 2 - 60, rect.right-rect.left - 2, 75, hWnd, (HMENU)1002, hInst, NULL);
          
     //Lables Creation.
     hStaticK = CreateWindow("STATIC","Object:",WS_VISIBLE | WS_CHILD , 10, 150, 50, 15, hWnd, (HMENU)10, hInst, NULL);
     hStaticA = CreateWindow("STATIC","x1:",WS_VISIBLE | WS_CHILD , 10, 20, 20, 15, hWnd, (HMENU)10, hInst, NULL);
     hStaticB = CreateWindow("STATIC","y1:",WS_VISIBLE | WS_CHILD ,(rect.right-rect.left)/2 , 20, 20, 15, hWnd, (HMENU)11, hInst, NULL);
     hStaticC = CreateWindow("STATIC","x2:",WS_VISIBLE | WS_CHILD , 10, 50, 20, 15, hWnd, (HMENU)12, hInst, NULL);
     hStaticD = CreateWindow("STATIC","y2:",WS_VISIBLE | WS_CHILD , (rect.right-rect.left)/2 , 50, 20, 15, hWnd, (HMENU)13, hInst, NULL);
     hStaticE = CreateWindow("STATIC","x3:",WS_VISIBLE | WS_CHILD , 10, 80, 20, 15, hWnd, (HMENU)14, hInst, NULL);
     hStaticF = CreateWindow("STATIC","y3:",WS_VISIBLE | WS_CHILD , (rect.right-rect.left)/2 , 80, 20, 15, hWnd, (HMENU)15, hInst, NULL);
     hStaticG = CreateWindow("STATIC","Height:",WS_VISIBLE | WS_CHILD , 10, 110, 50, 20, hWnd, (HMENU)16, hInst, NULL);
     hStaticH = CreateWindow("STATIC","l/r:",WS_VISIBLE | WS_CHILD , (rect.right-rect.left)/2 , 110, 50, 15, hWnd, (HMENU)17, hInst, NULL);
     hStaticI = CreateWindow("STATIC","tx:",WS_VISIBLE | WS_CHILD , 10, 20, 20, 15, hGroupBoxB, (HMENU)18, hInst, NULL);
     hStaticJ = CreateWindow("STATIC","ty:",WS_VISIBLE | WS_CHILD , 10, 50, 20, 15, hGroupBoxB, (HMENU)19, hInst, NULL);
     
     //ListBox Creation.
     ghListBoxObject = CreateWindow("COMBOBOX","",WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_UPPERCASE, (rect.right-rect.left)/2, 145, (rect.right-rect.left)/2 - 10, 15, hWnd, (HMENU)OBJECTLISTBOX, hInst, NULL);
     SendMessage(ghListBoxObject, CB_ADDSTRING, 0, (LPARAM)(LPSTR)"Triangle");
     SendMessage(ghListBoxObject, CB_ADDSTRING, 0, (LPARAM)(LPSTR)"Rectangle");   
     SendMessage(ghListBoxObject, CB_SETCURSEL, (WPARAM) 0, 0);
     ghConsoleArea = CreateWindow("listbox", "", WS_VISIBLE| WS_CHILD|LBS_HASSTRINGS|LBS_STANDARD, 5, (rect.bottom - rect.top)/2 + 20 , (rect.right - rect.left) - 10, (rect.bottom - rect.top)/2 -50 , hWnd, (HMENU) CONSOLEAREA, hInst, NULL);
     SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)0, (LPARAM)(LPSTR)"(X-Y) Values:");
     //Creation Of Up-Down Control.
     //ghUpDownX1, ghUpDownY1, ghUpDownX2, ghUpDownY2, ghUpDownX3, ghUpDownY3, ghUpDownHt, ghUpDownRs, ghUpDownX, ghUpDownY;
     ghUpDownX1 = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY ,65, 20,50, 20, hWnd, (HMENU) EDITBOX_X1, hInst, NULL);
     HWND ghUpDownX1UD = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, 75, 20, 20, 20, hWnd, EDITBOX_X1UD, hInst, ghUpDownX1, 50, -50, 15);
     
     ghUpDownY1 = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,(rect.right-rect.left)/2 + 60, 20,50, 20, hWnd, (HMENU) EDITBOX_Y1, hInst, NULL);
     HWND ghUpDownY1UD = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, 75, 20, 20, 20, hWnd, EDITBOX_Y1UD, hInst, ghUpDownY1, 50, -50, 15);
     
     ghUpDownX2 = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,65, 48,50, 20, hWnd, (HMENU) EDITBOX_X2, hInst, NULL);
     HWND ghUpDownX2UD = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, 75, 20, 20, 20, hWnd, EDITBOX_X2UD, hInst, ghUpDownX2, 50, -50, 15);
     
     ghUpDownY2 = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,(rect.right-rect.left)/2 + 60, 48,50, 20, hWnd, (HMENU) EDITBOX_Y2, hInst, NULL);
     HWND ghUpDownY2UD = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, 75, 20, 20, 20, hWnd, EDITBOX_Y2UD, hInst, ghUpDownY2, 50, -50, 15);
     
     ghUpDownX3 = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,65, 75,50, 20, hWnd, (HMENU) EDITBOX_X3, hInst, NULL);
     HWND ghUpDownX3UD = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, 75, 20, 20, 20, hWnd, EDITBOX_X3UD, hInst, ghUpDownX3, 50, -50, 15);
     
     ghUpDownY3 = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT| ES_READONLY ,(rect.right-rect.left)/2 + 60, 75,50, 20, hWnd, (HMENU) EDITBOX_Y3, hInst, NULL);
     HWND ghUpDownY3UD = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, 75, 20, 20, 20, hWnd, EDITBOX_Y3UD, hInst, ghUpDownY3, 50, -50, 15);
     
     ghUpDownHt = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,65, 108,50, 20, hWnd, (HMENU) EDITBOX_H, hInst, NULL);
     HWND ghUpDownHtUD = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, 75, 20, 20, 20, hWnd, EDITBOX_HUD, hInst, ghUpDownHt, 50, -50, 15);
     
     ghUpDownRs = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,(rect.right-rect.left)/2 + 60, 108,50, 20, hWnd, (HMENU) EDITBOX_R, hInst, NULL);
     HWND ghUpDownRsUD = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, 75, 20, 20, 20, hWnd, EDITBOX_RUD, hInst, ghUpDownRs, 50, -50, 15);
     
     
     ghUpDownX = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,(rect.right-rect.left)/2 - 55, 195,50, 20, hWnd, (HMENU) EDITBOX_X, hInst, NULL);
     ghUpDownXUD = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, 75, 20, 20, 20, hWnd, EDITBOX_XUD, hInst, ghUpDownX, 50, -50, 15);
     
     ghUpDownY = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,(rect.right-rect.left)/2 - 55, 220,50, 20, hWnd, (HMENU) EDITBOX_Y, hInst, NULL);
     ghUpDownYUD = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, 75, 20, 20, 20, hWnd, EDITBOX_YUD, hInst, ghUpDownY, 50, -50, 15);
     
     ghTranslateButton =  CreateWindow("button", "Translate", WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON, (rect.right-rect.left)/2 + 5, 200, 100, 45, hWnd, (HMENU)TRANSLATEBUTTON, hInst, NULL);
     EnableWindow(ghTranslateButton, FALSE);
     ghClearButton = CreateWindow("button", "Clear", WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON, (rect.right-rect.left)/2 + 5, rect.bottom - 35, 115, 25, hWnd, (HMENU)CLEARBUTTON, hInst, NULL);
     WNDCLASSEX glWnd;
     
    glWnd.hInstance = ghInstance;
    glWnd.lpszClassName = "glWindow";
    glWnd.lpfnWndProc = OpenglWindowProcedure;      /* This function is called by windows */
    glWnd.style = 0;                 /* Catch double-clicks */
    glWnd.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    glWnd.hIcon = LoadIcon (ghInstance, MAKEINTRESOURCE(APPICON));
    glWnd.hIconSm = LoadIcon (ghInstance, MAKEINTRESOURCE(APPICON));
    glWnd.hCursor = LoadCursor (NULL, IDC_ARROW);
    glWnd.lpszMenuName = NULL;                 /* No menu */
    glWnd.cbClsExtra = 0;                      /* No extra bytes after the window class */
    glWnd.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    glWnd.hbrBackground = (HBRUSH) COLOR_WINDOW;
     
     if (!RegisterClassEx (&glWnd)){
        MessageBox(NULL,"Unable to Register Window Class", "Error", MB_OK);
        }
        

    /* The class is registered, let's create the program*/
    ghGlWindow = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           "glWindow",         /* Classname */
           "phyxGraphy-Revised : Scene Window : Annim Banerjee",       /* Title Text */
           WS_OVERLAPPEDWINDOW | WS_VISIBLE , /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           650,                 /* The programs width */
           650,                 /* and height in pixels */
           NULL,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           ghInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
           
    if(ghGlWindow == NULL){
    MessageBox(NULL,"Unable to Create Window", "Error", MB_OK);
    }
    
    InitOpenGLEnvironment(ghHDC);
    RenderScene();
    SwapBuffers (ghHDC);    
    ValidateRect(ghGlWindow, NULL);
}

void __stdcall InitOpenGLEnvironment( HDC hdc){
     PIXELFORMATDESCRIPTOR ppfd;
   
     ZeroMemory (&ppfd, sizeof (ppfd));
     ppfd.nSize = sizeof (ppfd);
     ppfd.nVersion = 1;
     ppfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
     ppfd.iPixelType = PFD_TYPE_RGBA;
     ppfd.cColorBits = 24;
     ppfd.cDepthBits = 32;
     ppfd.iLayerType = PFD_MAIN_PLANE;
     int nPixelFormat = ChoosePixelFormat(hdc, &ppfd);
     SetPixelFormat(hdc, nPixelFormat, &ppfd);
     
     //create Rendering Context.
     ghHGLRC = wglCreateContext(hdc);
     wglMakeCurrent(hdc, ghHGLRC);
}

void __stdcall RenderScene(){
     RECT rcClient;  
     glEnable(GL_LINE_SMOOTH);
     glPointSize(1.5);
     GetClientRect(ghGlWindow, &rcClient); 
     glClearColor (0.0f, 0.0f, 0.0f, 1.0f);     
     glClear (GL_COLOR_BUFFER_BIT);
     glColor3f (1.0f, 1.0f, 1.0f);  
      
     
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();   
     gluOrtho2D(0, 100,0, 100 );// organize the Coordinate system...
     
     glBegin(GL_QUADS);
     glColor3f (0.0f, 0.0f, 0.0f);  
     glVertex2i (0, 0);  
     glColor3f (0.330f, 0.10f, 0.20f);  
     glVertex2i (100, 0);
     glColor3f (0.0f, 0.0f, 0.0f);
     glVertex2i (100, 100);
     glVertex2i (0, 100);
     glEnd(); 
       
     char sz[100];
     wsprintf(sz, "phyxGraphy-Revised : Scene Window : Annim Banerjee. (c)2012 : Window Width: %d \tWindow Height: %d",(rcClient.right - rcClient.left), rcClient.bottom - rcClient.top);
     SendMessage(ghGlWindow, WM_SETTEXT,  0 , (LPARAM)(LPSTR)sz);
     glColor3f (1.0f, 1.0f, 1.0f);  
     glBegin (GL_LINES);
     glVertex2i (0, 50);
     glVertex2i (100, 50);
     glVertex2i (50, 0);
     glVertex2i (50, 100);
     glEnd ();        
}

void __stdcall TriangleRender(){
    
     int x1, y1, x2, y2, x3, y3;
     
     char _x1[10],_y1[10], _x2[10], _y2[10], _x3[10], _y3[10];
     
     SendMessage(ghUpDownX1, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x1);                                
     SendMessage(ghUpDownY1, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y1); 
     SendMessage(ghUpDownX2, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x2); 
     SendMessage(ghUpDownY2, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y2);                                                                                                
     SendMessage(ghUpDownX3, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x3); 
     SendMessage(ghUpDownY3, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y3); 
     x1 = (int)atof(_x1);
     y1 = (int) atof(_y1);
     x2 = (int)atof(_x2);
     y2 = (int)atof(_y2); 
     x3 = (int)atof(_x3);
     y3 = (int)atof(_y3); 
     glEnable(GL_LINE_SMOOTH);
     glEnable( GL_POLYGON_SMOOTH);
     glEnable(GL_BLEND);
     glBegin(GL_TRIANGLES);
     glColor3f(0.5f, 0.2f, 0.3f);
     glVertex2i(50 + x1, 50 + y1);
     glColor3f(0.4f, 0.2f, 0.3f);
     glVertex2i( 50 + x2,50 + y2);
     glColor3f(0.7f, 0.6f, 0.3f);
     glVertex2i(50 + x3, 50 + y3);     
     glEnd();  
     SwapBuffers (ghHDC);     
     ValidateRect(ghGlWindow, NULL);                        
}

void __stdcall RectRender(){
     
     int x1, y1, h, l;
     
     char _x1[10],_y1[10], _h[10], _l[10];
     
     SendMessage(ghUpDownX1, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x1);                                
     SendMessage(ghUpDownY1, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y1); 
     SendMessage(ghUpDownHt, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_h); 
     SendMessage(ghUpDownRs, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_l);                                                                                                
  
     x1 = (int)atof(_x1);
     y1 = (int) atof(_y1);
     h = (int)atof(_h);
     l = (int)atof(_l); 
     
     
     glBegin(GL_QUADS);
     glColor3f(0.40456456f, 0.90456f, 0.90456456f);
     glVertex2i(50 + x1, 50 + y1);
     //glColor3f(0.0456456f, 0.0456f, 0.0456456f);
     glVertex2i(50 + x1 + l, 50 + y1);
     glColor3f(0.202134f, 0.0545f, 0.045f);
     glVertex2i(50 + x1 + l, 50 + y1 + h);
     glColor3f(0.5f, 0.2f, 0.5f);
     glVertex2i(50 + x1, 50 + y1 + h);
     glEnd();
     
     SwapBuffers (ghHDC);     
     ValidateRect(ghGlWindow, NULL); 
}

void __stdcall TrasnlateObjectPredict(){
     
     int indxSelection;
     int x1, y1, x2, y2, x3, y3, h, l, x, y;     
     char _x1[10],_y1[10], _x2[10], _y2[10], _x3[10], _y3[10], _x[10], _y[10], _h[10], _l[10];
     transFlag = true;
     indxSelection = SendMessage(ghListBoxObject, CB_GETCURSEL, 0, 0);
     SendMessage(ghUpDownX1, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x1);                                
     SendMessage(ghUpDownY1, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y1); 
     SendMessage(ghUpDownX2, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x2); 
     SendMessage(ghUpDownY2, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y2);                                                                                                
     SendMessage(ghUpDownX3, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x3); 
     SendMessage(ghUpDownY3, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y3); 
     SendMessage(ghUpDownHt, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_h); 
     SendMessage(ghUpDownRs, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_l); 
     
     SendMessage(ghUpDownX, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x); 
     SendMessage(ghUpDownY, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y); 
     x1 = (int)atof(_x1);
     y1 = (int) atof(_y1);
     x2 = (int)atof(_x2);
     y2 = (int)atof(_y2); 
     x3 = (int)atof(_x3);
     y3 = (int)atof(_y3); 
     h = (int)atof(_h);
     l = (int)atof(_l);     
     
     x = (int)atof(_x);
     y = (int)atof(_y);
     switch(indxSelection){
     case 0:  
          //case for drawing Triangle...
     RenderScene();     
     TriangleRender();                                 
     
     glEnable(GL_LINE_SMOOTH);
     glEnable( GL_POLYGON_SMOOTH);
     glEnable(GL_BLEND);
     glEnable(GL_LINE_STIPPLE);
     glLineStipple(1, 0x00FF);
     glLineWidth(2.0);
     glColor3f(1.0f, 1.0f, 1.0f);
     
     glBegin(GL_LINE_STRIP);
     glVertex2i(50 + x1, 50 + y1);
     glVertex2i(50 + x + x1, 50 + y + y1);
     glEnd();
     
     glBegin(GL_LINE_STRIP);
     glVertex2i(50 + x2, 50 + y2);
     glVertex2i(50 + x2 + x, 50 + y2 +y);
     glEnd();

     glBegin(GL_LINE_STRIP);
     glVertex2i(50 + x3, 50 + y3);
     glVertex2i(50 + x + x3, 50 + y + y3);
     glEnd();
     glDisable(GL_LINE_STIPPLE);
          break;
     case 1:
          //case for Drawing Rectangle.
          RenderScene();
          RectRender();
          
     glEnable(GL_LINE_SMOOTH);
     glEnable( GL_POLYGON_SMOOTH);
     glEnable(GL_BLEND);
     glEnable(GL_LINE_STIPPLE);
     glLineStipple(1, 0x00FF);
     glLineWidth(2.0);
     glColor3f(1.0f, 1.0f, 1.0f);
     
     glBegin(GL_LINE_STRIP);
     glVertex2i(50 + x1, 50 + y1);
     glVertex2i(50 + x + x1, 50 + y + y1);
     glEnd();
     
     glBegin(GL_LINE_STRIP);
     glVertex2i(50 + l + x1, 50 + y1);
     glVertex2i(50 + x + l + x1, 50 + y + y1);
     glEnd();

     glBegin(GL_LINE_STRIP);
     glVertex2i(50 + x1 + l, 50 + h + y1);
     glVertex2i(50 + x + x1 + l, 50 + y + h + y1);
     glEnd();
     
     glBegin(GL_LINE_STRIP);
     glVertex2i(50 + x1, 50 + h+ y1);
     glVertex2i(50 + x + x1, 50 + y + h+ y1);
     glEnd();
     
     glDisable(GL_LINE_STIPPLE);
          
          break;
     }
     
     SwapBuffers (ghHDC);     
     ValidateRect(ghGlWindow, NULL); 
}
void __stdcall TrasnlateObject(){
     int indx, nCount;
     translateFlag = true;
     indx = SendMessage(ghListBoxObject, CB_GETCURSEL, 0, 0);
     int x1, y1, x2, y2, x3, y3, h, l, x, y, tx, ty;    
     char _x1[10],_y1[10], _x2[10], _y2[10], _x3[10], _y3[10], _x[10], _y[10], _h[10], _l[10];
     char tChar[50];
   
     SendMessage(ghUpDownX1, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x1);                                
     SendMessage(ghUpDownY1, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y1); 
     SendMessage(ghUpDownX2, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x2); 
     SendMessage(ghUpDownY2, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y2);                                                                                                
     SendMessage(ghUpDownX3, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x3); 
     SendMessage(ghUpDownY3, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y3); 
     SendMessage(ghUpDownHt, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_h); 
     SendMessage(ghUpDownRs, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_l); 
     
     SendMessage(ghUpDownX, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_x); 
     SendMessage(ghUpDownY, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)_y); 
     
     x1 = (int)atof(_x1);
     y1 = (int) atof(_y1);
     x2 = (int)atof(_x2);
     y2 = (int)atof(_y2); 
     x3 = (int)atof(_x3);
     y3 = (int)atof(_y3); 
     h = (int)atof(_h);
     l = (int)atof(_l);     
     
     x = (int)atof(_x);
     y = (int)atof(_y);
     
     if(!transFlag){
                    //MessageBox(ghMainWindow, "Select The Destination Coordinate first.", "Prompt", MB_OK| MB_ICONINFORMATION);
                    return;
     }
     else if(transFlag){
          switch(indx){
          case 0:
               //Triangle Tralation condition.
               nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
               wsprintf(tChar, "Previous Data:");
               SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                wsprintf(tChar, "x:%d\t y: %d", x1, y1);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                wsprintf(tChar, "x:%d\t y: %d", x2, y2);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                wsprintf(tChar, "x:%d\t y: %d", x3, y3);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount , (LPARAM)(LPSTR)tChar);
               x1 = x1 + x;
               y1 = y1 + y;
               x2 = x2 + x;
               y2 = y2 + y;
               x3 = x3 + x;
               y3 = y3 + y;                         
               
               
               //  ghConsoleArea
                 nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
               wsprintf(tChar, "Current Data:");
               SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                wsprintf(tChar, "x:%d\t y: %d", x1, y1);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                wsprintf(tChar, "x:%d\t y: %d", x2, y2);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                wsprintf(tChar, "x:%d\t y: %d", x3, y3);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount , (LPARAM)(LPSTR)tChar);
                RenderScene();
                glEnable(GL_LINE_SMOOTH);
                glEnable( GL_POLYGON_SMOOTH);
                glEnable(GL_BLEND);
                glBegin(GL_TRIANGLES);
                glColor3f(0.5f, 0.2f, 0.3f);
                glVertex2i(50 + x1, 50 + y1);
                glColor3f(0.4f, 0.2f, 0.3f);
                glVertex2i( 50 + x2, 50 + y2);
                glColor3f(0.7f, 0.6f, 0.3f);
                glVertex2i( 50 + x3,  50 + y3);     
                glEnd();  
                SwapBuffers (ghHDC);     
                ValidateRect(ghGlWindow, NULL);
                TrasnlateObjectPredict();
          
          break;
          case 1:
               //Rectangle Drawing Condition.
               nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
               wsprintf(tChar, "Previous Data:");
               SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                wsprintf(tChar, "x:%d\t y: %d", x1, y1);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                wsprintf(tChar, "x:%d\t y: %d", x2, y2);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                wsprintf(tChar, "x:%d\t y: %d", x3, y3);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount , (LPARAM)(LPSTR)tChar);
               //x1 = x1 + x;
               //y1 = y1 + y;                                     
               
                 nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
               wsprintf(tChar, "Current Data:");
               SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                wsprintf(tChar, "x:%d\t y: %d", x1, y1);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                wsprintf(tChar, "x:%d\t y: %d", x2, y2);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount, (LPARAM)(LPSTR)tChar);
                wsprintf(tChar, "x:%d\t y: %d", x3, y3);
                nCount = SendMessage(ghConsoleArea, LB_GETCOUNT, 0, 0);
                SendMessage(ghConsoleArea, LB_INSERTSTRING, (WPARAM)nCount , (LPARAM)(LPSTR)tChar);
                RenderScene();
                glEnable(GL_LINE_SMOOTH);
                glEnable( GL_POLYGON_SMOOTH);
                glEnable(GL_BLEND);
                //glBegin(GL_QUADS);
//                             glColor3f(0.40456456f, 0.90456f, 0.90456456f);
//                             glVertex2i( 50 + x1, 50 + y1);     
//                             glVertex2i(50 + x1 + l, 50 + y1);
//                             glColor3f(0.202134f, 0.0545f, 0.045f);
//                             glVertex2i(50 + x1 + l, 50 + y1 + h);
//                             glColor3f(0.5f, 0.2f, 0.5f);
//                             glVertex2i(50 + x1, 50 + y1 + h);    
//                glEnd();  
                  glTranslatef(x , y, 0);
                  glColor3f(0.5f, 0.2f, 0.5f);
                  glRecti(x1 + 50, y1 + 50,x1 + 50 + l,y1 + 50 + h);
                SwapBuffers (ghHDC);     
                ValidateRect(ghGlWindow, NULL);
                TrasnlateObjectPredict();
          break;
          }
     }
}
