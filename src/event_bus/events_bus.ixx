module;

#include <list>
#include <map>
#include <memory>
#include <typeindex>

export module event_bus;
import logger;

export class Event {
public:
  Event() = default;
};

class IEventCallBack {
public:
  void execute(Event& event) {
    _call(event);
  }

  virtual ~IEventCallBack() = default;

private:
  virtual void _call(Event& event) = 0;
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallBack {
public:

private:
  typedef void (TOwner::*CallbackFunction)(TEvent&);

  TOwner* ownerInstance;
  CallbackFunction _callbackFunction;

  virtual void _call(Event& event) override {
    std::invoke(_callbackFunction, ownerInstance, static_cast<TEvent&>(event));
  }

public:
  EventCallback(TOwner* eventInstance, CallbackFunction callbackFunction) {
    this->ownerInstance = ownerInstance;
    this->_callbackFunction = callbackFunction;
  }
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