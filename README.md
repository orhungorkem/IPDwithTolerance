# Optimism Brings Accurate Perception in Iterated Prisoner's Dilemma
The code for simulation of the model from the paper named "Optimism brings accurate perception in Iterated Prisoner's Dilemma".


## How to run?

* Navigate to the code repository from terminal.
* Run command: g++ *.cpp
* Run command ./a.out
* When the simulation ends, the reuslts will appear in **results.csv**


Note that the main function exists in SystemEngine.cpp. You can edit the configurations to try other conditions. Also note that the current configurations will start a full simulation with 20 realizations from all model variables and this takes some time. To check a specific configuration, we recommend unccommenting the commented code in main function and commenting the totalsim() function execution.
