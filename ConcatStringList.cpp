#include "ConcatStringList.h"

ConcatStringList::ReferencesList ConcatStringList::refList = ConcatStringList::ReferencesList();
ConcatStringList::DeleteStringList ConcatStringList::delStrList = ConcatStringList::DeleteStringList();

string reverseStr(string s){
    string result="";
    for(int i=s.length()-1;i>=0;i--){
        result+=s[i];
    }
    return result;
}
//Implementation of CharALNode
CharALNode::CharALNode(const char* s){
    this->CharArrayList = string(s);
    this->next = NULL;
    this->size = string(s).length();
}
CharALNode* CharALNode::splitHead(int k){
    CharALNode *newNode = new CharALNode(this->CharArrayList.substr(k).c_str());
    return newNode;
}
CharALNode* CharALNode::splitTail(int k){
    CharALNode *newNode = new CharALNode(this->CharArrayList.substr(0,this->CharArrayList.length()-k).c_str());
    return newNode;
}
CharALNode::~CharALNode(){
    // CharArrayList = "";
    // delete next;
    // size = 0;
}
//Implementation of CSList
ConcatStringList::ConcatStringList(const char *s){
    CharALNode *newNode = new CharALNode(s);
    this->head = newNode;
    this->tail = head;
    this->size = string(s).length();
    refList.Add(head);
    refList.Add(tail);
}
ConcatStringList::ConcatStringList(CharALNode *head, CharALNode *tail){
    this->head = head;
    this->tail = tail;
    this->size = 0;
    CharALNode *temp = head;
    while(temp!=NULL){
        this->size += temp->size;
        temp = temp->next;
    }
    refList.Add(head);
    refList.Add(tail);
}
int ConcatStringList::length() const{
    return this->size;
}
char ConcatStringList::get(int index) const{
    if(index < this->size && index>= 0){
        char result;
        int traverse = 0;
        CharALNode *temp = head;
        while(temp != NULL){
            if(index >= traverse + temp->size){
                traverse += temp->size;
                temp = temp->next;
            }
            else {
                result = temp->CharArrayList[index-traverse];
                break;
            }
        }
        return result;
    }
    else{
        throw out_of_range("Index of string is invalid!");
    }
}
int ConcatStringList::indexOf(char c) const{
    int index = -1;
    string str = this->toString();
    for(int i=0;i<str.length();i++){
        if(str[i]==c) {
            index=i;
            break;
        }
    }
    return index;
}
std::string ConcatStringList::toString() const{
    string newStr = "";
    CharALNode *temp = this->head;
    while(temp!=NULL){
        // cout<<temp->getArrayList()<<" "<<endl;
        // temp = temp->getNext();
        newStr+=temp->CharArrayList;
        temp = temp->next;
    }
    return newStr;
}
ConcatStringList ConcatStringList::concat(const ConcatStringList & otherS)const{
    this->tail->next = otherS.head;
    ConcatStringList newNode = ConcatStringList(this->head,otherS.tail);
    return newNode;
}
ConcatStringList ConcatStringList::subString (int from, int to)const{
    if(from>=to){
        throw logic_error("Invalid range");
    }
    else if(from<0||to<0||from>this->length()||to>this->length()){
        throw out_of_range("Index of string is invalid");
    }
    else{// still not handle in the same node and do not split tail
        CharALNode *head, *tail;
        int findFrom = 0, findTo = 0;
        CharALNode *tempHead = this->head, *tempTail = this->head;
        //find head
        while(tempHead != NULL){
            if(findFrom + tempHead->size <= from){
                findFrom += tempHead->size;
            }
            else{
                head = tempHead->splitHead(from-findFrom);
                break;
            }
            tempHead = tempHead->next;
        }
        //find tail
        while(tempTail != NULL){
            if(findTo + tempTail->size <= to){
                findTo += tempTail->size;
            }
            else{
                findTo += tempTail->size;
                tail = tempTail->splitTail(findTo-to);
                break;
            }
            tempTail = tempTail->next;
        }
        if(tempHead!=tempTail){
            CharALNode *temp = tempHead->next;
            CharALNode *runPtr = head;
            while(temp!=tempTail){
                CharALNode *newNode = new CharALNode(temp->CharArrayList.c_str());
                runPtr->next = newNode;
                runPtr = runPtr->next;
                temp = temp->next;
            }
            if(tail->CharArrayList!=""){
                runPtr->next = tail;
            }
            else{ //14==14
                runPtr = tail;
                runPtr->next = NULL;
            }
        }
        else{
            head = new CharALNode(tempHead->CharArrayList.substr(from-findFrom,to-from).c_str());
            tail = head;
        }
        ConcatStringList subStr = ConcatStringList(head,tail);
        refList.Add(subStr.head);
        refList.Add(subStr.tail);
        return subStr;
    }
}
ConcatStringList ConcatStringList::reverse ()const{
    CharALNode *headCSList = new CharALNode(reverseStr(this->tail->CharArrayList).c_str());
    CharALNode *pivot = this->tail;
    CharALNode *runPtr = headCSList;
    if(pivot==head){
        ConcatStringList reverseCSList = ConcatStringList(headCSList,headCSList);
        refList.Add(reverseCSList.head);
        refList.Add(reverseCSList.tail);
        return reverseCSList;
    }
    else{
        while(pivot!=head->next){
            CharALNode *temp = this->head;
            while(temp->next != pivot) temp = temp->next;
            runPtr->next = new CharALNode(reverseStr(temp->CharArrayList).c_str());//add next
            //go to next;
            pivot = temp;
            runPtr = runPtr->next;
        }
        CharALNode *tailCSList = new CharALNode(reverseStr(this->head->CharArrayList).c_str());
        runPtr->next = tailCSList;
        ConcatStringList reverseCSList = ConcatStringList(headCSList,tailCSList);
        refList.Add(reverseCSList.head);
        refList.Add(reverseCSList.tail);
        return reverseCSList;
    }
}
ConcatStringList::~ConcatStringList(){
    refList.Remove(this->head, this->tail);
    // delStrList.Add(this->head, this->tail);
    // delete head;
    // delete tail;
    // size = 0;
}
//Implement ConcatStringList::ReferenceList
ConcatStringList::ReferencesList::ReferencesList(){
    this->head = new RefNode();
    this->sizeRL = 0;
}
void ConcatStringList::ReferencesList::Add(CharALNode *node){
    RefNode *temp = head;
    RefNode *newNode = new RefNode(node,1,NULL);//create new node;
    bool added = false;
    //added in the same node
    while(temp!=NULL){
        if(temp->node==node) {
            temp->data++;
            added = true;
            break;
        }
        temp = temp->next;
    } 
    //added in another node --> always add in the end
    temp = head;// redeclare temp
    if(!added){
        while(temp->next!=NULL){
            temp = temp->next;
        }
        temp->next = newNode;
        sizeRL++;
    }  
}

void ConcatStringList::ReferencesList::Insert(int data, CharALNode *node){
    RefNode *newNode = new RefNode(node,data,NULL);
    if(data == 0){
        RefNode *temp = head;
        while(temp->next!=NULL){
            temp = temp->next;
        }
        temp->next = newNode;
    }
    else{
        // RefNode* prevPtr = head->next;
        // cout<<"prevPtr: "<<prevPtr->data<<endl;
        // if(!prevPtr){
        //     cout<<"NULL"<<endl;
        //     head->next = newNode;
        // }
        // else if(!prevPtr->next){
        //     prevPtr->next = newNode;
        // }
        // else{
        //     while(prevPtr->next != NULL){
        //         if(prevPtr->next->data >= data){
        //             newNode->next = prevPtr->next;
        //             prevPtr->next = newNode;
        //             break;
        //         }
        //         else{
        //             prevPtr = prevPtr -> next;
        //         }
        //     }
        // }
        
    }
}
int ConcatStringList::ReferencesList::refCountAt(int index)const {
    if(index<0||index>=sizeRL) throw out_of_range("Index of string is invalid!");

    else{
        RefNode *temp = head->next;
        for(int i=1;i<=index;i++){
            temp = temp->next;
        }
        return temp->data;
    }
}
std::string ConcatStringList::ReferencesList::refCountsString()const {
    string result = "RefCounts[";
    RefNode *temp = this->head->next;// using dummy node
    if(temp!=NULL){
        while(temp->next!=NULL){
            result+=to_string(temp->data);
            result+=',';
            temp = temp->next;
        }
        result+=to_string(temp->data);
    }
    result+=']';
    return result;
}
int ConcatStringList::ReferencesList::size()const {
    return this->sizeRL;
}
void ConcatStringList::ReferencesList::removeNode(RefNode *node){
    RefNode *prevPtr = head;
    while(prevPtr->next != node&&prevPtr->next!=NULL){
        prevPtr = prevPtr->next;
    }
    RefNode *nextNode = prevPtr->next->next;
    // delete prevPtr->next;
    prevPtr->next = nextNode;
    // sizeRL--;
}
void ConcatStringList::ReferencesList::Remove(CharALNode *nodeHead, CharALNode *nodeTail){\
    RefNode *saveHead, *saveTail;
    RefNode *temp = head;
    while(temp!=NULL){
        if(temp->node == nodeHead){
            temp->data--;
            //sort in increment, 0 in the end
            int count = temp->data;
            CharALNode *saveNode = temp->node;
            saveHead = temp;
            if(count == 0){
                this->removeNode(temp);
                this->Insert(count, saveNode);
            }
            // else{
                // this->removeNode(temp);
                // this->Insert(count, saveNode);
            // }
            // //clear if head = 0
            // //head->next : head (dummy node)
            if(this->head->next->data==0){
                head->next = NULL;
                sizeRL = 0;
            }
            break;
        }
        temp = temp->next;
    }
    //for temp1
    RefNode *temp1 = head;
    while(temp1!=NULL){
        if(temp1->node == nodeTail){
            temp1->data--;
            //sort in increment, 0 in the end
            int count = temp1->data;
            CharALNode *saveNode = temp1->node;
            saveTail = temp1;
            if(count == 0){
                this->removeNode(temp1);
                this->Insert(count, saveNode);
            }
            else{
                // this->removeNode(temp);
                // this->Insert(count, saveNode);
            }
            // //clear if head = 0
            // //head->next : head (dummy node)
            if(this->head->next->data==0){
                head->next = NULL;
                sizeRL = 0;
            }
            break;
        }
        temp1 = temp1->next;
    }
    // delStrList.traverseToClear();
    delStrList.Add(saveHead, saveTail);
}
ConcatStringList::ReferencesList::~ReferencesList(){
    RefNode *temp = head;
    while(head!=NULL){
        head = head->next;
        delete temp;
        temp = head;
    }
    sizeRL = 0;
}
//Implement ConcatStringList::DeleteStringList
ConcatStringList::DeleteStringList::DeleteStringList(){
    this->head = NULL;
    this->sizeDL = 0;
}
int ConcatStringList::DeleteStringList::size()const {
    return this->sizeDL;
}
std::string ConcatStringList::DeleteStringList::totalRefCountsString()const {
    string result = "TotalRefCounts[";
    DeletedNode *temp = this->head;
    int allCount = 0;
    while(temp!=NULL){
        if(temp->head->node == temp->tail->node){
            allCount = temp->head->data;
        }
        else{
            allCount = temp->head->data + temp->tail->data;
        }
        temp = temp->next;
    }
    if(allCount) result += to_string(allCount);
    result += ']';
    return result;
}
void ConcatStringList::DeleteStringList::Add(RefNode *head, RefNode *tail){
    DeletedNode *newNode = new DeletedNode (head,tail);
    DeletedNode *temp = this->head;
    if(temp!=NULL){
        while(temp->next!=NULL){
            temp = temp->next;
        }
        temp->next = newNode;
    }
    else{
        this->head = newNode;
    }
    this->sizeDL++;
    // traverseToClear();
}
void ConcatStringList::DeleteStringList::traverseToClear(){//uncompleted
    cout<<"before check: "<<sizeDL<<endl;
    // DeletedNode *run = this->head;
    // while(run!=NULL){
    //     if(run->head->data==0&&run->tail->data==0){
    //         cout<<"decrease -1"<<endl;
    //         sizeDL--;
    //     }
    //     run = run->next;
    // }
}
ConcatStringList::DeleteStringList::~DeleteStringList(){
    // DeletedNode *temp = head;
}
void ConcatStringList::DeleteStringList::print(){// for testing deleted String List
    DeletedNode *temp = this->head;
    while(temp!=NULL){
        cout<<temp->head->data<<"   ";
        cout<<temp->tail->data<<endl;
        temp = temp->next;
    }
}