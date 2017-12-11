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

std::map<SDL_Keycode, bool> Keyboard::m_keyStatus;

Keyboard::Keyboard(){
	this->clear();
}

Keyboard::~Keyboard(){
}

void Keyboard::poll() {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      m_keyStatus[event.key.keysym.sym] = true;
    }
    if (event.type == SDL_KEYUP) {
      m_keyStatus[event.key.keysym.sym] = false;
    }
  }
}

void Keyboard::clear() {
  m_keyStatus.clear();
}

EMKey Keyboard::waitForKey() {
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
}

bool Keyboard::isKeyDown(int piKey) {
  if (piKey < 0) return false;
  std::map<SDL_Keycode, bool>::iterator keyStatusIterator = m_keyStatus.find(piKey);
  return keyStatusIterator != m_keyStatus.end() && keyStatusIterator->second;
}
