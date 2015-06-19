#include "Army.hpp"
#include <cstdlib>
#include <time.h>
#include <iostream>
#include "training.hpp"
#include "fstream"


int main(void)
{
    std::srand(time(NULL));
    std::vector<std::unique_ptr<Army> > champions;
    try {
        std::ifstream in("Army_10_100.save");
        Army army = Army::load(in);
        std::cout<<army<<std::endl;
        champions.push_back(std::unique_ptr<Army>(new Army(army)));
    } catch(...) {

    }
    std::unique_ptr<Army> army = train(10,100, 20, 10, 100, champions);
    std::cout<<*army<<std::endl;
    std::ofstream out("Army_10_100.save");
    army->save(out);
    out.flush();
    out.close();
    return 0;
}