/**************************
 * Includes
 *
 **************************/

#include <stdio.h>

/**************************
 * Variable Declarations
 * 
 **************************/

struct node
{
    const char *path;
    int x;
    int y;
    int w;
    int h;
    int l;
    struct node *next;
};

/**************************
 * Function Declarations
 *
 **************************/

void editNode(struct node *node, int node_number, const char *path, int x, int y, int w, int h, int l)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    node->path = path;
    node->x = x;
    node->y = y;
    node->w = w;
    node->h = h;
    node->l = l;
}

void editNodePath(struct node *node, int node_number, const char *path)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    node->path = path;
}

void editNodeX(struct node *node, int node_number, int x)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    node->x = x;
}

void editNodeY(struct node *node, int node_number, int y)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    node->y = y;
}

void editNodeW(struct node *node, int node_number, int w)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    node->w = w;
}

void editNodeH(struct node *node, int node_number, int h)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    node->h = h;
}

void editNodeL(struct node *node, int node_number, int l)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    node->l = l;
}

void pushNode(struct node **head, const char *path, int x, int y, int w, int h, int l) //insert new node to the first place
{
    struct node *newNode = new node;
    newNode->path = path;
    newNode->x = x;
    newNode->y = y;
    newNode->w = w;
    newNode->h = h;
    newNode->l = l;
    newNode->next = (*head);
    (*head) = newNode;
}

void insertNode(struct node *prev_node, const char *path, int x, int y, int w, int h, int l) //insert new node after a node
{
    struct node *newNode = new node;

    newNode->path = path;
    newNode->x = x;
    newNode->y = y;
    newNode->w = w;
    newNode->h = h;
    newNode->l = l;
    newNode->next = prev_node->next;
    prev_node->next = newNode;
}

void appendNode(struct node **head, const char *path, int x, int y, int w, int h, int l) //insert new node to the end of the list
{
    struct node *newNode = new node;
    struct node *last = *head;
    newNode->path = path;
    newNode->x = x;
    newNode->y = y;
    newNode->w = w;
    newNode->h = h;
    newNode->l = l;
    newNode->next = NULL;
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = newNode;
    return;
}

const char *getNodePath(struct node *node, int node_number)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    return node->path;
}

int getNodeX(struct node *node, int node_number)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    return node->x;
}

int getNodeY(struct node *node, int node_number)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    return node->y;
}

int getNodeW(struct node *node, int node_number)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    return node->w;
}

int getNodeH(struct node *node, int node_number)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    return node->h;
}

int getNodeL(struct node *node, int node_number)
{
    for (int i = 0; i < node_number; i++)
    {
        node = node->next;
    }
    return node->l;
}