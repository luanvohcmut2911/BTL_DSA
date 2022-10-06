#ifndef __CONCAT_STRING_LIST_H__
#define __CONCAT_STRING_LIST_H__

#include "main.h"
string reverseStr(string s);

class CharALNode {
private:
    string CharArrayList;
    CharALNode *next;
    int size;
    friend class ConcatStringList;// CSList can access to private attribute of CharALNode
public:
    CharALNode(const char* s = "");// constructor
    CharALNode* splitHead(int k);// for substr
    CharALNode* splitTail(int k);// for substr
    ~CharALNode();// destructor
};


class ConcatStringList {
public:
    class ReferencesList; // forward declaration
    class DeleteStringList; // forward declaration

public:
    static ReferencesList refList;
    static DeleteStringList delStrList;

    // TODO: may provide some attributes
public:
    CharALNode *head;
    CharALNode *tail;
    int size;//length of charArrayList
public:
    ConcatStringList(const char *s = "");// constructor O(n)
    ConcatStringList(CharALNode *head, CharALNode *tail);// constructor for concat O(n)
    int length() const;//get length of CSList O(1)
    char get(int index) const;// get index O(k) k: num of CharALNode
    int indexOf(char c) const;// O(l) l: size of CSList
    std::string toString() const;// O(l) l: size of CSList
    ConcatStringList concat(const ConcatStringList & otherS) const;// O(1)
    ConcatStringList subString(int from, int to) const;
    ConcatStringList reverse() const;
    ~ConcatStringList();

public:
    class RefNode{// use for referenceList
    public: 
        int data;
        CharALNode *node;
        RefNode *next;
    public:
        RefNode(CharALNode *node = new CharALNode(), int data = 0, RefNode *next = NULL){
            this->node = node;
            this->data = data;
            this->next = next;
        }
        ~RefNode(){}
    };
    class ReferencesList {
        // TODO: may provide some attributes
        private:
            RefNode *head;
            int sizeRL;
            int *arr;//for save node
        public:
            ReferencesList();
            void Add(CharALNode *node);// add a node to refList
            void Insert(int data, CharALNode *node);// insertion sort refNode
            void Remove(CharALNode *node, CharALNode *node1);
            int size() const;
            int refCountAt(int index) const;
            void removeNode(RefNode *node);
            int* checkAndSort();
            std::string refCountsString() const;
            ~ReferencesList();
    };
    class DeletedNode{
    public: 
        RefNode *head;
        RefNode *tail;
        DeletedNode *next;
    public:
        DeletedNode(RefNode *head = NULL, RefNode *tail = NULL){
            this->head = head;
            this->tail = tail;
            this->next = NULL;
        }
        ~DeletedNode(){
            delete head;
            delete tail;
            delete next;
        }
    };
    class DeleteStringList {
        // TODO: may provide some attributes
        private:
            DeletedNode *head;
            // DeletedNode *tail;
            int sizeDL;
        public:
            DeleteStringList();
            int size() const;
            std::string totalRefCountsString() const;
            void Add(RefNode *head, RefNode *tail);
            void traverseToClear();
            void print();
            ~DeleteStringList();
    };
};

#endif // __CONCAT_STRING_LIST_H__