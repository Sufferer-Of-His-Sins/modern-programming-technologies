#ifndef TASK_H
#define TASK_H

#include <string>

class Task
{
private:
    int id;
    std::string title;
    std::string description;
    std::string status;
    std::string priority;

public:
    Task(int id, std::string title);

    int GetId();
    std::string GetTitle();
    std::string GetDescription();
    std::string GetStatus();
    std::string GetPriority();

    void SetTitle(std::string title);
    void SetDescription(std::string description);
    void SetStatus(std::string status);
    void SetPriority(std::string priority);
};

#endif
