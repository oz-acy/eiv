/**************************************************************************
 *
 *  towallpaper.cpp
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *
 *  履歴
 *    2016.2.29  修正
 **************************************************************************/

#include <urania/registry.h>
#include "eiv.h"


/*============================================
 *  EIViewer::toWallPaper()
 *  壁紙に設定
 *
 *  2014.1.28 アスペクト比を維持しての擴大に變更
 *  2016.2.29 壁紙の「竝べ方」の設定を削除
 */
void EIViewer::toWallPaper()
{
  if (!qrgb_ && !pvd_)
    return;

  saveImage(NULL, wpPath_);

  urania::RegistryWriter regw(
    urania::REGKEY_CURRENT_USER, L"Control Panel\\desktop");
  if (regw)
  {
    regw.setStringData(L"WallPaper", wpPath_);
    regw.setStringData(L"WallPaperStyle", L"6");
    regw.setStringData(L"TileWallpaper", L"0");
  }

  SystemParametersInfo(
    SPI_SETDESKWALLPAPER, 0,
    reinterpret_cast<void*>(const_cast<wchar_t*>(wpPath_.c_str())),
    SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
}




//eof
