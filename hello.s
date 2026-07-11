  .text
  .globl main
main:
  li t0, 2
  sub t0, x0, t0
  li t1, 3
  sub t1, x0, t1
  mul t1, t1, t1
  mv a0, t1
  ret
