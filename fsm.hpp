#pragma once
#include <functional>
#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

/// Base State class
struct State {
  virtual ~State() = default;
  virtual void onEnter() {}
  virtual void onExit() {}
  virtual void onUpdate(const float dt = 0.f) {}
};

template <typename Event>
class FSM {
 private:
  std::unordered_map<std::type_index, std::shared_ptr<State>> states;
  std::unordered_map<std::type_index, std::unordered_map<Event, std::function<void()>>> transitions;
  std::shared_ptr<State> currentState;

 public:
  /// adding states
  template <typename StateType, typename... Args>
  void addState(Args&&... args) {
    states[std::type_index(typeid(StateType))] = std::make_shared<StateType>(std::forward<Args>(args)...);
  }

  /// setting start state
  template <typename StateType>
  void setInitialState() {
    auto it = states.find(std::type_index(typeid(StateType)));
    if (it != states.end()) {
      currentState = it->second;
      currentState->onEnter();
    } else {
      throw std::runtime_error("State not found");
    }
  }

  // add transaction
  template <typename FromState, typename ToState>
  void addTransition(Event event) {
    transitions[std::type_index(typeid(FromState))][event] = [this]() {
      auto& fromState = currentState;
      auto toStateIt = states.find(std::type_index(typeid(ToState)));
      if (toStateIt != states.end()) {
        fromState->onExit();
        currentState = toStateIt->second;
        currentState->onEnter();
      } else {
        throw std::runtime_error("Target state not found");
      }
    };
  }

  // handle event
  void handleEvent(Event event) {
    auto it = transitions.find(std::type_index(typeid(*currentState)));
    if (it != transitions.end()) {
      if (auto itEvent = it->second.find(event); itEvent != it->second.end()) {
        itEvent->second();
      }
    }
    // else {
    //     std::cout << "No transition defined for current state\n";
    // }
  }

  void update(const float dt = 0.f) {
    if (!currentState) {
      throw std::runtime_error("Current state not initialized");
    }
    currentState->onUpdate(dt);
  }
};
