/**************************************************************************
 *
 *  util.cpp
 *  by oZ/acy
 *  (c) 2002-2009 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  履歴
 *    14 Feb 2004  作成
 *    28 Dec 2005  修正
 *     4 Apr 2009  修正
 *    15 Nov 2009  修正
 *************************************************************************/

#include <themis/strconv.h>


/*==========================================================
 *  getFileExt()
 *  ファイル名から擴張子を取り出し、小文字で返す
 *
 *  備考 : パスに適用時は擧動が正しくない場合がある
 *========================================================*/
std::wstring getFileExt(const std::wstring& file)
{
  std::wstring::size_type n = file.rfind(L'.');
  if (n != std::wstring::npos)
    return themis::StringConverter::toLower(file.substr(n + 1));
  else
    return L"";
}


/*=========================================================
 *  getFileTitle()
 *  ファイルパスからファイル名のみ取り出す
 *
 *  備考 : パス區切は DOS/Windows 準據
 *=======================================================*/
std::wstring getFileTitle(const std::wstring& path)
{
  std::wstring::size_type n = path.rfind('\\');
  if (n != std::wstring::npos)
    return path.substr(n + 1);
  else
    return path;
}




//eof
