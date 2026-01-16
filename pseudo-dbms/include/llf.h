#ifndef LINKED_LIST_FUNCTION_HEADER
#define LINKED_LIST_FUNCTION_HEADER
#include "llf.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEADERS 3

typedef struct node
{
    int id;
    char* name;
    char* age;
    struct node* next;
}
node;

void greet();
void present_options();
void search_options();
void insert_options();
void update_options();
void delete_options();
void usage();
void display_list(node* head);
void display_display_list();
bool load(const char* csv);
bool unload();
node* create_node(int id[], char name[], char age[],int id_length);
node* append_node(node* head, int id[], char name[], char age[], int id_length);
node* insert_after_x(node* head, int key, int id[], char name[], char age[], int id_length);
node* delete_node(node* head, int key);
#endif
