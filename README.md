# xv6-labs-2020 — util lab1:
* sleep、pingpong、Primes、find、xargs

## Lab 1-1: sleep  
實作一個 xv6 使用者層程式 `sleep`，用來讓目前行程睡眠指定的 **ticks** 數。
- 作業筆記：
  https://www.notion.so/Lab-1-sleep-2d51bda52e4a80c0b21dd3b801872d68?source=copy_link

### 重點:
* 了解 如何lock
* sleep() 系統呼叫會讓 process 進入 SLEEPING 狀態
* sleep 標準模式：
```c
acquire(lk);
while(!condition){
  sleep(chan, lk);  // not busy-wait
}
... // condition true, safely use shared state
release(lk);
```
* sleep(chan, lk) 會：
    1. 原子地釋放 lk
    2. 將 process 設為 SLEEPING
    3. 呼叫 sched() 讓出 CPU
    4. 被 wakeup(chan) 喚醒後再重新 acquire lk

## Lab 1-2: pingpong
了解父行程和子行程溝通
- 作業筆記：
[https://www.notion.so/Lab-1-sleep-2d51bda52e4a80c0b21dd3b801872d68?source=copy_link](https://www.notion.so/Lab-1-2-pingpong-2d51bda52e4a80c6a457ef2c31609606?source=copy_link)
### 重點
* 了解 pipe(p): p[0] (read), p[1] (write)
* 了解 fork()、pid
* getpid()和pid差異

### 進階延伸
父行程 - 子行程互相取得對方PID並輸出
* 使用 pipe 做 PID 傳輸（非共享全域）
* 判斷 read、write 資料完整傳輸 (固定長度)


## Lab 1-3: primes
使用pipe和fork 輸出 2-35 中的質數
- 作業筆記：https://www.notion.so/Lab-1-3-Primes-2d61bda52e4a802397afef6d750caa17?source=copy_link
### 重點
* 多層pipe之間的傳輸
* pipe的端口控制
