/**************************************************************************
 *
 *  eivwmh.cpp
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *  class EIVWMHandler
 *
 *  @date 2016.10.12 onLButtonDown()、onRButtonDown()を追加
 */
#include "eiv.h"
#include <polymnia/dibio.h>
#include <polymnia/pngio.h>
#include <polymnia/jpegio.h>


/**
 *  キー押下時のコールバック
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


/**
 *  サイズ變更時のコールバック
 */
bool EIVWMHandler::onSize(urania::Window* win, int typ, int w, int h)
{
  EIViewer::get()->sizeHandleAndMore(win);
  return true;
}


/**
 *  アプリの終了處理
 */
bool EIVWMHandler::onDestroy()
{
  urania::System::quit(0);
  return true;
}


/**
 *  描畫處理
 */
void
EIVWMHandler::onPaint(urania::BasicWindow* win, urania::PaintDevice* pdev)
{
  (void)win;
  int bw, bh;
  EIViewer* eiv = EIViewer::get();

  if (eiv->getPaintDevice()) {
    if (eiv->getViewMode() == EIViewer::VIEW_ACTUAL_SIZE) {
      bw = eiv->getPaintDevice()->width();
      bh = eiv->getPaintDevice()->height();
      pdev->blt(
        0, 0, eiv->getPaintDevice(),
        eiv->getX(), eiv->getY(), bw - eiv->getX(), bh - eiv->getY());
    }
    else {
      pdev->clear(polymnia::RgbColor(255, 255, 255));
      pdev->blt(eiv->getPaintDevice());
    }
  }
  else if (eiv->getPaintDeviceIndexed()) {
    if (eiv->getViewMode() == EIViewer::VIEW_ACTUAL_SIZE) {
      bw = eiv->getPaintDeviceIndexed()->width();
      bh = eiv->getPaintDeviceIndexed()->height();
      pdev->blt(
        0, 0, eiv->getPaintDeviceIndexed(),
        eiv->getX(), eiv->getY(), bw - eiv->getX(), bh - eiv->getY());
    }
    else {
      pdev->clear(polymnia::RgbColor(255, 255, 255));
      pdev->blt(eiv->getPaintDeviceIndexed());
    }
  }
  else
  {
    pdev->clear(polymnia::RgbColor(255, 255, 255));
  }
}


/**
 *  ファイルドロップ時のメッセージハンドラ
 */
void
EIVWMHandler::onDropFiles(
  urania::Window* pw, std::vector<std::wstring>& fary, int x, int y)
{
  if (fary.size() != 1) {
    urania::System::notify(L"Error", L"You cannot drop multiple files.");
    return;
  }

  EIViewer::get()->openImage(pw, fary.front());
}


/**
 *  スクロール時のメッセージハンドラ
 */
void EIVWMHandler::onScroll(urania::Window* pw, int id, int pos)
{
  if (id == urania::ID_SBH)
    EIViewer::get()->setX(pw, pos);
  else
    EIViewer::get()->setY(pw, pos);
}


/**
 *  マウスホイールを廻した時のメッセージハンドラ
 */
void EIVWMHandler::onMouseWheel(
  urania::Window* pw, int delta, int key, int x, int y)
{
  pw->postMessage(WM_VSCROLL, (delta > 0) ? SB_PAGEUP : SB_PAGEDOWN, 0);
}

/**
 *  左ボタンを押したときのメッセージハンドラ
 */
bool EIVWMHandler::onLButtonDown(
  urania::Window* win, int x, int y, bool ctrl, bool shft,
  bool lb, bool mb, bool rb)
{
  EIViewer::get()->nextImage(win);
  return true;
}

/**
 *  右ボタンを押したときのメッセージハンドラ
 */
bool EIVWMHandler::onRButtonDown(
  urania::Window* win, int x, int y, bool ctrl, bool shft,
  bool lb, bool mb, bool rb)
{
  EIViewer::get()->prevImage(win);
  return true;
}




//eof
