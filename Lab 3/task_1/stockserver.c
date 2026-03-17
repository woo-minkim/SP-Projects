#include "csapp.h"
#include <stdbool.h>
// Define the data structure for stock items
typedef struct
{
    int ID;
    int left_stock;
    int price;
    int readcnt;
    sem_t mutex;
} item;

// Define the tree node structure
typedef struct node
{
    item data;
    struct node *left;
    struct node *right;
} node;

// Define the pool structure for handling clients
typedef struct
{
    int maxfd;
    fd_set read_set;
    fd_set ready_set;
    int nready;
    int maxi;
    int clientfd[FD_SETSIZE];
    rio_t clientrio[FD_SETSIZE];
} pool;

// Global variables
node *root = NULL;
int total_bytes = 0;
int active_clients = 0;
char response[50000];

// Function declarations
void show_stock(node *cur, char *res);
node *find_stock(node *cur, int ID, char *res);
void buy(node *cur, int ID, int qty, char *res);
void sell(node *cur, int ID, int qty, char *res);
node *add_node(node *cur, item data);
void load_stock();
void save_to_file(FILE *fp, node *cur);
void save_stock();
void handle_cmd(int fd, char *buf, pool *p);
void remove_client(int i, pool *p);
void init_pool(int listenfd, pool *p);
void add_client(int connfd, pool *p);
void handle_clients(pool *p);

// Initialize client pool
void init_pool(int listenfd, pool *p)
{
    p->maxi = -1;
    memset(p->clientfd, -1, sizeof(p->clientfd));

    active_clients = 0;
    p->maxfd = listenfd;
    FD_ZERO(&p->read_set);
    FD_SET(listenfd, &p->read_set); // Add listenfd to the read set
}

// Add new client to pool
void add_client(int connfd, pool *p)
{
    p->nready--;
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        if (p->clientfd[i] >= 0)
            continue;

        active_clients++;
        p->clientfd[i] = connfd;
        Rio_readinitb(&p->clientrio[i], connfd);
        FD_SET(connfd, &p->read_set); // Add new connection to read set

        p->maxfd = (connfd > p->maxfd) ? connfd : p->maxfd;
        p->maxi = (i > p->maxi) ? i : p->maxi;
        return;
    }
    app_error("Error : Too many clients");
}

// Display all stocks
void show_stock(node *root, char *res)
{
    node *stack[100]; // Stack for tree nodes
    int top = -1;     // Top of stack
    node *cur = root; // Current node

    // Traverse the tree
    while (cur != NULL || top != -1)
    {
        // Reach the left most Node of the current Node
        while (cur != NULL)
        {
            stack[++top] = cur;
            cur = cur->left;
        }

        // BackTrack from the empty subtree and visit the Node at the top of the stack; however, if the stack is empty you are done
        cur = stack[top--];

        char tmp[100];
        int remaining = sizeof(tmp);
        int written = snprintf(tmp, remaining, "%d %d %d\n", cur->data.ID, cur->data.left_stock, cur->data.price);
        if (written >= remaining || written < 0)
        {
            // Handle error
            return;
        }
        remaining -= written;

        strncat(res, tmp, written); // Use 'written' instead of 'remaining'

        // We have visited the node and its left subtree. Now, it's right subtree's turn
        cur = cur->right;
    }
}

// Find stock by ID
node *find_stock(node *cur, int ID, char *res)
{
    while (cur != NULL && cur->data.ID != ID)
    {
        if (ID < cur->data.ID)
            cur = cur->left; // Traverse left
        else
            cur = cur->right; // Traverse right
    }
    if (cur == NULL)
        strcat(res, "No such stock ID\n");

    return cur;
}

// Process stock purchase
void buy(node *cur, int ID, int qty, char *res)
{
    node *stock = find_stock(cur, ID, res);
    if (stock == NULL)
        return;

    if (stock->data.left_stock < qty)
    {
        strcat(res, "Not enough left stock\n");
        return;
    }
    stock->data.left_stock -= qty; // Decrease left_stock by qty
    strcat(res, "[buy] success\n");
    save_stock(); // Save changes to file
}
// Process stock sale
void sell(node *cur, int ID, int qty, char *res)
{
    node *stock = find_stock(cur, ID, res);
    if (stock == NULL)
    {
        strcat(res, "No such stock ID\n");
        return;
    }

    stock->data.left_stock += qty; // Increase left_stock by qty
    strcat(res, "[sell] success\n");
    save_stock(); // Save changes to file
}
// Handle client requests
void handle_clients(pool *p)
{
    int n;

    for (int i = 0; i <= p->maxi; i++)
    {
        if (p->nready <= 0)
            break;

        int connfd = p->clientfd[i];
        rio_t rio = p->clientrio[i];

        if ((connfd > 0) && (FD_ISSET(connfd, &p->ready_set)))
        {
            p->nready--;
            char buf[MAXLINE];
            if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
            {
                handle_cmd(connfd, buf, p); // Handle command from client
            }
            else
            {
                remove_client(i, p); // Remove client on EOF
            }
        }
    }
}

// Handle client command
void handle_cmd(int fd, char *buf, pool *p)
{
    total_bytes += strlen(buf);
    printf("Server received %lu (%d total) bytes on fd %d\n", strlen(buf), total_bytes, fd);
    // strncat(response, buf, sizeof(response) - strlen(response) - 1);

    if (!strcmp(buf, "show\n"))
    {
        show_stock(root, response); // Show all stocks
        Rio_writen(fd, response, MAXLINE);
        memset(response, 0, sizeof(response));
        return;
    }

    if (!strcmp(buf, "exit\n"))
    {
        Rio_writen(fd, response, MAXLINE);
        active_clients--;
        memset(response, 0, sizeof(response));
        exit(0);
    }

    char cmd[20];
    int ID;
    int qty;

    if (sscanf(buf, "%s %d %d", cmd, &ID, &qty) != 3)
    {
        strncat(response, "Invalid command, try again.\n", sizeof(response) - strlen(response) - 1);
        Rio_writen(fd, response, MAXLINE);
        memset(response, 0, sizeof(response));
        return;
    }

    if (!strcmp(cmd, "buy"))
        buy(root, ID, qty, response); // Process buy command
    else if (!strcmp(cmd, "sell"))
        sell(root, ID, qty, response); // Process sell command
    else
        strncat(response, "Invalid command, try again.\n", sizeof(response) - strlen(response) - 1);

    Rio_writen(fd, response, MAXLINE);
    memset(response, 0, sizeof(response));
}

// Remove client from pool
void remove_client(int i, pool *p)
{
    Close(p->clientfd[i]);
    FD_CLR(p->clientfd[i], &p->read_set); // Remove from read set
    p->clientfd[i] = -1;
    active_clients--;
}

// Add node to binary search tree
node *add_node(node *cur, item data)
{
    if (cur == NULL)
    {
        cur = (node *)malloc(sizeof(node));
        if (cur == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        cur->data = data;
        cur->left = NULL;
        cur->right = NULL;
        return cur;
    }
    if (data.ID > cur->data.ID)
        cur->right = add_node(cur->right, data); // Recurse on right child

    else if (data.ID < cur->data.ID)
        cur->left = add_node(cur->left, data); // Recurse on left child

    return cur;
}

// Load stock data from file
void load_stock()
{
    FILE *fp = fopen("stock.txt", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open file\n");
        exit(EXIT_FAILURE);
    }
    item data;
    while (fscanf(fp, "%d %d %d", &data.ID, &data.left_stock, &data.price) != EOF)
    {
        data.readcnt = 0;
        sem_init(&data.mutex, 0, 1);
        root = add_node(root, data); // Add node to BST
    }
    fclose(fp);
}
// Save stock data to file
void save_to_file(FILE *fp, node *cur)
{
    if (cur == NULL)
        return;
    if (fprintf(fp, "%d %d %d\n", cur->data.ID, cur->data.left_stock, cur->data.price) < 0)
    {
        fprintf(stderr, "Failed to write to file\n");
        exit(EXIT_FAILURE);
    }
    save_to_file(fp, cur->left);  // Recurse on left child
    save_to_file(fp, cur->right); // Recurse on right child
}

// Save all stock data to file
void save_stock()
{
    FILE *fp = fopen("stock.txt", "w");
    save_to_file(fp, root);
    fclose(fp);
}

// Main function
int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_host[MAXLINE], client_port[MAXLINE];
    static pool p;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        fprintf(stderr, "Please provide the port number as an argument.\n");
        exit(0);
    }
    load_stock(); // Load stock data from file
    listenfd = Open_listenfd(argv[1]);
    init_pool(listenfd, &p);

    while (1)
    {
        p.ready_set = p.read_set;
        p.nready = Select(p.maxfd + 1, &p.ready_set, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &p.ready_set))
        {
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            if (connfd < 0)
            {
                fprintf(stderr, "Accept error\n");
                continue;
            }
            add_client(connfd, &p);
            Getnameinfo((SA *)&clientaddr, clientlen, client_host, MAXLINE,
                        client_port, MAXLINE, 0);
            printf("Connected to (%s, %s)\n", client_host, client_port);
        }

        handle_clients(&p);

        if (active_clients == 0)
            save_stock(); // Save stock data to file
    }
    return 0;
}
