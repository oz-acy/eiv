/**************************************************************************
 *
 *  eiv.cpp
 *  by oZ/acy
 *  (c) 2002-2012 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *
 *  last update: 13 May MMXII
 *************************************************************************/
#include "eiv.h"
#include <polymnia/dibio.h>
#include <polymnia/pngio.h>
#include <polymnia/jpegio.h>




/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
*  class EIViewer の staticデータメンバ定義
*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

boost::scoped_ptr<EIViewer> EIViewer::eiv_S;




/*=====================================================
 *  EIViewer::get()
 *  インスタンスの取得
 *===================================================*/
EIViewer* EIViewer::get()
{
  if (!eiv_S)
    eiv_S.reset(new EIViewer());

  return eiv_S.get();
}




/*======================================
*  EIViewer::EIViewer()
*  update: 13 May 2012
*=====================================*/
EIViewer::EIViewer() : vx_(0), vy_(0), scrX_(false), scrY_(false)
{
  using namespace urania;

  // ファイルダイアログ
  opn_.reset(
    OpenFileDialog::create(
      L"ImageFile(.bmp .png .jpg)|*.bmp;*.png;*.jpg;*.jpeg|"));

#ifdef EIV_PLUS
  svd_.reset(
    SaveFileDialog::create(
      L"ImageFile(.bmp .png .jpg)|*.bmp;*.png;*.jpg;*.jpeg|", L"bmp"));

  // 壁紙用パス設定
  wchar_t tmpd[MAX_PATH + 1];
  GetTempPath(MAX_PATH, tmpd);
  wpPath_ = std::wstring(tmpd) + L"gpeiv_wall.bmp";
  // タイトル文字列
  appTitle_ = L"EIV+";
#else
  appTitle_ = L"EIV";
#endif // EIV_PLUS

}




/*================================================================
*  EIViewer::sizeHandleAndMore()
*  Window のリサイズ時などにスクロールバーの再設定をする
*===============================================================*/
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

    pw_TG->setSBRange(urania::ID_SBH, 0, bw - 1, w);
    pw_TG->setSBRange(urania::ID_SBV, 0, bh - 1, h);

    if (bw - w < vx_)
      vx_ = bw - w;
    if (vx_ < 0)
      vx_ = 0;

    if (bh - h < vy_)
      vy_ = bh - h;
    if (vy_ < 0)
      vy_ = 0;

    pw_TG->setHSBarPos(vx_);
    pw_TG->setVSBarPos(vy_);

    pw_TG->invalidate();
    pw_TG->update();

  }
  else
  {
    pw_TG->disableHSBar();
    pw_TG->disableVSBar();
  }
}



/*====================================================
*  EIViewer::loadImage()
*  画像ファイルを拡張子に応じてロードする
*===================================================*/
void EIViewer::loadImage(urania::Window* pw, const std::wstring& file)
{
  using namespace polymnia;
  using namespace urania;

  boost::scoped_ptr<PictureIndexed> ppc;
  boost::scoped_ptr<Picture> pict;
  std::wstring ext = getFileExt(file);

  std::string path = System::cnvWStr2MBStr(file);

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
    qrgb_.reset(NULL);

    itype = L" (256 Color)";
    w = pvd_->width();
    h = pvd_->height();
  }
  else if (pict)
  {
    qrgb_.reset(PaintMemDevice::create(pict.get()));
    pvd_.reset(NULL);

    itype = L" (True Color)";
    w = qrgb_->width();
    h = qrgb_->height();
  }
  else {
    std::wstring s = L"Fault reading image file ";
    s += getFileTitle(file);
    System::msgBox(L"Error", s + L".");
    return;
  }


  // 再描畫やウィンドウのタイトル變更など
  boost::scoped_ptr<PaintDevice> ppd(pw->getPaintDevice());
  ppd->clear(RgbColor(255, 255, 255));
  ppd.reset(NULL);

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
*  EIViewer::setXPos()
*  横スクロールバーのメッセージハンドラ
*  画像転送の左上座標の水平成分を設定
*===================================================*/
void EIViewer::setXPos(urania::Window* pw, int x)
{
  EIViewer* eiv = EIViewer::get();
  eiv->vx_ = x;
  pw->invalidate();
  pw->update();
}

/*=============================================================
*  EIViewer::setYPos()
*  縦スクロールバーのメッセージハンドラ
*  画像転送の左上座標の垂直成分を設定
*============================================================*/
void EIViewer::setYPos(urania::Window* pw, int y)
{
  EIViewer* eiv = EIViewer::get();
  eiv->vy_ = y;
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
    std::wstring fn = eiv->openFileDlg()->getPath();
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
  std::wstring str = EIVNAME L"  " VERSTR L"\n" COPYRIGHTSTR;
  urania::System::msgBox(L"About", str);
}




/*================================================
 *  EIVWMHManager::onKeyDown()
 *  キー押下時のコールバック
 */
bool
EIVWMHManager::onKeyDown(urania::Window* win, int code, int rep, bool prev)
{
  switch (code)
  {
  case VK_ESCAPE:
    win->close();
    break;
  case VK_F6:
    win->minimize();
    break;
  }
  return true;
}


/*================================================
 *  EIVWMHManager::onSize()
 *  サイズ變更時のコールバック
 */
bool EIVWMHManager::onSize(urania::Window* win, int typ, int w, int h)
{
  EIViewer::get()->sizeHandleAndMore(win);
  return true;
}


/*==============================================
 *  EIVWMHManager::onDestroy()
 *  アプリの終了處理
 */
bool EIVWMHManager::onDestroy()
{
  urania::System::quit(0);
  return true;
}


/*==============================================
 *  EIVWMHManager::onPaint()
 *  描畫處理
 */
void
EIVWMHManager::onPaint(urania::BasicWindow* win, urania::PaintDevice* pdev)
{
  (void)win;
  int bw, bh;
  EIViewer* eiv = EIViewer::get();

  if (eiv->getPaintDevice())
  {
    bw = eiv->getPaintDevice()->width();
    bh = eiv->getPaintDevice()->height();
    pdev->blt(
      0, 0, eiv->getPaintDevice(),
      eiv->getX(), eiv->getY(), bw - eiv->getX(), bh - eiv->getY());
  }
  else if (eiv->getPaintDeviceIndexed())
  {
    bw = eiv->getPaintDeviceIndexed()->width();
    bh = eiv->getPaintDeviceIndexed()->height();
    pdev->blt(
      0, 0, eiv->getPaintDeviceIndexed(),
      eiv->getX(), eiv->getY(), bw - eiv->getX(), bh - eiv->getY());
  }
  else
  {
    pdev->clear(polymnia::RgbColor(255, 255, 255));
  }
}


/*==================================================
 *  EIVWMHManager::onDropFiles()
 *  ファイルドロップ時のメッセージハンドラ
 *================================================*/
void
EIVWMHManager::onDropFiles(
  urania::Window* pw, std::vector<std::wstring>& fary, int x, int y)
{
  (void)x;
  (void)y;

  if (fary.size()==0)
    return;

  std::wstring ext;
  std::vector<std::wstring>::iterator itr;

  for (itr = fary.begin(); itr != fary.end(); itr++)
  {
    ext = getFileExt(*itr);
    if (ext==L"bmp")
      break;
    if (ext==L"png")
      break;
    if (ext==L"jpg")
      break;
    if (ext==L"jpeg")
      break;
//    if (ext==L"ppm")
//      break;
  }

  if (itr != fary.end())
  {
    EIViewer* eiv = EIViewer::get();
    eiv->loadImage(pw, *itr);
  }
}


/*==================================================
 *  EIVWMHManager::onDropFiles()
 *  スクロール時のメッセージハンドラ
 *================================================*/
void EIVWMHManager::onScroll(urania::Window* pw, int id, int pos)
{
  if (id == urania::ID_SBH)
    EIViewer::setXPos(pw, pos);
  else
    EIViewer::setYPos(pw, pos);
}




//eof
