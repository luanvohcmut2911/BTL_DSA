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
void swap(int *a, int *b){
    int *temp = a;
    a = b;
    b = temp;
    delete temp;
}
void sortArray(int *arr, int size){
    //bubble sort
    for(int i=0;i<size;i++){
        for(int j=i+1;j<size;j++){
            if(arr[j]<arr[i]) swap(arr[j],arr[i]);
        }
    }
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
    // cout<<this->CharArrayList<<"CharALNode is deleted"<<endl;
    CharArrayList = "";
    delete next;
    size = 0;
}
//Implementation of CSList

ConcatStringList::ConcatStringList(const char *s){
    CharALNode *newNode = new CharALNode(s);
    this->head = newNode;
    this->tail = head;
    this->size = string(s).length();
    refList.Add(head);// leak ?? sort ?
    refList.Add(tail);// leak ?? sort ?

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
    refList.Add(head);// leak ?? sort ?
    refList.Add(tail);// leak ?? sort ?
    delete temp;
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
        // delete temp;
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
    while(temp!=tail){
        newStr+=temp->CharArrayList;
        temp = temp->next;
    }
    newStr+=temp->CharArrayList;
    return newStr;
}
ConcatStringList ConcatStringList::concat(const ConcatStringList & otherS)const{
    this->tail->next = otherS.head;
    ConcatStringList *newNode = new ConcatStringList(this->head,otherS.tail);
    return *newNode;
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
        ConcatStringList *subStr = new ConcatStringList(head,tail);
        // delete tempHead;
        // delete tempTail;
        return *subStr;
    }
}
ConcatStringList ConcatStringList::reverse ()const{
    CharALNode *headCSList = new CharALNode(reverseStr(this->tail->CharArrayList).c_str());
    CharALNode *pivot = this->tail;
    CharALNode *runPtr = headCSList;
    if(pivot==head){
        ConcatStringList *reverseCSList = new ConcatStringList(headCSList,headCSList);
        return *reverseCSList;
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
        ConcatStringList *reverseCSList = new ConcatStringList(headCSList,tailCSList);
        delete pivot;
        // delete runPtr;
        return *reverseCSList;
    }
}
ConcatStringList::~ConcatStringList(){
    refList.Remove(this->head, this->tail);//leak ??
    // remove in refList                                     
}
//Implement ConcatStringList::ReferenceList
ConcatStringList::ReferencesList::ReferencesList(){
    this->head = new RefNode();
    this->sizeRL = 0;
    this->arr = NULL;
}
void ConcatStringList::ReferencesList::Add(CharALNode *node){
    RefNode *temp = head;
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
    //added in another node --> always add in the head
    temp = head->next;// redeclare temp
    if(!added){
        RefNode *newNode = new RefNode(node,1,NULL);//create new node;//leak??
        newNode->next = temp;
        head->next = newNode;
        sizeRL++;
    }  
    // delete temp;
    this->checkAndSort();
    // this->print();
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
}
int ConcatStringList::ReferencesList::refCountAt(int index)const {
    if(index<0||index>=sizeRL) throw out_of_range("Index of string is invalid!");
    else{
        return arr[index];
    }
}
void ConcatStringList::ReferencesList::checkAndSort(){
    if(arr!=NULL){
        delete[] arr;
        arr = NULL;
    }
    this->arr = new int[sizeRL];//check leak  
    int index = 0;
    int endpoint = 0;
    RefNode *temp = this->head->next;// using dummy node
    if(temp!=NULL){
        while(temp!=NULL){
            arr[index] = temp->data;
            if(temp->data!=0)endpoint++;
            temp = temp->next;
            index++;
        }
        sortArray(arr,endpoint);
    }
    delete temp;
}
std::string ConcatStringList::ReferencesList::refCountsString()const {// still ensure O(n)
    string result = "RefCounts[";
    if(sizeRL>0){
        for(int i=0;i<sizeRL-1;i++){
            result+=to_string(arr[i]);
            result+=",";
        }
        result+=to_string(arr[sizeRL-1]);
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
    RefNode *saveHead, *saveTail;// for saving to add to delStrList
    RefNode *temp = head;
    while(temp!=NULL){
        if(temp->node == nodeHead){
            temp->data--;
            //sort in increment, 0 in the end
            int count = temp->data;
            saveHead = temp;
            if(count == 0){
                this->Insert(count, temp->node);
                this->removeNode(temp);// delete temp
            }
            // //clear if head = 0
            // //head->next : head (dummy node)
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
            saveTail = temp1;
            if(count == 0){
                this->Insert(count, temp1->node);
                this->removeNode(temp1);
            }
            // //clear if head = 0
            // //head->next : head (dummy node)
            break;
        }
        temp1 = temp1->next;
    }
    if(this->head->next->data==0){//???
        RefNode *temp = head->next;
        while (temp!=NULL){
            head->next = head->next->next;
            // delete temp;
            temp = head->next;
        }
        head->next = NULL;
        sizeRL = 0;
    }
    this->checkAndSort();// convert to array
    delStrList.Add(saveHead, saveTail);
    // delete temp;
    // delete temp1;
}
ConcatStringList::ReferencesList::~ReferencesList(){
    RefNode *temp = head;
    while(head!=NULL){
        head = head->next;
        delete temp;
        temp = head;
    }
    delete[] arr;
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
    delete temp;
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
    // delete temp;
    traverseToClear();
}
void ConcatStringList::DeleteStringList::traverseToClear(){//completed
    DeletedNode *run = this->head;
    while(run!=NULL){
        if(run->head->data==0&&run->tail->data==0){// delete node run
            if(run==this->head){
                this->head = this->head->next;
                delete run;
                run = this->head;
            }
            else{
                DeletedNode *beforeRun = this->head;
                while(beforeRun!=NULL&&beforeRun->next!=run){
                    beforeRun = beforeRun->next;
                }
                
                if(run->next == NULL) beforeRun->next = NULL;
                else {
                    beforeRun->next = beforeRun->next->next;
                    delete run;
                    run = beforeRun->next;
                }
                run = run->next;
            }
            sizeDL--;
        }
        else {
            run = run->next;
        }
    }
    delete run;
}
ConcatStringList::DeleteStringList::~DeleteStringList(){
    DeletedNode *temp = this->head;
    while(temp!=NULL){
        head = head->next;
        delete temp;
        temp = this->head;
    }
    this->sizeDL = 0;
}
void ConcatStringList::DeleteStringList::print(){// for testing deleted String List
    DeletedNode *temp = this->head;
    while(temp!=NULL){
        cout<<temp->head->data<<"   ";
        cout<<temp->tail->data<<endl;
        temp = temp->next;
    }
}