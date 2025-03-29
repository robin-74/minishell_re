#include "../minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int str_length(char *input)
{
    int i = 0;
    while (input[i])
        i++;
    return i;
}

t_node *init_newNode(int size_str, char *input, int start)
{
    int idx = 0;
    char *token = (char *)malloc(sizeof(char) * (size_str + 1));
    if (!token)
        return NULL;

    while (idx < size_str)
    {
        token[idx] = input[start + idx];
        idx++;
    }
    token[idx] = '\0';

    t_node *new_node = (t_node *)malloc(sizeof(t_node));
    if (!new_node)
    {
        free(token);
        return NULL;
    }
    new_node->token = token;
    new_node->next = NULL;
    return new_node;
}

t_node *to_linked_list(char *input)
{
    int n = str_length(input);
    int i = 0;
    t_node *head = NULL;
    t_node *temp = NULL;

    while (i < n)
    {
        while (input[i] && (input[i] == ' ' || input[i] == '\t'))
            i++;

        int size_str = 0;
        while (input[i + size_str] && input[i + size_str] != ' ' && input[i + size_str] != '\t')
            size_str++;

        if (size_str > 0)
        {
            t_node *new_node = init_newNode(size_str, input, i);
            if (!new_node)
                return NULL;
            if (!head)
                head = new_node;
            else
                temp->next = new_node;
            temp = new_node;
            i += size_str;
        }
        i++;
    }
    return head;
}
//  void token_idtfy(t_node *node)
//  {
//     //it t over each node  create the info node fill it  based  switch  case  and next .... 
//     init(                                                                              
    


//  }


void print_linked_list(t_node *head)
{
    t_node *temp = head;
    while (temp)
    {
        printf("[ %s ] -> ", temp->token);
        temp = temp->next;
    }
    printf("NULL\n");
}

void execute_command(t_node *head)
{
    if (!head)
        return;

    int count = 0;
    t_node *temp = head;
    while (temp)
    {
        count++;
        temp = temp->next;
    }

    char **args = (char **)malloc(sizeof(char *) * (count + 1));
    if (!args)
        return;

    temp = head;
    for (int i = 0; i < count; i++)
    {
        args[i] = temp->token;
        temp = temp->next;
    }
    args[count] = NULL;

    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    }
    else
    {
        wait(NULL);
    }
    free(args);
}

int main(void)
{
    char *input = NULL;
    size_t len = 0;
    ssize_t nr = 0;

    while (1)
    {
        printf("\nminishell>> ");
        nr = ftgetline(&input, &len);

        if (nr < 0)
            break;
        if (nr > 0 && input[nr - 1] == '\n')
            input[nr - 1] = '\0';

        if (input[0] == '\0')
            continue;

        t_node *cmd_list = to_linked_list(input);
        print_linked_list(cmd_list);
      //  execute_command(cmd_list);

        free(input);
        input = NULL;
    }
    return 0;
}
