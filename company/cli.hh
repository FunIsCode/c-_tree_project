/*
#############################################################################
# TIE-02207 Programming 2, Autumn 2018                                      #
# Project3: Company                                                         #
# File: cli.hh                                                              #
# Description: Commandline interface for the program.                       #
# Notes: * This is a part of an exercise program                            #
#        * Student's don't touch this file.                                 #
#        * Understanding how the module works isn't necessary to complete   #
#          this exercise.                                                   #
#        * This is also an example how you can use function pointers to     #
#          a class' functions.                                              #
#        * This could be upgraded to have commandhistory with a relative    #
#          ease.                                                            #
#############################################################################
*/
#ifndef CLI_HH
#define CLI_HH

#include <string>
#include <vector>
#include <memory>

#include "company.hh"

/* In order to define a function pointer to a class you have to add
 * class as the visibility range before the pointer declaring * and
 * after that comes the type-name for the pointer.
 * In this case we have named the types according to the amount of
 * parameters the function receives.
 */
typedef void (Company::*oneparam)(std::ostream&) const;
typedef void (Company::*twoparam)(const std::string&, std::ostream&) const;
typedef void (Company::*threeparam)(const std::string&, const int, std::ostream&) const;

// CommandInfo contains the list of strings that select it and
// Function-pointers to the function that is going to be called through it.
struct CommandInfo
{
    std::vector<std::string> lines_;
    oneparam func0_;
    twoparam func1_;
    threeparam func2_;
};


class Cli
{
public:
    // The database that is going to be used is given as a parameter.
    Cli(std::shared_ptr<Company> db);

    /* Description: Runs the interface.
     * Return values:
     *  True    - Normal command
     *  False   - Exit command
     */
    bool exec_prompt();

private:
    std::shared_ptr<Company> database_;

    // Stores the arguments from current command.
    std::vector<std::string> args_;

    // Pointer to the recognized command.
    const CommandInfo* recognized_;

    // Printed for every query
    const std::string PROMPT = "> ";

    // Parses the given input to different arguments
    void parseCommand(const std::string& line);

    /* Description: Sets the recognized_ to point at the correct
     * CommandInfo.
     * Return values:
     *  Amount of arguments - Command can be executed
     *  -1                  - Exit command
     *  -2                  - Invalid amount of parameters
     *  -3                  - Command not recognized.
     */
    int checkCommandType();

    // To make code more readable. Could also be different constants but this
    // groups them better.
    enum Commandtypes {QUIT = -1, NO_PARAMETERS = 1, ONE_PARAMETER = 2,
                       TWO_PARAMETER = 3, EMPTY = 0, UNKNOWN = -3,
                       WRONG_PARAMETERS = -2};


    // Constant table that contains the different commands that the CLI can
    // recognize.
    CommandInfo const COMMANDS[11] = {
        {{"EXIT","QUIT","Q"}, nullptr, nullptr, nullptr},
        {{"COMPANY","FIRM","ENTERPRISE", "PRINT"}, &Company::printEmployees, nullptr,nullptr},
        {{"SUBORDINATES", "UNDERLINGS"}, nullptr, &Company::printSubordinates, nullptr},
        {{"DEPARTMENT", "UNIT", "LABORATORY"}, nullptr, &Company::printDepartment, nullptr},
        {{"COLLEAGUES", "CO_WORKERS"},nullptr,&Company::printColleagues,nullptr},
        {{"BOSS", "MANAGER", "HEAD"},nullptr,&Company::printBoss,nullptr},
        {{"LONGEST_TIME", "LONGEST"}, nullptr,&Company::printLongestTimeInLineManagement,nullptr},
        {{"SHORTEST_TIME", "SHORTEST"}, nullptr, &Company::printShortestTimeInLineManagement, nullptr},
        {{"ALL_SUBORDINATES", "ALL_UNDERLINGS"}, nullptr, nullptr,&Company::printSubordinatesN},
        {{"ALL_BOSSES", "ALL_MANAGERS", "ALL_HEADS"}, nullptr,nullptr,&Company::printBossesN},
        {{},nullptr,nullptr,nullptr}
    };


};

#endif // CLI_HH
