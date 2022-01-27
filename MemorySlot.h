
using namespace std;

class MemorySlot{

    public:
    int number_of_plays;
    int number_of_cooperations_perceived;    //that the opponent made
    int number_of_defections_perceived;
    int number_of_cooperations_heard;   //from recommendation   (this is used only if recom data are kept in memory)
    int number_of_defections_heard;  
    double coop_ratio_perceived;
    double coop_ratio_total;    //perceived+heard
    //agenta pointer tutmak gerekir mi düşün
    

    MemorySlot();

};