#include "company.hh"

//constructor
Company::Company(){}

//destructor
Company::~Company(){}

//parseEmployee returns a employee object and stores the relations in the datastruct  relations
void Company::addNewEmployee(const std::string& id, const std::string& dep, const double& time,
                             std::ostream &output){

    auto it = info.find(id);
    // if the person has been inserted
    if(it == info.end()){
        shared_ptr<Employee> person =make_shared<Employee>();
        person->id_ =id; person->department_ =dep; person->time_in_service_ = time;
        info[id] = person ;
    }else{
        output<<"Error. Employee already added."<<endl;
    };
}// addNewEmployee


void Company::printEmployees(std::ostream &output) const{
    for(auto it = info.begin(); it!=info.end();it++){
        output<<it->first<<", "<<it->second->department_<<", "<<it->second->time_in_service_<<endl;
    }
}



void Company::addRelation(const std::string& subordinate,
                          const std::string& boss,
                          std::ostream& output){
    auto sub_ptr = info.find(subordinate);
    auto boss_ptr = info.find(boss);

    //if both are in the database
    if(sub_ptr != info.end() and boss_ptr != info.end() ){
        sub_ptr->second->boss_ = boss_ptr->second.get();
        boss_ptr->second->subordinates_.push_back(sub_ptr->second.get());
    }else{
        if(sub_ptr == info.end() and !subordinate.empty()){
            printNotFound(subordinate, output);
        }
        if(boss_ptr == info.end() and !boss.empty()){
            printNotFound(boss, output);
        }
    }
}



void Company::printSubordinates(const std::string& id,
                                std::ostream& output) const{
    Employee* person = getPointer(id);
    //if the person founed
    if(person != nullptr){
        set<string> names;
        size_t num = person->subordinates_.size();
        //if he has subs
        if(num!=0){
            output<<person->id_<<" has "<<num<<" subordinates:"<<endl;
            for(auto it = person->subordinates_.begin() ; it!= person->subordinates_.end();it++){
                names.insert((*it)->id_);
            }
            for(auto it = names.begin(); it!=names.end(); it++){
                output<<*it<<endl;
            }
        }else{
            output<<id<<" has no subordinates."<<endl;
        }
    }else {
        printNotFound(id, output);
    }
}//


void Company::printBoss(const std::string& id,
                        std::ostream& output) const{
    Employee* person = getPointer(id);
    // if this guy has boss
    if(person != nullptr  and person->boss_ != nullptr){
        Employee* boss = person->boss_;
        output<<person->id_<<" has "<<1<<" boss:"<<endl;
        output<<boss->id_<<endl;
    }else if( person == nullptr){
        //person does not exist
        printNotFound(id, output);
    }else if( person->boss_ ==nullptr){
        // no boss
        output<<id<<" has no boss."<<endl;
    }
}// printboss func


void Company::printColleagues(const std::string& id,
                              std::ostream& output) const{
    Employee* person = getPointer(id);
    // the person exists and has boss
    if( person != nullptr and person->boss_  != nullptr ){
        Employee* boss = person->boss_;
        set<string> colleagues;
        size_t num = boss->subordinates_.size();
        output<< person->id_<<" has "<<num-1<<" colleagues:"<<endl;
        for(auto it = boss->subordinates_.begin(); it != boss->subordinates_.end(); it++){
            string temp_id = (*it)->id_;
            if( temp_id != person->id_){
                colleagues.insert(temp_id);
            }
        }
        for(auto it= colleagues.begin(); it != colleagues.end(); it++){
            output<<*it<<endl;
        }
        // person does not exist
    }else if( person == nullptr){
        printNotFound(id, output);
        // he is the boss
    }else if(person->boss_  == nullptr){
        output<<id<<" has no colleagues."<<endl;
    }
}//



void Company::printDepartment(const std::string& id,
                              std::ostream& output) const{
    Employee* person = getPointer(id);
    //if person exists
    const string bid_boss_id="big boss";
    Employee* big_boss =getPointer(bid_boss_id);
    if( person != nullptr){
        // he has no boss and subs
        if( person->boss_ == nullptr and person->subordinates_.size() ==0){
            output<<id<<" has no department colleagues."<<endl;
            return;
        }
        //handle the case when person is from administration department
        Employee* top = nullptr;
        if(person->department_ == big_boss->department_){
            top= big_boss;
            person = big_boss;
        }else{
            //normal department
            string dep = person->department_ ;
            while( person->department_ == dep){
                if( person->boss_ == nullptr) break;
                if(person->boss_->department_ != dep ) break;
                person = person->boss_;
            };
        }
        // normal case calling self made recursive function
        //init paramters
        map<string, int> names; double max = person->time_in_service_ ; double min = person->time_in_service_ ;
        string max_name =person->id_, min_name =person->id_; int levels =1;
        top = person;vector<Employee*>::iterator it = person->subordinates_.begin();
        recursive_func(person, *it ,top,names ,max, min ,max_name, min_name,levels ) ;
        if(top == big_boss){
            IdSet nameSet ;
            for(auto it= names.begin() ; it!=names.end();it++){
                auto it2 = info.find(it->first);
                if(it2->second->department_ == big_boss->department_){
                    nameSet.insert(it->first);
                }
            }
            cout<<id<<" has "<<nameSet.size()-1 <<" department colleagues: "<<endl;
            for(auto it =nameSet.begin();it!=nameSet.end(); it++){
                if(*it != id){
                    cout<<*it<<endl;
                }
            }

        }else{
            cout<<id<<" has "<<names.size() -1 <<" department colleagues: "<<endl;
            for( auto it=names.begin(); it!= names.end(); it++){
                if( it->first != id) output<< it->first<<endl;
            }
        }

    }else{
        printNotFound(id, output);
    }
}//



void Company::printLongestTimeInLineManagement(const std::string& id,
                                               std::ostream& output) const{
    Employee* person = getPointer(id);
    // if person is there adnd call the recursive function.
    if( person != nullptr){
        if( person->subordinates_.size() != 0){
            //init parameters
            map<string, int> names; double max = person->time_in_service_ ; double min = person->time_in_service_ ;
            string max_name =person->id_, min_name =person->id_;int levels = 1;
            Employee* top = person; vector<Employee*>::iterator it = person->subordinates_.begin();
            recursive_func(person, *it , top ,names ,max, min , max_name, min_name ,levels) ;
            if(max_name != id){
                output<<"With the time of "<<max<<", "<<max_name<<" is the longest-served employee in "<<id<<"'s line management."<<endl;
            }else{
                output<<"With the time of "<<max<<", "<<max_name<<" is the longest-served employee in their line management."<<endl;
            }
        }else{
            output<<"With the time of "<<person->time_in_service_<<", "<<id<<" is the longest-served employee in their line management."<<endl;
        }
    }else{
        printNotFound(id, output);
    }


};


void Company::printShortestTimeInLineManagement(const std::string& id,
                                                std::ostream& output) const{
    Employee* person = getPointer(id);
    // if person is there adnd call the recursive function.
    if( person != nullptr){
        if( person->subordinates_.size() != 0){
            //init paramters
            map<string, int> names;double max = person->time_in_service_ ; double min = person->time_in_service_ ;
            string max_name =person->id_, min_name =person->id_;int levels =1;
            Employee* top = person; vector<Employee*>::iterator it = person->subordinates_.begin();
            recursive_func(person, *it ,top,names ,max, min , max_name, min_name,levels) ;

            if(id != min_name){
                output<<"With the time of "<<min<<", "<<min_name<<" is the shortest-served employee in "<<id<<"'s line management."<<endl;
            }else{
                output<<"With the time of "<<min<<", "<<min_name<<" is the shortest-served employee in their line management."<<endl;
            }
        }else{
            output<<"With the time of "<<person->time_in_service_<<", "<<id<<" is the shortest-served employee in their line management."<<endl;
        }
    }else{
        printNotFound(id, output);
    }
};


// i cannot finish it and i dont have time to do it either. sorry!
void Company::printSubordinatesN(const std::string& id, const int n,
                                 std::ostream& output) const{
    if( n<1){
        output<<"Error. Level can't be less than 1."<<endl;
        return ;
    }
    Employee* person = getPointer(id);
    if(person !=nullptr){
        if(person->subordinates_.size() ==0  ){
            output<<id<<" has no subordinates."<<endl;
        }else{
            map<string, int> names;double max = person->time_in_service_ ; double min = person->time_in_service_ ;
            string max_name =person->id_, min_name =person->id_; int levels =1;
            Employee* top = person; vector<Employee*>::iterator it = person->subordinates_.begin();
            recursive_func(person, *it ,top,names ,max, min , max_name, min_name,levels) ;

            IdSet subs;
            for(auto it =names.begin();it!=names.end(); it++){
                if(it->second<=n){
                    subs.insert(it->first);
                }
            }
            cout<<id<<" has "<<subs.size()-1<<" subordinates:"<<endl;
            for(auto it =subs.begin();it!=subs.end(); it++ ){
                if(id != *it) cout<<*it<<endl;
            }
        }
    }else{
        printNotFound(id,output);
    }
}




void Company::printBossesN(const std::string& id, const int n,
                           std::ostream& output) const{
    if( n<1){
        output<<"Error. Level can't be less than 1."<<endl;
        return ;
    }
    Employee* person = getPointer(id);
    if( person != nullptr){
        if( person->boss_ == nullptr){
            output<<id<<" has no bosses."<<endl;
            return ;
        }
        int counter =0; IdSet names;
        while(  person->boss_ != nullptr){
            names.insert(person->boss_->id_);
            person = person->boss_;
            counter++;
            if(counter == n) break;
        }
        output<<id<<" has "<<counter<<" bosses:"<<endl;
        for( auto it=names.begin(); it!=names.end();it++){
            output<<*it<<endl;
        }
    }else{
        printNotFound(id, output);
    }
}



//private function
Employee* Company::getPointer(const std::string& id) const{

    Employee* person = nullptr ;
    auto it = info.find(id);
    if(it != info.end()){
        person = it->second.get();
    }
    return person;

}



// this is the recursive I did , acutally it is a bad design, but i spent much time so I decided to continue with this function.
// if you would like to print the first commented line in the function , you would  understand the code a bit.

//it does a couple of things, it calculates the longest-serve time and shortest-serve time. it also record the name fo longest service time, shortest service time.  it can record the level of each person in hierachical tree.
//it is divided into a couple of steps
//1. always find the first subordinate, if the first subordinate has not been checked, goes one level down
//2. if the first has been checked , move the next one and so on.
//3. if the next one has not been checked, first check if it has subs or not, if yes , goes one level down. otherwise add name and level into names map and move up one level
//4.if all the subs has been ckecked, and the person himself has not been added, add the person's name , level , and move one level up.
//5. if it reach the bottom, add the person, level and move upwards
//6. when the person moves to top again from the bottom, it stops
void  Company::recursive_func(Employee* boss ,  Employee* sub,Employee* top, map<string, int>& names , double& max, double& min ,string& max_name, string& min_name, int&level) const{

    //for the longest and shortest service time, name for each
    // cout<<"boss : "<<boss->id_ << " "<<" sub: "<<sub->id_<<" "<levels<<endl;
    if(sub->time_in_service_ >= max){ max = sub->time_in_service_; max_name = sub->id_;}
    if(sub->time_in_service_ < min){ min = sub->time_in_service_; min_name = sub->id_; }

    // here handles the cases which are not in the bottom. try to go down first
    if( boss->subordinates_.size() != 0   ){
        //first you check that if all the subs have been added or not
        bool all_exist = true;
        for( auto it = boss->subordinates_.begin(); it!=boss->subordinates_.end(); it++ ){
            if( names.find((*it)->id_) == names.end()){
                all_exist =false;
                break;
            }
        }
        // if all the subordinates have been added,  goes one level up
        if( all_exist){
            // go one level up, level decrement by 1
            level--;
            names[boss->id_] = level;
            if( boss->id_ == top->id_) return;
            sub = boss;
            boss = boss->boss_;
            recursive_func(boss,sub, top,names, max,min, max_name,min_name,level);
        } else if(  names.find(sub->id_) == names.end() ){
            // if the first element has not been checked , goes deeper
            boss =sub;
            if(boss->subordinates_.size() !=0){
                sub= *boss->subordinates_.begin();
            }else{
                sub =  boss;
            }
            if(boss->id_ != sub->id_){ level++;}
            recursive_func(boss,sub, top,names,max,min , max_name,min_name,level);
        }else{
            // if the first element has been check, move to the next
            auto it = boss->subordinates_.begin();
            for( ; it != boss->subordinates_.end(); it++){
                //find the current person
                if( (*it)->id_ == sub->id_){
                    if( it != boss->subordinates_.end() -1){
                        it++;
                    }
                    sub = *it;
                    //if the person has no subs , just add it
                    if(sub->subordinates_.size() == 0){
                        names[sub->id_] = level;
                        //names.insert(sub->id_);
                    }else{
                        boss = sub;
                        sub = *boss->subordinates_.begin();
                        level++;
                    }
                    break;
                }
            }
            recursive_func(boss,sub, top,names,max,min , max_name,min_name,level);
        }
        //second when its reachs the bottom, put the name in side
    }else if( sub->subordinates_.size() == 0  and  names.find(sub->id_) == names.end() ){
        //names.insert(sub->id_);
        names[sub->id_] = level;
        if( names.find(sub->id_) == names.end()) level--;
        sub = boss;
        boss = boss->boss_;
        recursive_func(boss,sub, top,names,max,min , max_name,min_name,level);
    }


};

void Company::printNotFound(const string &id, ostream &output) const{
    output<<"Error. "<<id<<" not found."<<endl;
}


