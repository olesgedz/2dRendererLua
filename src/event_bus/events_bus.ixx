module;

#include <list>
#include <map>
#include <memory>
#include <typeindex>

export module event_bus;
import logger;
import event;


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
private:
  typedef void (TOwner::*CallbackFunction)(TEvent&);

  TOwner* ownerInstance;
  CallbackFunction _callbackFunction;

  virtual void _call(Event& e) override {
    std::invoke(_callbackFunction, ownerInstance, static_cast<TEvent&>(e));
  }

public:
  EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
    this->ownerInstance = ownerInstance;
    this->_callbackFunction = callbackFunction;
  }

  virtual ~EventCallback() override = default;
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
  // Example: eventBus->subscribeToEvent<CollisionEvent>(this, &Game::onCollision);
  // void (TOwner::*callBackFunction)(TEvent& event) is pointer for a callback function from class TOwner 
  //
  template <typename TEvent, typename TOwner>
  void subscribeToEvent(TOwner* ownerInstance, void (TOwner::*callBackFunction)(TEvent& event)) {
    if (!_subscribers[typeid(TEvent)].get()) {
      _subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
    }
    auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callBackFunction);
    _subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
  }

  //
  // Emit an event of type <T>
  // As soon as something emits an event
  // we go ahead and execute all the listener callback functions
  // Example: eventBus->emitEvent<CollisionEvent>(player, enemy);
  //
  template <typename TEvent, typename... TArgs>
  void emitEvent(TArgs&&... args) {
    auto handlers = _subscribers[typeid(TEvent)].get();
    if (handlers) {
      for (auto it = handlers->begin(); it != handlers->end(); it++) {
        auto handler = it->get();

        TEvent event(std::forward<TArgs>(args)...);
        handler->execute(event);
      }
    }
  }

  void reset() {
    _subscribers.clear();
  }

private:
  std::map<std::type_index, std::unique_ptr<HandlerList>> _subscribers;
};