/***************************************************************************
                          EmFont.h  -  description
                             -------------------
    begin                : Mon Jul 23 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef EMFONT_H
#define EMFONT_H

//#define EM_FONT_X_2 17
//#define EM_FONT_Y_2 13

#ifndef PRIVATE_H
#error Must include Private.h before EmFont.h
#endif

#define EM_FONTSIZE_X 0.06f*EM_RIGHT
#define EM_FONTSIZE_Y 0.08f*EM_UP

#include "TextureUtil.h"

class Config;

/** A singleton utility for fonts. */
class EmFont {
 public:
  ~EmFont();
  /** Get singleton object. */
  static EmFont * getInstance();
  void loadFont(const char * fileName);
  /** If row is negative the row will be counted from bottom of screen. */
  void printRow(const char * buffer, float row);
  /** If row is negative the row will be counted from bottom of screen. */
  void printRowCenter(const char * buffer, float row);
 protected:
  /** With OpenGL -1,1 is upper left. 1,-1 is lower right. */
  void print(const char * buffer, float x, float y);
  EmFont();
 private:
  //EmImage * m_Font;
  EmTexture * m_Texture;
  int m_iSize;
  float m_fFontSizeX;
  float m_fFontSizeY;
  Config * p_Config;
  static EmFont * p_EmFont;
};

#endif // EMFONT_H
