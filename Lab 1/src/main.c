#include "bitmap.h"
#include "hash.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// dumpdata
void list_dumpdata(struct list *list);
void hash_dumpdata(struct hash *hash);
void bitmap_dumpdata(struct bitmap *bm);

// list
void list_back_func(struct list *list);
void list_empty_func(struct list *list);
void list_front_func(struct list *list);
void list_insert_func(struct list *list, int pos, int data);
void list_insert_ordered_func(struct list *list, int data);
void list_max_func(struct list *list);
void list_min_func(struct list *list);
void list_pop_front_func(struct list *list);
void list_pop_back_func(struct list *list);
void list_push_front_func(struct list *list, int data);
void list_push_back_func(struct list *list, int data);
void list_remove_func(struct list *list, int pos);
void list_reverse_func(struct list *list);
void list_shuffle_func(struct list *list);
void list_size_func(struct list *list);
void list_sort_func(struct list *list);
void list_splice_func(struct list *target, struct list *origin, int target_pos, int first, int last);
void list_swap_func(struct list *list, int pos1, int pos2);
void list_unique_func(struct list *list, struct list *duplicates);
void list_destroy(struct list *list);

// hash
void hash_apply_func(struct hash *hash);
void hash_clear_func(struct hash *hash);
void hash_delete_func(struct hash *hash, int data);
void hash_empty_func(struct hash *hash);
void hash_find_func(struct hash *hash, int data);
void hash_insert_func(struct hash *hash, int data);
void hash_replace_func(struct hash *hash, int data);
void hash_size_func(struct hash *hash);

// bitmap
void bitmap_all_func(struct bitmap *bm, int start, int cnt);
void bitmap_any_func(struct bitmap *bm, int start, int cnt);
void bitmap_contains_func(struct bitmap *bm, int start, int cnt, char *value);
void bitmap_count_func(struct bitmap *bm, int start, int cnt, char *value);
void bitmap_dump_func(struct bitmap *bm);
void bitmap_expand_func(struct bitmap **bm, int size);
void bitmap_flip_func(struct bitmap *bm, int pos);
void bitmap_mark_func(struct bitmap *bm, int pos);
void bitmap_none_func(struct bitmap *bm, int start, int cnt);
void bitmap_reset_func(struct bitmap *bm, int pos);
void bitmap_scan_func(struct bitmap *bm, int start, int cnt, char *value);
void bitmap_scan_and_flip_func(struct bitmap *bm, int start, int cnt, char *value);
void bitmap_set_func(struct bitmap *bm, int pos, char *value);
void bitmap_set_all_func(struct bitmap *bm, char *value);
void bitmap_set_multiple_func(struct bitmap *bm, int start, int cnt, char *value);
void bitmap_size_func(struct bitmap *bm);
void bitmap_test_func(struct bitmap *bm, int pos);

// etc
int find_index_func(char *str);

int main()
{
    struct list my_list[10];
    struct hash my_hash[10];
    struct bitmap *my_bitmap[10];

    bool list_in_use[10] = {false};
    bool hash_in_use[10] = {false};
    bool bitmap_in_use[10] = {false};

    char command[256];

    while (fgets(command, sizeof(command), stdin) != NULL)
    {

        char *token = strtok(command, " ");
        if (strcmp(token, "create") == 0)
        {
            token = strtok(NULL, " ");
            if (strcmp(token, "list") == 0)
            {
                int index = find_index_func(strtok(NULL, " "));
                list_init(&my_list[index]);
                list_in_use[index] = true;
            }
            else if (strcmp(token, "hashtable") == 0)
            {
                int index = find_index_func(strtok(NULL, " "));
                hash_init(&my_hash[index], hash_hash, hash_less, NULL);
                hash_in_use[index] = true;
            }
            else if (strcmp(token, "bitmap") == 0)
            {
                int index = find_index_func(strtok(NULL, " "));
                int size = atoi(strtok(NULL, " "));
                my_bitmap[index] = bitmap_create(size);
                bitmap_in_use[index] = true;
            }
        }

        else if (strcmp(token, "dumpdata") == 0)
        {
            token = strtok(NULL, " ");
            if (strncmp(token, "list", 4) == 0)
            {
                int index = find_index_func(token);
                list_dumpdata(&my_list[index]);
            }
            else if (strncmp(token, "hash", 4) == 0)
            {
                int index = find_index_func(token);
                hash_dumpdata(&my_hash[index]);
            }
            else if (strncmp(token, "bm", 2) == 0)
            {
                int index = find_index_func(token);
                bitmap_dumpdata(my_bitmap[index]);
            }
        }

        else if (strcmp(token, "delete") == 0)
        {
            token = strtok(NULL, " ");
            if (strcmp(token, "list") == 0)
            {
                int index = find_index_func(token);
                list_in_use[index] = false;
            }
            else if (strcmp(token, "hash") == 0)
            {
                int index = find_index_func(token);
                hash_in_use[index] = false;
            }
            else if (strcmp(token, "bitmap") == 0)
            {
                int index = find_index_func(token);
                bitmap_in_use[index] = false;
            }
        }

        // list ~
        else if (strncmp(token, "list", 4) == 0)
        {
            int index = find_index_func(strtok(NULL, " "));
            if (strcmp(token, "list_back") == 0)
            {
                list_back_func(&my_list[index]);
            }
            else if (strcmp(token, "list_empty") == 0)
            {
                list_empty_func(&my_list[index]);
            }
            else if (strcmp(token, "list_front") == 0)
            {
                list_front_func(&my_list[index]);
            }
            else if (strcmp(token, "list_insert") == 0)
            {
                int pos = atoi(strtok(NULL, " "));
                int data = atoi(strtok(NULL, " "));
                list_insert_func(&my_list[index], pos, data);
            }
            else if (strcmp(token, "list_insert_ordered") == 0)
            {
                int data = atoi(strtok(NULL, " "));
                list_insert_ordered_func(&my_list[index], data);
            }
            else if (strcmp(token, "list_max") == 0)
            {
                list_max_func(&my_list[index]);
            }
            else if (strcmp(token, "list_min") == 0)
            {
                list_min_func(&my_list[index]);
            }
            else if (strcmp(token, "list_pop_front") == 0)
            {
                list_pop_front_func(&my_list[index]);
            }
            else if (strcmp(token, "list_pop_back") == 0)
            {
                list_pop_back_func(&my_list[index]);
            }
            else if (strcmp(token, "list_push_front") == 0)
            {
                int data = atoi(strtok(NULL, " "));
                list_push_front_func(&my_list[index], data);
            }
            else if (strcmp(token, "list_push_back") == 0)
            {
                int data = atoi(strtok(NULL, " "));
                list_push_back_func(&my_list[index], data);
            }
            else if (strcmp(token, "list_remove") == 0)
            {
                int pos = atoi(strtok(NULL, " "));
                list_remove_func(&my_list[index], pos);
            }
            else if (strcmp(token, "list_reverse") == 0)
            {
                list_reverse_func(&my_list[index]);
            }
            else if (strcmp(token, "list_shuffle") == 0)
            {
                list_shuffle_func(&my_list[index]);
            }
            else if (strcmp(token, "list_size") == 0)
            {
                list_size_func(&my_list[index]);
            }
            else if (strcmp(token, "list_sort") == 0)
            {
                list_sort_func(&my_list[index]);
            }
            else if (strcmp(token, "list_splice") == 0)
            {
                int target_pos = atoi(strtok(NULL, " "));
                int org_index = find_index_func(strtok(NULL, " "));
                int first = atoi(strtok(NULL, " "));
                int last = atoi(strtok(NULL, " "));
                list_splice_func(&my_list[index], &my_list[org_index], target_pos, first, last);
            }
            else if (strcmp(token, "list_swap") == 0)
            {
                int pos1 = atoi(strtok(NULL, " "));
                int pos2 = atoi(strtok(NULL, " "));

                list_swap_func(&my_list[index], pos1, pos2);
            }
            else if (strcmp(token, "list_unique") == 0)
            {
                token = strtok(NULL, " ");
                if (token != NULL)
                {
                    int dup_index = find_index_func(token);
                    list_unique_func(&my_list[index], &my_list[dup_index]);
                }
                else
                {
                    list_unique_func(&my_list[index], NULL);
                }
            }
        }

        // hash ~
        else if (strncmp(token, "hash", 4) == 0)
        {
            int index = find_index_func(strtok(NULL, " "));
            if (strcmp(token, "hash_apply") == 0)
            {
                hash_apply_func(&my_hash[index]);
            }
            else if (strcmp(token, "hash_clear") == 0)
            {
                hash_clear_func(&my_hash[index]);
            }
            else if (strcmp(token, "hash_delete") == 0)
            {
                int data = atoi(strtok(NULL, " "));
                hash_delete_func(&my_hash[index], data);
            }
            else if (strcmp(token, "hash_empty") == 0)
            {
                hash_empty_func(&my_hash[index]);
            }
            else if (strcmp(token, "hash_find") == 0)
            {
                int data = atoi(strtok(NULL, " "));
                hash_find_func(&my_hash[index], data);
            }
            else if (strcmp(token, "hash_insert") == 0)
            {
                int data = atoi(strtok(NULL, " "));
                hash_insert_func(&my_hash[index], data);
            }
            else if (strcmp(token, "hash_replace") == 0)
            {
                int data = atoi(strtok(NULL, " "));
                hash_replace_func(&my_hash[index], data);
            }
            else if (strcmp(token, "hash_size") == 0)
            {
                hash_size_func(&my_hash[index]);
            }
        }

        // bitmap~
        else if (strncmp(token, "bitmap", 6) == 0)
        {
            int index = find_index_func(strtok(NULL, " "));
            if (strcmp(token, "bitmap_all") == 0)
            {
                int start = atoi(strtok(NULL, " "));
                int cnt = atoi(strtok(NULL, " "));
                bitmap_all_func(my_bitmap[index], start, cnt);
            }
            else if (strcmp(token, "bitmap_any") == 0)
            {
                int start = atoi(strtok(NULL, " "));
                int cnt = atoi(strtok(NULL, " "));
                bitmap_any_func(my_bitmap[index], start, cnt);
            }
            else if (strcmp(token, "bitmap_contains") == 0)
            {
                int start = atoi(strtok(NULL, " "));
                int cnt = atoi(strtok(NULL, " "));
                token = strtok(NULL, " ");
                bitmap_contains_func(my_bitmap[index], start, cnt, token);
            }
            else if (strcmp(token, "bitmap_count") == 0)
            {
                int start = atoi(strtok(NULL, " "));
                int cnt = atoi(strtok(NULL, " "));
                token = strtok(NULL, " ");
                bitmap_count_func(my_bitmap[index], start, cnt, token);
            }

            else if (strcmp(token, "bitmap_dump") == 0)
            {
                bitmap_dump_func(my_bitmap[index]);
            }
            else if (strcmp(token, "bitmap_expand") == 0)
            {
                int size = atoi(strtok(NULL, " "));
                bitmap_expand_func(&my_bitmap[index], size);
            }
            else if (strcmp(token, "bitmap_flip") == 0)
            {
                int pos = atoi(strtok(NULL, " "));
                bitmap_flip_func(my_bitmap[index], pos);
            }
            else if (strcmp(token, "bitmap_mark") == 0)
            {
                int pos = atoi(strtok(NULL, " "));
                bitmap_mark_func(my_bitmap[index], pos);
            }
            else if (strcmp(token, "bitmap_none") == 0)
            {
                int start = atoi(strtok(NULL, " "));
                int cnt = atoi(strtok(NULL, " "));
                bitmap_none_func(my_bitmap[index], start, cnt);
            }
            else if (strcmp(token, "bitmap_reset") == 0)
            {
                int pos = atoi(strtok(NULL, " "));
                bitmap_reset_func(my_bitmap[index], pos);
            }
            else if (strcmp(token, "bitmap_scan") == 0)
            {
                int start = atoi(strtok(NULL, " "));
                int cnt = atoi(strtok(NULL, " "));
                token = strtok(NULL, " ");
                bitmap_scan_func(my_bitmap[index], start, cnt, token);
            }
            else if (strcmp(token, "bitmap_scan_and_flip") == 0)
            {
                int start = atoi(strtok(NULL, " "));
                int cnt = atoi(strtok(NULL, " "));
                token = strtok(NULL, " ");
                bitmap_scan_and_flip_func(my_bitmap[index], start, cnt, token);
            }
            else if (strcmp(token, "bitmap_set") == 0)
            {
                int pos = atoi(strtok(NULL, " "));
                token = strtok(NULL, " ");
                bitmap_set_func(my_bitmap[index], pos, token);
            }
            else if (strcmp(token, "bitmap_set_all") == 0)
            {
                token = strtok(NULL, " ");
                bitmap_set_all_func(my_bitmap[index], token);
            }
            else if (strcmp(token, "bitmap_set_multiple") == 0)
            {
                int start = atoi(strtok(NULL, " "));
                int cnt = atoi(strtok(NULL, " "));
                token = strtok(NULL, " ");
                bitmap_set_multiple_func(my_bitmap[index], start, cnt, token);
            }
            else if (strcmp(token, "bitmap_size") == 0)
            {
                bitmap_size_func(my_bitmap[index]);
            }
            else if (strcmp(token, "bitmap_test") == 0)
            {
                int pos = atoi(strtok(NULL, " "));
                bitmap_test_func(my_bitmap[index], pos);
            }
        }

        else if (strcmp(token, "quit") == 0)
        {
            break;
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (list_in_use[i])
        {
            list_destroy(&my_list[i]);
        }
        if (hash_in_use[i])
        {
            hash_destroy(&my_hash[i], NULL);
        }
        if (bitmap_in_use[i])
        {
            bitmap_destroy(my_bitmap[i]);
        }
    }

    return 0;
}

/* ------------------------------- dumpdata ------------------------------- */

void list_dumpdata(struct list *list)
{
    if (list_empty(list))
    {
        return;
    }
    struct list_elem *e;
    struct list_item *item;

    for (e = list_begin(list); e != list_end(list); e = list_next(e))
    {
        item = list_entry(e, struct list_item, elem);
        printf("%d ", item->data);
    }
    printf("\n");
}

void hash_dumpdata(struct hash *hash)
{
    if (hash_empty(hash))
    {
        return;
    }
    struct hash_iterator iter;
    struct hash_elem *e;
    struct hash_item *item;

    hash_first(&iter, hash);
    while (hash_next(&iter) != NULL)
    {
        e = hash_cur(&iter);
        item = hash_entry(e, struct hash_item, elem);
        printf("%d ", item->data);
    }
    printf("\n");
}

void bitmap_dumpdata(struct bitmap *bm)
{
    for (int i = 0; i < bitmap_size(bm); i++)
    {
        if (bitmap_test(bm, i))
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
    printf("\n");
}

/* ------------------------------- list ------------------------------- */

void list_back_func(struct list *list)
{
    struct list_elem *e = list_back(list);
    struct list_item *item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void list_empty_func(struct list *list)
{
    if (list_empty(list))
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }
}

void list_front_func(struct list *list)
{
    struct list_elem *e = list_front(list);
    struct list_item *item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void list_insert_func(struct list *list, int pos, int data)
{
    struct list_item *item = (struct list_item *)malloc(sizeof(struct list_item));
    item->data = data;

    struct list_elem *e = list_begin(list);
    for (int i = 0; i < pos; i++)
    {
        e = list_next(e);
    }
    list_insert(e, &(item->elem));
}

void list_insert_ordered_func(struct list *list, int data)
{
    struct list_item *item = (struct list_item *)malloc(sizeof(struct list_item));
    item->data = data;
    list_insert_ordered(list, &(item->elem), list_less, NULL);
}

void list_max_func(struct list *list)
{
    struct list_elem *e = list_max(list, list_less, NULL);
    struct list_item *item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void list_min_func(struct list *list)
{
    struct list_elem *e = list_min(list, list_less, NULL);
    struct list_item *item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void list_pop_front_func(struct list *list)
{
    struct list_elem *e = list_front(list);
    struct list_item *item = list_entry(e, struct list_item, elem);
    list_pop_front(list);
    free(item);
}

void list_pop_back_func(struct list *list)
{
    struct list_elem *e = list_back(list);
    struct list_item *item = list_entry(e, struct list_item, elem);
    list_pop_back(list);
    free(item);
}

void list_push_front_func(struct list *list, int data)
{
    struct list_item *item = (struct list_item *)malloc(sizeof(struct list_item));
    item->data = data;
    list_push_front(list, &(item->elem));
}

void list_push_back_func(struct list *list, int data)
{
    struct list_item *item = (struct list_item *)malloc(sizeof(struct list_item));
    item->data = data;
    list_push_back(list, &(item->elem));
}

void list_remove_func(struct list *list, int pos)
{
    struct list_elem *e = list_begin(list);
    for (int i = 0; i < pos; i++)
    {
        e = list_next(e);
    }
    struct list_item *item = list_entry(e, struct list_item, elem);
    list_remove(e);
    free(item);
}

void list_reverse_func(struct list *list)
{
    list_reverse(list);
}

void list_shuffle_func(struct list *list)
{
    list_shuffle(list);
}

void list_size_func(struct list *list)
{
    printf("%zu\n", list_size(list));
}

void list_sort_func(struct list *list)
{
    list_sort(list, list_less, NULL);
}

void list_splice_func(struct list *target, struct list *origin, int target_pos, int first, int last)
{
    struct list_elem *before = list_begin(target);
    for (int i = 0; i < target_pos; i++)
    {
        before = list_next(before);
    }

    struct list_elem *first_elem = list_begin(origin);
    for (int i = 0; i < first; i++)
    {
        first_elem = list_next(first_elem);
    }

    struct list_elem *last_elem = list_begin(origin);
    for (int i = 0; i < last; i++)
    {
        last_elem = list_next(last_elem);
    }

    list_splice(before, first_elem, last_elem);
}

void list_swap_func(struct list *list, int pos1, int pos2)
{
    struct list_elem *a = list_begin(list);
    struct list_elem *b = list_begin(list);
    for (int i = 0; i < pos1; i++)
    {
        a = list_next(a);
    }
    for (int i = 0; i < pos2; i++)
    {
        b = list_next(b);
    }
    list_swap(a, b);
}

void list_unique_func(struct list *list, struct list *duplicates)
{
    list_unique(list, duplicates, list_less, NULL);
}

void list_destroy(struct list *list)
{
    struct list_elem *e;
    struct list_item *item;
    while (!list_empty(list))
    {
        e = list_pop_front(list);
        item = list_entry(e, struct list_item, elem);
        free(item);
    }
}

/* ------------------------------- hash ------------------------------- */

void hash_apply_func(struct hash *hash)
{
    char *token = strtok(NULL, " ");
    if (strncmp(token, "square", 6) == 0)
    {
        hash_apply(hash, hash_square);
    }
    else if (strncmp(token, "triple", 6) == 0)
    {
        hash_apply(hash, hash_triple);
    }
}

void hash_clear_func(struct hash *hash)
{
    hash_clear(hash, NULL);
}

void hash_delete_func(struct hash *hash, int data)
{
    struct hash_item item;
    item.data = data;
    hash_delete(hash, &(item.elem));
}

void hash_empty_func(struct hash *hash)
{
    if (hash_empty(hash))
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }
}

void hash_find_func(struct hash *hash, int data)
{
    struct hash_item item;
    item.data = data;
    struct hash_elem *e = hash_find(hash, &(item.elem));
    if (e != NULL)
    {
        struct hash_item *item = hash_entry(e, struct hash_item, elem);
        printf("%d\n", item->data);
    }
}

void hash_insert_func(struct hash *hash, int data)
{
    struct hash_item *item = (struct hash_item *)malloc(sizeof(struct hash_item));
    item->data = data;
    hash_insert(hash, &(item->elem));
}

void hash_replace_func(struct hash *hash, int data)
{
    struct hash_item *item = (struct hash_item *)malloc(sizeof(struct hash_item));
    item->data = data;
    hash_replace(hash, &(item->elem));
}

void hash_size_func(struct hash *hash)
{
    printf("%zu\n", hash_size(hash));
}

/* ------------------------------- bitmap ------------------------------- */

void bitmap_all_func(struct bitmap *bm, int start, int cnt)
{
    if (bitmap_all(bm, start, cnt))
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }
}

void bitmap_any_func(struct bitmap *bm, int start, int cnt)
{
    if (bitmap_any(bm, start, cnt))
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }
}

void bitmap_contains_func(struct bitmap *bm, int start, int cnt, char *value)
{
    bool val = false;
    if (strncmp(value, "true", 4) == 0)
    {
        val = true;
    }

    if (bitmap_contains(bm, start, cnt, val) == true)
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }
}

void bitmap_count_func(struct bitmap *bm, int start, int cnt, char *value)
{
    bool val = false;
    if (strncmp(value, "true", 4) == 0)
    {
        val = true;
    }

    printf("%zu\n", bitmap_count(bm, start, cnt, val));
}

void bitmap_dump_func(struct bitmap *bm)
{
    bitmap_dump(bm);
}

void bitmap_expand_func(struct bitmap **bm, int size)
{
    *bm = bitmap_expand(*bm, size);
}

void bitmap_flip_func(struct bitmap *bm, int pos)
{
    bitmap_flip(bm, pos);
}

void bitmap_mark_func(struct bitmap *bm, int pos)
{
    bitmap_mark(bm, pos);
}

void bitmap_none_func(struct bitmap *bm, int start, int cnt)
{
    if (bitmap_none(bm, start, cnt))
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }
}

void bitmap_reset_func(struct bitmap *bm, int pos)
{
    bitmap_reset(bm, pos);
}

void bitmap_scan_func(struct bitmap *bm, int start, int cnt, char *value)
{
    bool val = false;
    if (strncmp(value, "true", 4) == 0)
    {
        val = true;
    }
    printf("%zu\n", bitmap_scan(bm, start, cnt, val));
}

void bitmap_scan_and_flip_func(struct bitmap *bm, int start, int cnt, char *value)
{
    bool val = false;
    if (strncmp(value, "true", 4) == 0)
    {
        val = true;
    }
    printf("%zu\n", bitmap_scan_and_flip(bm, start, cnt, val));
}

void bitmap_set_func(struct bitmap *bm, int pos, char *value)
{
    bool val = false;
    if (strncmp(value, "true", 4) == 0)
    {
        val = true;
    }
    bitmap_set(bm, pos, val);
}

void bitmap_set_all_func(struct bitmap *bm, char *value)
{
    bool val = false;
    if (strncmp(value, "true", 4) == 0)
    {
        val = true;
    }
    bitmap_set_all(bm, val);
}

void bitmap_set_multiple_func(struct bitmap *bm, int start, int cnt, char *value)
{
    bool val = false;
    if (strncmp(value, "true", 4) == 0)
    {
        val = true;
    }
    bitmap_set_multiple(bm, start, cnt, val);
}

void bitmap_size_func(struct bitmap *bm)
{
    printf("%zu\n", bitmap_size(bm));
}

void bitmap_test_func(struct bitmap *bm, int pos)
{
    if (bitmap_test(bm, pos))
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }
}

/* ------------------------------- etc ------------------------------- */

int find_index_func(char *str)
{
    int index;
    char *ptr = str;

    while (*ptr != '\0')
    {
        if (*ptr >= '0' && *ptr <= '9')
        {
            index = atoi(ptr);
            break;
        }
        ptr++;
    }
    return index;
}