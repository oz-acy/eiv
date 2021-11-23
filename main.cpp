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
 *  @date 2021.3.23  修正 (v0.39) 表示モード追加
 *  @date 2021.4.4   修正 liburaniaの仕樣變更に追從
 *  @date 2021.11.23 修正 libpolymnia+libthemisからlibeunomiaに切り替へ
 *
 */
#include "eiv.h"


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
    wf.w = 400;
    wf.h = 350;

    auto hndlr = std::make_unique<EIVWMHandler>();
    hndlr->setCommand(EIV_MENU_OPEN, EIViewer::onMenuOpen);
    hndlr->setCommand(EIV_MENU_END, EIViewer::onMenuEnd);
    hndlr->setCommand(EIV_MENU_ABOUT, EIViewer::onMenuAbout);
    hndlr->setCommand(EIV_MENU_SAVE, EIViewer::onMenuSave);
    hndlr->setCommand(EIV_MENU_CNV_TO256, EIViewer::onMenuCnvTo256);
    hndlr->setCommand(EIV_MENU_CNV_GS, EIViewer::onMenuCnvGS);
    hndlr->setCommand(EIV_MENU_WALLPAPER, EIViewer::onMenuWallpaper);
    hndlr->setCommand(EIV_MENU_SORTBYNAME, EIViewer::onMenuSortByName);
    hndlr->setCommand(EIV_MENU_SORTBYTIME, EIViewer::onMenuSortByTime);
    hndlr->setCommand(EIV_MENU_ACTUAL_SIZE, EIViewer::onMenuActualSize);
    hndlr->setCommand(EIV_MENU_SCALING, EIViewer::onMenuScaling);

    auto win = wf.create(std::move(hndlr), EIV_MAIN_MENU);

    vector<wstring> args = System::getCmdLineArgsW();
    if (args.size() == 2)
      eiv->openImage(win.get(), args[1]);

    eiv->handleMenu(win.get());
    eiv->sizeHandleAndMore(win.get());

    int ret = System::messageLoop();

    return ret;
  }
  catch(eunomia::Exception& exc)
  {
    System::alert(L"error", L"Error");
    return -1;
  }
}




//eof
