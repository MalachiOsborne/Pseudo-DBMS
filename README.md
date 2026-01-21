# Pseudo-DBMS
### Video Demo: https://www.youtube.com/watch?v=t0hPnMyjVKo
### A pseudo database management system written in C
A simple DBMS that accepts a `.csv` file as a command-line input. The `.csv` must include the headers `id`,`name` and `age`. The reason it's called "pseudo" is because it doesn't allow for columns to be created/inserted, updated, searched and deleted. 
It has 4 modes: Search, Insert, Update and Delete (along with an exit)
### Search mode
In search mode, you can search using an id, name, age or display all entries.
### Insert mode
In insert mode, you can insert a name and an age and an id will automatically be assigned.
### Update mode
In update mode, you can update an id, name or age. An error message displays if: the id being updated doesn't exist, the new id is already being used, name wasn't found, age wasn't found. If naming/age conflicts exist, the program handles that by asking for the id of the desired entry to be altered.
### Delete mode
In delete mode, you delete an entry using its id, name or age. An error message displays if id/name/age aren't found. If naming/age conflicts exist, the program also handles it by asking for an id. Then a confirmation message appears.

## History of making the project
The project started out as a simple tracker: it loads the .csv into a linked list and it allows you to see what's going on in the .csv file. I started out with 3 files: `main.c`, `llf.c` where I have all the linked list functions and `llf.h`. As I was making the create node function, I realised that I could expand the project to not only be able to search but to insert, delete and update the .csv as well. This lead to the idea of making a pseudo DBMS in C. 

## Difficulties during the making
### Load function
The load function was one of the hardest parts of this project because I wouldn't be able to debug anything if I couldn't even get access to the .csv file. I wanted to use `fread` for reading the headers, but after i utilized `fgets` it made everything simpler. However, to try and stay true to my objective, I read the rest of the file using `fread` (which upon looking back is bad since I'm doing a syscall every time I call `fread` but that's an improvement to make later). In the current project, the `headers` array is obsolete. This was unintentional. Unfortunately, I didn't have the time nor the knowledge to be able to configure the struct to include the headers whatever their permutation. 
### File size
As i was implementing more and more functions into `llf.c`, it was becoming clearer and clearer not all the functions belong in the same file. When the file got to 1000+ lines, it became quite tedious to debug. At that point, I decided to split it into two files: `llf.c` where the helper linked list functions lie and the `dbmsf.c` where the DBMS functions that use the linked list functions lie. 
### Compatibility
Since I own two systems that I use frequently: Windows 10 and Arch Linux, switching back and forth was a nightmare to say the least. Refactoring my code to fit both systems was downright unacceptable. Therefore, I added the `windows_compatibility.h` file for my project to work on both Windows based systems as well as UNIX based ones. Note: I haven't tested it on MacOS so I am not sure whether it works there or not.

# Technical aspects of the project
## `llf.c`

### `create_node` function and the `node` struct
In this file, I implemented all the linked list functions necessary for my project to work. First and foremost, the struct that builds upon everything. It used to contain char buffers namely `char name[100]` and `char age[4]` to be able to include the age and name. I decided to make the age a string because there's no need to perform calculations using the number once it gets inserted into the struct. The char buffers were decent but I wanted as much flexibility as possible. This lead to making them both `char*` and `malloc`ing the memory needed. I made sure to free everything when deleting.

### `append_node` and `insert_after_x`
Those two functions are essential in building up the linked list, especially `append_node`. `insert_after_x` function was really important keeping the order in the linked list once there was a node deletion as well as keeping the .csv file organized by id.

### `delete_node` and `delete_first_node`
The `delete_node` function is important for deleting an entry. I was very careful to free any `malloc`s I made previously. The `delete_first_node` function was really only used for the `unload` function in `dbmsf.c`. 

### Display functions and update functions
The display functions are used in `search_options` function. They helped print the entry to the terminal by id, name or age respectively. They also helped the error handling logic by returning a boolean value (for `display_entry_by_id` function, since there may only be 1 of any id) or an int (for the other two functions because there may be multiple entries with the same name/age). 

### `generate_id`
This function was very important in keeping the id count low. Because deletion of an entry removes the id, the .csv would reach upwards of thousands of ids if there was constant deletion and insertion of the same entry. The `generate_id` function generates an id that is not a part of the `ids` array and filling the gaps if there are any.

## `dbmsf.c`

