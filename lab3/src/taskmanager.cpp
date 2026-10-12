#include "taskmanager.h"
/*Добавлено все*/

TaskManager::TaskManager(){
    nextTaskId = 1;
    nextProjectId = 1;
    taskRepository.Clear();
    projectRepository.Clear();
}

void TaskManager::AddTask(std::string title,
                 std::string description,
                 std::string priority,
                 std::string status){

                    Task task(nextTaskId, title);
                    nextTaskId++;

                    task.SetDescription(description);
                    task.SetPriority(priority);
                    task.SetStatus(status);
                    
                    taskRepository.Add(task);
                }

void TaskManager::UpdateTask(int index,
                    std::string title,
                    std::string description,
                    std::string priority,
                    std::string status) {

                        std::vector<Task>tasks = taskRepository.GetAll();
                        Task task = tasks[index];
                    
                        task.SetDescription(description);
                        task.SetPriority(priority);
                        task.SetStatus(status);
                        task.SetTitle(title);

                        taskRepository.Update(index, task);
}
void TaskManager::DeleteTask(int index){
    taskRepository.Remove(index);
}
std::vector<Task> TaskManager::GetTasks(){
    return taskRepository.GetAll();
}
void TaskManager::AddProject(std::string name){
    Project project(nextProjectId, name);
    nextProjectId++;
    projectRepository.Add(project);
}
void TaskManager::DeleteProject(int index){
    projectRepository.Remove(index);
}
std::vector<Project> TaskManager::GetProjects(){
    return projectRepository.GetAll();
}
