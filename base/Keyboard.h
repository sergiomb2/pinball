/***************************************************************************
                          Keyboard.h  -  description
                             -------------------
    begin                : Thu Feb 1 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef KEYBOARD_H
#define KEYBOARD_H

#ifndef PRIVATE_H
#error Must include Private.h before Keyboard.h
#endif

#include <SDL.h>
#define EMKey SDL_Keycode
#include  <map>

#ifndef KEY_MAX
#define KEY_MAX 512
#endif

/** @author Henrik Enqvist */

class Keyboard {
 public:
  Keyboard();
  ~Keyboard();
  static void clear();
  static void poll();
  static bool isKeyDown(int piKey);
  static EMKey waitForKey();
 private:
  static std::map<SDL_Keycode, bool> m_keyStatus;
};

#endif // KEYBOARD_H
