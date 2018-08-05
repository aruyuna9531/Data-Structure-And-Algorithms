#ifndef _QUICK_SORT
#define _QUICK_SORT
//快速排序
void swap(int &i, int &j){
        int tmp=i;
        i=j;
        j=tmp;
}
//普通数组快排
template<typename T>
void qsort(T *arr, int start, int end){
        if(start>=end)return;
        int sta=arr[start],i=start+1,j=end;
        while(i<=j){
            while(i<=end && arr[i]<=sta)i++;
            while(j>start && arr[j]>=sta)j--;
            if(i>j)break;
            swap(arr[i],arr[j]);
        }
        swap(arr[start],arr[j]);
        qsort(arr,start,j-1);
        qsort(arr,j+1,end);
}
//容器vector快排
template<typename T>
void qsort(vector<T>& arr, int start, int end){
        if(start>=end)return;
        int sta=arr[start],i=start+1,j=end;
        while(i<=j){
            while(i<=end && arr[i]<=sta)i++;
            while(j>start && arr[j]>=sta)j--;
            if(i>j)break;
            swap(arr[i],arr[j]);
        }
        swap(arr[start],arr[j]);
        qsort(arr,start,j-1);
        qsort(arr,j+1,end);
}
#endif //_QUICK_SORT
