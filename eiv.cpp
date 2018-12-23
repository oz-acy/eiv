/**************************************************************************
 *
 *  eiv.cpp
 *  by oZ/acy
 *  (c) 2002-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *
 *  履歴
 *    2016.2.29  修正 v0.35
 *    2016.10.12 修正 v0.36 openImage()、nextImage()、prevImage()追加
 *    2018.12.23 修正 C++17(<filesystem>)對應
 */
#include <algorithm>
#include <polymnia/dibio.h>
#include <polymnia/pngio.h>
#include <polymnia/jpegio.h>
#include "eiv.h"




/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
 *  class EIViewer の staticデータメンバ定義
 */
std::unique_ptr<EIViewer> EIViewer::eiv_S;


/**
 *  Singletonパターンによる唯一のインスタンスを取得する。
 *  @brief 唯一のインスタンスの取得
 */
EIViewer* EIViewer::get()
{
  if (!eiv_S)
    eiv_S.reset(new EIViewer());

  return eiv_S.get();
}


/**
 *  @date 2016.2.29  修正
 */
EIViewer::EIViewer()
: fdlg_(urania::FileDialog::create(
          L"ImageFile (*.bmp;*.png;*.jpg;*.jpeg)|*.bmp;*.png;*.jpg;*.jpeg|"
          L"All Files (*.*)|*.*|")),
  vx_(0), vy_(0), scrX_(false), scrY_(false)
{
  using namespace urania;

  // 壁紙用パス設定
  wchar_t tmpd[MAX_PATH];
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


/**
 *  @brief メニューの操作
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
  }
  else if (pvd_)
  {
    menu->enableItem(EIV_MENU_SAVE);
    menu->grayItem(EIV_MENU_CNV_TO256);
    menu->grayItem(EIV_MENU_CNV_GS);
    menu->enableItem(EIV_MENU_WALLPAPER);
  }
  else
  {
    menu->grayItem(EIV_MENU_SAVE);
    menu->grayItem(EIV_MENU_CNV_TO256);
    menu->grayItem(EIV_MENU_CNV_GS);
    menu->grayItem(EIV_MENU_WALLPAPER);
  }
}


/**
 * 畫像ファイルを「開く」
 */
void EIViewer::openImage(urania::Window* pw, const std::wstring& path)
{
  namespace fs = std::filesystem;

  tgt_ = path;
  if (!fs::exists(tgt_)) {
    std::wstring s = L"Image file ";
    s += tgt_.native();
    urania::System::notify(L"Error", s + L" does not exist.");
    return;
  }

  loadImage(pw);
}


/**
 * 「次」の畫像を讀み込む
 */
void EIViewer::nextImage(urania::Window* pw)
{
  namespace fs = std::filesystem;

  if (tgt_.native().empty())
    return;
  if (!fs::exists(tgt_)) {
    std::wstring s = L"Image file ";
    s += tgt_.native();
    urania::System::notify(L"Error", s + L" does not exist.");
    return;
  }

  std::vector<fs::path> vf;
  for (
    const fs::directory_entry& x :
    fs::directory_iterator(tgt_.parent_path()) ) {

    fs::path ext = x.path().extension();
    if (
      ext == ".bmp" || ext == ".png" || ext == ".jpg" || ext == ".jpeg"
      || ext == ".BMP" || ext == ".PNG" || ext == ".JPG" || ext == "JPEG") {

      vf.push_back(x.path());
    }
  }

  auto jt = std::find(vf.begin(), vf.end(), tgt_);
  if (jt == vf.end()) {
    jt = vf.begin();
  } else {
    ++jt;
    if (jt == vf.end()) {
      jt = vf.begin();
    }
  }

  if (jt != vf.end()) {
    tgt_ = *jt;
    loadImage(pw);
  }
}


/**
 * 「前」の畫像を讀み込む
 */
void EIViewer::prevImage(urania::Window* pw)
{
  namespace fs = std::filesystem;

  if (tgt_.native().empty())
    return;
  else if (!fs::exists(tgt_)) {
    std::wstring s = L"Image file ";
    s += tgt_.native();
    urania::System::notify(L"Error", s + L" does not exist.");
    return;
  }

  std::vector<fs::path> vf;
  for (
    const fs::directory_entry& x :
    fs::directory_iterator(tgt_.parent_path()) ) {

    fs::path ext = x.path().extension();
    if (
      ext == ".bmp" || ext == ".png" || ext == ".jpg" || ext == ".jpeg"
      || ext == ".BMP" || ext == ".PNG" || ext == ".JPG" || ext == "JPEG") {

      vf.push_back(x.path());
    }
  }

  auto jt = std::find(vf.rbegin(), vf.rend(), tgt_);
  if (jt == vf.rend()) {
    jt = vf.rbegin();
  } else {
    ++jt;
    if (jt == vf.rend()) {
      jt = vf.rbegin();
    }
  }

  if (jt != vf.rend()) {
    tgt_ = *jt;
    loadImage(pw);
  }
}


/**
 *  畫像ファイルを擴張子に應じて實際に讀み込む
 *    ※讀み込み對象のpathは豫め tgt_ に格納される
 */
void EIViewer::loadImage(urania::Window* pw)
{
  using namespace polymnia;
  using namespace urania;
  namespace fs = std::filesystem;

  fs::path ext = tgt_.extension();
  std::unique_ptr<PictureIndexed> ppc;
  std::unique_ptr<Picture> pict;

  // 擴張子に應じてロード
  if (ext == ".bmp" || ext == ".BMP")
  {
    IndexedDibLoader bpload;
    ppc.reset(bpload.load(tgt_));
    if (!ppc)
    {
      DibLoader bload;
      pict.reset(bload.load(tgt_));
    }
  }
  else if (ext == ".png" || ext == ".PNG")
  {
    IndexedPngLoader ppload;
    ppc.reset(ppload.load(tgt_));
    if (!ppc)
    {
      PngLoader pload;
      pict.reset(pload.load(tgt_));
    }
  }
  else if(ext == ".jpg" || ext == ".jpeg" || ext == ".JPG" || ext == ".JPEG")
  {
    JpegLoader jload;
    pict.reset(jload.load(tgt_));
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
    s += tgt_.wstring();
    System::notify(L"Error", s + L".");
    return;
  }


  // 再描畫やウィンドウのタイトル變更など
  handleMenu(pw);

  std::unique_ptr<PaintDevice> ppd(pw->getPaintDevice());
  ppd->clear(RgbColor(255, 255, 255));
  ppd.reset();

  vx_ = 0;
  vy_ = 0;
  pw->resizeScreen(w, h);

  //std::wstring str = fs::path(file).filename().wstring();
  std::wstring str = tgt_.filename().wstring();
  str += itype;
  str += L"  - ";
  str += appTitle_;
  pw->resetTitle(str);

  pw->invalidate();
  pw->update();
}


/**
 *  横スクロールバーのメッセージハンドラ。畫像轉送の左上X座標を設定する。
 *  @param pw ウィンドウ
 *  @param x 設定するX座標
 */
void EIViewer::setX(urania::Window* pw, int x)
{
  vx_ = x;
  pw->invalidate();
  pw->update();
}

/**
 *  縦スクロールバーのメッセージハンドラ。畫像轉送の左上Y座標を設定する。
 *  @param pw ウィンドウ
 *  @param y 設定するY座標
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
  if (eiv->fileDialog()->doModalOpenFile(win))
  {
    std::wstring fn = eiv->fileDialog()->getFilePath();
    eiv->openImage(win, fn);
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
