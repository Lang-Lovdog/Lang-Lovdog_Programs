from math import factorial

def C(n, k):
    return factorial(n) // (factorial(k) * factorial(n - k))

# Definimos la suma de combinatorias
def C_sum(n, k, x):
    m=n-x
    i= k-x if 0<(k-x) else 0
    f= m+1 if m<k else k+1
    acumulativo=0
    while f>i:
        acumulativo+=C(m,i)*C(x,k-i)
        i+=1
        
    return acumulativo

def P(n, k):
    return factorial(n) // factorial(n - k)

def P_sum(n, k, x):
    factk=factorial(k)
    m=n-x
    i= k-x if 0<(k-x) else 0
    f= m+1 if m<k else k+1
    acumulativo=0
    while f>i:
        acumulativo+=C(m,i)*C(x,k-i)*factk
        i+=1
        
    return acumulativo

