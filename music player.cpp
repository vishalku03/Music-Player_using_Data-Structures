#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <memory>  // For smart pointers

using namespace std;

struct node
{
    char song[100];
    shared_ptr<node> next;
    weak_ptr<node> prev;  // Use weak_ptr to avoid circular references
};

shared_ptr<node> top;

void tofile(const char a[])
{
    ofstream f1("playlist.txt", ios::out | ios::app);
    if (!f1) {
        cerr << "Error opening file for writing" << endl;
        return;
    }
    f1 << a << endl;
}

void add_node(shared_ptr<node> first)
{
    char a[100];
    while (first->next)
    {
        first = first->next;
    }
    first->next = make_shared<node>();
    first->next->prev = first;
    first = first->next;
    first->next = nullptr;
    cout << "\nEnter Song name- ";
    cin.ignore();  // Ignore leftover newline characters
    cin.getline(a, 100);  // Use getline to handle spaces
    strcpy(first->song, a);
    tofile(a);
}

void add_node_file(shared_ptr<node> first, const string &a)
{
    while (first->next)
    {
        first = first->next;
    }
    first->next = make_shared<node>();
    first->next->prev = first;
    first = first->next;
    strcpy(first->song, a.c_str());
    first->next = nullptr;
}

void delete_file(const char a[])
{
    ifstream f1("playlist.txt");
    ofstream f2("temp.txt");
    if (!f1 || !f2) {
        cerr << "Error opening file for reading/writing" << endl;
        return;
    }

    string line;
    bool found = false;
    while (getline(f1, line))
    {
        if (strcmp(a, line.c_str()) != 0)
        {
            f2 << line << endl;
        }
        else
        {
            found = true;
        }
    }
    f1.close();
    f2.close();
    remove("playlist.txt");
    rename("temp.txt", "playlist.txt");
    if (!found)
    {
        cout << "There is no song with the name you entered." << endl;
    }
    else
    {
        cout << "Song has been deleted." << endl;
    }
}

void del_node(shared_ptr<node> first)
{
    if (!first || !first->next)
    {
        cout << "List is empty or has only one node." << endl;
        return;
    }
    while (first->next->next)
    {
        first = first->next;
    }
    shared_ptr<node> temp = first->next;
    first->next = nullptr;
    cout << "Deleted" << endl;
}

void printlist(shared_ptr<node> first)
{
    cout << "\nPlaylist Name- ";
    while (first)
    {
        cout << first->song << endl;
        first = first->next;
    }
}

void count_nodes(shared_ptr<node> first)
{
    int i = 0;
    while (first)
    {
        first = first->next;
        i++;
    }
    cout << "\nTotal songs- " << i << endl;
}

shared_ptr<node> del_pos(shared_ptr<node> pointer, int pos)
{
    if (pos < 0)
    {
        cout << "Invalid position." << endl;
        return pointer;
    }
    shared_ptr<node> prev1 = nullptr;
    shared_ptr<node> temp = pointer;
    int i = 0;

    if (pos == 0)
    {
        if (pointer)
        {
            temp = pointer;
            delete_file(temp->song);
            pointer = pointer->next;
            if (pointer)
            {
                pointer->prev.reset();
            }
            cout << "\nThe list is updated\nUse the display function to check\n";
            return pointer;
        }
    }

    while (temp && i < pos)
    {
        prev1 = temp;
        temp = temp->next;
        i++;
    }

    if (!temp)
    {
        cout << "Position out of range." << endl;
        return pointer;
    }

    if (temp->next)
    {
        temp->next->prev = prev1;
    }
    if (prev1)
    {
        prev1->next = temp->next;
    }
    delete_file(temp->song);
    cout << "\nThe list is updated\nUse the display function to check\n";
    return pointer;
}

void search1(shared_ptr<node> first)
{
    char song[100];
    cout << "\nEnter song to be searched- ";
    cin.ignore();
    cin.getline(song, 100);
    bool found = false;

    while (first)
    {
        if (strcmp(first->song, song) == 0)
        {
            cout << "\n# Song Found" << endl;
            found = true;
            break;
        }
        first = first->next;
    }
    if (!found)
    {
        cout << "\n# Song Not found" << endl;
    }
}

void create()
{
    top = nullptr;
}

void push(const char data[])
{
    if (!top)
    {
        top = make_shared<node>();
        top->next = nullptr;
        strcpy(top->song, data);
    }
    else if (strcmp(top->song, data) != 0)
    {
        auto temp = make_shared<node>();
        temp->next = top;
        strcpy(temp->song, data);
        top = temp;
    }
}

void display()
{
    shared_ptr<node> top1 = top;
    if (!top1)
    {
        cout << "\n=> NO recently played tracks.\n";
        return;
    }
    cout << "\n# Recently played tracks-\n";
    while (top1)
    {
        cout << top1->song << endl;
        top1 = top1->next;
    }
}

void play(shared_ptr<node> first)
{
    char song[100];
    printlist(first);
    cout << "\nChoose song you wish to play- ";
    cin.ignore();
    cin.getline(song, 100);
    bool found = false;

    while (first)
    {
        if (strcmp(first->song, song) == 0)
        {
            cout << "\n=> Now Playing...... " << song << endl;
            found = true;
            push(song);
            break;
        }
        first = first->next;
    }
    if (!found)
    {
        cout << "\n# Song Not found" << endl;
    }
}

void recent()
{
    display();
}

void topelement()
{
    if (!top)
    {
        cout << "\n# NO last played tracks.\n";
        return;
    }
    cout << "\n=> Last Played Song - " << top->song << endl;
}

void sort(shared_ptr<node> &pointer)
{
    if (!pointer) return;
    shared_ptr<node> a = pointer;
    shared_ptr<node> b;
    shared_ptr<node> tmp;

    while (a)
    {
        b = a->next;
        while (b)
        {
            if (strcmp(a->song, b->song) > 0)
            {
                // Swap songs
                tmp = make_shared<node>();
                strcpy(tmp->song, a->song);
                strcpy(a->song, b->song);
                strcpy(b->song, tmp->song);
            }
            b = b->next;
        }
        a = a->next;
    }
}

void addplaylist(shared_ptr<node> start)
{
    ifstream f1("playlist.txt");
    if (!f1) {
        cerr << "Error opening file for reading" << endl;
        return;
    }
    string line;
    while (getline(f1, line))
    {
        add_node_file(start, line);
    }
    cout << "Playlist Added" << endl;
    f1.close();
}

void del_search(shared_ptr<node> start)
{
    char song[100];
    printlist(start);
    cout << "\nChoose song you wish to delete- ";
    cin.ignore();
    cin.getline(song, 100);
    bool found = false;
    shared_ptr<node> prev = nullptr;

    while (start)
    {
        if (strcmp(start->song, song) == 0)
        {
            cout << "\n# Song Found" << endl;
            delete_file(start->song);
            if (prev)
            {
                prev->next = start->next;
            }
            if (start->next)
            {
                start->next->prev = prev;
            }
            if (!prev)
            {
                start = start->next;
            }
            found = true;
            break;
        }
        prev = start;
        start = start->next;
    }
    if (!found)
    {
        cout << "\n# Song Not found" << endl;
    }
}

void deletemenu(shared_ptr<node> start)
{
    int c;
    cout << "Which type of delete do you want?\n1. By Search\n2. By Position" << endl;
    cin >> c;
    switch (c)
    {
    case 1:
        del_search(start);
        break;
    case 2:
        int pos;
        cout << "\nEnter the position of the song: ";
        cin >> pos;
        start = del_pos(start, pos - 1);
        break;
    default:
        cout << "Invalid choice" << endl;
    }
}

int main()
{
    int choice;
    char song[100];
    shared_ptr<node> start = make_shared<node>();
    cout << "\t\t\t**WELCOME**" << endl;
    cout << "\n** Please use '_' for space." << endl;
    cout << "\n\nEnter your playlist name- ";
    cin.ignore();
    cin.getline(start->song, 100);
    start->next = nullptr;
    start->prev.reset();
    create();

    do
    {
        cout << "\n1. Add New Song\n2. Delete Song\n3. Display Entered Playlist\n4. Total Songs\n5. Search Song\n6. Play Song\n7. Recently Played List\n8. Last Played\n9. Sorted Playlist\n10. Add From File\n11. Exit" << endl;
        cout << "Enter your choice- ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            add_node(start);
            break;
        case 2:
            deletemenu(start);
            break;
        case 3:
            printlist(start);
            break;
        case 4:
            count_nodes(start);
            break;
        case 5:
            search1(start);
            break;
        case 6:
            play(start);
            break;
        case 7:
            recent();
            break;
        case 8:
            topelement();
            break;
        case 9:
            sort(start);
            printlist(start);
            break;
        case 10:
            addplaylist(start);
            break;
        case 11:
            return 0;
        default:
            cout << "Invalid choice" << endl;
        }
    } while (choice != 11);

    return 0;
}