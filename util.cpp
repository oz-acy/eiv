/**************************************************************************
 *
 *  util.cpp
 *  by oZ/acy
 *  (c) 2002-2009 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  ����
 *    14 Feb 2004  �쐬
 *    28 Dec 2005  �C��
 *     4 Apr 2009  �C��
 *    15 Nov 2009  �C��
 *************************************************************************/

#include <themis/strconv.h>


/*==========================================================
 *  getFileExt()
 *  �t�@�C�������睰���q�����o���A�������ŕԂ�
 *
 *  ���l : �p�X�ɓK�p���͝������������Ȃ��ꍇ������
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
 *  �t�@�C���p�X����t�@�C�����̂ݎ��o��
 *
 *  ���l : �p�X���؂� DOS/Windows ����
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
