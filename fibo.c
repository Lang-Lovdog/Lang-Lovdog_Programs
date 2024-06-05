#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  unsigned int 
    n=0,
    m[2]={0, 1}
  ;


  (--argc) ? ({ goto gotit; }) : ({ goto tellme; });

  tellme:
  printf("Tell me the number of elements"
      "in the Fibonacci sequence: ");
  scanf("%d", &n);
  goto letsgo;

  gotit:
  n=atoi(argv[1]);
  goto letsgo;

  letsgo:
  *(m+(n+1)%2)=n%2;
  *(m+n%2)=(n+1)%2;
  // Los primeros n elementos de la serie de Fibonacci
  bucleFibonacci:
    --n;
    printf("%d%s", m[n%2],n?", ":"\n");
    m[n%2]=m[(n+1)%2]+m[n%2];
  n? ({ goto bucleFibonacci; }) : ({ goto byebye; });


  byebye:
  printf("Bye bye\n");
  return 0;
}
