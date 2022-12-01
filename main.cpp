/**
 * 	Vojenská simulace "mobilizace"
 *
 * @author Alena Klimecká (xklimecXX), Tony Pham (xphamt00)
 * @version 1.0
 */

#include <iostream>
#include <getopt.h>
#include "simlib.h"
using namespace std;

/**
 * Struktura uchovávajíci data o základně.
 *
 * @struct army_base
 */
typedef struct army_base {
    long soldiers = 0;
    long vehicles = 0;
    long time_of_first = 0;
    long time_of_second = 0;
} base;

/**
 * Funkce co ukonci program a vypise spravu na stderr.
 *
 * @param error_code vystupni hodnota
 * @param message popis chyby
 */
void error_exit(int error_code, const std::string &message) {
    std::cerr << "ERROR: " << message << "\n";
    std::exit(error_code);
}

/**
 * Funknce rozdeli string podle posledni ':'.
 *
 * @param str string co chceme rozdelit
 * @param port cast stringu obsahujici port
 */
void SplitArg(const string &str, string &unit, string &number) {
    std::size_t found = str.find_last_of(":\\");
    unit = str.substr(0, found);
    number = str.substr(found + 1);
}
Store warehouse_f("Misto ve skladu", 40);
Store mechanics ("Mechanicic připravující tatry", 4);
Facility rampa("Nakladaci rampa");
int person_ready = 0;
int city = 0;

class Trans : public Process {
    void Behavior() {
            // military entry
            Wait(Exponential(5));
            // warehouse
            Enter(warehouse_f,1);
            Wait(Exponential(5));
            Leave(warehouse_f,1);
            // suit up
            Wait(Exponential(3));
            person_ready++;
    };
};

class Vehicle : public Process {
    void Behavior() {
        while (1) {
        // warehouse
        Enter(mechanics, 1);
        Wait(Exponential(5));
        Leave(mechanics, 1);
        Wait(Exponential(2));
        Seize(rampa); // postavi se na rampu
            // bere 40 vojaku
            for (int a=0; a<40; a++) {
                _WaitUntil(person_ready>0); // ceka na hotove vojaky
                person_ready--;
                Wait(Exponential(10)); // nalozi ji
            }
            Release(rampa);
            Wait(60);
            city += 40;
            Wait(60);
        }
    };
};
int main(int argc, char *argv[]) {
    // počet základen v poli
    army_base cities[16];
    // defaultni hodnoty
    long pluk_min = 300;
    long pluk_max = 1500;
    long prapor_min = 900;
    long prapor_max = 3050;
    long vehicles = 40;
    double time_until_end = 24*60;

    char *check;
    int option;
    string unit;
    string  number_of_unit;
    // zkontroluju vsechny argumenty
    while ((option = getopt(argc, argv, "S:V:T:")) != -1) {
        string parse;
        switch (option) {
            case 'S':
                parse = optarg;
                SplitArg(parse, unit, number_of_unit);
                if(unit == "pluk_min") {
                    long tmp = strtol(number_of_unit.c_str(), &check, 10);
                    if (tmp > pluk_max)
                        error_exit(1, "min can not be bigger than max");
                    pluk_min = tmp;
                } else if (unit == "pluk_max") {
                    long tmp = strtol(number_of_unit.c_str(), &check, 10);
                    if (tmp < pluk_min)
                        error_exit(1, "max can not be smaller than min");
                    pluk_max = tmp;
                } else if (unit == "prapor_min") {
                    long tmp = strtol(number_of_unit.c_str(), &check, 10);
                    if (tmp > prapor_max)
                        error_exit(1, "min can not be bigger than max");
                    prapor_min = tmp;
                } else if (unit == "prapor_max") {
                    long tmp = strtol(number_of_unit.c_str(), &check, 10);
                    if (tmp < prapor_min)
                        error_exit(1, "max can not be smaller than min");
                    prapor_max = tmp;
                }
                break;

            case 'V':
                vehicles = strtol(optarg, &check, 10);
                break;
            case 'T':
                time_until_end = stod(optarg);
                break;
                // Pokud najdu něco co tam nepaatří.
            default:
                error_exit(1, "Wrong arguments");
                break;
        }
    }


    for (int i = 0; i < 16; i++) {
        srand(i);
        switch (i) {
            case 0: // Bechyne
                cities[i].time_of_first = 120;
                cities[i].soldiers = (2 * prapor_min) + (rand() % (2 * prapor_max - 2 * prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 1: // Bucovice
                cities[i].time_of_first = 29;
                cities[i].time_of_second = 45;
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 2: // Hranice
                cities[i].time_of_first = 16;
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 3: // Chrudim
                cities[i].time_of_first = 87;
                cities[i].soldiers = pluk_min + (rand() % (pluk_max - pluk_min));
                cities[i].vehicles = vehicles;
                break;
            case 4: // Jince
                cities[i].time_of_first = 167;
                cities[i].soldiers = pluk_min + (rand() % (pluk_max - pluk_min));
                cities[i].vehicles = vehicles;
                break;
            case 5: // Jindruchuv_hradec
                cities[i].time_of_first = 108;
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 6: // Klatovy
                cities[i].time_of_first = 220;
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 7: // Liberec
                cities[i].time_of_first = 190;
                cities[i].soldiers = (2 * prapor_min) + (rand() % (2 * prapor_max - 2 * prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 8: // Lipnik_nad_becvou
                cities[i].time_of_first = 24;
                cities[i].time_of_second = 47;
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 9: // Olomouc
                cities[i].time_of_first = 45;
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 10: // Opava
                cities[i].time_of_first = 38;
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 11: // Pardubice
                cities[i].time_of_first = 100;
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 12: // Prostejov
                cities[i].time_of_first = 83;
                cities[i].time_of_second = 50;
                cities[i].soldiers = (2 * prapor_min) + (rand() % (2 * prapor_max - 2 * prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 13: // Praslavice
                cities[i].time_of_first = 52;
                cities[i].time_of_second = 40;
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 14: // Tabor
                cities[i].time_of_first = 111;
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 15: // Zatec
                cities[i].time_of_first = 192;
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            default:
                break;
        }
    }

    Init(0, time_until_end);

    // generate military (300-1500)
    for (int i=0; i<600; i++){
        (new Trans)->Activate();
    }

    // generate military (300-1500)
    for (int i=0; i<vehicles; i++){
        (new Vehicle)->Activate();
    }

    Run();
    warehouse_f.Output();
    mechanics.Output();
    std::cout<<person_ready<< "\n";
    std::cout<<city<< "\n";
}