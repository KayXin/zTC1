#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include"timed_task/timed_task.h"

pTimedTask task_top = NULL;
int task_count = 0;

bool AddTask(pTimedTask task)
{
    task_count++;
    if (task_top == NULL)
    {
        task->next = NULL;
        task_top = task;
        return true;
    }

    if (task->prs_time <= task_top->prs_time)
    {
        task->next = task_top;
        task_top = task;
        return true;
    }

    pTimedTask tmp = task_top;
    while (tmp)
    {
        if (tmp->next == NULL
            || (task->prs_time >= tmp->prs_time
             && task->prs_time < tmp->next->prs_time))
        {
            task->next = tmp->next;
            tmp->next = task;
            return true;
        }
        tmp = tmp->next;
    }
    task_count--;
    return false;
}

bool DelFirstTask()
{
    if (task_top)
    {
        pTimedTask tmp = task_top;
        task_top = task_top->next;
        free(tmp);
        task_count--;
        return true;
    }
    return false;
}

bool DelTask(int time)
{
    if (task_top == NULL)
    {
        return false;
    }

    if (time == task_top->prs_time)
    {
        pTimedTask tmp = task_top;
        task_top = task_top->next;
        free(tmp);
        task_count--;
        return true;
    }
    else if (task_top->next == NULL)
    {
        return false;
    }

    pTimedTask pre_tsk = task_top;
    pTimedTask tmp_tsk = task_top->next;
    while (tmp_tsk)
    {
        if (time == tmp_tsk->prs_time)
        {
            pre_tsk->next = tmp_tsk->next;
            free(tmp_tsk);
            task_count--;
            return true;
        }
        tmp_tsk = tmp_tsk->next;
    }
    return false;
}

char* GetTaskStr()
{
    char* str = (char*)malloc(sizeof(char)*(task_count*60+2));
    pTimedTask tmp_tsk = task_top;
    char* tmp_str = str;
    tmp_str[0] = '[';
    tmp_str[2] = 0;
    tmp_str++;
    while (tmp_tsk)
    {
        char buffer[26];
        struct tm* tm_info;
        tm_info = localtime(&tmp_tsk->prs_time);
        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

        sprintf(tmp_str, "{'prs_time':'%s','socket_idx':%d,'on':%d},",
            buffer, tmp_tsk->socket_idx, tmp_tsk->on);
        tmp_str += strlen(tmp_str);
        tmp_tsk = tmp_tsk->next;
    }
    if (task_count > 0) --tmp_str;
    *tmp_str = ']';
    return str;
}
