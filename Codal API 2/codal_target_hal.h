/*
The MIT License (MIT)

Copyright (c) 2017 Lancaster University.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef CODAL_TARGET_HAL_H
#define CODAL_TARGET_HAL_H

#include "platform_includes.h"

extern "C"
{
  void target_enable_irq();

  void target_disable_irq();

  void target_reset();

  void target_wait(uint32_t milliseconds);

  void target_wait_us(uint32_t us);

  int target_seed_random(uint32_t rand);

  int target_random(int max);

  uint64_t target_get_serial();

  void target_scheduler_idle();

  void target_wait_for_event();

  void target_deepsleep();

  void target_panic(int statusCode);

  PROCESSOR_WORD_TYPE fiber_initial_stack_base();
  /**
   * Configures the link register of the given tcb to have the value function.
   *
   * @param tcb The tcb to modify
   * @param function the function the link register should point to.
   */
  void tcb_configure_lr(void *tcb, PROCESSOR_WORD_TYPE function);

  void *tcb_allocate();

  /**
   * Configures the link register of the given tcb to have the value function.
   *
   * @param tcb The tcb to modify
   * @param function the function the link register should point to.
   */
  void tcb_configure_sp(void *tcb, PROCESSOR_WORD_TYPE sp);

  void tcb_configure_stack_base(void *tcb, PROCESSOR_WORD_TYPE stack_base);

  PROCESSOR_WORD_TYPE tcb_get_stack_base(void *tcb);

  PROCESSOR_WORD_TYPE get_current_sp();

  PROCESSOR_WORD_TYPE tcb_get_sp(void *tcb);

  void tcb_configure_args(void *tcb, PROCESSOR_WORD_TYPE ep, PROCESSOR_WORD_TYPE cp, PROCESSOR_WORD_TYPE pm);

  // Preprocessor Directive to ignore redecleration when using clang
#ifndef __clang__
  /**
   * Default implementation of atomic fetch and add opertaion.
   * GCC provides this where possible, but this is not supported on some CPU architectures...
   *
   * @param ptr pointer to the memory to access.
   * @param value the value to add to the memory location.
   * @return the value of th ememory location BEFORE the add operation took place.
   */
  short unsigned int __sync_fetch_and_add_2(volatile void *ptr, short unsigned int value);

#endif
}

// This is re-defined in targets with external flash, that require certain functions to be placed in RAM
#ifndef REAL_TIME_FUNC
#define REAL_TIME_FUNC /* */
#endif

// This is for cycle-precise wait even in presence of flash caches (forces function to sit in RAM)
#ifndef FORCE_RAM_FUNC
#if defined(__clang__)
// LLVM doesn't support long_call attribute for ARM: https://github.com/llvm/llvm-project/issues/39969
#define FORCE_RAM_FUNC __attribute__((noinline, section(".data.ramfuncs")))
#else
#define FORCE_RAM_FUNC __attribute__((noinline, long_call, section(".data.ramfuncs")))
#endif
#endif

#endif
