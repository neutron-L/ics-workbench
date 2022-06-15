#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  // return a + b;
  int64_t t;
  asm("addq %1, %2\n\t"
      "movq %2, %0\n\t"
    : "=r"(t)
    : "r"(a), "r"(b)
  );
  return t;
}

int asm_popcnt(uint64_t x) {
  // int s = 0;
  // for (int i = 0; i < 64; i++) {
  //   if ((x >> i) & 1) s++;
  // }
  // return s;
  int64_t count;
  asm(
        "xor %0, %0\n\t"
        "movq %1, %%rax\n\t"
    ".l: movq %%rax, %%rbx\n\t"
        "neg %%rbx\n\t"
        "and %%rax, %%rbx\n\t"
        "je .r\n\t"
        "addq $1, %0\n\t"
        "subq %%rbx, %%rax\n\t"
        "jmp .l\n\t"
    ".r: "
    :"=&r"(count)
    : "r"(x)
    : "%rax", "%rbx"
  );
  return count;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  // return memcpy(dest, src, n);
  
	// if(NULL == dest || NULL == src){
	// 	return NULL;
	// }
	
	// void *ret = dest;
	
	// if(dest <= src || (char *)dest >= (char *)src + n){
	// 	//没有内存重叠，从低地址开始复制
	// 	while(n--){
	// 		*(char *)dest = *(char *)src;
	// 		dest = (char *)dest + 1;
	// 		src = (char *)src + 1;
	// 	}
	// }else{
	// 	//有内存重叠，从高地址开始复制
	// 	src = (char *)src + n - 1;
	// 	dest = (char *)dest + n - 1;
	// 	while(n--){
	// 		*(char *)dest = *(char *)src;
	// 		dest = (char *)dest - 1;
	// 		src = (char *)src - 1;
	// 	}
	// }
	// return ret;

  void * ret = dest;

  asm(
    "movq %1, %0\n\t"
    "test %3, %3\n\t"
    "je .d\n\t"
    "test %1, %1\n\t"
    "je .d\n\t"
    "test %2, %2\n\t"
    "jne .a\n\t"
    "movq 0, %0\n\t"
    "jmp .d\n\t"
  ".a: \n\t"
    "cmpq %2, %1\n\t"
    "jbe .c\n\t"
    "movq %2, %%rax\n\t"
    "addq %3, %%rax\n\t"
    "cmpq %%rax, %1\n\t"
    "jae .c\n\t"
  
    "addq %3, %1\n\t"
    "addq %3, %2\n\t"
    "dec %1\n\t"
    "dec %2\n\t"
  ".b: \n\t"
    "movzbl (%2), %%eax\n\t"
    "movb %%al, (%1)\n\t"
    "dec %1\n\t"
    "dec %2\n\t"
    "dec %3\n\t"
    "je .d\n\t"
    "jmp .b\n\t"

  ".c:\n\t"
    "movzbl (%2), %%eax\n\t"
    "movb %%al, (%1)\n\t"
    "inc %1\n\t"
    "inc %2\n\t"
    "dec %3\n\t"
    "je .d\n\t"
    "jmp .c\n\t"

  ".d: \n\t"
    : "=&r" (ret)
    :"r"(dest), "r"(src), "r"(n)
    : "%rax", "%rdi", "%rsi"
  );

  return ret;
}

// int asm_setjmp(asm_jmp_buf env) {
//   // return setjmp(env);
//   asm(
//     "push %rbp\n\t"
//     "movq %rbx,(%rdi)\n\t"
//     "movq (%rsp), %rax\n\t"
//     "movq %rax, 0x8(%rdi)\n\t"
//     "movq %r12, 0x10(%rdi)\n\t"
//     "movq %r13, 0x18(%rdi)\n\t"
//     "movq %r14, 0x20(%rdi)\n\t"
//     "movq %r15, 0x28(%rdi)\n\t"
//     "movq %rsp, %rax\n\t"
//     "addq $16, %rax\n\t"
//     "movq %rax, 0x30(%rdi)\n\t"
//     "movq 0x8(%rsp), %rax\n\t"
//     "movq %rax, 0x38(%rdi)\n\t"
//     "pop %rbp\n\t"
//   );
//   return 0;
// }

// void asm_longjmp(asm_jmp_buf env, int val) {
//   // longjmp(env, val);
//   asm(
//     "movq (%%rdi), %%rbx\n\t"
//     "movq 0x8(%%rdi), %%rbp\n\t"
//     "movq 0x10(%%rdi), %%r12\n\t"
//     "movq 0x18(%%rdi), %%r13\n\t"
//     "movq 0x20(%%rdi), %%r14\n\t"
//     "movq 0x28(%%rdi), %%r15\n\t"
//     "movq 0x30(%%rdi), %%rsp\n\t"
//     "mov %0, %%eax\n\t"
//     "jmp *0x38(%%rdi)\n\t"
//     :
//     : "r"(val)
//     : "%rax", "%rbx", "%r12", "%r13", "%r14", "%r15"
//   );
// }

int asm_setjmp(asm_jmp_buf env) {
  asm volatile(
      "push %%rbp;"
      "mov %%rsp,%%rbp;"
      "movq (%%rbp),%%rax;"
      "movq %%rax,(%%rdi);"//true rbp
      "leaq 0x10(%%rsp),%%rax;"
      "movq %%rax,0x8(%%rdi);"//true rsp
      "movq %%rbx,0x10(%%rdi);"//rbx
      "movq 0x8(%%rbp),%%rax;" //pc
      "movq %%rax,0x18(%%rdi);"
      "movq %%r12,0x20(%%rdi);"
      "movq %%r13,0x28(%%rdi);"
      "movq %%r14,0x30(%%rdi);"
      "movq %%r15,0x38(%%rdi);"
      "pop %%rbp;"
    :
    :"rdi"(env)
  );
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  asm volatile(
    "movq 0x10(%%rdi),%%rbx;"
    "movq 0x20(%%rdi),%%r12;"
    "movq 0x28(%%rdi),%%r13;"
    "movq 0x30(%%rdi),%%r14;"
    "movq 0x38(%%rdi),%%r15;"
    "movq (%%rdi),%%rbp;"
    "movq 0x8(%%rdi),%%rsp;"
    "movq %%rsi,%%rax;"
    "movq 0x18(%%rdi),%%rdi;"
    "jmp *%%rdi;" //jmp to pc
    :
    :"rdi"(env),"rsi"(val)
  );
  
}