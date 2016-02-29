/**************************************************************************
 *
 *  eiv.h
 *
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *
 *  履歴
 *    2016.2.29  修正 (v 0.35)
 *
 */
#ifndef INC_EIV_HEADER_
#define INC_EIV_HEADER_

#include <urania/win.h>
#include <urania/dialog.h>
#include <urania/cmndlg.h>
#include <memory>
#include "eiv_res.h"


constexpr wchar_t EIVNAME[] = L"Easy Image Viewer PLUS";
constexpr wchar_t VERSTR[] = L"Ver. 0.35";
constexpr wchar_t COPYRIGHTSTR[]
 = L"Copyright © 2000-2016 oZ/acy. All Right Reserved.";




/*------------------------------------------------
 *  class EIViewer
 *  EIVアプリケーションクラス
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
  std::unique_ptr<urania::FileDialog> fdlg_;
  //std::unique_ptr<urania::OpenFileDialog> opn_;
  //std::unique_ptr<urania::SaveFileDialog> svd_;

private:
  EIViewer();

public:
  ~EIViewer() {}

  static EIViewer* get();

  urania::FileDialog* fileDialog() { return fdlg_.get(); }
  //urania::OpenFileDialog* openFileDlg() { return opn_.get(); }
  //urania::SaveFileDialog* saveFileDlg() { return svd_.get(); }

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
  static void onMenuWallpaper(urania::Window* win);

  void setX(urania::Window* pw, int x);
  void setY(urania::Window* pw, int y);

  void loadImage(urania::Window* pw, const std::wstring& file);
  void saveImage(urania::Window* pw, const std::wstring& file);

  void sizeHandleAndMore(urania::Window* pw);
  void handleMenu(urania::Window* pw);

  void to256(urania::Window* pw);
  void toGrayScale(urania::Window* pw);

  void toWallPaper();
};


/*-----------------------------------------------------
 *  EIVWMHManager
 *  メインウィンドウ用のメッセージマネジャー
 */
class EIVWMHandler : public urania::WMHandler
{
public:
  bool onDestroy();
  bool onSize(urania::Window* win, int typ, int w, int h);
  bool onKeyDown(urania::Window* win, int code, int rep, bool prev);
  void onPaint(urania::BasicWindow* win, urania::PaintDevice* pd);
  void onScroll(urania::Window* win, int id, int pos);
  void onMouseWheel(urania::Window* win, int delta, int key, int x, int y);

  void onDropFiles(
    urania::Window* win, std::vector<std::wstring>& fa, int x, int y);
};


/* 雜多な函數  */
std::wstring getFileExt(const std::wstring& file);
std::wstring getFileTitle(const std::wstring& path);


#endif
