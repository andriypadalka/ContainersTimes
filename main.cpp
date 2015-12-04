// times are dependent on compiler

#include <iostream>
#include <iomanip>
#include <unordered_set>
#include <set>
#include <vector>
#include <deque>
#include <algorithm>
#include <list>
#include <ctime>
#include <stdint.h>
using namespace std;

//for x86_64 architecture
#ifdef _WIN32                //  Windows
#include <intrin.h>
uint64_t rdtsc()
{
    return __rdtsc();
}
#else                        //  Linux/GCC
uint64_t rdtsc()
{
    unsigned int lo, hi;
    __asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}
#endif //_WIN32

#define EXPR_TIME_CONS_AVER_OUT(nCount, expr, bShowCurTime, outV)\
{\
    char ExprText[sizeof(#expr)] = { 0 }; \
    memcpy(ExprText, #expr, sizeof(#expr)); \
if (bShowCurTime == true)\
    cout << "=== " << ExprText << " === start" << endl; \
    uint64_t ui1, ui2, uiTicks, uiAverage = 0; \
for (int iIn = 0; iIn < nCount; ++iIn)\
{\
    ui1 = rdtsc(); \
    expr; \
    ui2 = rdtsc(); \
    uiTicks = ui2 - ui1; \
    uiAverage += uiTicks; \
if (bShowCurTime == true)\
    cout << uiTicks << endl; \
}\
    outV = uiAverage / nCount; \
    cout << "=== " << ExprText << " average == " << uiAverage / nCount << "\n\n"; \
}


int main()
{
    srand((unsigned int)time(0));
    cout << "Enter size of a storage" << endl;
    int nSize;
    cin >> nSize;
    int * pAr = new int[nSize], *pAr2 = new int[nSize];
    for (int i = 0; i < nSize; ++i)
        pAr[i] = rand();
    vector<int> vc;
    vector<int>::iterator vcIt;
    deque<int> dq;
    deque<int>::iterator dqIt;
    list<int> lst;
    list<int>::iterator lstIt;
    set<int>  st;
    set<int>::iterator  stIt;
    unordered_set<int> unset;
    unordered_set<int>::iterator unsetIt;
    int nCount = 50;

    cout << " ===================  INSERTION  ===================" << endl;
    unsigned __int64 vcPush, dqPush, lstPush, stIns, unsetIns;
    EXPR_TIME_CONS_AVER_OUT(1, for (int i = 0; i < nSize; ++i) vc.push_back(pAr[i]), false, vcPush);    
    EXPR_TIME_CONS_AVER_OUT(1, for (int i = 0; i < nSize; ++i) dq.push_front(pAr[i]), false, dqPush);
    EXPR_TIME_CONS_AVER_OUT(1, for (int i = 0; i < nSize; ++i) lst.push_back(pAr[i]), false, lstPush);
    EXPR_TIME_CONS_AVER_OUT(1, for (int i = 0; i < nSize; ++i) st.insert(pAr[i]), false, stIns);
    EXPR_TIME_CONS_AVER_OUT(1, for (int i = 0; i < nSize; ++i) unset.insert(pAr[i]), false, unsetIns);

    cout << " ===================  SORT  ===================" << endl;
    unsigned __int64 vcSort, dqSort, lstSort;
    EXPR_TIME_CONS_AVER_OUT(1, sort(vc.begin(), vc.end()), false, vcSort);
    EXPR_TIME_CONS_AVER_OUT(1, sort(dq.begin(), dq.end()), false, dqSort);
    EXPR_TIME_CONS_AVER_OUT(1, lst.sort(), false, lstSort);    

    cout << " ===================  SEARCH  ===================" << endl;
    unsigned __int64 vcSearch, dqSearch, lstSearch, stSearch, unsetSearch;
#define VECTOR_LIST_SEARCH(cont, iter) \
    for (int i = 0; i < nSize; ++i)\
    {\
        iter = find(cont.begin(), cont.end(), pAr[i]); \
        pAr2[i] = *iter; \
    }
    EXPR_TIME_CONS_AVER_OUT(nCount, VECTOR_LIST_SEARCH(vc, vcIt), false, vcSearch);
    EXPR_TIME_CONS_AVER_OUT(nCount, VECTOR_LIST_SEARCH(dq, dqIt), false, dqSearch);
    EXPR_TIME_CONS_AVER_OUT(nCount, VECTOR_LIST_SEARCH(lst, lstIt), false, lstSearch);

#define SET_UNORD_SEARCH(cont, iter) \
    for (int i = 0; i < nSize; ++i)\
    {\
        iter = cont.find(pAr[i]); \
        pAr2[i] = *iter; \
    }
    EXPR_TIME_CONS_AVER_OUT(nCount, SET_UNORD_SEARCH(st, stIt), false, stSearch);
    EXPR_TIME_CONS_AVER_OUT(nCount, SET_UNORD_SEARCH(unset, unsetIt), false, unsetSearch);

    int nSetw = 14;
    cout << "+++++  RESULTS  +++++"<<setw(nSetw-1)<<"Total "<<setw(nSetw)<<"push/ins "<<setw(nSetw)<<"search "<<setw(nSetw)<<"sort \n"<< endl;
    cout << "vector<int>        " << setw(nSetw) << vcPush + vcSort + vcSearch << setw(nSetw)<<vcPush<< setw(nSetw)<<vcSearch<<setw(nSetw)<<vcSort<<endl;
    cout << "deque<int>         " << setw(nSetw) << dqPush + dqSort + dqSearch << setw(nSetw)<<dqPush<< setw(nSetw)<<dqSearch<<setw(nSetw)<<dqSort<<endl;
    cout << "list<int>          " << setw(nSetw) << lstPush + lstSort + lstSearch<< setw(nSetw)<<lstPush<< setw(nSetw)<<lstSearch<<setw(nSetw)<<lstSort<<endl;
    cout << "set<int>           " << setw(nSetw) << stIns + stSearch << setw(nSetw)<<stIns<< setw(nSetw)<<stSearch<< endl;
    cout << "unordered_set<int> " << setw(nSetw) << unsetIns + unsetSearch<< setw(nSetw)<<unsetIns<< setw(nSetw)<<unsetSearch <<endl<< endl;


    // ////////////////////////////////////////  DELETION
    delete[] pAr;
    delete[] pAr2;

    return 0;
}

