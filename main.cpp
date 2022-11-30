#include <iostream>
#include "simlib.h"
using namespace std;

typedef struct army_base {
    int soldiers = 0;
    int vehicles = 0;
    int time_of_first = 0;
    int time_of_second = 0;
} base;

typedef struct army_bases {
    struct army_base Bechyne;
    struct army_base Bucovice;
    struct army_base Hranice;
    struct army_base Chrudim;
    struct army_base Jince;
    struct army_base Jindruchuv_hradec;
    struct army_base Klatovy;
    struct army_base Liberec;
    struct army_base Lipnik_nad_becvou;
    struct army_base Olomouc;
    struct army_base Opava;
    struct army_base Pardubice;
    struct army_base Praha_Kbely;
    struct army_base Prostesjov;
    struct army_base Praslavice;
    struct army_base Sedlec;
    struct army_base Tabor;
    struct army_base Zatec;
} bases;


Facility Linka("Obsluzna linka");
Stat dobaObsluhy("Doba obsluhy na lince");
Histogram dobaVSystemu("Celkova doba v systemu", 0, 40, 20);

int bezCekani = 0;

class Transakce : public Process {
    void Behavior() {
        double tvstup = Time;
        double obsluha;

        if (!Linka.Busy()) bezCekani++;

        Seize(Linka);
        obsluha = Exponential(10); Wait(obsluha); dobaObsluhy(obsluha);
        Release(Linka);
        dobaVSystemu(Time - tvstup);
    }
};

class Generator : public Event {
    void Behavior() {
        (new Transakce)->Activate();
        Activate(Time + Exponential(11));
    }
};

int main()
{
    Init(0, 100000);
    (new Generator)->Activate();
    Run();

    dobaObsluhy.Output();
    Linka.Output();
    dobaVSystemu.Output();

    Print("Bez cekani: %d\n", bezCekani);
}