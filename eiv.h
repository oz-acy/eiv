/**************************************************************************
 *
 *  eiv.h
 *
 *  by oZ/acy
 *  (c) 2002-2012 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *
 *  last update: 13 May MMXII
 *
 *************************************************************************/
#ifndef INC_EIV_HEADER_
#define INC_EIV_HEADER_

#include <urania/win.h>
#include <urania/dialog.h>
#include <urania/cmndlg.h>
#include <boost/scoped_ptr.hpp>
#include "eiv_res.h"


#ifdef EIV_PLUS
#define EIVNAME L"Easy Image Viewer PLUS"
#define VERSTR  L"Ver. 0.34+"
#else
#define EIVNAME L"Easy Image Viewer"
#define VERSTR  L"Ver. 0.34"
#endif

#define COPYRIGHTSTR L"Copyright (C) 2000-2012 oZ/acy. All Right Reserved."


/*------------------------------------------------
 *  class EIViewer
 *  EIVアプリケーションクラス
 */
class EIViewer
{
private:
  static boost::scoped_ptr<EIViewer> eiv_S;

  std::wstring appTitle_;
  std::wstring wpPath_;
  boost::scoped_ptr<urania::PaintMemDevice> qrgb_;
  boost::scoped_ptr<urania::PaintMemDeviceIndexed> pvd_;
  int vx_, vy_;
  bool scrX_, scrY_;
  boost::scoped_ptr<urania::OpenFileDialog> opn_;
  boost::scoped_ptr<urania::SaveFileDialog> svd_;


public:
  EIViewer();
  ~EIViewer() {}

  static EIViewer* get();

  urania::OpenFileDialog* openFileDlg() { return opn_.get(); }
  urania::SaveFileDialog* saveFileDlg() { return svd_.get(); }

  urania::PaintMemDevice* getPaintDevice() { return qrgb_.get(); }
  urania::PaintMemDeviceIndexed* getPaintDeviceIndexed() { return pvd_.get(); }
  int getX() const { return vx_; }
  int getY() const { return vy_; }

  static void onMenuOpen(urania::Window* win);
  static void onMenuSave(urania::Window* win);
  static void onMenuEnd(urania::Window* win);
  static void onMenuAbout(urania::Window* win);
  static void onMenuCnvTo256(urania::Window* win);
  static void onMenuCnvGS(urania::Window* win);
  static void onMenuWallCenter(urania::Window* win);
  static void onMenuWallTile(urania::Window* win);
  static void onMenuWallExt(urania::Window* win);

  static void setXPos(urania::Window* pw, int x);
  static void setYPos(urania::Window* pw, int y);

  void loadImage(urania::Window* pw, const std::wstring& file);
  void saveImage(urania::Window* pw, const std::wstring& file);

  void sizeHandleAndMore(urania::Window* pw);


  void to256(urania::Window* pw);
  void toGrayScale(urania::Window* pw);

  void toWallPaper(int mode);
};


/*-----------------------------------------------------
 *  EIVWMHManager
 *  メインウィンドウ用のメッセージマネジャー
 */
class EIVWMHManager : public urania::WMHManager
{
public:
  bool onDestroy();
  bool onSize(urania::Window* win, int typ, int w, int h);
  bool onKeyDown(urania::Window* win, int code, int rep, bool prev);
  void onPaint(urania::BasicWindow* win, urania::PaintDevice* pd);

  void onDropFiles(
    urania::Window* win, std::vector<std::wstring>& fa, int x, int y);

  void onScroll(urania::Window* win, int id, int pos);

};


/* 雜多な函數  */
std::wstring getFileExt(const std::wstring& file);
std::wstring getFileTitle(const std::wstring& path);


#endif
