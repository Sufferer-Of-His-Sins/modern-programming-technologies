#include "task.h"
/*Добавлено все*/

Task::Task(int id, std::string title){
    this->id = id;
    this->title = title;
    this->description= "";
    this->status = "Todo";
    this->priority = "Medium";
}

int Task::GetId(){
    return id;
}
std::string Task::GetTitle(){
    return title;
}
std::string Task::GetStatus(){
    return status;
}
std::string Task::GetPriority(){
    return priority;
}
std::string Task::GetDescription(){
    return description;
}

void Task::SetTitle(std::string title){
    this->title = title;
}
void Task::SetDescription(std::string description){
    this->description = description;
}
void Task::SetPriority(std::string priority){
    if(priority == "Low" || priority == "High" || priority == "Medium"){
        this->priority = priority;
    }
}
void Task::SetStatus(std::string status){
    if(status == "Todo" || status == "Done" || status == "In Progres"){
        this->status = status;
    }
}