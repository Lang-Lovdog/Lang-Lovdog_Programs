#include<stdio.h>
#include<stdlib.h>


int main(){
  float total=1000;
  float discount=0;
  float tax=0;

  //  0   total < 10   -> 10%
  //  9 < total < 100  -> 20%
  // 99 < total < 1000 -> 30%

  tax = total * (
    (total<10) * 0.2 +
    (total>9  && total<100) * 0.1
  );
  discount = total * (
    (total>9  && total<100) * 0.2 +
    (total>99) * 0.3
  );

  printf("Subtotal: %.2f\n", total);
  printf("Tax: %.2f\n", tax);
  printf("Discount: %.2f\n", discount);
  printf("Total: %.2f\n", total + tax - discount);

  return 0;
}
