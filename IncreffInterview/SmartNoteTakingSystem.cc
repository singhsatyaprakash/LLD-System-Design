#include<bits/stdc++.h>
using namespace std;

// ---------------- SHARED STORAGE ----------------
class INoteStorage {
protected:
    static map<string, string> notes;
};

map<string, string> INoteStorage::notes;

// ---------------- INTERFACES ----------------
class ICreateNote {
public:
    virtual void CreateNote(string title, string content) = 0;
};

class IEditNote {
public:
    virtual string EditNote(string title, string content) = 0;
};

class ISearchNote {
public:
    virtual vector<string> search(string text) = 0;
};

class IListNote {
public:
    virtual void listAllNotes() = 0;
};

// ---------------- CREATE ----------------
class CreateNoteBook : public INoteStorage, public ICreateNote {
private:
    bool validateFormat(string &title){
        for(char ch : title){
            if(!isalnum(ch) && ch!=' ') return false;
        }
        return true;
    }

public:
    void CreateNote(string title, string content){
        if(!validateFormat(title)){
            cout<<"Invalid title format\n";
            return;
        }

        if(notes.count(title)){
            cout<<"Note already exists\n";
            return;
        }

        notes[title] = content;
        cout<<"Note created successfully\n";
    }
};

// ---------------- EDIT + UNDO REDO ----------------
class EditNoteBook : public INoteStorage, public IEditNote {
private:
    map<string, stack<string>> undoStack;
    map<string, stack<string>> redoStack;

public:
    string EditNote(string title, string newContent){
        if(!notes.count(title)){
            return "Note not found";
        }

        undoStack[title].push(notes[title]);

        // clear redo stack
        while(!redoStack[title].empty()) redoStack[title].pop();

        notes[title] = newContent;
        return "Success";
    }

    void undo(string title){
        if(!notes.count(title) || undoStack[title].empty()){
            cout<<"Nothing to undo\n";
            return;
        }

        redoStack[title].push(notes[title]);
        notes[title] = undoStack[title].top();
        undoStack[title].pop();

        cout<<"Undo done\n";
    }

    void redo(string title){
        if(!notes.count(title) || redoStack[title].empty()){
            cout<<"Nothing to redo\n";
            return;
        }

        undoStack[title].push(notes[title]);
        notes[title] = redoStack[title].top();
        redoStack[title].pop();

        cout<<"Redo done\n";
    }
};

// ---------------- SEARCH ----------------
class SearchNoteBook : public INoteStorage, public ISearchNote {
public:
    vector<string> search(string text){
        vector<string> result;

        string query = text;
        transform(query.begin(), query.end(), query.begin(), ::tolower); //covert text to lower case

        for(auto &it : notes){
            string content = it.second;
            transform(content.begin(), content.end(), content.begin(), ::tolower);

            if(content.find(query) != string::npos){
                result.push_back(it.first);
            }
        }
        return result;
    }
};

// ---------------- LIST ALL NOTES ----------------
class ListNoteBook : public INoteStorage, public IListNote {
public:
    void listAllNotes(){
        if(notes.empty()){
            cout<<"No notes available\n";
            return;
        }

        cout<<"All Notes:\n";
        for(const auto &it : notes){
            cout<<"Title: "<<it.first<<"\n";
            cout<<"Content: "<<it.second<<"\n";
            cout<<"----------------\n";
        }
    }
};

// ---------------- MENU ----------------
int main(){
    CreateNoteBook createObj;
    EditNoteBook editObj;
    SearchNoteBook searchObj;
    ListNoteBook listObj;

    int choice;

    do{
        cout<<"\n1. Create Note\n2. Edit Note\n3. Undo\n4. Redo\n5. Search\n6. View All Notes\n7. Exit\n";
        cout<<"Enter choice: ";
        cin>>choice;

        string title, content;

        switch(choice){
        case 1:
            cin.ignore();
            cout<<"Enter title: ";
            getline(cin, title);
            cout<<"Enter content: ";
            getline(cin, content);
            createObj.CreateNote(title, content);
            break;

        case 2:
            cin.ignore();
            cout<<"Enter title: ";
            getline(cin, title);
            cout<<"Enter new content: ";
            getline(cin, content);
            cout<<editObj.EditNote(title, content)<<endl;
            break;

        case 3:
            cin.ignore();
            cout<<"Enter title: ";
            getline(cin, title);
            editObj.undo(title);
            break;

        case 4:
            cin.ignore();
            cout<<"Enter title: ";
            getline(cin, title);
            editObj.redo(title);
            break;

        case 5:
            cin.ignore();
            cout<<"Enter search text: ";
            getline(cin, content);
            {
                vector<string> res = searchObj.search(content);
                if(res.empty()){
                    cout<<"No matching notes found\n";
                } else {
                    cout<<"Matching Notes:\n";
                    for(auto &t : res){
                        cout<<t<<endl;
                    }
                }
            }
            break;

        case 6:
            listObj.listAllNotes();
            break;

        case 7:
            cout<<"Exiting...\n";
            break;

        default:
            cout<<"Invalid choice\n";
        }

    }while(choice != 7);

    return 0;
}