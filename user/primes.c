#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void sieve(int pleft[2]){
    // 只接收左端傳來的值，關閉左端的寫
    close(pleft[1]);
    int n, cc;
    cc = read(pleft[0], &n, sizeof(n));  // 一次讀取一個int

    if(cc != sizeof(n)){
        close(pleft[0]);  // close read
        exit(0);  // read 0：EOF(所有寫端都關閉，而且 buffer 也為空)
    }
    printf("prime %d\n", n);  // 第一個數一定是質數

    // 建立右管道，將資料寫往右寫
    int pright[2];
    pipe(pright);
    // 繼續往右建立行程
    int pid = fork();
    if(pid==0){
        // 右端: 讀 (左 pleft 中 pright 右)
        close(pright[1]);  // close write
        close(pleft[0]);   // 不用讀 pleft

        sieve(pright);  // 遞迴調用
        exit(0);
    }else{
        // 中端: 往右端寫
        close(pright[0]);  // 只寫不讀 close read
        int r;
        while(read(pleft[0], &r, sizeof(r)) == sizeof(r))  // 往左讀
        {
            if(r%n!=0){  // 不能整除->仍有可能是質數
                write(pright[1], &r, sizeof(r));  // 往右寫
            }
        }
        // if(cc==0){
        //     write(pright[1], &r, sizeof(r));  // 寫入結束
        // }
        close(pright[1]);
        close(pleft[0]);
        wait(0);
        exit(0);
    }
}

int main(void){
    int p[2];
    pipe(p);
    // 建立子行程
    int pid = fork();

    if(pid==0){
        // child (右端-只讀不寫)
        close(p[1]);  // close write
        sieve(p);
        exit(0);
    }else{
        // parent (左端-只寫不讀)
        close(p[0]);  // close read
        int i;
        for(i=2; i<=35; ++i){
            write(p[1], &i, sizeof(i));
        }
        // 寫入-1表示傳輸完成
        // i = -1;
        // write(p[1], &i, sizeof(i));
        close(p[1]);  // 寫完 close write
        wait(0);
        exit(0);
    }
}