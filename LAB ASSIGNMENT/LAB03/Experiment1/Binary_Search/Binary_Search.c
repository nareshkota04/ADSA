#include<stdio.h>
int binarySearch_iterative(int arr[],int n,int target){
    int low=0,high=n-1;
    while(low<=high){
        int mid=low+(high-low)/2;
        if(arr[mid]==target)return mid;
        else if(arr[mid]<target)low=mid+1;
        else high=mid-1;
    }
    return -1;
}
int binarySearch_recursive(int arr[],int low,int high,int target){
    if(low<=high){
        int mid=low+(high-low)/2;
        if(arr[mid]==target)return mid;
        else if(arr[mid]<target)return binarySearch_recursive(arr,mid+1,high,target);
        else binarySearch_recursive(arr,low,mid-1,target);
    }
    return -1;
}
int main()
{
    int n,target;
    printf("Enter Size of Array: \n");
    scanf("%d",&n);
    int arr[n];
    for(int i=0;i<n;i++){
        scanf("%d",&arr[i]);
    }
    printf("Enter key Value: \n");
    scanf("%d",&target);
    
    //int res=binarySearch_iterative(arr,n,target);
    int res=binarySearch_recursive(arr,0,n-1,target);
    if(res==-1)
        printf("Element not found");
    else
        printf("Element found at index %d",res);
    
}