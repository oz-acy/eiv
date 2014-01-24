/**************************************************************************
 *
 *  painthandler.h
 *
 *  by oZ/acy
 *  (c) 2009 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  PaintHandler of Easy Image Viewer
 *
 *  last update: 8 Nov MMIX
 *
 */
#include "eiv.h"
#include "painthandler.h"

void
PaintHandler::operator()(urania::BasicWindow* win, urania::PaintDevice* pdev)
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


//eof
