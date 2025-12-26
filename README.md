# xv6-labs-2020 — util lab1:
* sleep
* pingpong
* Primes
* find
* xargs

## Lab 1: sleep  
實作一個 xv6 使用者層程式 `sleep`，用來讓目前行程睡眠指定的 **ticks** 數。
- 作業說明參考：
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
