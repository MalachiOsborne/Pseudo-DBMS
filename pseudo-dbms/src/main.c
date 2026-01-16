#include "llf.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    if(argc<2 || argc>2)
    {
        usage();
        return -1;
    }
    bool loaded=load(argv[1]);
    if(loaded)
    {
        present_options();
    }

    char ans;
    bool accepted=false;
    while(!accepted)
    {
        /*
            the space before the `%c` is actually very crucial because it "eats
            any newline characters that the user enters when they enter a wrong
            letter.
            when u type in "r" its actually "r\n" which scanf reads but it only
            putts the "r" in the ans because that's how large the buffer is and
            leaves the "\n" to the next scanf
        */
        scanf(" %c",&ans);
        ans=tolower(ans);
        if(ans!='s'&&ans!='i'&&ans!='u'&&ans!='d')
        {
            printf("Invalid input\n");
            printf("Enter action: (S)earch, (I)nsert, (U)pdate and (D)elete: ");
        }
        else
        {
            accepted=true;
        }
    }

    switch (ans)
    {
        case 's':
            search_options();
            break;
        case 'i':
            insert_options();
            break;
        case 'u':
            update_options();
            break;
        case 'd':
            delete_options();
            break;
    }



    /*
    int id1[]={1};
    int id2[]={2};
    char name1[]={'J','o','h','n'};
    char name2[]={'A','l','i','c','e'};
    char age1[]={'4','2','\0'};
    char age2[]={'3','0','\0'};
    int ilen=1;
    int wlen1=4;
    int wlen2=5;

    node* head=create_node(id1,name1,age1,ilen,wlen1);
    head=append_node(head,id2,name2,age2,ilen,wlen2);

    display_list(head);
    */


    unload();


    return 0;
}
