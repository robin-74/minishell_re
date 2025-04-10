#include "../minishell.h"

int str_length(char *input)
{
    int i = 0;
    while (input[i])
        i++;
    return i;
}

t_node *init_new_node(int size_str, char *input, int start)
{
    int i = 0;
    char *token = malloc(size_str + 1);
    if (!token)
        return NULL;
    while (i < size_str)
    {
        token[i] = input[start + i];
        i++;
    }
    token[i] = '\0';
    t_node *new_node = malloc(sizeof(t_node));
    if (!new_node)
    {
        free(token);
        return NULL;
    }
    new_node->token = token;
    new_node->type = ARG;
    new_node->group = 0;
    new_node->quotes_type = 0;
    new_node->info_node = NULL;
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
        while (input[i] == ' ' || input[i] == '\t')
            i++;
        int len = 0;
        while (input[i + len] && input[i + len] != ' ' && input[i + len] != '\t')
            len++;
        if (len > 0)
        {
            t_node *new_node = init_new_node(len, input, i);
            if (!new_node)
                return NULL;
            if (!head)
                head = new_node;
            else
                temp->next = new_node;
            temp = new_node;
            i += len;
        }
        else
            i++;
    }
    return head;
}

void check_type(t_node *node)
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

void assign_groups(t_node *head, int group)
{
    int group_id = group;
    while (head)
    {
        if (head->type == TOKEN_PIPE)
            group_id++;
        head->group = group_id;
        head = head->next;
    }
}

t_cmd_node *init_cmd_node()
{
    t_cmd_node *new_cmd = malloc(sizeof(t_cmd_node));
    if (!new_cmd)
        return NULL;
    new_cmd->arr = NULL;
    new_cmd->in = NULL;
    new_cmd->out = NULL;
    new_cmd->heredoc = NULL;
    new_cmd->append = 0;
    new_cmd->err = 0;
    new_cmd->ex_heredoc = 0;
    new_cmd->node_list = NULL;
    new_cmd->next = NULL;
    return new_cmd;
}

void group_by_group(t_cmd_node **cmd_head, t_node *head)
{
    t_cmd_node *temp = NULL;
    int prev_group = -1;
    while (head)
    {
        if (head->group != prev_group)
        {
            t_cmd_node *new_cmd = init_cmd_node();
            if (!new_cmd)
                return;
            if (!*cmd_head)
                *cmd_head = new_cmd;
            else
                temp->next = new_cmd;
            temp = new_cmd;
            temp->node_list = head;
            prev_group = head->group;
        }
        head = head->next;
    }
}

void fill_herdock(t_cmd_node *cmd_head)
{
    while (cmd_head)
    {
        t_node *node = cmd_head->node_list;
        int arg_count = 0;
        t_node *tmp = node;
        while (tmp)
        {
            if (tmp->type == ARG)
                arg_count++;
            tmp = tmp->next;
        }
        cmd_head->arr = malloc(sizeof(char *) * (arg_count + 1));
        int i = 0;
        while (node)
        {
            if (node->type == ARG)
                cmd_head->arr[i++] = ft_strdup(node->token);
            else if (node->type == RID_IN && node->next)
            {
                cmd_head->in = ft_strdup(node->next->token);
                node = node->next;
            }
            else if (node->type == HERED && node->next)
            {
                cmd_head->heredoc = ft_strdup(node->next->token);
                node = node->next;
            }
            else if (node->type == RID_OUT && node->next)
            {
                cmd_head->out = ft_strdup(node->next->token);
                cmd_head->append = 0;
                node = node->next;
            }
            else if (node->type == RED_APPEND && node->next)
            {
                cmd_head->out = ft_strdup(node->next->token);
                cmd_head->append = 1;
                node = node->next;
            }
            node = node->next;
        }
        cmd_head->arr[i] = NULL;
        cmd_head = cmd_head->next;
    }
}

void clean_node(t_node *head)
{
    t_node *tmp;
    while (head)
    {
        tmp = head->next;
        free(head->token);
        free(head);
        head = tmp;
    }
}

void identify_node_types(t_node *head)
{
    while (head)
    {
        check_type(head);
        head = head->next;
    }
}

int check_quotes(char *str)
{
    t_char_node *stack = NULL;
    char first_open = 0;
    int i = 0;
    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
        {
            if (!is_empty(stack) && top_stack(stack) == str[i])
                pop_stack(&stack);
            else
            {
                push_stack(&stack, str[i]);
                if (!first_open)
                    first_open = str[i];
            }
        }
        i++;
    }
    if (!is_empty(stack))
    {
        while (!is_empty(stack))
            pop_stack(&stack);
        return -1;
    }
    if (first_open == '\'')
        return 1;
    if (first_open == '"')
        return 2;
    return 0;
}

void idtfy_quotes_types(t_node *head)
{
    while (head)
    {
        head->quotes_type = check_quotes(head->token);
        if (head->quotes_type == -1)
            printf("unclosed quotes: %s\n", head->token);
        head = head->next;
    }
}

void print_types(t_node *head)
{
    while (head)
    {
        printf("token: %s | quotes_type: %d\n", head->token, head->quotes_type);
        head = head->next;
    }
}

int main(void)
{
    char *input = NULL;
    size_t len = 0;
    ssize_t nr;
    int group = 0;
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
        t_node *head = to_linked_list(input);
        identify_node_types(head);
        idtfy_quotes_types(head);
        print_types(head);
        assign_groups(head, group);
        t_cmd_node *cmd_node = NULL;
        group_by_group(&cmd_node, head);
        fill_herdock(cmd_node);
        clean_node(head);
        free(input);
        input = NULL;
    }
    return 0;
}