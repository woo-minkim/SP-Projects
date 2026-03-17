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

// global variables
node *root = NULL;
int total_bytes = 0;
int readcnt = 0;
int active_clients = 0;
sem_t mutex, state_mutex;
char response[50000];

// Function declarations
void show_stock(node *cur, char *res);
node *find_stock(node *cur, int ID, char *res);
void process_stock(node *cur, int ID, int qty, char *res, bool is_buy);
void buy(node *cur, int ID, int qty, char *res);
void sell(node *cur, int ID, int qty, char *res);
node *add_node(node *cur, item data);
void load_stock();
void save_to_file(FILE *fp, node *cur);
void save_stock();
void handle_cmd(int fd, char *buf);
void *thread(void *vargp);

// Display all stocks
void show_stock(node *root, char *res)
{
    P(&mutex);
    readcnt++;
    if (readcnt == 1)
        P(&state_mutex);
    V(&mutex);

    // Critical section: show stock
    node *stack[100];
    int top = -1;
    node *cur = root;

    while (cur != NULL || top != -1)
    {
        while (cur != NULL)
        {
            stack[++top] = cur;
            cur = cur->left;
        }

        cur = stack[top--];

        char tmp[100];
        int written = snprintf(tmp, sizeof(tmp), "%d %d %d\n", cur->data.ID, cur->data.left_stock, cur->data.price);
        strncat(res, tmp, written);

        cur = cur->right;
    }

    P(&mutex);
    readcnt--;
    if (readcnt == 0)
        V(&state_mutex);
    V(&mutex);
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

void process_stock(node *cur, int ID, int qty, char *res, bool is_buy)
{
    node *stock = find_stock(cur, ID, res);
    if (stock == NULL)
        return;

    int updated_stock = is_buy ? stock->data.left_stock - qty : stock->data.left_stock + qty;

    if (updated_stock < 0)
    {
        strcat(res, "Not enough left stock\n");
        return;
    }

    P(&stock->data.mutex);
    stock->data.left_stock = updated_stock;
    V(&stock->data.mutex);

    strcat(res, is_buy ? "[buy] success\n" : "[sell] success\n");
    save_stock();
}

void buy(node *cur, int ID, int qty, char *res)
{
    process_stock(cur, ID, qty, res, true);
}

void sell(node *cur, int ID, int qty, char *res)
{
    process_stock(cur, ID, qty, res, false);
}

// Handle client command
void handle_cmd(int fd, char *buf)
{
    total_bytes += strlen(buf);
    printf("Server received %lu (%d total) bytes on fd %d\n", strlen(buf), total_bytes, fd);
    if (!strcmp(buf, "show\n"))
        show_stock(root, response); // Show all stocks

    else if (!strcmp(buf, "exit\n"))
    {
        Rio_writen(fd, response, MAXLINE);
        active_clients--;
        memset(response, 0, sizeof(response));
        exit(0);
    }
    else
    {
        char cmd[20];
        int ID;
        int qty;

        sscanf(buf, "%s %d %d", cmd, &ID, &qty);
        if (!strcmp(cmd, "buy"))
            buy(root, ID, qty, response); // Process buy command

        else if (!strcmp(cmd, "sell"))
            sell(root, ID, qty, response); // Process sell command

        else
            strcat(response, "Invalid command, try again.\n");
    }
    Rio_writen(fd, response, MAXLINE);
    memset(response, 0, sizeof(response));
}

// Thread function to handle client requests
void *thread(void *vargp)
{
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self());
    Free(vargp);

    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);

    while (Rio_readlineb(&rio, buf, MAXLINE) != 0)
        handle_cmd(connfd, buf); // Handle command from client

    Close(connfd);
    save_stock();
    P(&state_mutex);
    active_clients--;
    V(&state_mutex);
    return NULL;
}

// Add node to binary search tree
node *add_node(node *cur, item data)
{
    if (cur == NULL)
    {
        cur = (node *)malloc(sizeof(node));
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
    fprintf(fp, "%d %d %d\n", cur->data.ID, cur->data.left_stock, cur->data.price);
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
    int listenfd, *connfdp;
    char client_host[MAXLINE], client_port[MAXLINE];
    pthread_t tid;
    socklen_t len;
    struct sockaddr_storage client_addr;

    Sem_init(&state_mutex, 0, 1);
    Sem_init(&mutex, 0, 1);

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    load_stock(); // Load stock data from file
    listenfd = Open_listenfd(argv[1]);

    while (1)
    {
        len = sizeof(struct sockaddr_storage);
        connfdp = Malloc(sizeof(int));
        *connfdp = Accept(listenfd, (SA *)&client_addr, &len);
        Getnameinfo((SA *)&client_addr, len, client_host, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_host, client_port);
        P(&state_mutex);
        active_clients++;
        V(&state_mutex);
        Pthread_create(&tid, NULL, thread, connfdp);
    }
    return 0;
}
