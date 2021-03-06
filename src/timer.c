#include <stdio.h> // NULL
#include <sys/time.h>

#include "timer.h"

static struct perf_event_attr attr;
static int fdperf = -1;
static struct perf_event_mmap_page *buf = 0;

static void __init_fdperf() {
  attr.type = PERF_TYPE_HARDWARE;
  attr.config = PERF_COUNT_HW_CPU_CYCLES;
  attr.exclude_kernel = 1;
  fdperf = syscall(__NR_perf_event_open, &attr, 0, -1, -1, 0);
  if (fdperf == -1) {
    return;
  }
  buf = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ, MAP_SHARED, fdperf, 0);
  return;
}
static void __measuresuite_time_pmc(uint64_t *t) {
  // eax: low 32
  // edx: high 32
#if defined(__x86_64__) || defined(__amd64__)
  long long result;
  unsigned int seq;
  long long offset;

  do {
    seq = buf->lock;
    // barrier for cc
    asm volatile("" ::: "memory");
    offset = buf->offset;
    // barrier for cpu
    asm volatile("lfence;\n\t"
                 "cpuid;\n\t" ::
                     : "rax", "rbx", "rcx", "rdx");
    asm volatile("rdpmc;shlq $32,%%rdx;orq %%rdx,%%rax"
                 : "=a"(result)
                 : "c"(buf->index - 1)
                 : "%rdx");

    // barrier for cpu
    asm volatile("lfence;\n\t"
                 "cpuid;\n\t" ::
                     : "rax", "rbx", "rcx", "rdx");

    // barrier for cc
    asm volatile("" ::: "memory");
  } while (buf->lock != seq);

  *t = (result + offset) & 0xffffffffffff;

  // ARM on Darwin does not support mrc p15.
  // clang complains with:
  // unrecognized instruction mnemonic, did you mean: mrs, msr, smc
  // TODO: have a look here:
  // https://lemire.me/blog/2021/03/24/counting-cycles-and-instructions-on-the-apple-m1-processor/
  // https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/blob/master/2021/03/24/benchmark.cpp
  // or here
  // https://github.com/google/benchmark/blob/v1.1.0/src/cycleclock.h#L116
#elif (__linux__ &&                                                            \
       __ARM_ARCH >=                                                           \
           6) // V6 is the earliest arch that has a standard cyclecount
  uint32_t pmccntr;
  uint32_t pmuseren;
  uint32_t pmcntenset;
  // Read the user mode perf monitor counter access permissions. (__ARM_ARCH >=
  // 6)
  asm volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(pmuseren));
  if (pmuseren & 1) { // Allows reading perfmon counters for user mode code.
    asm volatile("mrc p15, 0, %0, c9, c12, 1" : "=r"(pmcntenset));
    if (pmcntenset & 0x80000000ul) { // Is it counting?
      asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(pmccntr));
      // The counter is set up to count every 64th cycle
      *t = (uint64_t)(pmccntr)*64; // Should optimize to << 6
    }
  }
#else
  // fallback
  *t = current_timestamp();
#endif
}

uint64_t ms_current_timestamp() {
  struct timeval time;
  gettimeofday(&time, NULL); // get current time
  uint64_t milliseconds =
      time.tv_sec * 1000LL + time.tv_usec / 1000; // calculate milliseconds
  // printf("milliseconds: %lld\n", milliseconds);
  return milliseconds;
}

static void __measuresuite_time_rdtscp(uint64_t *t) {
  // barrier for cc
  asm volatile("" ::: "memory");
  __asm__ __volatile__("LFENCE;\n\t"
                       "RDTSCP;\n\t"
                       "shl $0x20, %%rdx; \n\t"
                       "or %%rdx, %%rax; \n\t"
                       "mov %%rax, %[time]; \n\t"
                       "CPUID; \n\t"
                       : [time] "=&m"(*t)::"rax", "rbx", "rcx", "rdx");
  // barrier for cc
  asm volatile("" ::: "memory");
}

// prefer pmc
static void (*__timer_function)(uint64_t *) = __measuresuite_time_pmc;

/**
 * This function checks if we use PMC or fall back to something different
 */
void ms_init_timer() {

  // try to initialize
  __init_fdperf();

  if (fdperf == -1) {
    // if that  failed, we need to resort to RDTSCP
    __timer_function = __measuresuite_time_rdtscp;
  }
}

// exposed
void ms_start_timer(uint64_t *start) {
  // we need to reset the PMC if we are using them.
  if (__timer_function == __measuresuite_time_pmc) {
    ioctl(fdperf, PERF_EVENT_IOC_RESET, 0);
  }
  __timer_function(start);
}

uint64_t ms_stop_timer(uint64_t start) {

  uint64_t now = 0;
  __timer_function(&now);
  uint64_t delta = now - start;

  return delta;
}
