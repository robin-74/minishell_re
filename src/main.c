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

void assign_groups(t_node *head,int group)
{
    int group_id = group;
    while (head)
    {
        if (head->type == TOKEN_PIPE)
        {
            while(head->type == TOKEN_PIPE)
                head = head->next;
            group_id++;
        }
        else
            head->group = group_id;
        head = head->next;
    }
}

const char *type_to_string(t_token_type type)
{
    if (type == ARG) return "ARG";
    if (type == RID_IN) return "IN";
    if (type == RID_OUT) return "OUT";
    if (type == RED_APPEND) return "APPEND";
    if (type == HERED) return "HEREDOC";
    if (type == TOKEN_PIPE) return "PIPE";
    return "UNKNOWN";
}

void print_token_list(t_node *head)
{
	while (head)
	{
		printf("Token: %s  | Group: %d\n",
			head->token,  head->group);
		head = head->next;
	}
}

int main(void)
{
    char *input = NULL;
    size_t len = 0;
    ssize_t nr;
    int group = 0 ;
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
        assign_groups(head,group);
        print_token_list(head);

        free(input);
        input = NULL;
    }

    return 0;
}
