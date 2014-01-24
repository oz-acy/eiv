/**************************************************************************
 *
 *  eiv2.cpp
 *  by oZ/acy
 *  (c) 2002-2014 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer PLUS
 *
 *  last update: 25 Jan MMXIV
 ************************************************************************/
#include "eiv.h"
#include <polymnia/pictcvt.h>


/*================================================
 *  EIViewer::onMenuCnvTo256()
 */
void EIViewer::onMenuCnvTo256(urania::Window* win)
{
  EIViewer::get()->to256(win);
}


/*====================================================
 *  EIViewer::to256()
 *  256に減色
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
    Dialog::doOwnedModeless(EIV_WAITDLG, qw, NULL, NULL, dProc));


  std::unique_ptr<Picture> pict(qrgb_->createPicture());
  std::unique_ptr<PictureIndexed> ppc(reducePictureColors(pict.get()));

  pvd_.reset(PaintMemDeviceIndexed::create(ppc.get()));

  qrgb_.reset();
  wtdlg.reset();

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
 *  モノクロ化
 *==================================================*/
void EIViewer::toGrayScale(urania::Window* qw)
{
  using namespace polymnia;
  using namespace urania;


  if (!qrgb_)
    return;


  std::unique_ptr<Picture> pict(qrgb_->createPicture());

  std::unique_ptr<PictureIndexed> 
    ppc(createPictureGrayScaleIndexed(pict.get()));

  pvd_.reset(PaintMemDeviceIndexed::create(ppc.get()));

  qrgb_.reset();

  std::wstring str = L"(256 Color Image)";
  str += std::wstring(L"  - ") + appTitle_;
  qw->resetTitle(str);

  qw->invalidate();
  qw->update();
}




/*================================================
 *  EIViewer::onMenuWallCenter()
 */
void EIViewer::onMenuWallCenter(urania::Window* win)
{
  EIViewer::get()->toWallPaper(0);
}

/*================================================
 *  EIViewer::onMenuWallTile()
 */
void EIViewer::onMenuWallTile(urania::Window* win)
{
  EIViewer::get()->toWallPaper(1);
}

/*================================================
 *  EIViewer::onMenuWallExt()
 */
void EIViewer::onMenuWallExt(urania::Window* win)
{
  EIViewer::get()->toWallPaper(2);
}



//eof
