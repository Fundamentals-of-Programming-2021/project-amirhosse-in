#include "game.h"

struct Soldier{
    int x;
    int y;
    int speed;
    int team;
    
};

//0 means white
struct City{
    int x;
    int y;
    int team;
    int soldier_counts;
    int growth_rate;
}
