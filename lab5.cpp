#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <climits>
using namespace std;

class Ac
{
private:
    unsigned int acno;
    int bal;
    Ac *next;

    Ac()
    {
        acno = 10001;
        bal = 1000;
        next = nullptr;
    }

    Ac(unsigned int acno)
    {
        this->acno = acno;
        bal = 1000;
        next = nullptr;
    }

    friend class AcSLL;
    // friend class txnDLL;
};

class AcSLL
{
private:
    Ac *first;
    Ac *last;
    int len;

public:
    AcSLL()
    {
        first = nullptr;
        last = nullptr;
        len = 0;
    }
    friend class txnDLL;
    void insertAc(int);
    void changebal(unsigned int, int);
    int getbal(unsigned int);
    int GX(int);
    void max();
};
int AcSLL::getbal(unsigned int X)
{
    Ac *temp = first;
    while (temp != last->next)
    {
        if (temp->acno == X)
        {
            return temp->bal;
        }
        temp = temp->next;
    }
    return 0;
}
void AcSLL::max()
{
    int m = INT_MIN;
    Ac *temp = first;
    while (temp != last->next)
    {
        if (temp->bal > m)
            m = temp->bal;
        temp = temp->next;
    }
    temp = first;
    while (temp != last->next)
    {
        if (temp->bal == m)
            cout << temp->acno << " ";
        temp = temp->next;
    }
    cout << endl;
}
int AcSLL::GX(int X)
{
    Ac *temp = first;
    int count = 0;
    while (temp != last->next)
    {
        if (temp->bal >= X)
            count++;
        temp = temp->next;
    }
    return count;
}
void AcSLL::insertAc(int acno)
{
    if (len == 0)
    {
        first = last = new Ac(acno);
        len++;
    }
    else
    {
        last->next = new Ac(acno);
        last = last->next;
        len++;
    }
}
void AcSLL::changebal(unsigned int ac, int change)
{
    Ac *temp = first;
    while (temp != last->next)
    {
        if (temp->acno == ac)
        {
            temp->bal += change;
        }
        temp = temp->next;
    }
}
class txn
{
private:
    unsigned int AccountNo;
    char type;
    int amt;
    txn *next;
    txn *prev;

    txn()
    {
        AccountNo = 10001;
        type = 'x';
        amt = 50001;
        next = nullptr;
        prev = nullptr;
    }

    txn(unsigned int AccountNo, char type, int amt)
    {
        this->AccountNo = AccountNo;
        this->type = type;
        this->amt = amt;
        next = nullptr;
        prev = nullptr;
    }

public:
    void printTxn()
    {
        cout << AccountNo << " " << type << " " << amt << endl;
    }
    friend class txnDLL;
};

class txnDLL
{
private:
    txn *head;
    txn *tail;
    txn *cursor;

public:
    txnDLL()
    {
        head = new txn();
        tail = new txn();
        cursor = new txn();
        head->next = tail;
        head->prev = nullptr;
        tail->prev = head;
        tail->next = nullptr;
        cursor = head;
    }
    void insert(txn *, unsigned int, char, int);
    void insertk(int k, unsigned int, char, int, AcSLL &);
    void insertEnd(unsigned int, char, int);
    void deletetxn(txn *);
    void DAM(unsigned int, int, AcSLL &);
    void printall(unsigned int);
    void processall(AcSLL &);
    void processX(int, AcSLL &);
    void unprocessX(int, AcSLL &);
};
void txnDLL::printall(unsigned int Y)
{
    txn *temp = head;
    if (head != cursor)
    {
        temp = head->next;
    }
    while (temp != cursor->next)
    {
        if (temp->AccountNo == Y)
            temp->printTxn();
        temp = temp->next;
    }
}

void txnDLL::insert(txn *addHere, unsigned int ac, char type, int amt)
{
    txn *newtxn = new txn(ac, type, amt);
    newtxn->next = addHere->next;
    newtxn->prev = addHere;
    (newtxn->next)->prev = newtxn;
    addHere->next = newtxn;
}
void txnDLL::insertk(int k, unsigned int A, char type, int amt, AcSLL &aclist)
{
    txn *temp = head;
    int flag = 0;
    for (int i = 0; i < k; i++)
    {
        if (temp == cursor)
            flag = 1;
        temp = temp->next;
    }
    if (flag != 1)
    {
        int at = amt;
        if (type == 'W')
            at = 0 - at;
        aclist.changebal(A, at);
    }
    insert(temp, A, type, amt);
}
void txnDLL::insertEnd(unsigned int ac, char type, int amt)
{
    insert(tail->prev, ac, type, amt);
}
void txnDLL::deletetxn(txn *toDel)
{
    (toDel->prev)->next = toDel->next;
    (toDel->next)->prev = toDel->prev;
    delete (toDel);
}
void txnDLL::DAM(unsigned int A, int M, AcSLL &aclist)
{
    if (M > 0)
    {
        txn *temp = cursor->next;
        int i = 0;
        while (i < M && temp != tail && temp != nullptr)
        {
            if (temp->AccountNo == A)
            {
                txn *temp2 = temp;
                temp = temp->next;
                deletetxn(temp2);
                i++;
            }
            else
            {
                temp = temp->next;
            }
        }
    }
    else
    {
        txn *temp = cursor->prev;
        int i = 0;
        while (i < abs(M) && temp != head && temp != nullptr)
        {
            if (temp->AccountNo == A)
            {
                txn *temp2 = temp;
                temp = temp->prev;
                int amt = temp2->amt;
                if (temp2->type == 'D')
                    amt = 0 - amt;
                aclist.changebal(temp2->AccountNo, amt);
                deletetxn(temp2);
                i++;
            }
            else
            {
                temp = temp->prev;
            }
        }
    }
}
void txnDLL::processX(int X, AcSLL &aclist)
{
    txn *temp = cursor->next;
    int i = 0;
    while (i < X && temp != tail && temp != nullptr)
    {
        int acno = temp->AccountNo;
        char ty = temp->type;
        int at = temp->amt;
        if (ty == 'W')
            at = 0 - at;
        aclist.changebal(acno, at);
        i++;
        temp = temp->next;
    }
    cursor = temp->prev;
}
void txnDLL::unprocessX(int X, AcSLL &aclist)
{
    txn *temp = cursor;
    int i = 0;
    while (i < X && temp != head && temp != nullptr)
    {
        int acc = temp->AccountNo;
        char ty = temp->type;
        int at = temp->amt;
        if (ty == 'D')
            at = 0 - at;
        aclist.changebal(acc, at);
        i++;
        temp = temp->prev;
    }
    cursor = temp;
}
void txnDLL::processall(AcSLL &aclist)
{
    txn *temp = cursor->next;
    while (temp != tail)
    {
        int acno = temp->AccountNo;
        char ty = temp->type;
        int at = temp->amt;
        if (ty == 'W')
            at = 0 - at;
        aclist.changebal(acno, at);
        temp = temp->next;
    }
    cursor = temp->prev;
}
int main()
{

    int C;
    cin >> C;
    AcSLL acList;
    for (int i = 0; i < C; i++)
    {
        int ac;
        cin >> ac;
        acList.insertAc(ac);
    }
    int N;
    cin >> N;
    txnDLL txnList;
    for (int i = 0; i < N; i++)
    {
        unsigned int ac;
        cin >> ac;
        char type;
        cin >> type;
        int amt;
        cin >> amt;
        txnList.insertEnd(ac, type, amt);
    }
    string S;
    cin >> S;
    while (S != "END")
    {
        if (S == "F")
        {
            int X;
            cin >> X;
            txnList.processX(X, acList);
        }
        else if (S == "R")
        {
            int X;
            cin >> X;
            txnList.unprocessX(X, acList);
        }
        else if (S == "I")
        {
            unsigned int A;
            char type;
            int amt;
            cin >> A >> type >> amt;
            int k;
            cin >> k;
            txnList.insertk(k, A, type, amt, acList);
        }
        else if (S == "D")
        {
            unsigned int A;
            int M;
            cin >> A >> M;
            txnList.DAM(A, M, acList);
        }
        else if (S == "C")
        {
            txnList.processall(acList);
        }
        else if (S == "S")
        {
            unsigned int Y;
            cin >> Y;
            txnList.printall(Y);
        }
        else if (S == "G")
        {
            int X;
            cin >> X;
            cout << acList.GX(X) << endl;
        }
        else if (S == "M")
        {
            acList.max();
        }
        else if (S == "V")
        {
            unsigned int X;
            cin >> X;
            cout << acList.getbal(X) << endl;
        }
        cin >> S;
    }
    return 0;
}
