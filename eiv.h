/**************************************************************************
 *
 *  eiv.h
 *
 *  by oZ/acy
 *  (c) 2002-2014 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *
 *  last update: 25 Jan MMXIV
 *
 */
#ifndef INC_EIV_HEADER_
#define INC_EIV_HEADER_

#include <urania/win.h>
#include <urania/dialog.h>
#include <urania/cmndlg.h>
#include <memory>
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
 *  EIV�A�v���P�[�V�����N���X
 */
class EIViewer
{
private:
  static std::unique_ptr<EIViewer> eiv_S;

  std::wstring appTitle_;
  std::wstring wpPath_;
  std::unique_ptr<urania::PaintMemDevice> qrgb_;
  std::unique_ptr<urania::PaintMemDeviceIndexed> pvd_;
  int vx_, vy_;
  bool scrX_, scrY_;
  std::unique_ptr<urania::OpenFileDialog> opn_;
  std::unique_ptr<urania::SaveFileDialog> svd_;

private:
  EIViewer();

public:
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

  void setX(urania::Window* pw, int x);
  void setY(urania::Window* pw, int y);

  void loadImage(urania::Window* pw, const std::wstring& file);
  void saveImage(urania::Window* pw, const std::wstring& file);

  void sizeHandleAndMore(urania::Window* pw);
  void handleMenu(urania::Window* pw);

  void to256(urania::Window* pw);
  void toGrayScale(urania::Window* pw);

  void toWallPaper(int mode);
};


/*-----------------------------------------------------
 *  EIVWMHManager
 *  ���C���E�B���h�E�p�̃��b�Z�[�W�}�l�W���[
 */
class EIVWMHandler : public urania::WMHandler
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


/* 趑��Ȕ���  */
std::wstring getFileExt(const std::wstring& file);
std::wstring getFileTitle(const std::wstring& path);


#endif
