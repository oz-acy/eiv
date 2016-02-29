/**************************************************************************
 *
 *  eiv.cpp
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *
 *  履歴
 *    2016.2.29  修正 v0.35
 *************************************************************************/

#include <polymnia/dibio.h>
#include <polymnia/pngio.h>
#include <polymnia/jpegio.h>
#include "eiv.h"




/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
 *  class EIViewer の staticデータメンバ定義
 */
std::unique_ptr<EIViewer> EIViewer::eiv_S;




/*=====================================================
 *  EIViewer::get()
 *  インスタンスの取得
 */
EIViewer* EIViewer::get()
{
  if (!eiv_S)
    eiv_S.reset(new EIViewer());

  return eiv_S.get();
}




/*======================================
 *  EIViewer::EIViewer()
 *  update: 13 May 2012
 */
EIViewer::EIViewer() : vx_(0), vy_(0), scrX_(false), scrY_(false)
{
  using namespace urania;

  // ファイルダイアログ
  opn_.reset(
    OpenFileDialog::create(
      L"ImageFile(.bmp .png .jpg)|*.bmp;*.png;*.jpg;*.jpeg|"));

  svd_.reset(
    SaveFileDialog::create(
      L"ImageFile(.bmp .png .jpg)|*.bmp;*.png;*.jpg;*.jpeg|", L"bmp"));

  // 壁紙用パス設定
  wchar_t tmpd[MAX_PATH + 1];
  GetTempPath(MAX_PATH, tmpd);
  wpPath_ = std::wstring(tmpd) + L"gpeiv_wall.bmp";
  // タイトル文字列
  appTitle_ = L"EIV";
}




/*================================================================
 *  EIViewer::sizeHandleAndMore()
 *  Window のリサイズ時などにスクロールバーの再設定をする
 */
void EIViewer::sizeHandleAndMore(urania::Window* pw_TG)
{
  if (qrgb_ || pvd_)
  {
    int w = pw_TG->getClientWidth();
    int h = pw_TG->getClientHeight();
    int bw, bh;

    if (qrgb_)
    {
      bw = qrgb_->width();
      bh = qrgb_->height();
    }
    else
    {
      bw = pvd_->width();
      bh = pvd_->height();
    }

    pw_TG->setRangeSB(urania::ID_SBH, 0, bw - 1, w);
    pw_TG->setRangeSB(urania::ID_SBV, 0, bh - 1, h);

    if (bw - w < vx_)
      vx_ = bw - w;
    if (vx_ < 0)
      vx_ = 0;

    if (bh - h < vy_)
      vy_ = bh - h;
    if (vy_ < 0)
      vy_ = 0;

    pw_TG->setPosHSB(vx_);
    pw_TG->setPosHSB(vy_);

    pw_TG->invalidate();
    pw_TG->update();

  }
  else
  {
    pw_TG->disableHSB();
    pw_TG->disableVSB();
  }
}


/*=================================
 *  EIViewer::handleMenu()
 *  メニューの操作
 */
void EIViewer::handleMenu(urania::Window* pw)
{
  urania::Menu* menu = pw->getMenu().get();
  if (qrgb_)
  {
    menu->enableItem(EIV_MENU_SAVE);
    menu->enableItem(EIV_MENU_CNV_TO256);
    menu->enableItem(EIV_MENU_CNV_GS);
    menu->enableItem(EIV_MENU_WALLPAPER);
    //menu->enableItem(EIV_MENU_WALL_CENTER);
    //menu->enableItem(EIV_MENU_WALL_TILE);
    //menu->enableItem(EIV_MENU_WALL_EXT);
  }
  else if (pvd_)
  {
    menu->enableItem(EIV_MENU_SAVE);
    menu->grayItem(EIV_MENU_CNV_TO256);
    menu->grayItem(EIV_MENU_CNV_GS);
    menu->enableItem(EIV_MENU_WALLPAPER);
    //menu->enableItem(EIV_MENU_WALL_CENTER);
    //menu->enableItem(EIV_MENU_WALL_TILE);
    //menu->enableItem(EIV_MENU_WALL_EXT);
  }
  else
  {
    menu->grayItem(EIV_MENU_SAVE);
    menu->grayItem(EIV_MENU_CNV_TO256);
    menu->grayItem(EIV_MENU_CNV_GS);
    menu->grayItem(EIV_MENU_WALLPAPER);
    //menu->grayItem(EIV_MENU_WALL_CENTER);
    //menu->grayItem(EIV_MENU_WALL_TILE);
    //menu->grayItem(EIV_MENU_WALL_EXT);
  }
}



/*====================================================
 *  EIViewer::loadImage()
 *  画像ファイルを拡張子に応じてロードする
 */
void EIViewer::loadImage(urania::Window* pw, const std::wstring& file)
{
  using namespace polymnia;
  using namespace urania;

  std::unique_ptr<PictureIndexed> ppc;
  std::unique_ptr<Picture> pict;
  std::wstring ext = getFileExt(file);

  std::string path = System::strcpyWideToMultiByte(file);

  // 擴張子に應じてロード
  if (ext==L"bmp")
  {
    IndexedDibLoader bpload;
    ppc.reset(bpload.load(path));
    if (!ppc)
    {
      DibLoader bload;
      pict.reset(bload.load(path));
    }
  }
  else if (ext==L"png")
  {
    IndexedPngLoader ppload;
    ppc.reset(ppload.load(path));
    if (!ppc)
    {
      PngLoader pload;
      pict.reset(pload.load(path));
    }
  }
  else if(ext==L"jpg" || ext==L"jpeg")
  {
    JpegLoader jload;
    pict.reset(jload.load(path));
  }


  // ロードしたデータをきちんと格納する
  int w, h;
  std::wstring itype;
  if (ppc)
  {
    pvd_.reset(PaintMemDeviceIndexed::create(ppc.get()));
    qrgb_.reset();

    itype = L" (256 Color)";
    w = pvd_->width();
    h = pvd_->height();
  }
  else if (pict)
  {
    qrgb_.reset(PaintMemDevice::create(pict.get()));
    pvd_.reset();

    itype = L" (True Color)";
    w = qrgb_->width();
    h = qrgb_->height();
  }
  else {
    std::wstring s = L"Fault reading image file ";
    s += getFileTitle(file);
    System::notify(L"Error", s + L".");
    return;
  }


  // 再描畫やウィンドウのタイトル變更など
  handleMenu(pw);

  std::unique_ptr<PaintDevice> ppd(pw->getPaintDevice());
  ppd->clear(RgbColor(255, 255, 255));
  ppd.reset();

  vx_ = vy_ = 0;
  pw->resizeScreen(w, h);

  std::wstring str = getFileTitle(file);
  str += itype;
  str += L"  - ";
  str += appTitle_;
  pw->resetTitle(str);

  pw->invalidate();
  pw->update();
}


/*====================================================
 *  EIViewer::setX()
 *  横スクロールバーのメッセージハンドラ
 *  画像転送の左上座標の水平成分を設定
 */
void EIViewer::setX(urania::Window* pw, int x)
{
  vx_ = x;
  pw->invalidate();
  pw->update();
}

/*=============================================================
 *  EIViewer::setY()
 *  縦スクロールバーのメッセージハンドラ
 *  画像転送の左上座標の垂直成分を設定
 */
void EIViewer::setY(urania::Window* pw, int y)
{
  vy_ = y;
  pw->invalidate();
  pw->update();
}


/*================================================
 *  EIViewer::onMenuOpen()
 */
void EIViewer::onMenuOpen(urania::Window* win)
{
  using namespace urania;
  EIViewer* const eiv = EIViewer::get();
  if (eiv->openFileDlg()->doModal(win))
  {
    std::wstring fn = eiv->openFileDlg()->getFilePath();
    eiv->loadImage(win, fn);
  }
}

/*================================================
 *  EIViewer::onMenuEnd()
 */
void EIViewer::onMenuEnd(urania::Window* win)
{
  win->close();
}

/*================================================
 *  EIViewer::onMenuAbout()
 */
void EIViewer::onMenuAbout(urania::Window* win)
{
  std::wstring str
    = std::wstring(EIVNAME) + L"  " + VERSTR + L"\n" + COPYRIGHTSTR;
  urania::System::notify(L"About", str);
}




//eof
