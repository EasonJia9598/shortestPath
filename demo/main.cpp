
#include <iostream>
#include <time.h>
#include <unistd.h>

using namespace std;


int main(){
    srand((int)time(NULL));
    printf("Enter edge' weight limit\n");
    int n;
    scanf("\n%d",&n);
    int lim , l;
    printf("Enter nodes limitation\n");
    scanf("\n%d",&lim);
    int count = 0;
    for (int x = 0; x < lim; x++) {
        for (int m = 0; m < lim; m++) {
            int exit = 0;
            int k = rand() % n + 1;
            int choose = rand() % lim;
//            do{
//                i = rand() % lim + 1 ;
//                j = rand() % lim + 1 ;
//                k = rand() % lim + 1 ;
//                if (i != j) {
//                    exit = 0;
//                }
//            }while(exit);
            
            if(x != m && choose % 5 == 0 && choose % 2 == 0) {
                count++;
                printf("%d %d %d\n" ,x + 1 , m + 1 , k);
            }
        }
    }
    printf("%d \n", count);
}
