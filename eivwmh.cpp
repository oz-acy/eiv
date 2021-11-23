/**************************************************************************
 *
 *  eivwmh.cpp
 *  by oZ/acy
 *  (c) 2002-2021 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *  class EIVWMHandler
 *
 *  @date 2016.10.12 onLButtonDown()、onRButtonDown()を追加
 *  @date 2021.3.23 onPaint()、onMouseWheel()を修正
 *  @date 2021.11.23 libpolymnia+libthemisからlibeunomiaに切り替へ
 */
#include "eiv.h"
#include <eunomia/dibio.h>
#include <eunomia/pngio.h>
#include <eunomia/jpegio.h>


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

  auto bgcolor = eunomia::RgbColour(0, 0, 0);

  if (eiv->getPaintDevice()) {
    if (eiv->getViewMode() == EIViewer::VIEW_ACTUAL_SIZE) {
      bw = eiv->getPaintDevice()->width();
      bh = eiv->getPaintDevice()->height();
      pdev->blt(
        *eiv->getPaintDevice(), 
          eiv->getX(), eiv->getY(), bw - eiv->getX(), bh - eiv->getY(),
        0, 0);
    }
    else {
      //pdev->clear(eunomia::RgbColour(255, 255, 255));
      pdev->clear(bgcolor);
      pdev->blt(*eiv->getPaintDevice());
    }
  }
  else if (eiv->getPaintDeviceIndexed()) {
    if (eiv->getViewMode() == EIViewer::VIEW_ACTUAL_SIZE) {
      bw = eiv->getPaintDeviceIndexed()->width();
      bh = eiv->getPaintDeviceIndexed()->height();
      pdev->blt(
        *eiv->getPaintDeviceIndexed(),
          eiv->getX(), eiv->getY(), bw - eiv->getX(), bh - eiv->getY(),
        0, 0);
    }
    else {
      //pdev->clear(eunomia::RgbColour(255, 255, 255));
      pdev->clear(bgcolor);
      pdev->blt(*eiv->getPaintDeviceIndexed());
    }
  }
  else
  {
    //pdev->clear(eunomia::RgbColour(255, 255, 255));
    pdev->clear(eunomia::RgbColour(127, 127, 127));
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
  if (EIViewer::get()->getViewMode() == EIViewer::VIEW_ACTUAL_SIZE)
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
