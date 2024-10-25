module;

#include <list>
#include <map>
#include <memory>
#include <typeindex>

export module event_bus;
import logger;

export class Event {
};

class IEventCallBack {
};

template <typename TEvent>
class EventCallback : public IEventCallBack {
};

typedef std::list<std::unique_ptr<IEventCallBack>> HandlerList;

export class EventBus {
public:
  EventBus() {
    Logger::log("EventBus constructor called!");
  }

  ~EventBus() {
    Logger::log("EventBus destructor called!");
  }

  //
  // Subscribe to an event type <T
  // In our implementation, a listener subscribes to an event
  // Example: eventBus->subscribeToEvent<CollisionEvent>(&Game::onCollision);
  //
  template <typename EventType>
  void subscribeToEvent() {
  }

  //
  // Emit an event of type <T>
  // As soon as somthing emits an event
  // we go ahead and execute all the listener callback functions
  // Example: eventBus->emitEvent<CollisionEvent>(player, enemy);
  //
  template <typename EventType>
  void emitEvent() {
  }

private:
  std::map<std::type_index, std::unique_ptr<HandlerList>> _subscribers;
};