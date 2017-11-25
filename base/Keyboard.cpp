/***************************************************************************
                          Keyboard.cpp  -  description
                             -------------------
    begin                : Thu Feb 1 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <cstring>
#include "Private.h"
#include "Keyboard.h"

#if EM_USE_SDL
std::map<SDL_Keycode, bool> Keyboard::m_keyStatus;
#endif

Keyboard::Keyboard(){
	this->clear();
}

Keyboard::~Keyboard(){
}

void Keyboard::poll() {
#if EM_USE_SDL
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      m_keyStatus[event.key.keysym.sym] = true;
    }
    if (event.type == SDL_KEYUP) {
      m_keyStatus[event.key.keysym.sym] = false;
    }
  }
#endif
#if EM_USE_ALLEGRO
  poll_keyboard();
#endif
}

void Keyboard::clear() {
#if EM_USE_SDL
  m_keyStatus.clear();
#endif
#if EM_USE_ALLEGRO
  clear_keybuf();
#endif
}

EMKey Keyboard::waitForKey() {
#if EM_USE_SDL
  while(true) {
    SDL_Event event;
    SDL_WaitEvent(&event);
    if (event.type == SDL_KEYDOWN && event.key.state == SDL_PRESSED) {
      break;
    }
  }
  while (true) {
    SDL_Event event;
    SDL_WaitEvent(&event);
    if (event.type == SDL_KEYUP && event.key.state == SDL_RELEASED) {
      return event.key.keysym.sym;
    }
  }
#endif
#if EM_USE_ALLEGRO
  // TODO
  return (readkey() >> 8);
#endif
}

bool Keyboard::isKeyDown(int piKey) {
  if (piKey < 0) return false;
#if EM_USE_SDL
  std::map<SDL_Keycode, bool>::iterator keyStatusIterator = m_keyStatus.find(piKey);
  return keyStatusIterator != m_keyStatus.end() && keyStatusIterator->second;
#endif
#if EM_USE_ALLEGRO
  return key[piKey];
#endif
}
