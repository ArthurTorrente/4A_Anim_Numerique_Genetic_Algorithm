#include "training.hpp"
#include "battle.hpp"
#include <memory>
#include <vector>
#include <iterator>


//struct storing together an army and its score,
//usefull for storing and sorting the armies
struct ArmyWrapper {
    int score;
    std::shared_ptr<Army> army;
    ArmyWrapper(Army* army_ptr):score(0),army(army_ptr) {}

    bool operator<(const ArmyWrapper& aw)const
    {
        return score > aw.score;
    }
};





//Train armies by genetic algorithm,  with fitness function obtained by tournament
//params :
// - iterations : the maximum number of iterations of the genetic algorithm
// - threshold : the score to reach for an army to be considered as winner
// - populationSize : number of armies in one iteration
// - armySize : number of units per army
// - unitLevel : global level of each unit in each army
// - champions : optionnal armies which have to be defeated
std::unique_ptr<Army> train(int iterations, int threshold, int populationSize, int armySize, int unitLevel, std::vector<std::unique_ptr<Army> >& champions)
{
    std::vector<ArmyWrapper> armies;
    int championsBonus = 1;
    if(!champions.empty())
        championsBonus =  1+(populationSize/champions.size());


    //generation of the initial population
    armies.reserve(populationSize);
    std::generate_n(std::back_inserter(armies), populationSize, [armySize, unitLevel]() {
        return new Army(armySize, unitLevel);
    });

    //start the training
    while(iterations--) {
        //tornament of all armies
        for(auto itA = armies.begin(); itA != armies.end(); ++itA) {

            for(auto itB = itA+1; itB != armies.end(); ++itB){
            	int sA, sB;
            	fight(*(itA->army), *(itB->army), sA, sB);
            	itA->score += sA;
            	itB->score += sB;
            }
            for(auto itC = champions.begin(); itC != champions.end(); ++itC) {
                int sA, sC;
                fight(*(itA->army), **itC, sA, sC);
                itA->score += sA*championsBonus;
                itA->score -= (sC*championsBonus)/2;
            }
        }


        //sort by score
        std::sort(armies.begin(), armies.end());

        std::cout<<"Remaining Iterations : "<<iterations<<" -> ";
        std::for_each(armies.begin(), armies.end(), [](const ArmyWrapper& aw) {
            std::cout<<aw.score<<" ";
        });
        std::cout<<std::endl;

        //if an army reach the fixed threshold, we are done
        if(armies[0].score>threshold)break;



        //generation of the new population
        std::vector<ArmyWrapper> newArmies;
        newArmies.reserve(populationSize);

        //copy the best armies
        int keepAmount = 0.1*populationSize;
        std::copy_n(armies.begin(), keepAmount, std::back_inserter(newArmies));
        std::for_each(newArmies.begin(), newArmies.end(), [](ArmyWrapper& aw) {
            aw.score = 0;
        });

        //crossing of the best armies
        int crossingAmount = 0.3*populationSize;
        auto stop = armies.begin() + crossingAmount;
        for(auto it = armies.begin(); it!=stop; ++it) {
            newArmies.push_back(new Army(*(it->army) * (*(it+(1+std::rand()%std::distance(it+1, armies.end())))->army)));
        }

        //mutations of the best armies
        int mutatedAmount = 0.3*populationSize;
        std::transform(armies.begin(), armies.begin()+mutatedAmount, std::back_inserter(newArmies),[](const ArmyWrapper& aw) {
            return new Army(aw.army->mutate());
        });

        //generate new random armies
        int newAmount = populationSize - (keepAmount+crossingAmount+mutatedAmount);
        std::generate_n(std::back_inserter(newArmies), newAmount, [armySize, unitLevel]() {
            return new Army(armySize, unitLevel);
        });

        //replace old population by new one
        std::swap(armies, newArmies);



    }
    return std::unique_ptr<Army>(new Army(*(armies[0].army)));
}

