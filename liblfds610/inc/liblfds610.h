#ifndef __LIBLFDS610_H

  /***** library header *****/
  #define LFDS610_RELEASE_NUMBER_STRING  "6.1.0"




  /***** lfds610_abstraction *****/

  /***** defines *****/
  #if (defined _WIN64 && defined _MSC_VER && !defined WIN_KERNEL_BUILD)
    // TRD : 64-bit Windows user-mode with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <windows.h>
    #include <intrin.h>
    typedef unsigned __int64                 lfds610_atom_t;
    #define LFDS610_INLINE                   __forceinline
    #define LFDS610_ALIGN(alignment)         __declspec( align(alignment) )
    #define LFDS610_ALIGN_SINGLE_POINTER     8
    #define LFDS610_ALIGN_DOUBLE_POINTER     16
    #define LFDS610_BARRIER_COMPILER_LOAD    _ReadBarrier()
    #define LFDS610_BARRIER_COMPILER_STORE   _WriteBarrier()
    #define LFDS610_BARRIER_COMPILER_FULL    _ReadWriteBarrier()
    #define LFDS610_BARRIER_PROCESSOR_LOAD   _mm_lfence()
    #define LFDS610_BARRIER_PROCESSOR_STORE  _mm_sfence()
    #define LFDS610_BARRIER_PROCESSOR_FULL   _mm_mfence()
  #endif

  #if (!defined _WIN64 && defined _WIN32 && defined _MSC_VER && !defined WIN_KERNEL_BUILD)
    // TRD : 32-bit Windows user-mode with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <windows.h>
    #include <intrin.h>
    typedef unsigned long int                lfds610_atom_t;
    #define LFDS610_INLINE                   __forceinline
    #define LFDS610_ALIGN(alignment)         __declspec( align(alignment) )
    #define LFDS610_ALIGN_SINGLE_POINTER     4
    #define LFDS610_ALIGN_DOUBLE_POINTER     8
    #define LFDS610_BARRIER_COMPILER_LOAD    _ReadBarrier()
    #define LFDS610_BARRIER_COMPILER_STORE   _WriteBarrier()
    #define LFDS610_BARRIER_COMPILER_FULL    _ReadWriteBarrier()
    #define LFDS610_BARRIER_PROCESSOR_LOAD   _mm_lfence()
    #define LFDS610_BARRIER_PROCESSOR_STORE  _mm_sfence()
    #define LFDS610_BARRIER_PROCESSOR_FULL   _mm_mfence()

    // TRD : this define is documented but missing in Microsoft Platform SDK v7.0
    #define _InterlockedCompareExchangePointer(destination, exchange, compare) _InterlockedCompareExchange((volatile long *) destination, (long) exchange, (long) compare)
  #endif

  #if (defined _WIN64 && defined _MSC_VER && defined WIN_KERNEL_BUILD)
    // TRD : 64-bit Windows kernel with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <wdm.h>
    typedef unsigned __int64                 lfds610_atom_t;
    #define LFDS610_INLINE                   __forceinline
    #define LFDS610_ALIGN(alignment)         __declspec( align(alignment) )
    #define LFDS610_ALIGN_SINGLE_POINTER     8
    #define LFDS610_ALIGN_DOUBLE_POINTER     16
    #define LFDS610_BARRIER_COMPILER_LOAD    _ReadBarrier()
    #define LFDS610_BARRIER_COMPILER_STORE   _WriteBarrier()
    #define LFDS610_BARRIER_COMPILER_FULL    _ReadWriteBarrier()
    #define LFDS610_BARRIER_PROCESSOR_LOAD   _mm_lfence()
    #define LFDS610_BARRIER_PROCESSOR_STORE  _mm_sfence()
    #define LFDS610_BARRIER_PROCESSOR_FULL   _mm_mfence()
  #endif

  #if (!defined _WIN64 && defined _WIN32 && defined _MSC_VER && defined WIN_KERNEL_BUILD)
    // TRD : 32-bit Windows kernel with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <wdm.h>
    typedef unsigned long int                lfds610_atom_t;
    #define LFDS610_INLINE                   __forceinline
    #define LFDS610_ALIGN(alignment)         __declspec( align(alignment) )
    #define LFDS610_ALIGN_SINGLE_POINTER     4
    #define LFDS610_ALIGN_DOUBLE_POINTER     8
    #define LFDS610_BARRIER_COMPILER_LOAD    _ReadBarrier()
    #define LFDS610_BARRIER_COMPILER_STORE   _WriteBarrier()
    #define LFDS610_BARRIER_COMPILER_FULL    _ReadWriteBarrier()
    #define LFDS610_BARRIER_PROCESSOR_LOAD   _mm_lfence()
    #define LFDS610_BARRIER_PROCESSOR_STORE  _mm_sfence()
    #define LFDS610_BARRIER_PROCESSOR_FULL   _mm_mfence()

    // TRD : this define is documented but missing in Microsoft Platform SDK v7.0
    #define _InterlockedCompareExchangePointer(destination, exchange, compare) _InterlockedCompareExchange((volatile long *) destination, (long) exchange, (long) compare)
  #endif

  #if (defined __unix__ && defined __x86_64__ && __GNUC__)
    // TRD : any UNIX with GCC on x64
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    typedef unsigned long long int           lfds610_atom_t;
    #define LFDS610_INLINE                   inline
    #define LFDS610_ALIGN(alignment)         __attribute__( (aligned(alignment)) )
    #define LFDS610_ALIGN_SINGLE_POINTER     8
    #define LFDS610_ALIGN_DOUBLE_POINTER     16
    #define LFDS610_BARRIER_COMPILER_LOAD    __asm__ __volatile__ ( "" : : : "memory" )
    #define LFDS610_BARRIER_COMPILER_STORE   __asm__ __volatile__ ( "" : : : "memory" )
    #define LFDS610_BARRIER_COMPILER_FULL    __asm__ __volatile__ ( "" : : : "memory" )
    #define LFDS610_BARRIER_PROCESSOR_LOAD   __sync_synchronize()
    #define LFDS610_BARRIER_PROCESSOR_STORE  __sync_synchronize()
    #define LFDS610_BARRIER_PROCESSOR_FULL   __sync_synchronize()
  #endif

  #if (defined __unix__ && defined __i686__ && __GNUC__)
    // TRD : any UNIX with GCC on x86
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    typedef unsigned long int                lfds610_atom_t;
    #define LFDS610_INLINE                   inline
    #define LFDS610_ALIGN(alignment)         __attribute__( (aligned(alignment)) )
    #define LFDS610_ALIGN_SINGLE_POINTER     4
    #define LFDS610_ALIGN_DOUBLE_POINTER     8
    #define LFDS610_BARRIER_COMPILER_LOAD    __asm__ __volatile__ ( "" : : : "memory" )
    #define LFDS610_BARRIER_COMPILER_STORE   __asm__ __volatile__ ( "" : : : "memory" )
    #define LFDS610_BARRIER_COMPILER_FULL    __asm__ __volatile__ ( "" : : : "memory" )
    #define LFDS610_BARRIER_PROCESSOR_LOAD   __sync_synchronize()
    #define LFDS610_BARRIER_PROCESSOR_STORE  __sync_synchronize()
    #define LFDS610_BARRIER_PROCESSOR_FULL   __sync_synchronize()
  #endif

  #if (defined __unix__ && defined __arm__ && __GNUC__)
    // TRD : any UNIX with GCC on ARM
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    typedef unsigned long int                lfds610_atom_t;
    #define LFDS610_INLINE                   inline
    #define LFDS610_ALIGN(alignment)         __attribute__( (aligned(alignment)) )
    #define LFDS610_ALIGN_SINGLE_POINTER     4
    #define LFDS610_ALIGN_DOUBLE_POINTER     8
    #define LFDS610_BARRIER_COMPILER_LOAD    __asm__ __volatile__ ( "" : : : "memory" )
    #define LFDS610_BARRIER_COMPILER_STORE   __asm__ __volatile__ ( "" : : : "memory" )
    #define LFDS610_BARRIER_COMPILER_FULL    __asm__ __volatile__ ( "" : : : "memory" )
    #define LFDS610_BARRIER_PROCESSOR_LOAD   __sync_synchronize()
    #define LFDS610_BARRIER_PROCESSOR_STORE  __sync_synchronize()
    #define LFDS610_BARRIER_PROCESSOR_FULL   __sync_synchronize()
  #endif

  #define LFDS610_BARRIER_LOAD   LFDS610_BARRIER_COMPILER_LOAD; LFDS610_BARRIER_PROCESSOR_LOAD; LFDS610_BARRIER_COMPILER_LOAD
  #define LFDS610_BARRIER_STORE  LFDS610_BARRIER_COMPILER_STORE; LFDS610_BARRIER_PROCESSOR_STORE; LFDS610_BARRIER_COMPILER_STORE
  #define LFDS610_BARRIER_FULL   LFDS610_BARRIER_COMPILER_FULL; LFDS610_BARRIER_PROCESSOR_FULL; LFDS610_BARRIER_COMPILER_FULL

  /***** enums *****/
  enum lfds610_data_structure_validity
  {
    LFDS610_VALIDITY_VALID,
    LFDS610_VALIDITY_INVALID_LOOP,
    LFDS610_VALIDITY_INVALID_MISSING_ELEMENTS,
    LFDS610_VALIDITY_INVALID_ADDITIONAL_ELEMENTS,
    LFDS610_VALIDITY_INVALID_TEST_DATA
  };

  /***** structs *****/
  struct lfds610_validation_info
  {
    lfds610_atom_t
      min_elements,
      max_elements;
  };

  /***** public prototypes *****/
  void *lfds610_abstraction_malloc( size_t size );
  void lfds610_abstraction_free( void *memory );





  /***** lfds610_freelist *****/

  /***** enums *****/
  enum lfds610_freelist_query_type
  {
    LFDS610_FREELIST_QUERY_ELEMENT_COUNT,
    LFDS610_FREELIST_QUERY_VALIDATE
  };

  /***** incomplete types *****/
  struct lfds610_freelist_state;
  struct lfds610_freelist_element;

  /***** public prototypes *****/
  int lfds610_freelist_new( struct lfds610_freelist_state **fs, lfds610_atom_t number_elements, int (*user_data_init_function)(void **user_data, void *user_state), void *user_state );
  void lfds610_freelist_use( struct lfds610_freelist_state *fs );
  void lfds610_freelist_delete( struct lfds610_freelist_state *fs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  lfds610_atom_t lfds610_freelist_new_elements( struct lfds610_freelist_state *fs, lfds610_atom_t number_elements );

  struct lfds610_freelist_element *lfds610_freelist_pop( struct lfds610_freelist_state *fs, struct lfds610_freelist_element **fe );
  struct lfds610_freelist_element *lfds610_freelist_guaranteed_pop( struct lfds610_freelist_state *fs, struct lfds610_freelist_element **fe );
  void lfds610_freelist_push( struct lfds610_freelist_state *fs, struct lfds610_freelist_element *fe );

  void *lfds610_freelist_get_user_data_from_element( struct lfds610_freelist_element *fe, void **user_data );
  void lfds610_freelist_set_user_data_in_element( struct lfds610_freelist_element *fe, void *user_data );

  void lfds610_freelist_query( struct lfds610_freelist_state *fs, enum lfds610_freelist_query_type query_type, void *query_input, void *query_output );





  /***** lfds610_liblfds *****/

  /***** public prototypes *****/
  void lfds610_liblfds_abstraction_test_helper_increment_non_atomic( lfds610_atom_t *shared_counter );
  void lfds610_liblfds_abstraction_test_helper_increment_atomic( volatile lfds610_atom_t *shared_counter );
  void lfds610_liblfds_abstraction_test_helper_cas( volatile lfds610_atom_t *shared_counter, lfds610_atom_t *local_counter );
  void lfds610_liblfds_abstraction_test_helper_dcas( volatile lfds610_atom_t *shared_counter, lfds610_atom_t *local_counter );





  /***** lfds610_queue *****/

  /***** enums *****/
  enum lfds610_queue_query_type
  {
    LFDS610_QUEUE_QUERY_ELEMENT_COUNT,
    LFDS610_QUEUE_QUERY_VALIDATE
  };

  /***** incomplete types *****/
  struct lfds610_queue_state;

  /***** public prototypes *****/
  int lfds610_queue_new( struct lfds610_queue_state **sq, lfds610_atom_t number_elements );
  void lfds610_queue_use( struct lfds610_queue_state *qs );
  void lfds610_queue_delete( struct lfds610_queue_state *qs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  int lfds610_queue_enqueue( struct lfds610_queue_state *qs, void *user_data );
  int lfds610_queue_guaranteed_enqueue( struct lfds610_queue_state *qs, void *user_data );
  int lfds610_queue_dequeue( struct lfds610_queue_state *qs, void **user_data );

  void lfds610_queue_query( struct lfds610_queue_state *qs, enum lfds610_queue_query_type query_type, void *query_input, void *query_output );





  /***** lfds610_ringbuffer *****/

  /***** enums *****/
  enum lfds610_ringbuffer_query_type
  {
    LFDS610_RINGBUFFER_QUERY_VALIDATE
  };

  /***** incomplete types *****/
  struct lfds610_ringbuffer_state;

  /***** public prototypes *****/
  int lfds610_ringbuffer_new( struct lfds610_ringbuffer_state **rs, lfds610_atom_t number_elements, int (*user_data_init_function)(void **user_data, void *user_state), void *user_state );
  void lfds610_ringbuffer_use( struct lfds610_ringbuffer_state *rs );
  void lfds610_ringbuffer_delete( struct lfds610_ringbuffer_state *rs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  struct lfds610_freelist_element *lfds610_ringbuffer_get_read_element( struct lfds610_ringbuffer_state *rs, struct lfds610_freelist_element **fe );
  struct lfds610_freelist_element *lfds610_ringbuffer_get_write_element( struct lfds610_ringbuffer_state *rs, struct lfds610_freelist_element **fe, int *overwrite_flag );

  void lfds610_ringbuffer_put_read_element( struct lfds610_ringbuffer_state *rs, struct lfds610_freelist_element *fe );
  void lfds610_ringbuffer_put_write_element( struct lfds610_ringbuffer_state *rs, struct lfds610_freelist_element *fe );

  void lfds610_ringbuffer_query( struct lfds610_ringbuffer_state *rs, enum lfds610_ringbuffer_query_type query_type, void *query_input, void *query_output );





  /***** lfds610_slist *****/

  /***** incomplete types *****/
  struct lfds610_slist_state;
  struct lfds610_slist_element;

  /***** public prototypes *****/
  int lfds610_slist_new( struct lfds610_slist_state **ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );
  void lfds610_slist_use( struct lfds610_slist_state *ss );
  void lfds610_slist_delete( struct lfds610_slist_state *ss );

  struct lfds610_slist_element *lfds610_slist_new_head( struct lfds610_slist_state *ss, void *user_data );
  struct lfds610_slist_element *lfds610_slist_new_next( struct lfds610_slist_element *se, void *user_data );

  int lfds610_slist_logically_delete_element( struct lfds610_slist_state *ss, struct lfds610_slist_element *se );
  void lfds610_slist_single_threaded_physically_delete_all_elements( struct lfds610_slist_state *ss );

  int lfds610_slist_get_user_data_from_element( struct lfds610_slist_element *se, void **user_data );
  int lfds610_slist_set_user_data_in_element( struct lfds610_slist_element *se, void *user_data );

  struct lfds610_slist_element *lfds610_slist_get_head( struct lfds610_slist_state *ss, struct lfds610_slist_element **se );
  struct lfds610_slist_element *lfds610_slist_get_next( struct lfds610_slist_element *se, struct lfds610_slist_element **next_se );
  struct lfds610_slist_element *lfds610_slist_get_head_and_then_next( struct lfds610_slist_state *ss, struct lfds610_slist_element **se );





  /***** lfds610_stack *****/

  /***** enums *****/
  enum lfds610_stack_query_type
  {
    LFDS610_STACK_QUERY_ELEMENT_COUNT,
    LFDS610_STACK_QUERY_VALIDATE
  };

  /***** incomplete types *****/
  struct lfds610_stack_state;

  /***** public prototypes *****/
  int lfds610_stack_new( struct lfds610_stack_state **ss, lfds610_atom_t number_elements );
  void lfds610_stack_use( struct lfds610_stack_state *ss );
  void lfds610_stack_delete( struct lfds610_stack_state *ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  void lfds610_stack_clear( struct lfds610_stack_state *ss, void (*user_data_clear_function)(void *user_data, void *user_state), void *user_state );

  int lfds610_stack_push( struct lfds610_stack_state *ss, void *user_data );
  int lfds610_stack_guaranteed_push( struct lfds610_stack_state *ss, void *user_data );
  int lfds610_stack_pop( struct lfds610_stack_state *ss, void **user_data );

  void lfds610_stack_query( struct lfds610_stack_state *ss, enum lfds610_stack_query_type query_type, void *query_input, void *query_output );





  #define __LIBLFDS610_H

#endif

