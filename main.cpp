/**
 * 	Vojenská simulace "mobilizace"
 *
 * @author Alena Klimecká (xklimec47), Tony Pham (xphamt00)
 * @version 1.0
 */

#include <iostream>
#include <getopt.h>
#include "simlib.h"
using namespace std;
long vehicles = 40;
long actual_base = 0;

/**
 * Struktura uchovávajíci data o základně.
 *
 * @struct army_base
 */
typedef struct army_base {
    string name_of_base;
    long soldiers = 0;
    long vehicles = 0;
    double path_duration = 0;
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
Store warehouse_f("Místo ve skladu", 80);
Store mechanics ("Mechanici připravující tatry", 4);
Facility platform("Nakládací platforma");
Histogram prepare_time("Doba přípravy vojáka",0,10,10);
int person_ready = 0;
int city = 0;
// počet základen v poli
army_base cities[16];
class Trans : public Process {
    void Behavior() {
        int start_time = Time;
        // military entry
        Wait(Exponential(5));
        // warehouse
        Enter(warehouse_f,1);
        Wait(Exponential(3));
        Leave(warehouse_f,1);
        // suit up
        Wait(Exponential(3));
        person_ready++;
        prepare_time(Time-start_time);
    };
};

class Vehicle : public Process {
    void Behavior() {
        while (1) {
            // warehouse
            int check = 0;
            Enter(mechanics, 1);
            Wait(Exponential(5));
            Leave(mechanics, 1);
            Wait(Exponential(2));
            Seize(platform); // postavi se na rampu

            // bere 40 vojaku
            for (int a=0; a<40; a++){
                check_p:
                if(person_ready>0){
                    Wait(Exponential(1));
                    person_ready--;
                }
                    // wait 20 min and leave
                else{
                    if(check==20){
                        break;
                    }
                    Wait(Exponential(1));
                    check+=1;
                    goto check_p;
                }
            }
            if(check==20){
                Release(platform);
                break;
            }

            Release(platform);
            Wait(cities[actual_base].path_duration);
            city += 40;
            Wait(cities[actual_base].path_duration);
        }
    };
};

int main(int argc, char *argv[]) {
    // defaultni hodnoty
    long pluk_min = 300;
    long pluk_max = 1250;
    long prapor_min = 900;
    long prapor_max = 2000;
    double time_until_end = 24*60;

    char *check;
    int option;
    string unit;
    string  number_of_unit;
    // zkontroluju vsechny argumenty
    while ((option = getopt(argc, argv, "S:V:T:B:")) != -1) {
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
            case 'B':
                actual_base = strtol(optarg, &check, 10);
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
                cities[i].name_of_base = "Bechyne";
                cities[i].path_duration = Exponential(120);
                cities[i].soldiers = (2 * prapor_min) + (rand() % (2 * prapor_max - 2 * prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 1: // Bucovice
                cities[i].name_of_base = "Bechyne";
                cities[i].path_duration = Exponential(29);
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 2: // Hranice
                cities[i].name_of_base = "Hranice";
                cities[i].path_duration = Exponential(16);
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 3: // Chrudim
                cities[i].name_of_base = "Chrudim";
                cities[i].path_duration = Exponential(87);
                cities[i].soldiers = pluk_min + (rand() % (pluk_max - pluk_min));
                cities[i].vehicles = vehicles;
                break;
            case 4: // Jince
                cities[i].name_of_base = "Jince";
                cities[i].path_duration = Exponential(167);
                cities[i].soldiers = pluk_min + (rand() % (pluk_max - pluk_min));
                cities[i].vehicles = vehicles;
                break;
            case 5: // Jindruchuv_hradec
                cities[i].name_of_base = "Jindruchuv_hradec";
                cities[i].path_duration = Exponential(108);
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 6: // Klatovy
                cities[i].name_of_base = "Klatovy";
                cities[i].path_duration = Exponential(220);
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 7: // Liberec
                cities[i].name_of_base = "Liberec";
                cities[i].path_duration = Exponential(190);
                cities[i].soldiers = (2 * prapor_min) + (rand() % (2 * prapor_max - 2 * prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 8: // Lipnik_nad_becvou
                cities[i].name_of_base = "Lipnik_nad_becvou";
                cities[i].path_duration = Exponential(24);
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 9: // Olomouc
                cities[i].name_of_base = "Olomouc";
                cities[i].path_duration = Exponential(45);
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 10: // Opava
                cities[i].name_of_base = "Opava";
                cities[i].path_duration = Exponential(38);
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 11: // Pardubice
                cities[i].name_of_base = "Pardubice";
                cities[i].path_duration = Exponential(100);
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 12: // Prostejov
                cities[i].name_of_base = "Prostejov";
                cities[i].path_duration = Exponential(50);
                cities[i].soldiers = (2 * prapor_min) + (rand() % (2 * prapor_max - 2 * prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 13: // Praslavice
                cities[i].name_of_base = "Praslavice";
                cities[i].path_duration = Exponential(40);
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 14: // Tabor
                cities[i].name_of_base = "Tabor";
                cities[i].path_duration = Exponential(111);
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            case 15: // Zatec
                cities[i].name_of_base = "Zatec";
                cities[i].path_duration = Exponential(192);
                cities[i].soldiers = prapor_min + (rand() % (prapor_max - prapor_min));
                cities[i].vehicles = vehicles;
                break;
            default:
                break;
        }
    }
    Init(0, time_until_end);
    person_ready = 0;
    for (int i=0; i<cities[actual_base].soldiers; i++){
        (new Trans)->Activate();
    }
    for (int i=0; i<vehicles; i++){
        (new Vehicle)->Activate();
    }
    Run();

    std::cout<< "Město ve kterém je základna: " << cities[actual_base].name_of_base<< "\n";
    std::cout<< "Původní počet vojáků na základně: " << cities[actual_base].soldiers<< "\n";
    std::cout<<"Vojáci v cílové destinaci: " <<city<< "\n";
    std::cout<<"Vojáci co zůstali na základně: "<< cities[actual_base].soldiers-city<<"\n";
    warehouse_f.Output();
    prepare_time.Output();
}