/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */
 
//we need openfilename stuff... MMP
#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif

#include "prtypes.h"
#include <stdio.h>
#include "resources.h"
#include "nsIImageManager.h"
#include "nsIImageGroup.h"
#include "nsIImageRequest.h"
#include "nsIImageObserver.h"
#include "nsIRenderingContext.h"
#include "nsIImage.h"
#include "nsIWidget.h"
#include "nsGUIEvent.h"
#include "nsRect.h"
#include "nsWidgetsCID.h"
#include "nsGfxCIID.h"

static NS_DEFINE_IID(kIWidgetIID, NS_IWIDGET_IID);
static NS_DEFINE_IID(kIImageObserverIID, NS_IIMAGEREQUESTOBSERVER_IID);

static NS_DEFINE_IID(kCWindowIID, NS_WINDOW_CID);
static NS_DEFINE_IID(kCChildWindowIID, NS_CHILD_CID);
static NS_DEFINE_IID(kCScrollbarIID, NS_VERTSCROLLBAR_CID);

static char* class1Name = "ImageTest";

static HANDLE gInstance, gPrevInstance;
static nsIImageManager *gImageManager = nsnull;
static nsIImageGroup *gImageGroup = nsnull;
static nsIImageRequest *gImageReq = nsnull;
static HWND gHwnd;
static nsIWidget *gWindow = nsnull;
static nsIImage *gImage = nsnull;
static PRBool gInstalledColorMap = PR_FALSE;

class MyObserver : public nsIImageRequestObserver {
public:
    MyObserver();
    ~MyObserver();
 
    NS_DECL_ISUPPORTS

    virtual void Notify(nsIImageRequest *aImageRequest,
                        nsIImage *aImage,
                        nsImageNotification aNotificationType,
                        PRInt32 aParam1, PRInt32 aParam2,
                        void *aParam3);

    virtual void NotifyError(nsIImageRequest *aImageRequest,
                             nsImageError aErrorType);
};

MyObserver::MyObserver()
{
}

MyObserver::~MyObserver()
{
}

NS_IMPL_ISUPPORTS(MyObserver, kIImageObserverIID)

void  
MyObserver::Notify(nsIImageRequest *aImageRequest,
                   nsIImage *aImage,
                   nsImageNotification aNotificationType,
                   PRInt32 aParam1, PRInt32 aParam2,
                   void *aParam3)
{
    switch (aNotificationType) {
       case nsImageNotification_kDimensions:
       {
           char buffer[40];
           sprintf(buffer, "Image:%d x %d", aParam1, aParam2);
           ::SetWindowText(gHwnd, buffer);
       }
       break;

       case nsImageNotification_kPixmapUpdate:
       case nsImageNotification_kImageComplete:
       case nsImageNotification_kFrameComplete:
       {
           if (gImage == nsnull && aImage) {
               gImage = aImage;
               NS_ADDREF(aImage);
           }

           if (!gInstalledColorMap && gImage) {
               nsColorMap *cmap = gImage->GetColorMap();

               if (cmap != nsnull && cmap->NumColors > 0) {
                 gWindow->SetColorMap(cmap);
               }
               gInstalledColorMap = PR_TRUE;
           }
           nsRect *rect = (nsRect *)aParam3;
           nsIRenderingContext *drawCtx = gWindow->GetRenderingContext();
           
           if (gImage) {
               drawCtx->DrawImage(gImage, 0, 0, 
                                  gImage->GetWidth(), gImage->GetHeight());
           }
       }
       break;
    }
}

void 
MyObserver::NotifyError(nsIImageRequest *aImageRequest,
                        nsImageError aErrorType)
{
  ::MessageBox(NULL, "Image loading error!",
               class1Name, MB_OK);
}

nsEventStatus PR_CALLBACK 
MyHandleEvent(nsGUIEvent *aEvent)
{
    nsEventStatus result = nsEventStatus_eConsumeNoDefault;
    switch(aEvent->message) {
        case NS_PAINT:
        {
            // paint the background
            nsIRenderingContext *drawCtx = ((nsPaintEvent*)aEvent)->renderingContext;
            drawCtx->SetColor(aEvent->widget->GetBackgroundColor());
            drawCtx->FillRect(*(((nsPaintEvent*)aEvent)->rect));

            if (gImage) {
                drawCtx->DrawImage(gImage, 0, 0, 
                                   gImage->GetWidth(), gImage->GetHeight());
            }
            
            return nsEventStatus_eConsumeNoDefault;
        }
        break;
    }

    return nsEventStatus_eIgnore; 
}

void
MyReleaseImages()
{
    if (gImageReq) {
        NS_RELEASE(gImageReq);
        gImageReq = NULL;
    }
    if (gImage) {
        NS_RELEASE(gImage);
        gImage = NULL;
    }

    gInstalledColorMap = PR_FALSE;
}

void
MyInterrupt()
{
    if (gImageGroup) {
        gImageGroup->Interrupt();
    }
}

#define FILE_URL_PREFIX "file://"


void
MyLoadImage(char *aFileName)
{
    char fileURL[256];
    char *str;

    MyInterrupt();
    MyReleaseImages();

    if (gImageGroup == NULL) {
        nsIRenderingContext *drawCtx = gWindow->GetRenderingContext();
        if (NS_NewImageGroup(&gImageGroup) != NS_OK ||
            gImageGroup->Init(drawCtx) != NS_OK) {
                ::MessageBox(NULL, "Couldn't create image group",
                             class1Name, MB_OK);
                NS_RELEASE(drawCtx);
                return;
            }
        NS_RELEASE(drawCtx);
    }

    strcpy(fileURL, FILE_URL_PREFIX);
    strcpy(fileURL + strlen(FILE_URL_PREFIX), aFileName);

    str = fileURL;
    while ((str = strchr(str, '\\')) != NULL)
        *str = '/';

    nscolor white;
    MyObserver *observer = new MyObserver();
            
    NS_ColorNameToRGB("white", &white);
    gImageReq = gImageGroup->GetImage(fileURL,
                                      observer,
                                      white, 0, 0, 0);
    if (gImageReq == NULL) {
      ::MessageBox(NULL, "Couldn't create image request",
                   class1Name, MB_OK);
    }
}

PRBool
OpenFileDialog(char *aBuffer, int32 aBufLen)
{
    BOOL result = FALSE;
    OPENFILENAME ofn;

    // *.js is the standard File Name on the Save/Open Dialog
    ::strcpy(aBuffer, "*.gif;*.png;*.jpg;*.jpeg");

    // fill the OPENFILENAME sruct
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = gHwnd;
    ofn.hInstance = gInstance;
    ofn.lpstrFilter = "All Images (*.gif,*.png,*.jpg,*.jpeg)\0*.gif;*png;*.jpg;*.jpeg\0GIF Files (*.gif)\0*.gif\0PNG Files (*.png)\0*.png\0JPEG Files (*.jpg,*.jpeg)\0*.jpg;*.jpeg\0All Files\0*.*\0\0";
    ofn.lpstrCustomFilter = NULL; 
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 1; // the first one in lpstrFilter
    ofn.lpstrFile = aBuffer; // contains the file path name on return
    ofn.nMaxFile = aBufLen;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL; // use default
    ofn.lpstrTitle = NULL; // use default
    ofn.Flags = OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    ofn.nFileOffset = 0; 
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = "gif"; // default extension is .js
    ofn.lCustData = NULL; 
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;

    // call the open file dialog or the save file dialog according to aIsOpenDialog
    result = ::GetOpenFileName(&ofn);

    return (PRBool)result;
}

long PASCAL
WndProc(HWND hWnd, UINT msg, WPARAM param, LPARAM lparam)
{
  HMENU hMenu;

  switch (msg) {
    case WM_COMMAND:
      hMenu = GetMenu(hWnd);

      switch (LOWORD(param)) {
        case TIMER_OPEN: 
        {
            char szFile[256];

            if (!OpenFileDialog(szFile, 256))
                return 0L;

            MyLoadImage(szFile);
            break;
        }
        case TIMER_EXIT:
            ::DestroyWindow(hWnd);
            exit(0);
        default:
            break;
      }
      break;

    case WM_CREATE:
      // Initialize image library
      if (NS_NewImageManager(&gImageManager) != NS_OK ||
          gImageManager->Init() != NS_OK) {
            ::MessageBox(NULL, "Can't initialize the image library",
                         class1Name, MB_OK);
      }	  
      gImageManager->SetCacheSize(1024*1024);
      break;

    case WM_DESTROY:
      MyInterrupt();
      MyReleaseImages();
      if (gImageGroup != nsnull) {
          NS_RELEASE(gImageGroup);
      }
      if (gImageManager != nsnull) {
          NS_RELEASE(gImageManager);
      }
      PostQuitMessage(0);
      break;

    default:
      break;
  }

  return DefWindowProc(hWnd, msg, param, lparam);
}

static HWND CreateTopLevel(const char* clazz, const char* title,
                                  int aWidth, int aHeight)
{
  // Create a simple top level window
  HWND window = ::CreateWindowEx(WS_EX_CLIENTEDGE,
                                 clazz, title,
                                 WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
                                 CW_USEDEFAULT, CW_USEDEFAULT,
                                 aWidth, aHeight,
                                 HWND_DESKTOP,
                                 NULL,
                                 gInstance,
                                 NULL);

  nsRect rect(0, 0, aWidth, aHeight);  

  nsresult rv = NSRepository::CreateInstance(kCChildWindowIID, NULL, kIWidgetIID, (void**)&gWindow);

  if (NS_OK == rv) {
      gWindow->Create((nsNativeWindow)window, rect, MyHandleEvent, NULL);
  }

  ::ShowWindow(window, SW_SHOW);
  ::UpdateWindow(window);
  return window;
}

#define WIDGET_DLL "raptorwidget.dll"
#define GFXWIN_DLL "raptorgfxwin.dll"

int PASCAL
WinMain(HANDLE instance, HANDLE prevInstance, LPSTR cmdParam, int nCmdShow)
{
  gInstance = instance;

  NSRepository::RegisterFactory(kCWindowIID, WIDGET_DLL, PR_FALSE, PR_FALSE);
  NSRepository::RegisterFactory(kCChildWindowIID, WIDGET_DLL, PR_FALSE, PR_FALSE);
  NSRepository::RegisterFactory(kCScrollbarIID, WIDGET_DLL, PR_FALSE, PR_FALSE);

  static NS_DEFINE_IID(kCRenderingContextIID, NS_RENDERING_CONTEXT_CID);
  static NS_DEFINE_IID(kCDeviceContextIID, NS_DEVICE_CONTEXT_CID);
  static NS_DEFINE_IID(kCFontMetricsIID, NS_FONT_METRICS_CID);
  static NS_DEFINE_IID(kCImageIID, NS_IMAGE_CID);

  NSRepository::RegisterFactory(kCRenderingContextIID, GFXWIN_DLL, PR_FALSE, PR_FALSE);
  NSRepository::RegisterFactory(kCDeviceContextIID, GFXWIN_DLL, PR_FALSE, PR_FALSE);
  NSRepository::RegisterFactory(kCFontMetricsIID, GFXWIN_DLL, PR_FALSE, PR_FALSE);
  NSRepository::RegisterFactory(kCImageIID, GFXWIN_DLL, PR_FALSE, PR_FALSE);

  if (!prevInstance) {
    WNDCLASS wndClass;
    wndClass.style = 0;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = gInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
    wndClass.lpszMenuName = class1Name;
    wndClass.lpszClassName = class1Name;
    RegisterClass(&wndClass);
  }

  // Create our first top level window
  HWND gHwnd = CreateTopLevel(class1Name, "Raptor HTML Viewer", 620, 400);

  // Process messages
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
  }
  return msg.wParam;
}

void main(int argc, char **argv)
{
  WinMain(GetModuleHandle(NULL), NULL, 0, SW_SHOW);
}
