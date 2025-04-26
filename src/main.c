#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int ft_strlen_simple(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return (i);
}

t_node *new_token(int len, char *str, int start)
{
    int i = 0;
    char *token = malloc(len + 1);
    t_node *node;

    if (!token)
        return (NULL);
    while (i < len)
    {
        token[i] = str[start + i];
        i++;
    }
    token[i] = '\0';
    node = malloc(sizeof(t_node));
    if (!node)
        return (free(token), NULL);
    node->token = token;
    node->type = ARG;
    node->group = 0;
    node->quotes_type = 0;
    node->info_node = NULL;
    node->next = NULL;
    return (node);
}

t_node *split_tokens(char *str)
{
    int i = 0;
    int len;
    t_node *head = NULL;
    t_node *curr = NULL;

    while (str[i])
    {
        while (str[i] == ' ' || str[i] == '\t')
            i++;
        if (str[i] == '\0')
            break;
        if (str[i] == '\'' || str[i] == '"')
        {
            char quote = str[i];
            int start = i++;
            while (str[i] && str[i] != quote)
                i++;
            if (str[i] == quote)
                i++;
            len = i - start;
        }
        else
        {
            int start = i;
            while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\'' && str[i] != '"')
                i++;
            len = i - start;
        }
        t_node *new = new_token(len, str, i - len);
        if (!new)
            return (NULL);
        if (!head)
            head = new;
        else
            curr->next = new;
        curr = new;
    }
    return (head);
}

void check_node_type(t_node *node)
{
    if (!node || !node->token)
        return;
    if (ft_strcmp(node->token, "<") == 0)
        node->type = RID_IN;
    else if (ft_strcmp(node->token, ">") == 0)
        node->type = RID_OUT;
    else if (ft_strcmp(node->token, ">>") == 0)
        node->type = RED_APPEND;
    else if (ft_strcmp(node->token, "<<") == 0)
        node->type = HERED;
    else if (ft_strcmp(node->token, "|") == 0)
        node->type = TOKEN_PIPE;
    else
        node->type = ARG;
}

void set_node_types(t_node *head)
{
    while (head)
    {
        check_node_type(head);
        head = head->next;
    }
}

void set_groups(t_node *head, int group)
{
    int grp = group;

    while (head)
    {
        if (head->type == TOKEN_PIPE)
            grp++;
        head->group = grp;
        head = head->next;
    }
}

t_cmd_node *new_cmd(void)
{
    t_cmd_node *cmd = malloc(sizeof(t_cmd_node));
    if (!cmd)
        return (NULL);
    cmd->arr = NULL;
    cmd->in = NULL;
    cmd->out = NULL;
    cmd->heredoc = NULL;
    cmd->append = 0;
    cmd->err = 0;
    cmd->ex_heredoc = 0;
    cmd->node_list = NULL;
    cmd->next = NULL;
    return (cmd);
}

char *read_heredoc(const char *lim)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t n;
    char *res = NULL;

    while (1)
    {
        printf("> ");
        n = ftgetline(&line, &len);
        if (n <= 0)
            break;
        if (line[n - 1] == '\n')
            line[n - 1] = '\0';
        if (ft_strcmp(line, (char *)lim) == 0)
            break;
        res = ft_strjoin(res, line);
        res = ft_strjoin(res, "\n");
    }
    free(line);
    return (res);
}

void fill_cmd_arr(t_cmd_node *cmd)
{
    while (cmd)
    {
        t_node *cur = cmd->node_list;
        int count = 0;
        t_node *tmp = cur;

        while (tmp)
        {
            if (tmp->type == ARG)
                count++;
            tmp = tmp->next;
        }
        cmd->arr = malloc(sizeof(char *) * (count + 1));
        int i = 0;
        while (cur)
        {
            if (cur->type == ARG)
                cmd->arr[i++] = ft_strdup(cur->token);
            else if (cur->type == RID_IN && cur->next)
            {
                cmd->in = ft_strdup(cur->next->token);
                cur = cur->next;
            }
            else if (cur->type == RID_OUT && cur->next)
            {
                cmd->out = ft_strdup(cur->next->token);
                cmd->append = 0;
                cur = cur->next;
            }
            else if (cur->type == RED_APPEND && cur->next)
            {
                cmd->out = ft_strdup(cur->next->token);
                cmd->append = 1;
                cur = cur->next;
            }
            else if (cur->type == HERED && cur->next)
            {
                cmd->heredoc = read_heredoc(cur->next->token);
                cur = cur->next;
            }
            cur = cur->next;
        }
        cmd->arr[i] = NULL;
        cmd = cmd->next;
    }
}

void group_cmds(t_cmd_node **cmd, t_node *head)
{
    t_cmd_node *tmp = NULL;
    int grp = -1;

    while (head)
    {
        if (head->group != grp)
        {
            t_cmd_node *new = new_cmd();
            if (!new)
                return;
            if (!*cmd)
                *cmd = new;
            else
                tmp->next = new;
            tmp = new;
            tmp->node_list = head;
            grp = head->group;
        }
        head = head->next;
    }
}

void print_cmds(t_cmd_node *cmd)
{
    int i;

    while (cmd)
    {
        printf("\n[Group]\n");
        if (cmd->arr)
        {
            for (i = 0; cmd->arr[i]; i++)
                printf("arg[%d]: %s\n", i, cmd->arr[i]);
        }
        printf("in: %s\nout: %s\nappend: %d\n", cmd->in ? cmd->in : "(none)", cmd->out ? cmd->out : "(none)", cmd->append);
        if (cmd->heredoc)
            printf("heredoc:\n%s\n", cmd->heredoc);
        else
            printf("heredoc: (none)\n");
        cmd = cmd->next;
    }
}

int main(void)
{
    char *input = NULL;
    size_t len = 0;
    ssize_t nr;
    int grp = 0;

    while (1)
    {
        printf("\nminishell>> ");
        nr = ftgetline(&input, &len);
        if (nr < 0)
            break;
        if (nr > 0 && input[nr - 1] == '\n')
            input[nr - 1] = '\0';
        if (input[0] == '\0')
        {
            free(input);
            input = NULL;
            continue;
        }
        t_node *head = split_tokens(input);
        set_node_types(head);
        set_groups(head, grp);
        t_cmd_node *cmd = NULL;
        group_cmds(&cmd, head);
        fill_cmd_arr(cmd);
        print_cmds(cmd);
        free(input);
        input = NULL;
    }
    return (0);
}