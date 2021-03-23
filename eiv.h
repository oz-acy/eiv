/**************************************************************************
 *
 *  eiv.h
 *
 *  by oZ/acy
 *  (c) 2002-2019 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Easy Image Viewer
 *
 *  履歴
 *    2016.2.29  修正 (v 0.35)
 *    2016.10.12 修正 (v 0.36)
 *    2018.12.23 修正 (v 0.37)
 *    2019.4.24  修正 (v 0.38)
 *    2021.x.xx  修正 (v 0.39)
 *
 */
#ifndef INC_EIV_HEADER_
#define INC_EIV_HEADER_

#include <urania/window.h>
#include <urania/dialog.h>
#include <urania/cmndlg.h>
#include <filesystem>
#include <memory>
#include "eiv_res.h"


constexpr wchar_t EIVNAME[] = L"Easy Image Viewer";
constexpr wchar_t VERSTR[] = L"Ver. 0.39";
constexpr wchar_t COPYRIGHTSTR[]
 = L"Copyright © 2000-2021 oZ/acy. All Right Reserved.";




/*------------------------------------------------
 *  class EIViewer
 *  EIVアプリケーションクラス
 */
class EIViewer
{
public:
  enum ViewMode {
    VIEW_ACTUAL_SIZE = 0,
    VIEW_SCALING = 1
  };


private:
  static std::unique_ptr<EIViewer> eiv_S;

  std::wstring appTitle_;
  std::wstring wpPath_;

  std::filesystem::path tgt_;

  std::unique_ptr<urania::PaintMemDevice> qrgb_;
  std::unique_ptr<urania::PaintMemDeviceIndexed> pvd_;
  int vx_, vy_;
  bool scrX_, scrY_;
  std::unique_ptr<urania::FileDialog> fdlg_;

  ViewMode vmode_;


private:
  EIViewer();

public:
  ~EIViewer() {}

  static EIViewer* get();

  urania::FileDialog* fileDialog() { return fdlg_.get(); }

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
  static void onMenuSortByName(urania::Window* win);
  static void onMenuSortByTime(urania::Window* win);
  static void onMenuActualSize(urania::Window* win);
  static void onMenuScaling(urania::Window* win);

  void setX(urania::Window* pw, int x);
  void setY(urania::Window* pw, int y);

  void openImage(urania::Window* pw, const std::wstring& path);
  void nextImage(urania::Window* pw);
  void prevImage(urania::Window* pw);
  void loadImage(urania::Window* pw);
  void saveImage(urania::Window* pw, const std::wstring& path);

  void sizeHandleAndMore(urania::Window* pw);
  void handleMenu(urania::Window* pw);

  void to256(urania::Window* pw);
  void toGrayScale(urania::Window* pw);

  void toWallPaper();

  ViewMode getViewMode() const { return vmode_; }
  void setViewMode(ViewMode v) { vmode_ = v; }
};


/*-----------------------------------------------------
 *  EIVWMHManager
 *  メインウィンドウ用のメッセージマネジャー
 */
class EIVWMHandler : public urania::WMHandler
{
public:
  bool onDestroy() override;
  bool onSize(urania::Window* win, int typ, int w, int h) override;
  bool onKeyDown(urania::Window* win, int code, int rep, bool prev) override;
  bool onLButtonDown(
    urania::Window* win, int x, int y, bool ctrl, bool shft,
    bool lb, bool mb, bool rb) override;
  bool onRButtonDown(
    urania::Window* win, int x, int y, bool ctrl, bool shft,
    bool lb, bool mb, bool rb) override;
  void onPaint(urania::BasicWindow* win, urania::PaintDevice* pd) override;
  void onScroll(urania::Window* win, int id, int pos) override;
  void onMouseWheel(
    urania::Window* win, int delta, int key, int x, int y) override;

  void onDropFiles(
    urania::Window* win, std::vector<std::wstring>& fa, int x, int y) override;
};


/* 雜多な函數  */
//std::wstring getFileExt(const std::wstring& file);
//std::wstring getFileTitle(const std::wstring& path);


#endif
