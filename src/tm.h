/* ============================================================================
 * 
 * tm.h
 *
 * Utility defines for transactional memory
 *
 * ============================================================================
 *
 * Author: Lorenzo Rutigliano
 * Date:   August 2017
 *
 * ============================================================================
 *
 * Copyright (C) Lorenzo Rutigliano, 2017. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ============================================================================
 */


#ifndef __TM_H
#define __TM_H

/* ============================================================================
 * STM - Software Transactional Memory
 *
 * Based on 'Transactional Memory in GCC', https://gcc.gnu.org/wiki/TransactionalMemory
 *
 *
 * TM_SAFE
 *     Safe transaction attribute
 *
 * TM_BEGIN()
 *     Begin atomic transaction
 *
 * TM_END()
 *     End atomic transaction
 *     
 * ============================================================================
 */

#if defined(TM)  

  #define TM_SAFE __attribute__((transaction_safe))
  #define TM_BEGIN() __transaction_atomic {
  #define TM_END() }
  
#else

  #define TM_SAFE
  #define TM_BEGIN() printf("%s\n", "begin transaction");
  #define TM_END() printf("%s\n", "end transaction");

#endif

/* ========================================================================= */
#endif // __TM_H