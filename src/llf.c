#include "llf.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//saves all the ids in there
int ids[256];
int ids_counter=0;
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

    if(id_length==1)
    {
        new_node->id=id[0];
    }
    else
    {
        int value=0;
        for(int i=id_length;i>0;i--)
        {
            value+=id[id_length-i]*pow(10,i-1);
        }
        new_node->id=value;
    }
    ids[ids_counter]=new_node->id;
    ids_counter++;

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
        return head;
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
        //to delete, just replace it with the rest of the
        //array instead of making it 0
        for(int i=temp->id;i<ids_counter-1;i++)
        {
            ids[i]=ids[i+1];
        }
        ids_counter--;

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
    for(int i=ptr->id;i<ids_counter-1;i++)
    {
        ids[i]=ids[i+1];
    }
    ids_counter--;
    free(ptr->name);
    free(ptr->age);
    free(ptr);

    return head;
}

bool display_entry_by_id(node* head, int key)
{
    node* ptr=head;
    if(head==NULL)
    {
        printf("Error: No entries detected\n");
        return false;
    }
    while(ptr!=NULL)
    {
        if(ptr->id==key)
            break;
        ptr=ptr->next;
    }
    if(ptr==NULL)
    {
        printf("Error: Entry not found\n");
        return false;
    }
    else
    {
        printf("id: %i",ptr->id);
        printf(" name: %s",ptr->name);
        printf(" age: %s\n",ptr->age);
        return true;
    }
}

int generate_id(void)
{
    if(ids_counter==0||ids[0]!=1)
        return 1;
    for(int i=0;i<ids_counter;i++)
    {
        bubble_sort(ids,ids_counter);
        if(ids[i+1]!=ids[i]+1)
            return ids[i]+1;
    }
    return ids[ids_counter-1]+1;
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
    printf("This is a simple DBMS written in c. A database management system ");
    printf("has 4 main functions:\n");
}

void search_options(void)
{

    bool done=false;
    while(!done)
    {
        printf("Search mode\n");
        printf("Search (I)d, (A)ll, (E)xit\n");
        printf("Enter action: ");
        char search_type;
        bool accepted=false;
        while(!accepted)
        {
            scanf(" %c",&search_type);
            search_type=tolower(search_type);
            if(search_type!='i'&&search_type!='a'&&search_type!='e')
            {
                printf("Invalid input\n");
                printf("Enter action: Search (I)d, (A)ll (E)xit: ");
            }
            else
            {
                accepted=true;
            }
        }
        //this one is for the next while loop
        accepted=false;
        int id;
        switch(search_type)
        {
            case 'i':
                printf("Search by id\n");
                while(!accepted)
                {
                    printf("Enter id: ");
                    scanf(" %i",&id);
                    accepted=display_entry_by_id(headofheads,id);
                }
                break;
            case 'a':
                printf("Search all\n");
                display_list(headofheads);
                break;
            case 'e':
                done=true;
                break;
        }
    }
}
void insert_options(const char* csv)
{
    bool done=false;
    while(!done)
    {
        printf("Insert mode\n");
        char name_ans[64];
        printf("Enter name: ");
        //now this is some c hocus pocus
        /*
            we're using a "scanset"
            63 is the max size it will read because we need space for `\0`
            [^\n] means it will read everything except newline
            no need for `&` again because when passing in an array, it decays
            to a pointer to the first elemnt
        */
        scanf(" %63[^\n]",name_ans);
        char age_ans[4];
        printf("Enter age: ");
        scanf(" %3s",age_ans);
        //gotta use an array here cuz create node needs it
        //didn't need to for name and age because for some reason it works out
        int generated_id[]={generate_id()};
        //the length is always 1 because i'm storing the generated id all in the
        //first element of the array
        FILE* file=fopen(csv,"a");
        if(file==NULL)
        {
            printf("Error: Couldn't open file to insert");
            continue;
        }
        fprintf(file, "%i,%s,%s\n",generated_id[0],name_ans,age_ans);
        fclose(file);
        headofheads=insert_after_x(headofheads,generated_id[0]-1,generated_id,name_ans,age_ans,1);
        printf("Data entered successfully\n");

        bool accepted=false;
        char more;
        while(!accepted)
        {
            printf("Insert more data? Y/n: ");
            scanf(" %c",&more);
            more=tolower(more);
            if(more!='y'&&more!='n')
            {
                printf("Invalid input\n");
            }
            else
            {
                accepted=true;
            }
        }
        if(more=='n')
            done=true;
    }
}
void update_options(const char* csv)
{
    bool accepted=false;
    while(!accepted)
    {
        printf("Update mode\n");
        printf("Update (I)d, (N)ame, (A)ge, (E)xit \n");
        printf("Enter action: ");
        accepted=true;
    }
}
void delete_options(const char* csv)
{
    printf("Delete mode\n");
    printf("Delete by (I)d, (N)ame, (A)ge, (E)xit\n");
    bool accepted=false;
    while(!accepted)
    {
        printf("Enter action: ");
        accepted=true;
    }
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

void bubble_sort(int a[], int size)
{
    for(int i=0;i<size-1;i++)
    {
        for(int j=0;j<size-i-1;j++)
        {
            if(a[j]>a[j+1])
            {
                int temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
        }
    }
}
