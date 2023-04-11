#pragma once

#define alloc(n,T)      (T*)malloc(n*sizeof(T))
#define copy(d,s,n,T)   memcpy((void*)d, (const void*)s, n*sizeof(T))
