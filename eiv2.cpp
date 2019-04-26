/**************************************************************************
 *
 *  eiv2.cpp
 *  by oZ/acy
 *  (c) 2002-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer (PLUS)
 *
 *  ����
 *    2016.2.29  �C�� v0.35
 *    2018.12.18 ���C�u�����d��̍X�ɒǜn
 */
#include "eiv.h"


/*================================================
 *  EIViewer::onMenuCnvTo256()
 */
void EIViewer::onMenuCnvTo256(urania::Window* win)
{
  EIViewer::get()->to256(win);
}


/*====================================================
 *  EIViewer::to256()
 *  256�Ɍ��F
 *==================================================*/
BOOL dProc(urania::Dialog*, UINT, WPARAM, LPARAM)
{
  return FALSE;
}

void EIViewer::to256(urania::Window* qw)
{
  using namespace polymnia;
  using namespace urania;

  if (!qrgb_)
    return;

  std::unique_ptr<Dialog> wtdlg(
    Dialog::doOwnedModeless(EIV_WAITDLG, qw, nullptr, nullptr, dProc));


  std::unique_ptr<Picture> pict(qrgb_->createPicture());
  std::unique_ptr<PictureIndexed> ppc(pict->duplicatePictureIndexed());
  pvd_.reset(PaintMemDeviceIndexed::create(ppc.get()));
  qrgb_.reset();
  wtdlg.reset();

  handleMenu(qw);

  std::wstring str = L"(256 Color Image)";
  str += std::wstring(L"  - ") + appTitle_;
  qw->resetTitle(str);

  qw->invalidate();
  qw->update();
}




/*================================================
 *  EIViewer::onMenuCnvGS()
 */
void EIViewer::onMenuCnvGS(urania::Window* win)
{
  EIViewer::get()->toGrayScale(win);
}


/*====================================================
 *  EIViewer::toGrayScale()
 *  ���m�N����
 *==================================================*/
void EIViewer::toGrayScale(urania::Window* qw)
{
  using namespace polymnia;
  using namespace urania;

  if (!qrgb_)
    return;

  std::unique_ptr<Picture> pict(qrgb_->createPicture());
  std::unique_ptr<PictureIndexed> 
    ppc(pict->createPictureGrayScaleIndexed());
  pvd_.reset(PaintMemDeviceIndexed::create(ppc.get()));
  qrgb_.reset();


  handleMenu(qw);

  std::wstring str = L"(256 Color Image)";
  str += std::wstring(L"  - ") + appTitle_;
  qw->resetTitle(str);

  qw->invalidate();
  qw->update();
}




/*================================================
 *  EIViewer::onMenuWallpaper()
 */
void EIViewer::onMenuWallpaper(urania::Window* win)
{
  EIViewer::get()->toWallPaper();
}


//eof
