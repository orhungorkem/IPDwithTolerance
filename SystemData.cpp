#include "SystemData.h"

SystemData::SystemData(){

    this->coop_vs_coop_accepted = 0;
    this->coop_vs_def_accepted = 0;
    this->def_vs_def_accepted = 0;
    this->coop_vs_coop_rejected = 0;
    this->coop_vs_def_rejected = 0;
    this->def_vs_def_rejected = 0;

    this->coop_mean = 0.0;
    this->def_mean = 0.0;

    this->coop_mean_optimism_threshold = 0.0;
    this->def_mean_optimism_threshold = 0.0;


    //Memory usages
    this->coop_in_memory = 0.0;
    this->def_in_memory = 0.0;
    this->undef_in_memory = 0.0;
    this->actual_coop_in_memory = 0.0;
    this->actual_def_in_memory = 0.0;
    this->memory_usage = 0.0;

    // During the decision to play
    this->coop_coop_by_perception = 0;
    this->coop_def_by_perception = 0;
    this->def_coop_by_perception = 0;
    this->def_def_by_perception = 0;
    this->coop_coop_by_recom = 0;
    this->coop_def_by_recom = 0;
    this->def_coop_by_recom = 0;
    this->def_def_by_recom = 0;
    this->perception_success = 0.0;
    this->recommendation_success = 0.0;

    //During the recommendation
    this->positive_recom_for_coop = 0;
    this->negative_recom_for_coop = 0;
    this->positive_recom_for_def = 0;
    this->negative_recom_for_def = 0;
    this->system_recom_for_coop = 0.0;
    this->system_recom_for_def = 0.0;

    //Total system counts
    this->number_of_recommendation = 0;
    this->number_of_percepted_evaluation = 0;
    this->number_of_default_play = 0;
    this->number_of_tolerance_play = 0;
    

    //System success
    this->error_in_coop = 0.0;
    this->error_in_def = 0.0;
    this->error_in_total = 0.0;
    this->system_perception_coop = 0.0;
    this->system_perception_def = 0.0;

}

SystemData SystemData::reset(){
    return SystemData();
}