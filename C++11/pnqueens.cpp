/******************************************************************
  pnqueens.C - N-Queens parallel implementation

  Skand Hurkat & Berkin Ilbeyi

******************************************************************/

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
// required for abs function
#include <cstdlib>
// required for memcpy function
#include <cstring>
// for limits
#include <limits.h>
#ifdef _MSC_VER
#include <cstdint>
#else
#include <inttypes.h>
#endif
#include <assert.h>
#ifdef _MSC_VER
static unsigned long __builtin_ctz(uint64_t x)
{
	unsigned long r;
	_BitScanForward64(&r, x);
	return r;
}
#endif
using namespace std;

// Define a bunch of bit level macros
#define BIT_VECTOR uint64_t
#define MAX_SIZE 32
#define __BV(x) (uint64_t(1)<<(x))
#define SET_BIT(x,b) x |= (b)
#define GET_BIT(x,b) (x&(b))
#define RESET_BIT(x,b) x&=~(b)

// number of elements that are fixed per core
#define PERM_FIXED_ELEMENTS 2
#define PERM_FIX_ELEMENTS PERM_FIXED_ELEMENTS

typedef struct {
  int n, p;
  BIT_VECTOR sol[PERM_FIX_ELEMENTS+3];
  int iStack[PERM_FIX_ELEMENTS];
  int taskQueueEmpty;
  uint64_t num_sol;
  int best_prof;
  BIT_VECTOR best_sol[MAX_SIZE];
  mutex lck;
} GM;

GM *gm;

// Tiny backtracking algorithm to get the next element from the task queue
inline void __pull_from_task_queue(BIT_VECTOR* soln)
{
  memcpy(soln,gm->sol,PERM_FIXED_ELEMENTS*sizeof(BIT_VECTOR));
  memcpy((soln+gm->n),(gm->sol+PERM_FIXED_ELEMENTS),3*sizeof(BIT_VECTOR));
  int i;
  int row;
  int n;
  int halfway;
  n = gm->n;
  if(GET_BIT(n,1))
    halfway = (n>>1);
  else
    halfway = (n>>1)-1;
  row = PERM_FIXED_ELEMENTS-1;
  i = gm->iStack[row]-1;
  RESET_BIT(gm->sol[row],__BV(i));
  RESET_BIT(gm->sol[PERM_FIXED_ELEMENTS], __BV(i));
  RESET_BIT(gm->sol[PERM_FIXED_ELEMENTS+1], __BV(n-1-row+i));
  RESET_BIT(gm->sol[PERM_FIXED_ELEMENTS+2], __BV(row+i));
  BIT_VECTOR temp;
  while(1)
  {
    if(row < PERM_FIXED_ELEMENTS)
    {
      for(i = gm->iStack[row]; i < n; i++)
      {
        if(row == 0 && i > halfway) break;
        temp = (~((gm->sol[PERM_FIXED_ELEMENTS])|
              ((gm->sol[PERM_FIXED_ELEMENTS+1])>>(n-1-row))|
              ((gm->sol[PERM_FIXED_ELEMENTS+2])>>row)))&
          ((BIT_VECTOR)(-1)<<i)&((1<<n)-1);
        SET_BIT(temp,__BV(n));
        i = __builtin_ctz(temp);
        if(i == n) break;
        SET_BIT(gm->sol[row],__BV(i));
        SET_BIT(gm->sol[PERM_FIXED_ELEMENTS],__BV(i));
        SET_BIT(gm->sol[PERM_FIXED_ELEMENTS+1],__BV(n-1-row+i));
        SET_BIT(gm->sol[PERM_FIXED_ELEMENTS+2],__BV(i+row));
        gm->iStack[row++] = i+1;
        break;
      }
      if(i == n)
      {
        gm->iStack[row] = 0;
        i = gm->iStack[--row] - 1;
        RESET_BIT(gm->sol[row],__BV(i));
        RESET_BIT(gm->sol[PERM_FIXED_ELEMENTS],__BV(i));
        RESET_BIT(gm->sol[PERM_FIXED_ELEMENTS+1],__BV(n-1-row+i));
        RESET_BIT(gm->sol[PERM_FIXED_ELEMENTS+2],__BV(i+row));
      }
      if(row == 0 && i > halfway)
      {
        gm->taskQueueEmpty = 1;
        break;
      }
    }
    else
    {
      break;
    }
  }
}

inline void recurse_down(int n, uint64_t* num_sol, int* best_prof, 
    BIT_VECTOR* best_sol, BIT_VECTOR* soln, int gRow);

void pnqueens(void)
{
  BIT_VECTOR soln[MAX_SIZE+3];
  BIT_VECTOR best_sol[MAX_SIZE];
  int n = gm->n;
  uint64_t num_sol = 0;
  int best_prof = 0;
  int atTheCorner = 0;
  BIT_VECTOR halfway = __BV(n>>1);
  while(1)
  {
    gm->lck.lock();
    if(gm->taskQueueEmpty)
    {
      if(best_prof > gm->best_prof)
      {
        gm->best_prof = best_prof;
        memcpy(gm->best_sol,best_sol,n*sizeof(BIT_VECTOR));
      }
      if(!atTheCorner)
        num_sol <<= 1;
      gm->num_sol += num_sol;
      gm->lck.unlock();
      break;
    }
    else
    {
      __pull_from_task_queue(soln);
    }
    if(!atTheCorner)
    {
      if(GET_BIT(soln[0],halfway))
      {
        atTheCorner = 1;
        num_sol <<= 1;
      }
    }
    gm->lck.unlock();
    recurse_down(n, &num_sol, &best_prof, best_sol, soln,
        PERM_FIXED_ELEMENTS);
  }
}

inline void recurse_down(int n, uint64_t* num_sol, int* best_prof,
    BIT_VECTOR* best_sol, BIT_VECTOR* soln, int gRow)
{
  int iStack[MAX_SIZE];
  register int i;
  register int row = gRow;
  BIT_VECTOR colsy = soln[n];
  BIT_VECTOR diag_l = soln[n+1];
  BIT_VECTOR diag_r = soln[n+2];
  BIT_VECTOR temp;
  int p1, p2;
  for(i = 0; i < MAX_SIZE; i++)
    iStack[i] = 0;
  while(1)
  {
    if(row < n)
    {
      for(i = iStack[row]; i < n; i++)
      {
        temp = (~(colsy|(diag_l>>(n-1-row))|(diag_r>>row)))&
          ((BIT_VECTOR)(-1)<<i)&((1<<n)-1);
        SET_BIT(temp,__BV(n));
        i = __builtin_ctz(temp);
        if(i == n) break;
        SET_BIT(soln[row],__BV(i));
        SET_BIT(colsy,__BV(i));
        SET_BIT(diag_l,__BV(n-1-row+i));
        SET_BIT(diag_r,__BV(i+row));
        iStack[row++] = i+1;
        break;
      }
      if(i == n)
      {
        if(row == gRow) break;

        iStack[row] = 0;
        i = iStack[--row] - 1;
        RESET_BIT(soln[row],__BV(i));
        RESET_BIT(colsy,__BV(i));
        RESET_BIT(diag_l,__BV(n-1-row+i));
        RESET_BIT(diag_r,__BV(i+row));
      }
    }
    else
    {
      (*num_sol)++;
      p1 = p2 = 0;
      for(i=1; i<n; i++)
      {
        if(GET_BIT(diag_l,__BV(n-1-i)))
          p1 += i;
        if(GET_BIT(diag_l,__BV(n-1+i)))
          p1 += i;
        if(GET_BIT(diag_r,__BV(n-1-i)))
          p2 += i;
        if(GET_BIT(diag_r,__BV(n-1+i)))
          p2 += i;
      }

      if(p1 > *best_prof)
      {
        *best_prof = p1;
        memcpy(best_sol, soln, n*sizeof(BIT_VECTOR));
      }
      else if(p2 > *best_prof)
      {
        *best_prof = p2;
        memcpy(best_sol, soln, n*sizeof(BIT_VECTOR));
      }
      iStack[row] = 0;
      i = iStack[--row] - 1;
      RESET_BIT(soln[row],__BV(i));
      RESET_BIT(colsy,__BV(i));
      RESET_BIT(diag_l,__BV(n-1-row+i));
      RESET_BIT(diag_r,__BV(i+row));
    }
  }
}

void print_sol(int n, BIT_VECTOR *sol) {
  int i, j;

  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      if(GET_BIT(sol[i], __BV(j)))
        printf("Q");
      else
        printf(".");
    }

    // print new line
    printf("\n");
  } 
}

int main(int argc, char **argv) {
  int n, p;
  
  clock_t t1, t2;
  BIT_VECTOR temp;

  if (argc != 3) {
    printf("Usage: pnqueens <# proc> <size>\nAborting...\n");
    exit(1);
  }
  
  gm = new GM;

  p = gm->p = atoi(argv[1]);
  n = gm->n = atoi(argv[2]);
  // do not allow n < 2
  assert(n > 1);
  assert(n <= MAX_SIZE);
  assert(p > 0);
  assert(p <= 8);

  gm->best_prof = 0;
  gm->num_sol = 0;
  gm->taskQueueEmpty = 0;

  int i, row;
  for(i=0; i<PERM_FIXED_ELEMENTS+3; i++)
    gm->sol[i] = 0;
  for(i=0; i<MAX_SIZE; i++)
    gm->best_sol[i] = 0;
  for(i=0; i<PERM_FIXED_ELEMENTS; i++)
    gm->iStack[i] = 0;

  vector<thread> threadGroup;

  t1 = clock();

  i = 0; row = 0;
  while(1)
  {
    if(row < PERM_FIXED_ELEMENTS)
    {
      for(i = gm->iStack[row]; i < n; i++)
      {
        temp = (~((gm->sol[PERM_FIXED_ELEMENTS])|
              ((gm->sol[PERM_FIXED_ELEMENTS+1])>>(n-1-row))|
              ((gm->sol[PERM_FIXED_ELEMENTS+2])>>row)))&
          ((BIT_VECTOR)(-1)<<i)&((1<<n)-1);
        SET_BIT(temp,__BV(n));
        i = __builtin_ctz(temp);
        if(i == n) break;
        SET_BIT(gm->sol[row],__BV(i));
        SET_BIT(gm->sol[PERM_FIXED_ELEMENTS],__BV(i));
        SET_BIT(gm->sol[PERM_FIXED_ELEMENTS+1],__BV(n-1-row+i));
        SET_BIT(gm->sol[PERM_FIXED_ELEMENTS+2],__BV(i+row));
        gm->iStack[row++] = i+1;
        break;
      }
      if(i == n)
      {
        if(row == 0) break;

        gm->iStack[row] = 0;
        i = gm->iStack[--row] - 1;
        RESET_BIT(gm->sol[row],__BV(i));
        RESET_BIT(gm->sol[PERM_FIXED_ELEMENTS],__BV(i));
        RESET_BIT(gm->sol[PERM_FIXED_ELEMENTS+1],__BV(n-1-row+i));
        RESET_BIT(gm->sol[PERM_FIXED_ELEMENTS+2],__BV(i+row));
      }
    }
    else break;
  }

  for (i = 0; i < p-1; i++) 
    threadGroup.push_back(thread(pnqueens));
 
  pnqueens();

  for(thread& t : threadGroup)
	  t.join();

  t2 = clock();

  cout << "Elapsed            : " << (t2-t1)/CLOCKS_PER_SEC << " seconds" 
       << endl
       << "N                  : " << gm->n << endl
       << "Number of solutions: " << gm->num_sol << endl
       << "Best profit        : " << gm->best_prof << endl
       << "# of cores         : " << p << endl;

  // do not print solution if there isn't any
  if (gm->num_sol)
    print_sol(n, gm->best_sol);
  delete gm;

  return 0;
}