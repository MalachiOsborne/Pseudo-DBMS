#include "llf.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* headers[MAX_HEADERS];
node* headofheads;
bool loaded=false;

node* create_node(int id[], char name[], char age[],int id_length)
{
    node* new_node = malloc(sizeof(node));
    if(new_node == NULL)
    {
        return NULL;
    }
    int value=0;
    for(int i=id_length;i>0;i--)
    {
        value+=id[id_length-i]*pow(10,i-1);
    }
    new_node->id=value;

    new_node->name=malloc(strlen(name)+1);
    strcpy(new_node->name,name);

    new_node->age=malloc(strlen(age)+1);
    strcpy(new_node->age,age);

    new_node->next=NULL;

    return new_node;
}

node* append_node(node* head, int id[], char name[], char age[], int id_len)
{
    node* new_node=create_node(id,name,age,id_len);
    if(new_node==NULL)
        return new_node;
    node* end=head;
    //go to end
    while(end->next!=NULL)
    {
        end=end->next;
    }
    //assign the last node to the new node that was created
    end->next=new_node;
    return head;
}

node* insert_after_x(node* head, int key, int id[], char name[], char age[], int id_length)
{
    if(head==NULL)
    {
        return head;
    }
    node* temp=head;
    while(temp!=NULL)
    {
        if(temp->id==key)
        {
            break;
        }
        temp=temp->next;
    }
    if(temp==NULL)
    {
        return head;
    }
    else
    {
        node* new_node=create_node(id,name,age,id_length);
        if(new_node==NULL)
        {
            return head;
        }
        //gotta point to the next node
        new_node->next=temp->next;
        //before linking the rest of the list
        temp->next=new_node;

        return head;
    }
}

node* delete_node(node* head, int key)
{
    if(head==NULL)
    {
        return head;
    }
    node* temp=head;
    //prev is NULL so we can deal with
    //if the head is the key case
    node* prev=NULL;
    while(temp!=NULL)
    {
        if(temp->id==key)
        {
            break;
        }
        //prev always behind
        prev=temp;
        temp=temp->next;
    }
    if(temp==NULL)
    {
        return head;
    }
    else
    {
        //if the head is the key
        if(prev==NULL)
        {
            head=temp->next;
        }
        //every other case
        else
        {
            prev->next=temp->next;
        }
        free(temp->name);
        free(temp->age);
        free(temp);
        return head;
    }
}

node* delete_first_node(node* head)
{
    if(head==NULL)
        return head;
    node* ptr=head;
    head=head->next;
    free(ptr->name);
    free(ptr->age);
    free(ptr);

    return head;
}

void display_list(node* head)
{
    node* temp=head;
    int current_id=1;
    while(temp!=NULL)
    {
        printf("%i)",current_id);
        printf(" id: %i",temp->id);
        printf(" name: %s",temp->name);
        printf(" age: %s\n",temp->age);
        current_id++;
        temp=temp->next;
    }
}

void display_display_list(void)
{
    display_list(headofheads);
}

void present_options(void)
{
    for(int i=0;i<9;i++)
    {
        printf("\n");
    }
    printf("This is a program that lets you:\n");
    printf("(S)earch, (I)nsert, (U)pdate and (D)elete");
    printf(" entries from a .csv file\n");
    printf("Enter action: ");
}

void search_options(void)
{
    printf("Search mode\n");
    printf("Search (I)d, (A)ll\n");
    printf("Enter action: ");
}
void insert_options(void)
{
    printf("Insert mode\n");
}
void update_options(void)
{
    printf("Update mode\n");
    printf("Update (I)d, (N)ame, (A)ge \n");
    printf("Enter action: ");
}
void delete_options(void)
{
    printf("Delete mode\n");
    printf("Delete by (I)d, (N)ame, (A)ge, (E)xtra\n");
    printf("Enter action: ");
}

void usage(void)
{
    printf("Usage: ./tracker [.csv file]\n");
}

bool load(const char* csv)
{
    FILE* file=fopen(csv,"r");
    if(file==NULL)
    {
        printf("Error: Cannot open file\n");
        return false;
    }

    //this stores the number of bytes from the `fgets`
    const int MAX_HEADER_SIZE=32;
    char line[MAX_HEADER_SIZE];

    /*
        apparently the reason i can't use`&line` is because it decays to char(*)[64]
        while `line` decays to char* which is the correct type

        info on fgets:
        fgets(char s[], int size, FILE* file)
        return type: s on success and NULL on failure/EOF
    */
    if(!fgets(line,sizeof(line),file))
    {
        printf("Error: Couldn't load headers\n");
    }

    int separator_idx=0;
    for(int i=0;i<MAX_HEADERS;i++)
    {
        int header_len=0;
        for(int j=separator_idx;j<MAX_HEADER_SIZE;j++)
        {
            if(line[j]==','||line[j]=='\n')
            {
                separator_idx=j+1;
                break;
            }
            header_len++;
        }
        //+1 for the null terminator '\0'
        headers[i]=malloc(header_len+1);
        if(!headers[i])
        {
            printf("Error: Couldn't find free memory");
            return false;
        }
        for(int j=header_len;j>0;j--)
        {
            headers[i][header_len-j]=line[separator_idx-j-1];
        }
        headers[i][header_len]='\0';
    }

    char c;
    //buffers
    int id[4];
    char name[100];
    char age[4];
    int id_idx=0;
    int age_idx=0;
    int name_idx=0;

    int separator_counter=0;

    headofheads=NULL;

    //because of fgets, fread is now at the start of the first entry
    while(fread(&c,sizeof(c),1,file))
    {
        if(isdigit(c)&&separator_counter<=1)
        {
            //subtract 48 because 1 in ascii is 49 2 is 50 and so on
            id[id_idx]=(int)c-48;
            id_idx++;
        }
       else if(isdigit(c))
        {
            age[age_idx]=c;
            age_idx++;
        }
        else if(isalpha(c))
        {
            name[name_idx]=c;
            name_idx++;
        }
        else if(c==','||c=='\n')
        {
            separator_counter++;
        }

        if(separator_counter>=MAX_HEADERS)
        {
            age[age_idx]='\0';
            name[name_idx]='\0';
            separator_counter=0;
            if(headofheads==NULL)
                headofheads=create_node(id,name,age,id_idx);
            else
                headofheads=append_node(headofheads,id,name,age,id_idx);
            name_idx=0;
            age_idx=0;
            id_idx=0;
        }

    }
    fclose(file);

    loaded=true;
    return true;
}

bool unload(void)
{
    if(!loaded)
        return false;
    while(headofheads!=NULL)
    {
        headofheads=delete_first_node(headofheads);
    }
    for(int i=0;i<MAX_HEADERS;i++)
    {
        free(headers[i]);
    }
    return true;
}

