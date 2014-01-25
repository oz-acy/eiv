/**************************************************************************
 *
 *  eivwmh.cpp
 *  by oZ/acy
 *  (c) 2002-2014 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *  class EIVWMHandler
 *
 *  last update: 25 Jan MMXIV
 */
#include "eiv.h"
#include <polymnia/dibio.h>
#include <polymnia/pngio.h>
#include <polymnia/jpegio.h>


/*================================================
 *  EIVWMHandler::onKeyDown()
 *  �L�[�������̃R�[���o�b�N
 */
bool
EIVWMHandler::onKeyDown(urania::Window* win, int code, int rep, bool prev)
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
 *  EIVWMHandler::onSize()
 *  �T�C�Y�̍X���̃R�[���o�b�N
 */
bool EIVWMHandler::onSize(urania::Window* win, int typ, int w, int h)
{
  EIViewer::get()->sizeHandleAndMore(win);
  return true;
}


/*==============================================
 *  EIVWMHandler::onDestroy()
 *  �A�v���̏I���|��
 */
bool EIVWMHandler::onDestroy()
{
  urania::System::quit(0);
  return true;
}


/*==============================================
 *  EIVWMHandler::onPaint()
 *  �`�`�|��
 */
void
EIVWMHandler::onPaint(urania::BasicWindow* win, urania::PaintDevice* pdev)
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
 *  EIVWMHandler::onDropFiles()
 *  �t�@�C���h���b�v���̃��b�Z�[�W�n���h��
 */
void
EIVWMHandler::onDropFiles(
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
  }

  if (itr != fary.end())
  {
    EIViewer::get()->loadImage(pw, *itr);
  }
}


/*==================================================
 *  EIVWMHandler::onScroll()
 *  �X�N���[�����̃��b�Z�[�W�n���h��
 */
void EIVWMHandler::onScroll(urania::Window* pw, int id, int pos)
{
  if (id == urania::ID_SBH)
    EIViewer::get()->setX(pw, pos);
  else
    EIViewer::get()->setY(pw, pos);
}


/*=======================================================
 *  EIVWMHandler::onMouseWheel()
 *  �}�E�X�z�C�[�����􂵂����̃��b�Z�[�W�n���h��
 */
void EIVWMHandler::onMouseWheel(
  urania::Window* pw, int delta, int key, int x, int y)
{
  pw->postMessage(WM_VSCROLL, (delta > 0) ? SB_PAGEUP : SB_PAGEDOWN, 0);
}


//eof
