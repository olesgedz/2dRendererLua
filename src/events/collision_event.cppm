module;

export module events:collision_event;

import ecs;
import event_bus;

export class CollisionEvent : public Event {
public:
  Entity a;
  Entity b;

  CollisionEvent(Entity a, Entity b): a(a), b(b) {
  }
};