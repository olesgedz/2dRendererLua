module;

export module events:key_press_event;

import ecs;
import event_bus;
import event;
#include <SDL_keycode.h>

export class KeyPressEvent : public Event {
public:
  SDL_Keycode keycode;

  explicit KeyPressEvent(SDL_Keycode keycode): keycode(keycode) {
  }
};