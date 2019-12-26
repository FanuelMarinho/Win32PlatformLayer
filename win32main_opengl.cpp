#include <windows.h>
#include <gl/gl.h>

static char* ClassName = "Win32PlatformLayer";
static char* WindowName = "OpenGL";
static int WindowHeight = 720;
static int WindowWidth = 1280;
static bool Running = false;

LRESULT CALLBACK WindowProcedure(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT result;

    switch(Message)
    {
        case WM_DESTROY:
        {
            Running = false; 
            result = 0;
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Handle, &Paint);
            EndPaint(Handle, &Paint);
            result = 0; 
        } break;

        case WM_CLOSE:
        {
            Running = false;
            result = 0;
        } break;

        default:
            result = DefWindowProc(Handle, Message, WParam, LParam);
    }

    return result;
}


int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, LPSTR CommandLine, int CommandShow)
{
    WNDCLASSEX WindowClass;
    HWND Handle;
    MSG Message;

    WindowClass.cbSize = sizeof(WindowClass);
    WindowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    WindowClass.lpfnWndProc = WindowProcedure;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = Instance;
    WindowClass.hIcon = 0;
    WindowClass.hCursor = 0;
    WindowClass.hbrBackground = 0;
    WindowClass.lpszMenuName = 0;
    WindowClass.lpszClassName = ClassName;
    WindowClass.hIconSm = 0;

    if(!RegisterClassEx(&WindowClass))
    {
        //show error message 
    }

    Handle = CreateWindowEx(
            0, 
            ClassName, 
            WindowName, 
            WS_OVERLAPPEDWINDOW, 
            CW_USEDEFAULT,
            CW_USEDEFAULT, 
            WindowWidth, 
            WindowHeight, 
            NULL, 
            NULL, 
            Instance, 0);
    
    if(!Handle)
    {
        //show error message
    } 

    ShowWindow(Handle, CommandShow);
    UpdateWindow(Handle);

    PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
    DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
    DesiredPixelFormat.nVersion = 1;
    DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
    DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
    DesiredPixelFormat.cColorBits = 32;
    DesiredPixelFormat.cAlphaBits = 8;
    DesiredPixelFormat.cDepthBits = 24;
    DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

    //NOTE(Fanuel): I'm not sure if it's wise to keep de DeviceContext for the entire application
    // filespan without releasing it. Should we release it after we're done rendering
    // or maybe the CS_OWNDC flag is already a sign we can keep de HDC.
    HDC DeviceContext = GetDC(Handle);
    int WindowsChoosedPixelFormat;
    WindowsChoosedPixelFormat = ChoosePixelFormat(DeviceContext, &DesiredPixelFormat);
    SetPixelFormat(DeviceContext, WindowsChoosedPixelFormat, &DesiredPixelFormat);

    HGLRC OpenGLContext = wglCreateContext(DeviceContext);
    wglMakeCurrent(DeviceContext, OpenGLContext);

    Running = true;
    while(Running)
    {
        while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        
        }
    
        //Display();        
        //SwapBuffers(DeviceContext);        
    }


    return 0;

}
