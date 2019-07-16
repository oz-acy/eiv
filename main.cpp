/**********************************************************************//**
 *
 *  @file main.cpp
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief EIVのWinMain()
 *
 *  @date 2016.2.29  修正 (v0.35)
 *  @date 2016.10.11 修正 (v0.35 again) liburaniaの仕樣變更に追從
 *  @date 2019.4.24  修正 (v0.38) 巡覽順指定にかかる擴張
 *//*
 *  (c) 2002-2019 oZ/acy. ALL RIGHTS RESERVED.
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
    hndlr->regist(EIV_MENU_OPEN, EIViewer::onMenuOpen);
    hndlr->regist(EIV_MENU_END, EIViewer::onMenuEnd);
    hndlr->regist(EIV_MENU_ABOUT, EIViewer::onMenuAbout);
    hndlr->regist(EIV_MENU_SAVE, EIViewer::onMenuSave);
    hndlr->regist(EIV_MENU_CNV_TO256, EIViewer::onMenuCnvTo256);
    hndlr->regist(EIV_MENU_CNV_GS, EIViewer::onMenuCnvGS);
    hndlr->regist(EIV_MENU_WALLPAPER, EIViewer::onMenuWallpaper);
    hndlr->regist(EIV_MENU_SORTBYNAME, EIViewer::onMenuSortByName);
    hndlr->regist(EIV_MENU_SORTBYTIME, EIViewer::onMenuSortByTime);

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
