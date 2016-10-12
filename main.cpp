/**************************************************************************
 *
 *  main.cpp
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *  WinMain() ëº
 *
 *  óöó
 *    2016.2.29  èCê≥ (v0.35)
 *    2016.10.11 èCê≥ (v0.35 again)
 */

#include <themis/strconv.h>
#include "eiv.h"


/*==================================================
 *  WinMain()
 */
int WINAPI wWinMain(HINSTANCE hi, HINSTANCE, LPWSTR, int)
{
  using namespace std;
  using namespace urania;

  try
  {
    System::start(hi);

    // EIV Initialize
    EIViewer* eiv = EIViewer::get();

    // CreateWindow
    WindowFactory de;

    de.icon = EIV_ICON;
    de.title = EIVNAME;
    de.resizeable = true;
    de.h_scrollbar = true;
    de.v_scrollbar = true;
    de.minbox = true;
    de.maxbox = true;
    de.drag_and_drop = true;
    de.w = 300;
    de.h = 280;

    unique_ptr<EIVWMHandler> hndlr(new EIVWMHandler);
    hndlr->set(EIV_MENU_OPEN, EIViewer::onMenuOpen);
    hndlr->set(EIV_MENU_END, EIViewer::onMenuEnd);
    hndlr->set(EIV_MENU_ABOUT, EIViewer::onMenuAbout);
    hndlr->set(EIV_MENU_SAVE, EIViewer::onMenuSave);
    hndlr->set(EIV_MENU_CNV_TO256, EIViewer::onMenuCnvTo256);
    hndlr->set(EIV_MENU_CNV_GS, EIViewer::onMenuCnvGS);
    hndlr->set(EIV_MENU_WALLPAPER, EIViewer::onMenuWallpaper);

    unique_ptr<Window> pw(de.create(hndlr.release(), EIV_MAIN_MENU));

    vector<wstring> args = System::getCmdLineArgsW();
    if (args.size() == 2)
      eiv->openImage(pw.get(), args[1]);

    eiv->handleMenu(pw.get());
    eiv->sizeHandleAndMore(pw.get());


    System::messageLoop();


    return 0;
  }
  catch(themis::EmptyException exc)
  {
    System::alert(L"error", L"Error");
    return 1;
  }
}




//eof
