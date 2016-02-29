/**************************************************************************
 *
 *  savepct.cpp
 *
 *  (c) 2002-2016 oZ/acy.  All Rights Reserved.
 *
 *  Easy Image Viewer
 *  ファイル保存關聯
 *
 *  履歴
 *    2016.2.29  修正 v0.35
 *************************************************************************/


#include <memory>
#include <polymnia/dibio.h>
#include <polymnia/pngio.h>
#include <polymnia/jpegio.h>
#include <themis/except.h>
#include "eiv.h"


/*================================================
 *  EIViewer::onMenuSave()
 */
void EIViewer::onMenuSave(urania::Window* win)
{
  using namespace urania;
  EIViewer* const eiv = EIViewer::get();
  if (eiv->fileDialog()->doModalSaveFile(win))
  {
    std::wstring fn = eiv->fileDialog()->getFilePath();
    eiv->saveImage(win, fn);
  }
}




namespace
{

BOOL pngDlgProc__(urania::Dialog* dlg, UINT msg, WPARAM wp, LPARAM lp)
{
  (void)lp;

  switch (msg)
  {
  case WM_COMMAND:
    switch (wp)
    {
    case EIV_PNGOK:
      if (dlg->isChecked(EIV_PNGCHECK))
        dlg->endModal(1);
      else
        dlg->endModal(0);

      return TRUE;

    case EIV_PNGCANCEL:
      dlg->endModal(-1);
      return TRUE;
    }
    break;
  }

  return FALSE;
}

BOOL pngDlgProc2__(urania::Dialog* dlg, UINT msg, WPARAM wp, LPARAM lp)
{
  (void)lp;

  switch (msg)
  {
  case WM_COMMAND:
    switch (wp)
    {
    case EIV_PNGIDXOK:
      {
        int r = 0;
        r = dlg->isChecked(EIV_PNGIDXCHECK) ? 1 : 0;
        r |= dlg->isChecked(EIV_PNGIDXTRANS) ? 2 : 0;
        dlg->endModal(r);
      }
      return TRUE;

    case EIV_PNGIDXCANCEL:
      dlg->endModal(-1);
      return TRUE;
    }
    break;
  }

  return FALSE;
}

void 
savePng__(
  urania::Window* win, polymnia::Picture* pct, polymnia::PictureIndexed* pidx,
  const std::wstring& file)
{
  std::string path = urania::System::strcpyWideToMultiByte(file);
  if (pidx)
  {
    int res = 
      urania::Dialog::doOwnedModal(
        EIV_PNGIDXDLG, win, NULL, NULL, pngDlgProc2__);

    if (res < 0)
      return;

    polymnia::IndexedPngSaver ps08;

    if (res & 1)
      ps08.enableInterlace();
    if (res & 2)
      ps08.enableTransparent(pidx->pixel(0, 0));

    ps08.save(pidx, path);
  }
  else if (pct)
  {
    int res = 
      urania::Dialog::doOwnedModal(EIV_PNGDLG, win, NULL, NULL, pngDlgProc__);

    if (res < 0)
      return;

    polymnia::PngSaver psave;
    if (res)
      psave.enableInterlace();
    psave.save(pct, path);
  }
}




} // end of namespace





/*========================================================
 *  EIViewer::saveImage()
 *  表示している畫像を擴張子に應じた形式で保存する
 */
void EIViewer::saveImage(urania::Window* win, const std::wstring& file)
{
  using namespace polymnia;
  using namespace urania;

  std::unique_ptr<Picture> pict;
  std::unique_ptr<PictureIndexed> ppc;
  if (pvd_)
    ppc.reset(pvd_->createPicture());
  else if (qrgb_)
    pict.reset(qrgb_->createPicture());
  else
    return;

  std::wstring ext = getFileExt(file);
  std::string path = System::strcpyWideToMultiByte(file);

  if (ext==L"png")
    savePng__(win, pict.get(), ppc.get(), file);
  else if (ext==L"jpg" || ext==L"jpeg")
  {
    if (ppc)
      pict.reset(ppc->duplicatePicture());
    JpegSaver jsave;
    jsave.save(pict.get(), path);
  }
  else //if (ext=="bmp")
  {
    if (ppc)
    {
      IndexedDibSaver bs08;
      bs08.save(ppc.get(), path);
    }
    else
    {
      DibSaver bsave;
      bsave.save(pict.get(), path);
    }
  }

}



//eof
