#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define MSG 16

// 回傳寫入幾個字元
// 把整數 x 轉成十進位字串放進 buf
// 回傳字串長度（不含結尾 '\0'）
int pid2str(int x, char *buf)
{
  int i = 0;
  if (x == 0) {
    buf[i++] = '0';
    buf[i] = '\0';
    return i;
  }
  unsigned int ux = x;
  while (ux > 0) {
    buf[i++] = '0' + (ux % 10);
    ux /= 10;
  }
  for (int l = 0, r = i - 1; l < r; l++, r--) {
    char t = buf[l];
    buf[l] = buf[r];
    buf[r] = t;
  }
  buf[i] = '\0';
  return i;
}

// 處理pid長度判斷
// 讀滿 n bytes
int readn(int fd, void *buf, int n){
  int tot = 0;
  char *p = buf;
  while(tot < n){
    int r = read(fd, p + tot, n - tot);
    if(r <= 0) return r;
    tot += r;
  }
  return tot;
}
// 寫滿 n bytes
int writen(int fd, void *buf, int n){
  int tot = 0;
  char *p = buf;
  while(tot < n){
    int r = write(fd, p + tot, n - tot);
    if(r < 0) return -1;
    tot += r;
  }
  return tot;
}

int main(void){
    int p2c[2], c2p[2];
    pipe(p2c);
    pipe(c2p);

    int pid = fork();  // 建立子行程

    if(pid < 0){
      fprintf(2, "fork failed\n");
      exit(1);
    }

    if(pid==0){
        // 子行程執行
        close(p2c[1]);  // p->c write close
        close(c2p[0]);  // c->p read close

        // read parent
        char buf[MSG] = {0};
        int n = readn(p2c[0], buf, MSG);
        // 用固定長度來判斷
        if(n != MSG){
          fprintf(2, "child: readn failed\n");
          exit(1);
        }
        buf[MSG-1] = 0; 
        printf("child pid: %d, parent pid: %s\n", getpid(), buf);

        // write to parent
        char pidbuf[MSG]= {0};  // 初始化
        pid2str(getpid(), pidbuf);
        if(writen(c2p[1], pidbuf, MSG) != MSG){
          fprintf(2, "child: writen failed\n");
          exit(1);
        }

        close(c2p[1]);  // c->p write close
        close(p2c[0]);  // p->c read close
        exit(0);
    }
    else{
        // 父行程執行
        close(p2c[0]);  // p->c read close
        close(c2p[1]);  // c->p write close

        // write
        char pidbuf[MSG]= {0};
        pid2str(getpid(), pidbuf);
        if(writen(p2c[1], pidbuf, MSG) != MSG){
          fprintf(2, "parent: writen failed\n");
          exit(1);
        }
        
        // read child
        char buf[MSG] = {0};
        int n = readn(c2p[0], buf, MSG);
        if(n != MSG){
          fprintf(2, "parent: readn failed\n");
          exit(1);
        }
        buf[MSG-1] = 0; 
        printf("parent pid: %d, child pid: %s\n", getpid(), buf);

        close(p2c[1]);  // p->c write close
        close(c2p[0]);  // c->p read close
        wait(0);
        exit(0);
    }
}