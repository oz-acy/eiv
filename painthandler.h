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
#ifndef INC_EIV_PAINT_HANDLER_
#define INC_EIV_PAINT_HANDLER_

#include <urania/win.h>

class PaintHandler : public urania::WMsgHandler
{
private:
  LRESULT operator()(urania::WndMessage* msg)
  {
    return msg->window->onPaint(*this, msg->wparam, msg->lparam);
  }

public:
  void operator()(urania::BasicWindow* pw, urania::PaintDevice* pdev);
};


#endif // INC_EIV_PAINT_HANDLER_
