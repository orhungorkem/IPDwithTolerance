

class SystemData{

    public:

    int coop_vs_coop_accepted;
    int coop_vs_def_accepted;
    int def_vs_def_accepted;
    int coop_vs_coop_rejected;
    int coop_vs_def_rejected;
    int def_vs_def_rejected;
    double coop_mean;
    double def_mean;
    double coop_mean_optimism_threshold;
    double def_mean_optimism_threshold;
    double memory_usage;
    double coop_in_memory;
    double def_in_memory;
    double undef_in_memory;
    double actual_coop_in_memory;
    double actual_def_in_memory;

    //decisions of cooperators
    int coop_coop_by_perception;
    int coop_def_by_perception;   //cooperator perceived as def
    int def_coop_by_perception;   //reverse
    int def_def_by_perception;
    int coop_coop_by_recom;
    int coop_def_by_recom;   
    int def_coop_by_recom;
    int def_def_by_recom;
    double perception_success;
    double recommendation_success;
    int number_of_recommendation;
    int number_of_percepted_evaluation;
    int number_of_default_play;
    int number_of_tolerance_play;

    int positive_recom_for_coop;
    int negative_recom_for_coop;
    int positive_recom_for_def;
    int negative_recom_for_def;
    double system_recom_for_coop;
    double system_recom_for_def;

    double error_in_coop;
    double error_in_def;
    double error_in_total;
    double system_perception_coop;
    double system_perception_def;


    SystemData();
    SystemData reset();

};