/**************************************************************************
 *
 *  towallpaper.cpp
 *  by oZ/acy
 *  (c) 2002-2014 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *
 *  last update: 28 Jan MMXIV
 **************************************************************************/
#include <urania/registry.h>
#include "eiv.h"


/*============================================
 *  EIViewer::toWallPaper()
 *  壁紙に設定
 *
 *  int mode : 0: 中央, 1:竝べる 2:擴大
 *
 *  2014.1.28: アスペクト比を維持しての擴大に變更
 */
void EIViewer::toWallPaper(int mode)
{
  if (!qrgb_ && !pvd_)
    return;

  saveImage(NULL, wpPath_);

  urania::RegistryWriter regw(
                           urania::REGKEY_CURRENT_USER,
                           L"Control Panel\\desktop");

  if (regw)
  {
    std::wstring ss1, ss2;
    bool modechange;
    switch (mode)
    {
    case 0:
      ss1 = L"0";
      ss2 = L"0";
      modechange = true;
      break;

    case 1:
      ss1 = L"1";
      ss2 = L"1";
      modechange = true;
      break;

    case 2:
      ss1 = L"6";
      ss2 = L"0";
      modechange = true;
      break;

    default:
      modechange = false;
      break;
    }

    regw.setStringData(L"WallPaper", wpPath_);
    if (modechange) {
      regw.setStringData(L"WallPaperStyle", ss1);
      regw.setStringData(L"TileWallpaper", ss2);
    }
  }


  SystemParametersInfo(
    SPI_SETDESKWALLPAPER, 0,
    reinterpret_cast<void*>(const_cast<wchar_t*>(wpPath_.c_str())),
    SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
}




//eof
