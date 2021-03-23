/**********************************************************************//**
 *
 *  @file main.cpp
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief EIVのWinMain()
 *
 *  @date 2016.2.29  修正 (v0.35)
 *  @date 2016.10.11 修正 (v0.35 again) liburaniaの仕樣變更に追從
 *  @date 2019.4.24  修正 (v0.38) 巡覽順指定にかかる擴張
 *  @date 2019.8.30  修正 exceptionのcatchの仕方を修正。uraniaに追從。
 *
 */
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
    WindowFactory wf;

    wf.icon = EIV_ICON;
    wf.title = EIVNAME;
    wf.resizeable = true;
    wf.h_scrollbar = true;
    wf.v_scrollbar = true;
    wf.minbox = true;
    wf.maxbox = true;
    wf.drag_and_drop = true;
    wf.w = 300;
    wf.h = 280;

    auto hndlr = std::make_unique<EIVWMHandler>();
    hndlr->regist(EIV_MENU_OPEN, EIViewer::onMenuOpen);
    hndlr->regist(EIV_MENU_END, EIViewer::onMenuEnd);
    hndlr->regist(EIV_MENU_ABOUT, EIViewer::onMenuAbout);
    hndlr->regist(EIV_MENU_SAVE, EIViewer::onMenuSave);
    hndlr->regist(EIV_MENU_CNV_TO256, EIViewer::onMenuCnvTo256);
    hndlr->regist(EIV_MENU_CNV_GS, EIViewer::onMenuCnvGS);
    hndlr->regist(EIV_MENU_WALLPAPER, EIViewer::onMenuWallpaper);
    hndlr->regist(EIV_MENU_SORTBYNAME, EIViewer::onMenuSortByName);
    hndlr->regist(EIV_MENU_SORTBYTIME, EIViewer::onMenuSortByTime);
    hndlr->regist(EIV_MENU_ACTUAL_SIZE, EIViewer::onMenuActualSize);
    hndlr->regist(EIV_MENU_SCALING, EIViewer::onMenuScaling);

    auto win = wf.create(std::move(hndlr), EIV_MAIN_MENU);

    vector<wstring> args = System::getCmdLineArgsW();
    if (args.size() == 2)
      eiv->openImage(win.get(), args[1]);

    eiv->handleMenu(win.get());
    eiv->sizeHandleAndMore(win.get());

    System::messageLoop();


    return 0;
  }
  catch(themis::EmptyException& exc)
  {
    System::alert(L"error", L"Error");
    return 1;
  }
}




//eof
