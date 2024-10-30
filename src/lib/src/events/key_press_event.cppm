module;

export module events:key_press_event;

import ecs;
import event_bus;
import event;
#include <SDL_keycode.h>

export class KeyPressEvent : public Event {
public:
  SDL_Keycode symbol;

  KeyPressEvent(SDL_Keycode symbol): symbol(symbol) {
  }
};