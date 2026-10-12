#include "taskmanager.h"

TaskManager::TaskManager()
{
    nextTaskId = 1;
    nextProjectId = 1;
    taskRepository.Clear();
    projectRepository.Clear();
}

void TaskManager::AddTask(std::string title,
                          std::string description,
                          std::string priority,
                          std::string status,
                          int projectId)
{
    Task task(nextTaskId, title, projectId);
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
                             std::string status)
{
    std::vector<Task> tasks = taskRepository.GetAll();
    if (index < 0 || index >= static_cast<int>(tasks.size()))
    {
        return;
    }

    Task task = tasks[index];

    task.SetTitle(title);
    task.SetDescription(description);
    task.SetPriority(priority);
    task.SetStatus(status);

    taskRepository.Update(index, task);
}

void TaskManager::DeleteTask(int index)
{
    if (index < 0 || index >= static_cast<int>(taskRepository.Size()))
    {
        return;
    }
    taskRepository.Remove(index);
}
std::vector<Task> TaskManager::GetTasks()
{
    return taskRepository.GetAll();
}

bool TaskManager::MoveTaskToProject(int taskIndex, int projectId)
{
    std::vector<Task> tasks = taskRepository.GetAll();
    if (taskIndex < 0 || taskIndex >= static_cast<int>(tasks.size()))
    {
        return false;
    }

    if (projectId != 0)
    {
        std::vector<Project> projects = projectRepository.GetAll();
        bool found = false;
        for (int i = 0; i < static_cast<int>(projects.size()); i++)
        {
            if (projects[i].getId() == projectId)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            return false;
        }
    }

    Task task = tasks[taskIndex];
    task.SetProjectId(projectId);
    taskRepository.Update(taskIndex, task);
    return true;
}

int TaskManager::CountTasksInProject(int projectId)
{
    std::vector<Task> tasks = taskRepository.GetAll();
    int count = 0;
    for (int i = 0; i < static_cast<int>(tasks.size()); i++)
    {
        if (tasks[i].GetProjectId() == projectId)
        {
            count++;
        }
    }
    return count;
}

int TaskManager::CountTasksInProjectByStatus(int projectId, std::string status)
{
    std::vector<Task> tasks = taskRepository.GetAll();
    int count = 0;
    for (int i = 0; i < static_cast<int>(tasks.size()); i++)
    {
        if (tasks[i].GetProjectId() == projectId && tasks[i].GetStatus() == status)
        {
            count++;
        }
    }
    return count;
}

double TaskManager::CompletionPercent(int projectId)
{
    int total = CountTasksInProject(projectId);
    if (total == 0)
    {
        return 0.0;
    }
    int done = CountTasksInProjectByStatus(projectId, "Done");
    return 100.0 * static_cast<double>(done) / static_cast<double>(total);
}

void TaskManager::AddProject(std::string name)
{
    Project project(nextProjectId, name);
    nextProjectId++;
    projectRepository.Add(project);
}

void TaskManager::DeleteProject(int index)
{
    std::vector<Project> projects = projectRepository.GetAll();
    if (index < 0 || index >= static_cast<int>(projects.size()))
    {
        return;
    }

    int projectId = projects[index].getId();

    std::vector<Task> tasks = taskRepository.GetAll();
    for (int i = static_cast<int>(tasks.size()) - 1; i >= 0; i--)
    {
        if (tasks[i].GetProjectId() == projectId)
        {
            taskRepository.Remove(i);
        }
    }

    projectRepository.Remove(index);
}

std::vector<Project> TaskManager::GetProjects()
{
    return projectRepository.GetAll();
}