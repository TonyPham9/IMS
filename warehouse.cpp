#include <iostream>
#include "simlib.h"

Facility warehouse[40];
Queue Q;
class Trans : public Process {
    void Behavior() {
        //double time_start = Time;
        double equip;

        int place = -1;
        back:
        for (int i=0; i<40; i++){
            if (!warehouse[i].Busy()){ // when is free get position
                place = i;
                break;
            }
        }

        // if everything is busy back to queue
        if(place == -1){
            Into(Q);
            Passivate();
            goto back;
        }

        Seize(warehouse[place]);
        equip = Exponential(5);
        Wait(equip);
        Release(warehouse[place]);

        if (Q.Length()>0){
            (Q.GetFirst())->Activate(); // planed
        }

        //sys_time(Time - time_start);
    };
};

class Generator : public Event {
    void Behavior() {
        (new Trans)->Activate();
        //Activate(Time + Exponential(11));
    }
};

int main()
{
    Init(0, 100000);
    (new Generator)->Activate();
    Run();

}