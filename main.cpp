/**************************************************************************
 *
 *  main.cpp
 *  by oZ/acy
 *  (c) 2002-2012 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *  userMain() 他
 *
 *  last update: 13 May MMXII
 *************************************************************************/
#include "eiv.h"
#include <memory>
#include <themis/strconv.h>


/*==================================================
*  userMain()
*  gplib GUI 使用時のユーザ用開始地点
*=================================================*/
int userMain()
{
  using namespace urania;

  try
  {
    // EIV Initialize
    EIViewer* eiv = EIViewer::get();

    // CreateWindow
    WindowFactory de;

    de.icon = EIV_ICON;
    de.title = L"Easy Image Viewer";
    de.resizeable = true;
    de.h_scrollbar = true;
    de.v_scrollbar = true;
    de.minbox = true;
    de.maxbox = true;
    de.drag_and_drop = true;
    de.w = 300;
    de.h = 280;

    EIVWMHManager* mng = new EIVWMHManager;

    mng->set(EIV_MENU_OPEN, EIViewer::onMenuOpen);
    mng->set(EIV_MENU_END, EIViewer::onMenuEnd);
    mng->set(EIV_MENU_ABOUT, EIViewer::onMenuAbout);

#ifdef EIV_PLUS
    mng->set(EIV_MENU_SAVE, EIViewer::onMenuSave);
    mng->set(EIV_MENU_CNV_TO256, EIViewer::onMenuCnvTo256);
    mng->set(EIV_MENU_CNV_GS, EIViewer::onMenuCnvGS);
    mng->set(EIV_MENU_WALL_CENTER, EIViewer::onMenuWallCenter);
    mng->set(EIV_MENU_WALL_TILE, EIViewer::onMenuWallTile);
    mng->set(EIV_MENU_WALL_EXT, EIViewer::onMenuWallExt);
#endif // EIV_PLUS

    std::auto_ptr<Window> pw(de.create(mng, EIV_MAIN_MENU));

    std::vector<std::string> args = System::getCmdLineArgs();
    if (args.size() == 2)
      eiv->loadImage(
             pw.get(),
             System::getLongPathName(System::cnvMBStr2WStr(args[1])));

    eiv->sizeHandleAndMore(pw.get());

    // Apprication Start
    System::start();

    // Apprication Finish
    return 0;
  }
  catch(themis::EmptyException exc)
  {
    System::alertBox(L"error", L"Error");
    return 1;
  }
}



//eof
