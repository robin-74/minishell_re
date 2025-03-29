#include "../minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum e_token_type {
    ARG,
    RID_IN,
    RID_OUT,
    RED_APPEND,
    HERED,
    TOKEN_PIPE
} t_token_type;

typedef struct s_node {
    char *token;
    int group;
    t_token_type type;
    struct s_node *next;
} t_node;

int str_length(char *input) {
    int i = 0;
    while (input[i])
        i++;
    return i;
}

t_node *init_newNode(int size_str, char *input, int start) {
    int idx = 0;
    char *token = malloc(size_str + 1);
    if (!token)
        return NULL;
    while (idx < size_str) {
        token[idx] = input[start + idx];
        idx++;
    }
    token[idx] = '\0';
    t_node *new_node = malloc(sizeof(t_node));
    if (!new_node) {
        free(token);
        return NULL;
    }
    new_node->token = token;
    new_node->next = NULL;
    new_node->group = 0;
    new_node->type = ARG;
    return new_node;
}

t_node *to_linked_list(char *input) {
    int n = str_length(input);
    int i = 0;
    t_node *head = NULL;
    t_node *temp = NULL;

    while (i < n) {
        while (input[i] && (input[i] == ' ' || input[i] == '\t'))
            i++;
        int size_str = 0;
        while (input[i + size_str] && input[i + size_str] != ' ' && input[i + size_str] != '\t')
            size_str++;
        if (size_str > 0) {
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

int ft_strcmp(char *s1, char *s2) {
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void check_type(t_node *node) {
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

void idtfy_node_type(t_node *head) {
    t_node *temp = head;
    while (temp) {
        check_type(temp);
        temp = temp->next;
    }
}

void group_node(t_node *head) {
    t_node *temp = head;
    int pipe = 0;
    while (temp) {
        if (temp->type == TOKEN_PIPE)
            pipe++;
        temp->group = pipe;
        temp = temp->next;
    }
}

const char *type_to_str(t_token_type type) {
    if (type == ARG) return "ARG";
    if (type == RID_IN) return "IN";
    if (type == RID_OUT) return "OUT";
    if (type == RED_APPEND) return "APPEND";
    if (type == HERED) return "HEREDOC";
    if (type == TOKEN_PIPE) return "PIPE";
    return "UNKNOWN";
}

void print_tokens_by_group(t_node *head) {
    t_node *temp = head;
    while (temp) {
        printf("Token: %-10s | Type: %-7s | Group: %d\n",
               temp->token,
               type_to_str(temp->type),
               temp->group);
        temp = temp->next;
    }
}

int main(void) {
    char *input = NULL;
    size_t len = 0;
    ssize_t nr = 0;

    while (1) {
        printf("\nminishell>> ");
        nr = getline(&input, &len, stdin);
        if (nr < 0)
            break;
        if (nr > 0 && input[nr - 1] == '\n')
            input[nr - 1] = '\0';
        if (input[0] == '\0')
            continue;
        t_node *head = to_linked_list(input);
        idtfy_node_type(head);
        group_node(head);
        print_tokens_by_group(head);
        free(input);
        input = NULL;
    }
    return 0;
}
