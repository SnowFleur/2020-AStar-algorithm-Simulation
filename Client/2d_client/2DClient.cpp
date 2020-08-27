// PROG14_1_16b.CPP - DirectInput keyboard demo
#ifdef _DEBUG
#include<iostream>
#endif // DEBUG

#include<memory>		//Smart Pointer
#include"directX.h"		//DirectX 9
#include"World.h"		//World class
#include"MouseController.h" //Mouse Input

// defines for windows 
#define WINDOW_CLASS_NAME L"A* Simulation"  // class name

#define WINDOW_WIDTH    1280   // size of window
#define WINDOW_HEIGHT   720

// game console
int Game_Init(void* parms = NULL);
int Game_Shutdown(void* parms = NULL);
int Game_Main(void* parms = NULL);

// GLOBALS ////////////////////////////////////////////////
HWND main_window_handle = NULL; // save the window handle

int g_left_x = 0;
int g_top_y = 0;

std::unique_ptr<CWorld> g_worldHandle;


LRESULT CALLBACK WindowProc(HWND hwnd,
    UINT msg,
    WPARAM wparam,
    LPARAM lparam)
{
    // this is the main message handler of the system
    PAINTSTRUCT	ps;		   // used in WM_PAINT
    HDC			hdc;	   // handle to a device context

                           // what is the message 
    switch (msg)
    {
    case WM_LBUTTONDOWN: {
    
        CMouseController::SetControllerByMouse(LOWORD(lparam), HIWORD(lparam));
#ifdef _DEBUG
        //std::cout << "Click Mouse Left  "<< LOWORD(lparam) <<" "<< HIWORD(lparam) <<"\n";
        int x = LOWORD(lparam);
        int y = HIWORD(lparam);

        D3DXVECTOR3 pos = D3DXVECTOR3(8,8, 0.0);




#endif
        break;
    }
    case WM_KEYDOWN: {

        switch (wparam) {
        case VK_SPACE: {
            break;
        }
        default:
            break;
        }
        break;
    }
    case WM_CREATE:
    {
        // do initialization stuff here
        return(0);
    } break;

    case WM_PAINT:
    {
        // start painting
        hdc = BeginPaint(hwnd, &ps);

        // end painting
        EndPaint(hwnd, &ps);
        return(0);
    } break;

    case WM_DESTROY:
    {
        // kill the application			
        PostQuitMessage(0);
        return(0);
    }

    default:
        break;

    } // end switch

          // process any messages that we didn't take care of 
    return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

  // WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hinstance,
    HINSTANCE hprevinstance,
    LPSTR lpcmdline,
    int ncmdshow)
{
    // this is the winmain function

    WNDCLASS winclass;	// this will hold the class we create
    HWND	 hwnd;		// generic window handle
    MSG		 msg;		// generic message


                        // first fill in the window class stucture
    winclass.style = CS_DBLCLKS | CS_OWNDC |
        CS_HREDRAW | CS_VREDRAW;
    winclass.lpfnWndProc = WindowProc;
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = hinstance;
    winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winclass.lpszMenuName = NULL;
    winclass.lpszClassName = WINDOW_CLASS_NAME;

    // register the window class
    if (!RegisterClass(&winclass))
        return(0);

    // create the window, note the use of WS_POPUP
    if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME, // class
        L"A* Simulation ",	 // title
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0,	   // x,y
        WINDOW_WIDTH,  // width
        WINDOW_HEIGHT, // height
        NULL,	   // handle to parent 
        NULL,	   // handle to menu
        hinstance,// instance
        NULL)))	// creation parms
        return(0);
    main_window_handle = hwnd;


    // perform all game console specific initialization
    Game_Init();

    // enter main event loop
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // test if this is a quit
            if (msg.message == WM_QUIT)
                break;

            // translate any accelerator keys
            TranslateMessage(&msg);

            // send the message to the window proc
            DispatchMessage(&msg);
        } // end if

          // main game processing goes here
        Game_Main();

    } // end while

      // shutdown game and release all resources
    Game_Shutdown();

    // return to Windows like this
    return(msg.wParam);

} // end WinMain

  ///////////////////////////////////////////////////////////

  // WINX GAME PROGRAMMING CONSOLE FUNCTIONS ////////////////

int Game_Init(void* parms)
{
    // this function is where you do all the initialization 
    // for your game

    // set up screen dimensions
    screen_width = WINDOW_WIDTH;
    screen_height = WINDOW_HEIGHT;
    screen_bpp = 32;

    // initialize directdraw
    DD_Init(screen_width, screen_height, screen_bpp);

    g_worldHandle = std::make_unique<CWorld>();


    // return success
    return(1);

} // end Game_Init

  ///////////////////////////////////////////////////////////

int Game_Shutdown(void* parms) {
    // this function is where you shutdown your game and
    // release all resources that you allocated

    // kill the reactor

    // shutdonw directdraw
    DD_Shutdown();

    // return success
    return(1);
} // end Game_Shutdown

  ///////////////////////////////////////////////////////////

int Game_Main(void* parms)
{

    // start the timing clock
    auto time = Start_Clock();

    // clear the drawing surface
    DD_Fill_Surface(D3DCOLOR_ARGB(255, 0, 0, 0));




    g_pd3dDevice->BeginScene();
    /*
    30프레임고정으로 진행하자
    */
    static DWORD size;
    g_worldHandle->Update(++size);


    g_pd3dDevice->EndScene();

    // flip the surfaces
    DD_Flip();

    // sync to 3o fps
    //Wait_Clock(30);


    // return success
    return(1);

} // end Game_Main

  //////////////////////////////////////////////////////////