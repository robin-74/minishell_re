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

void identify_node_types(t_node *head)
{
    while (head)
    {
        check_type(head);
        head = head->next;
    }
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

char *read_heredoc(const char *limiter)
{
    char *line = NULL;
    size_t len = 0;
    char *result = NULL; // error: should be initialized with empty string

    while (1)
    {
        printf("> ");
        if (getline(&line, &len, stdin) == 0) // logic error: should be == -1
            break;
        if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
            break; // error: doesn’t check if it ends with \n
        result = ft_strjoin(result, line); // error: memory leak if result not freed
    }
    return result; // error: missing free for line
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
                cmd_head->heredoc = read_heredoc(node->token); // error: should pass node->next->token
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

void print_cmd_nodes(t_cmd_node *cmd)
{
    int i = 0;
    while (cmd)
    {
        printf("🟩 New Command Group:\n");
        printf("  ARGS: ");
        if (cmd->arr)
        {
            i = 0;
            while (cmd->arr[i])
                printf("[%s] ", cmd->arr[i++]);
        }
        printf("\n");
        if (cmd->in)
            printf("  IN   : %s\n", cmd->in);
        if (cmd->out)
            printf("  OUT  : %s (%s)\n", cmd->out, cmd->append ? "APPEND" : "TRUNC");
        if (cmd->heredoc)
            printf("  HEREDOC:\n%s\n", cmd->heredoc);
        printf("----------------------\n");
        cmd = cmd->next;
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
        assign_groups(head, group);
        t_cmd_node *cmd_node = NULL;
        group_by_group(&cmd_node, head);
        fill_herdock(cmd_node);
        print_cmd_nodes(cmd_node);
        //clean_node(head);
        free(input);
        input = NULL;
    }
    return 0;
}
