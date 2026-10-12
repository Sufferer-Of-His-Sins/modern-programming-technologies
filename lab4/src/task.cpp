#include "task.h"
/*Добавлено все*/

Task::Task(int id, std::string title, int projectId){
    this->id = id;
    this->projectId = projectId;
    this->title = title;
    this->description= "";
    this->status = "Todo";
    this->priority = "Medium";
}

int Task::GetId() const{
    return id;
}
int Task::GetProjectId() const{
    return projectId;
}
std::string Task::GetTitle()const{
    return title;
}
std::string Task::GetStatus()const{
    return status;
}
std::string Task::GetPriority()const{
    return priority;
}
std::string Task::GetDescription() const{
    return description;
}

void Task::SetTitle(std::string title){
    this->title = title;
}
void Task::SetProjectId(int projectId)
{
    this->projectId = projectId;
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
    if(status == "Todo" || status == "Done" || status == "In Progress"){
        this->status = status;
    }
}