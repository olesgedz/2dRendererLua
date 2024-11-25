module;

#include <SDL_keycode.h>

export module key_press_event;

import ecs;
import event_bus;
import event;

export class KeyPressEvent : public Event {
public:
  SDL_Keycode symbol;

  KeyPressEvent(SDL_Keycode symbol): symbol(symbol) {
  }
};