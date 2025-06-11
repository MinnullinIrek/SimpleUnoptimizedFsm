# SimpleUnoptimizedFsm
Simple unoptimized Fsm on  C++

#using:

```cpp
enum class Event { Start, Stop, Error };

// States:
struct IdleState : State {
    void onEnter() override { std::cout << "IdleState: Entering idle state\n"; }
    void onExit() override { std::cout << "IdleState: Exiting idle state\n"; }
    void onUpdate(const float dt = 0.f) override {}
};

struct ActiveState : State {
    void onEnter() override { std::cout << "ActiveState: Entering active state\n"; }
    void onExit() override { std::cout << "ActiveState: Exiting active state\n"; }
    void onUpdate(const float dt = 0.f) override {}
};

struct ErrorState : State {
    void onEnter() override { std::cout << "ErrorState: Entering error state\n"; }
    void onExit() override { std::cout << "ErrorState: Exiting error state\n"; }
    void onUpdate(const float dt = 0.f) override {}
};


int main() {
    FSM<Event> fsm;

    // adding states
    fsm.addState<IdleState>();
    fsm.addState<ActiveState>();
    fsm.addState<ErrorState>();

    // Set Iddle state
    fsm.setInitialState<IdleState>();

    // adding transactions
    fsm.addTransition<IdleState, ActiveState>(Event::Start);
    fsm.addTransition<ActiveState, IdleState>(Event::Stop);
    fsm.addTransition<ActiveState, ErrorState>(Event::Error);

    // events handling
    fsm.handleEvent(Event::Start);  //  Idle -> Active
    fsm.handleEvent(Event::Stop);  // Active -> Idle
    fsm.handleEvent(Event::Error); // no Idle to Error. Still Idle

    fsm.update(0); 

    return 0;
}

```