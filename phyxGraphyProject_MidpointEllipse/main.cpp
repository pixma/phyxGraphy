
#include "genericHeader.h"
#include "resource.h"

HWND gMainWindowHandle, gTabControlHandle, gHandleStatusBar;
HWND gLeftSplitterWindow, gRightSplitterWindow;
HWND gBuddyEditAX, gUDControlAX, gBuddyEditAY, gUDControlAY;
HWND gBuddyEditBX, gUDControlBX , gBuddyEditBY, gUDControlBY;
HWND gGroupBox, gGroupBoxCoordinateA, gGroupBoxCoordinateB;
HWND hWndStaticA, hWndStaticB, hWndStaticC, hWndStaticD;
//HWND ghConsoleTextArea;
HWND hListBoxAxisX, hListBoxAxisY;
bool paintFlag;
MSG msg;
HINSTANCE gInstance;
HDC gLeftHdc;
HGLRC ghGLRC;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow){
    
    WNDCLASS wcex;
    paintFlag = false;
    HICON hIconSm = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_APPICON));
    if(hIconSm == NULL){
    MessageBox(NULL, TEXT("Failed to Load Icon"), TEXT("phyxGraphy: Error"), MB_ICONERROR|MB_OK);
    PostQuitMessage(0);
    }
    //Filling of windowclass structure.
    //ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    gInstance = hInstance;
    wcex.style   =             CS_HREDRAW|CS_VREDRAW;
    wcex.lpfnWndProc =         (WNDPROC)WindowProcedure;
    wcex.cbClsExtra  =         0;
    wcex.cbWndExtra  =         0;
    wcex.hInstance   =         hInstance;
    wcex.hIcon       =         LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPICON));
    wcex.hCursor     =         LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground=        (HBRUSH)COLOR_WINDOW ;
    wcex.lpszMenuName=         "APPLICATIONMENU";
    wcex.lpszClassName=        TEXT("phyxgraphywindow");  
    
    if(!RegisterClass(&wcex)){
    MessageBox(NULL, TEXT("Failed to Register Class"), TEXT("phyxGraphy: Error"), MB_ICONERROR|MB_OK);
    PostQuitMessage(0);
    }
    InitCommonControls();//Load The DLL for Common Controls.
    gMainWindowHandle = CreateWindow(TEXT("phyxgraphywindow"), TEXT("phyxGraphy : Ellipse Drawing: Midpoint Alogorithm"), WS_VISIBLE|WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 750, 550, NULL, NULL, hInstance, NULL);
    
    if(gMainWindowHandle == NULL){
    MessageBox(NULL, TEXT("Failed to Create Window"), TEXT("phyxGraphy: Error"), MB_ICONERROR|MB_OK);
    PostQuitMessage(0);
    }
    SendMessage(gMainWindowHandle, WM_SETICON ,(WPARAM)ICON_SMALL ,(LPARAM)(HICON) hIconSm);
    InitializeWindowComponents(gMainWindowHandle);
    RenderScene();
    ShowWindow(gMainWindowHandle, SW_NORMAL);
    UpdateWindow(gMainWindowHandle);
    
    
    while(GetMessage( &msg, NULL, 0, 0) > 0){
                      TranslateMessage( &msg );
                      DispatchMessage( &msg );
    }
    return 0;
}//end of WinMain Function.


LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        
        switch(msg){
        case WM_COMMAND:
             switch(wParam){
                            case IDM_APPEXIT:
                                 DestroyWindow(hWnd);
                                 PostQuitMessage(0);
                                 break;
                            case IDM_CLEAR:
                                 glClearColor (0.0f, 0.0f, 0.0f, 0.0f);     
                                 glClear (GL_COLOR_BUFFER_BIT);
                                 RenderScene();
                                 break;
                            case IDM_APPABOUT:
                                 MessageBox(hWnd, TEXT("Application created by Annim Banerjee."), TEXT("About Us : phyxGraphy"), MB_OK | MB_ICONINFORMATION);
                                 break;
             }//closing of switch here.
             break;         
        case WM_CREATE:
             break;
        case WM_DESTROY:
             DestroyWindow(hWnd);
             PostQuitMessage(0);
             break;
        case WM_PAINT:
             break;
        case WM_SIZE:
             RECT rcRect;
             GetClientRect(gMainWindowHandle, &rcRect);
             MoveWindow(gHandleStatusBar, 0,0,0,0, TRUE);
             MoveWindow(gLeftSplitterWindow, 5, rcRect.top + 5, (rcRect.right - rcRect.left)/2, (rcRect.bottom - rcRect.top)/2 -32, TRUE);
             MoveWindow(gRightSplitterWindow, (rcRect.right - rcRect.left)/2 + 10, rcRect.top + 5, (rcRect.right - rcRect.left)/2 - 15, (rcRect.bottom - rcRect.top)/2 -32, TRUE);
           //  MoveWindow(ghConsoleTextArea, 5, (rcRect.bottom - rcRect.top)/2 - 20, (rcRect.right - rcRect.left) - 10, (rcRect.bottom - rcRect.top)/2 -10, TRUE);             
             MoveWindow(hListBoxAxisX,5, (rcRect.bottom - rcRect.top)/2 - 25, (rcRect.right - rcRect.left)/2, (rcRect.bottom - rcRect.top)/2 ,TRUE);
             MoveWindow(hListBoxAxisY, (rcRect.right - rcRect.left)/2 + 10, (rcRect.bottom - rcRect.top)/2 - 25, (rcRect.right - rcRect.left)/2 - 15, (rcRect.bottom - rcRect.top)/2  ,TRUE);
             break;
             case WM_NOTIFY:

                  break;
        default:

                break;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT CALLBACK LeftWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        PAINTSTRUCT ps;
        RECT rcClient;
        switch(msg){
        case WM_COMMAND:
             
             break;         
        case WM_CREATE:
             gLeftHdc = GetDC(hWnd);
             break;
        case WM_DESTROY:
             wglMakeCurrent(gLeftHdc, NULL);
             wglDeleteContext(ghGLRC);
             break;
        case WM_PAINT:             
             glViewport(0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
             RenderScene();      
             if(paintFlag){
              int _x0, _y0, _x1, _y1;
             char x0[10],y0[10], x1[10], y1[10];
             SendMessage(gBuddyEditAX, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)x0);                                
             SendMessage(gBuddyEditAY, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)y0); 
             SendMessage(gBuddyEditBX, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)x1); 
             SendMessage(gBuddyEditBY, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)y1);                                                                                                
             _x0 = (int)atof(x0);
             _y0 = (int) atof(y0);
             _x1 = (int)atof(x1);
             _y1 = (int)atof(y1);                               
             MidpointEllipse(_x0, _y0, _x1, _y1);  
             }      
             break;
        case WM_SIZE:
             GetClientRect(hWnd, &rcClient);
             glViewport(0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);              
             break;
        default:

                break;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK RightWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        RECT rRect;
         switch(msg){
        case WM_COMMAND:
             
             break;         
        case WM_CREATE:
             break;
        case WM_DESTROY:
            
             break;
        case WM_PAINT:            
             break;
        case WM_SIZE:
             //gGroupBox
             GetClientRect(hWnd, &rRect);
             MoveWindow(gGroupBox, 2, 2,rRect.right - rRect.left - 5, rRect.bottom - rRect.top - 5, TRUE);
             MoveWindow(gGroupBoxCoordinateA, 10, 25, rRect.right - rRect.left - 25, (rRect.bottom - rRect.top)/2 - 25, TRUE);
             MoveWindow(gGroupBoxCoordinateB, 10, (rRect.bottom - rRect.top)/2 , rRect.right - rRect.left - 25, (rRect.bottom - rRect.top)/2 - 15, TRUE);

             GetClientRect(hWnd, &rRect);
             MoveWindow(gBuddyEditAX, (rRect.right - rRect.left)/2+100, 45, 20, 20, TRUE);
             MoveWindow(gUDControlAX, (rRect.right - rRect.left)/2 + 120, 45, 20, 20, TRUE);
             MoveWindow(gBuddyEditAY, (rRect.right - rRect.left)/2+100, 85, 20, 20, TRUE);
             MoveWindow(gUDControlAY, (rRect.right - rRect.left)/2 + 120, 85, 20, 20, TRUE);
             
             GetClientRect(hWnd, &rRect);
             MoveWindow(gBuddyEditBX, (rRect.right - rRect.left)/2+100, (rRect.bottom - rRect.top)/2 + 25, 20, 20, TRUE);
             MoveWindow(gUDControlBX, (rRect.right - rRect.left)/2 + 120, (rRect.bottom - rRect.top)/2 +25, 20, 20, TRUE);
             MoveWindow(gBuddyEditBY, (rRect.right - rRect.left)/2+100, (rRect.bottom - rRect.top)/2 + 65 , 20, 20, TRUE);
             MoveWindow(gUDControlBY, (rRect.right - rRect.left)/2 + 120, (rRect.bottom - rRect.top)/2 +65, 20, 20, TRUE);
             
             MoveWindow(hWndStaticA, 50, 48, 200, 20, TRUE);
             MoveWindow(hWndStaticB, 50, 88, 200, 20, TRUE);
             MoveWindow(hWndStaticC, 50, (rRect.bottom - rRect.top)/2 +28, 200, 20, TRUE);
             MoveWindow(hWndStaticD, 50, (rRect.bottom - rRect.top)/2 +68, 200, 20, TRUE);
                          

             break;
             case WM_NOTIFY:
                  switch(((NMHDR FAR *) lParam)->code){
                           case UDN_DELTAPOS:
                                paintFlag = true;
                                char x0[10],y0[10], x1[10], y1[10];
                                int _x0, _y0, _x1, _y1;
                                SendMessage(gBuddyEditAX, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)x0);                                
                                SendMessage(gBuddyEditAY, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)y0); 
                                SendMessage(gBuddyEditBX, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)x1); 
                                SendMessage(gBuddyEditBY, WM_GETTEXT, (WPARAM)10, (LPARAM)(LPCSTR)y1);                                                                                                
                                _x0 = (int)atof(x0);
                                _y0 = (int)atof(y0);
                                _x1 = (int)atof(x1);
                                _y1 = (int)atof(y1);                               
                                SwapBuffers(gLeftHdc);
                                RenderScene();
                                glClearColor (0.0f, 0.0f, 0.0f, 0.0f);     
                                glClear (GL_COLOR_BUFFER_BIT);
                                glColor3f (1.0f, 1.0f, 1.0f);       
                                MidpointEllipse(_x0 , _y0 , _x1, _y1 );                 
                                break;
                  }//ending of switch control.
                  break;
        default:

                break;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
}

void __stdcall InitializeWindowComponents(HWND hWindow){
     RECT rcClient;     
     GetClientRect(hWindow, &rcClient);                  
     WNDCLASS wc;
    //Filling of windowclass structure.
 
     wc.style   =             CS_HREDRAW|CS_VREDRAW;
     wc.lpfnWndProc =         (WNDPROC)LeftWindowProc;
     wc.cbClsExtra  =         0;
     wc.cbWndExtra  =         0;
     wc.hInstance   =         gInstance;
     wc.hIcon       =         NULL;
     wc.hCursor     =         LoadCursor(NULL, IDC_ARROW);
     wc.hbrBackground=        (HBRUSH)COLOR_WINDOW;
     wc.lpszMenuName=         NULL;
     wc.lpszClassName=        TEXT("leftWindowPane");  
     
     RegisterClass(&wc);
    
     gHandleStatusBar = CreateStatusWindow(WS_CHILD|WS_VISIBLE|SBARS_SIZEGRIP, TEXT("Application Ready."), hWindow, STATUSCONTROL);
  //   ghConsoleTextArea = CreateWindow("EDIT","", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY, 5, (rcClient.bottom - rcClient.top)/2 - 20, (rcClient.right - rcClient.left) - 10,(rcClient.bottom - rcClient.top)/2 -10, hWindow, (HMENU) EDITCONSOLE, gInstance, NULL);
     hListBoxAxisX = CreateWindow("listbox", "", WS_VISIBLE| WS_CHILD|LBS_HASSTRINGS|LBS_STANDARD, 5, (rcClient.bottom - rcClient.top)/2 - 25, (rcClient.right - rcClient.left)/2, (rcClient.bottom - rcClient.top)/2 , hWindow, (HMENU) LB_AXIX_X, gInstance, NULL);
     hListBoxAxisY = CreateWindow("listbox", "", WS_VISIBLE| WS_CHILD|LBS_HASSTRINGS|LBS_STANDARD, (rcClient.right - rcClient.left)/2 + 10, (rcClient.bottom - rcClient.top)/2 - 25, (rcClient.right - rcClient.left)/2 - 15, (rcClient.bottom - rcClient.top)/2 , hWindow, (HMENU) LB_AXIX_Y, gInstance, NULL);
     SendMessage(hListBoxAxisX, LB_INSERTSTRING, 0, (LPARAM)(LPSTR)"X - Axix Values:");
     SendMessage(hListBoxAxisY, LB_INSERTSTRING, 0, (LPARAM)(LPSTR)"Y - Axix Values:");
     gLeftSplitterWindow = CreateWindow("leftWindowPane","", WS_CHILD | WS_VISIBLE | WS_BORDER,5, rcClient.top + 5, (rcClient.right - rcClient.left)/2, (rcClient.bottom - rcClient.top)/2 -32, hWindow, (HMENU) 101, gInstance, NULL);
     
     wc.lpfnWndProc =         (WNDPROC)RightWindowProc;
     wc.lpszClassName=        TEXT("rightWindowPane");
     RegisterClass(&wc);
     
     gRightSplitterWindow = CreateWindow("rightWindowPane","", WS_CHILD | WS_VISIBLE | WS_BORDER,(rcClient.right - rcClient.left)/2 + 10, rcClient.top + 5, (rcClient.right - rcClient.left)/2 - 15, (rcClient.bottom - rcClient.top)/2 -32, hWindow, (HMENU) 102, gInstance, NULL);     
     GetClientRect(gRightSplitterWindow, &rcClient);
     gGroupBox = CreateWindow("button","Controls: Circle Drawing Center Coordinates", WS_CHILD| WS_VISIBLE| BS_GROUPBOX, 2, 2, rcClient.right - rcClient.left - 5, rcClient.bottom - rcClient.top - 5, gRightSplitterWindow, (HMENU)103, gInstance, NULL);
     gGroupBoxCoordinateA = CreateWindow("button","First Coordinate", WS_CHILD| WS_VISIBLE| BS_GROUPBOX , 10, 25, rcClient.right - rcClient.left - 25, (rcClient.bottom - rcClient.top)/2 - 25, gGroupBox, (HMENU)104, gInstance, NULL);
     gGroupBoxCoordinateB = CreateWindow("button","Values", WS_CHILD| WS_VISIBLE| BS_GROUPBOX , 10, (rcClient.bottom - rcClient.top)/2 , rcClient.right - rcClient.left - 25, (rcClient.bottom - rcClient.top)/2 - 15, gGroupBox, (HMENU)105, gInstance, NULL);
     
     GetClientRect(gRightSplitterWindow, &rcClient);
     gBuddyEditAX = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,(rcClient.right - rcClient.left)/2+100, 45, 60, 20, gRightSplitterWindow, (HMENU) 106, gInstance, NULL);
     gUDControlAX = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, (rcClient.right - rcClient.left)/2+165, 25, 20, 20, gRightSplitterWindow, UPDOWNCONTROLCOORDINATEAX, gInstance, gBuddyEditAX, 50, -50, 0);
     gBuddyEditAY = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,(rcClient.right - rcClient.left)/2+100, 85, 60, 20, gRightSplitterWindow, (HMENU) 107, gInstance, NULL); 
     gUDControlAY = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, (rcClient.right - rcClient.left)/2+75, 65, 20, 20, gRightSplitterWindow, UPDOWNCONTROLCOORDINATEAY, gInstance, gBuddyEditAY, 50, -50, 0);
     
   
     gBuddyEditBX = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,(rcClient.right - rcClient.left)/2+100, (rcClient.bottom - rcClient.top)/2 + 25,60, 20, gRightSplitterWindow, (HMENU) 108, gInstance, NULL);
     gUDControlBX = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, (rcClient.right - rcClient.left)/2+75, 65, 20, 20, gRightSplitterWindow, UPDOWNCONTROLCOORDINATEBX, gInstance, gBuddyEditBX, 50, 0, 15);
     gBuddyEditBY = CreateWindow("EDIT", "", WS_CHILD|WS_VISIBLE|ES_LEFT | ES_READONLY,(rcClient.right - rcClient.left)/2+100,(rcClient.bottom - rcClient.top)/2 + 65, 60, 20, gRightSplitterWindow, (HMENU) 109, gInstance, NULL);
     gUDControlBY = CreateUpDownControl(WS_VISIBLE| WS_CHILD| WS_BORDER| UDS_ALIGNRIGHT|UDS_AUTOBUDDY|UDS_SETBUDDYINT| UDS_ARROWKEYS, (rcClient.right - rcClient.left)/2+75, 65, 20, 20, gRightSplitterWindow, UPDOWNCONTROLCOORDINATEBY, gInstance, gBuddyEditBY, 50, 0, 20);
     
     hWndStaticA = CreateWindow("STATIC", "Value of Coordinate x:", WS_CHILD|WS_VISIBLE|SS_LEFT,50,  48, 200, 20, gRightSplitterWindow, (HMENU) 110, gInstance, NULL);
     hWndStaticB = CreateWindow("STATIC", "Value of Coordinate y:", WS_CHILD|WS_VISIBLE|SS_LEFT,50,  88, 200, 20, gRightSplitterWindow, (HMENU) 111, gInstance, NULL);
     
     hWndStaticC = CreateWindow("STATIC", "Value of Radius (x):", WS_CHILD|WS_VISIBLE|SS_LEFT,50,  (rcClient.bottom - rcClient.top)/2 +28, 200, 20, gRightSplitterWindow, (HMENU) 110, gInstance, NULL);
     hWndStaticD = CreateWindow("STATIC", "Value of Radius (y):", WS_CHILD|WS_VISIBLE|SS_LEFT,50,  (rcClient.bottom - rcClient.top)/2 +68, 200, 20, gRightSplitterWindow, (HMENU) 111, gInstance, NULL);
     
     InitOpenGLEnvironment(gLeftHdc);
     RenderScene();
     

     ValidateRect(gLeftSplitterWindow, NULL);
}
void __stdcall RenderScene(){
     RECT rcClient;  
   
     glPointSize(1.5);
     GetClientRect(gLeftSplitterWindow, &rcClient); 
     glClearColor (0.0f, 0.0f, 0.0f, 1.0f);     
     glClear (GL_COLOR_BUFFER_BIT);

     glColor3f (1.0f, 1.0f, 1.0f);  
     int indx;       
     
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();   
     gluOrtho2D(0, 100,0, 100 );// organize the Coordinate system...
     
     glBegin(GL_QUADS);
     glColor3f (0.0f, 0.0f, 0.0f);  
     glVertex2i (0, 0);  
     glColor3f (0.15f, 0.445f, 0.640f);  
     glVertex2i (100, 0);
     glColor3f (0.065f, 0.545f, 0.60f);
     glVertex2i (100, 100);
     glVertex2i (0, 100);
     glEnd(); 
       
     char sz[50];
     wsprintf(sz, "Window Width: %d \tWindow Height: %d",(rcClient.right - rcClient.left), rcClient.bottom - rcClient.top);
     SendMessage(gHandleStatusBar, WM_SETTEXT,  0 , (LPARAM)(LPSTR)sz);
     glColor3f (1.0f, 1.0f, 1.0f);  
     glBegin (GL_LINES);
     glVertex2i (0, 50);
     glVertex2i (100, 50);
     glVertex2i (50, 0);
     glVertex2i (50, 100);
     glEnd ();  
    // for(float i =0.0; i<5.0;i = i+0.2){   
//      glClearColor (1.0f, 0.0f, 0.0f, 0.0f); 
//     glBegin( GL_POINTS);
//     glVertex2f(-0.8f + i, 0.34f);
//     glEnd(); 
//     }
      
    
     //glPopMatrix();
     SwapBuffers (gLeftHdc);
     ValidateRect(gLeftSplitterWindow, NULL);
}
void __stdcall InitOpenGLEnvironment(HDC hdc){
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
     ghGLRC = wglCreateContext(hdc);
     wglMakeCurrent(hdc, ghGLRC);
}

void __stdcall MidpointEllipse(int x0, int y0, int rx, int ry){
     
  
     int rx2 = rx * rx;
     int ry2 = ry * ry;
     int tworx2 = rx2 * 2, twory2 = ry2 * 2;
     int p, x = 0, y = ry;
     int  px = 0, py = tworx2 * y;
     
     SendMessage(hListBoxAxisX, LB_RESETCONTENT, 0, 0);
     SendMessage(hListBoxAxisY, LB_RESETCONTENT, 0, 0);
     SendMessage(hListBoxAxisX, LB_INSERTSTRING, 0, (LPARAM)(LPSTR)"(X, Y) - Coordinate Values:");
     SendMessage(hListBoxAxisY, LB_INSERTSTRING, 0, (LPARAM)(LPSTR)"Center (xc, yc) - Values:");
     RenderScene();   
     
     glPointSize(5.0);
     glBegin(GL_POINTS);
     glVertex2i(50 + x0, 50 + y0);
     glEnd();
     
     glPointSize(2.0);
     
     plotEllipsePoint(x0, y0, x, y);//Plotting of initiale Point.
    
    //Region 1
    p = ceil(ry2 - (rx2 * ry) + (0.25 * rx2));
    while(px < py){
             x++;
             px += twory2;
             if( p <0)
                 p += ry2 + px;
             else{
                 y--;
                 py -= tworx2;
                 p += ry2 + px - py;
                 }
                 insertAtListBox(x0, y0, x, y);
             plotEllipsePoint(x0 , y0, x, y);
    }
    //Region 2
    p = ceil(ry2 * (x + 0.5) * (x + 0.5) + rx2 * ( y - 1) * (y - 1) - rx2 * ry2);
    while(y > 0){
             y--;
             py -= tworx2;
             if( p > 0)
                 p += rx2 - py;
             else{
                 x++;
                 px -= twory2;
                 p += rx2 - py + px;
                 }
                 insertAtListBox(x0, y0, x, y);
             plotEllipsePoint(x0 , y0, x, y);
    }
    
    
    SwapBuffers(gLeftHdc);

    ValidateRect(gLeftSplitterWindow, NULL);
}
void __stdcall plotEllipsePoint(int xc, int yc, int x, int y){
     glBegin(GL_POINTS);
     glVertex2i(50 + xc + x, 50 + yc + y);
     glVertex2i(50 + xc - x, 50 + yc + y);
     glVertex2i(50 + xc + x, 50 + yc - y);     
     glVertex2i(50 + xc - x, 50 + yc - y);
     glEnd();
}

void __stdcall insertAtListBox(int rx, int ry, int x, int y){
     //hListBoxAxisX, hListBoxAxisY
     char ch[100];
     wsprintf(ch, "(x, y) = (%d, %d)", x, y);
     int nCount = SendMessage(hListBoxAxisX, LB_GETCOUNT, 0, 0);
     SendMessage(hListBoxAxisX, LB_INSERTSTRING, nCount, (LPARAM)(LPSTR)ch);
     wsprintf(ch, "(R(x), R(y)):\t (%d, %d)", rx, ry);
     SendMessage(hListBoxAxisY, LB_INSERTSTRING, nCount, (LPARAM)(LPSTR)ch);
}
