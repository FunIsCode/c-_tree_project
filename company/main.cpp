/*
#############################################################################
# TIE-02207 Programming 2, Autumn 2018                                      #
# Project3: Company                                                         #
# Program description: Queries user for CSV-file and allows simple searches #
#       based on the data.                                                  #
# File: main.cpp                                                            #
# Description: Main-module performs the followin operations:                #
#       * Query for input file.                                             #
#       * Parse the CSV-data                                                #
#       * Pass the parsed data to Company-module                            #
#       * Launch Cli-module                                                 #
# Notes: * This is an exercise program.                                     #
#        * Student's don't touch this file.                                 #
#############################################################################
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "company.hh"
#include "cli.hh"

// Constants to make CSV-parsing more readable.
const char CSV_DELIMITER = ';';
const int CSV_VALUES = 4;
const int CSV_NAME=0, CSV_DEPARTMENT = 1, CSV_TIME=2, CSV_BOSS=3;


// Struct to group-up boss-subordinate relations.
struct Relation
{
    std::string subordinate_;
    std::string boss_;
};

bool populateDatabase(std::ifstream &datafile, std::shared_ptr<Company> database);
Employee parseEmployee(const std::string &line, std::vector<Relation> &relations);

int main()
{
//    std::string cmd_string =  "/Users/yuanqi/Desktop/data.csv";
    std::string cmd_string ;
    std::shared_ptr<Company> database(new Company);

    // File query
     std::cout << "Input file: ";
     std::getline(std::cin, cmd_string);

    // Query result must be transformed from std::string to c_string so it can
    //  be passed to ifstream's constructor.
    std::ifstream datafile(cmd_string.c_str());

    if(not datafile)
    {
        std::cout << "Could not open file: " << cmd_string << std::endl;
        return EXIT_FAILURE;
    }

    // populateDatabase returns false if there was some error in file.
    if(not populateDatabase(datafile, database))
    {
        return EXIT_FAILURE;
    }

    // Constructing the commandlineinterpreter with the given main datastructure
    Cli commandline(database);

    // Empty loop that runs the CLI.
    // CLI returns false only on exit-command
    while(commandline.exec_prompt()){}

    return EXIT_SUCCESS;
}


/* Description: Reads throught the datafile and populates the database with it.
 * Parameters:
 *  Param1: Datafile-object
 *  Param2: Pointer to the database
 * Return values:
 *  True    - No errors in file
 *  False   - Errors in file
 */

bool populateDatabase(std::ifstream& datafile, std::shared_ptr<Company> database)
{
    std::string line = "";
    //std::vector<std::string> parsed_line;
    std::vector<Relation> relations;
    int line_number = 0;

    while(std::getline(datafile, line))
    {
        // Linenumbering for error-printing.
        ++line_number;

        // Skip empty and commented lines.
        if(line.empty() or line[0] == '#')
        {
            continue;
        }

        // Parse the line to a new employee and relation.
        Employee new_employee = parseEmployee(line, relations);
        if( new_employee.id_ == EMPTY or new_employee.time_in_service_ == NO_TIME )
        {
            std::cout << "Error in datafile, line " << line_number << std::endl;
            return false;
        }

        // Add the new employee to the database.
        database->addNewEmployee(new_employee.id_, new_employee.department_, new_employee.time_in_service_, std::cout);
    }

    // After the employees have been created. Add the boss-subordinate relations.
    for( auto it = relations.begin(); it != relations.end(); ++it)
    {
        database->addRelation(it->subordinate_, it->boss_, std::cout);
    }
    return true;
}

/* Description: Parse the string to a new employee and boss-subordinate relation.
 * Parameters:
 *  Param1: String to parse
 *  Param2: Container for Relations
 * Return values:
 *  Employee-struct that was created.
 *      id_               - EMPTY if wrong amount of parameters
 *      time_in_service_  - NO_TIME if wrong amount of parameters or the
 *                          string-to-int conversion failed.
 */

Employee parseEmployee(const std::string& line, std::vector<Relation> &relations)
{
    std::string tmp = line;
    std::vector<std::string> parsed_data;
    size_t pos = 0;
    Employee new_employee;
    Relation new_relation;

    // Split the ';' separated fields to a vector.
    while (true)
    {
        pos = tmp.find(CSV_DELIMITER);
        parsed_data.push_back(tmp.substr(0, pos));
        if(pos == std::string::npos){break;}
        tmp = tmp.substr(pos+1);
    }

    // Check the that there was correct amount of fields in the line.
    if( parsed_data.size() == CSV_VALUES )
    {
        new_employee.id_ = parsed_data[CSV_NAME];
        new_employee.department_ = parsed_data[CSV_DEPARTMENT];
        // Exception safety.
        // The NO_TIME assignment isn't necessary, but makes things clearer.
        try{
            new_employee.time_in_service_ = std::stod(parsed_data[CSV_TIME]);
        }
        catch(std::invalid_argument&)
        {
            std::cout << "Caught invalid_argument exception." << std::endl;
            new_employee.time_in_service_ = NO_TIME;
        }
        catch(std::out_of_range&)
        {
            std::cout << "Caught out_of_range exception." << std::endl;
            new_employee.time_in_service_ = NO_TIME;
        }
        new_relation.subordinate_ = parsed_data[CSV_NAME];
        new_relation.boss_ = parsed_data[CSV_BOSS];
        relations.push_back(new_relation);
    }

    return new_employee;
}
