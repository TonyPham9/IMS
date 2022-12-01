#include <iostream>
#include "simlib.h"

Store warehouse_f("Misto ve skladu", 40);
int person_ready = 0;

class Trans : public Process {
    void Behavior() {
        // prepare cycle

        // military entry
        Wait(Exponential(5));
        // warehouse
        Enter(warehouse_f, 1);
        Wait(Exponential(5));
        Leave(warehouse_f, 1);
        // suit up
        Wait(Exponential(3));
        person_ready++;
    };
};

int main() {
    Init(0, 60);

    // generate military (300-1500)
    for (int i = 0; i < 600; i++) {
        (new Trans)->Activate();
    }

    Run();
    warehouse_f.Output();
    std::cout << person_ready;
}