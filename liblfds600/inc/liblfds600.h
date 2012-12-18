#ifndef __LIBLFDS600_H

  /***** library header *****/
  #define LFDS600_RELEASE_NUMBER 1




  /***** lfds600_abstraction *****/

  /***** defines *****/
  #if (defined _WIN64 && defined _MSC_VER && !defined WIN_KERNEL_BUILD)
    // TRD : 64-bit Windows user-mode with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <windows.h>
    #include <intrin.h>
    typedef unsigned __int64             lfds600_atom_t;
    #define LFDS600_INLINE                extern __forceinline
    #define LFDS600_ALIGN(alignment)      __declspec( align(alignment) )
    #define LFDS600_ALIGN_SINGLE_POINTER  8
    #define LFDS600_ALIGN_DOUBLE_POINTER  16
  #endif

  #if (!defined _WIN64 && defined _WIN32 && defined _MSC_VER && !defined WIN_KERNEL_BUILD)
    // TRD : 32-bit Windows user-mode with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <windows.h>
    #include <intrin.h>
    typedef unsigned long int            lfds600_atom_t;
    #define LFDS600_INLINE                extern __forceinline
    #define LFDS600_ALIGN(alignment)      __declspec( align(alignment) )
    #define LFDS600_ALIGN_SINGLE_POINTER  4
    #define LFDS600_ALIGN_DOUBLE_POINTER  8

    // TRD : this define is documented but missing in Microsoft Platform SDK v7.0
    #define _InterlockedCompareExchangePointer(destination, exchange, compare) _InterlockedCompareExchange((volatile long *) destination, (long) exchange, (long) compare)
  #endif

  #if (defined _WIN64 && defined _MSC_VER && defined WIN_KERNEL_BUILD)
    // TRD : 64-bit Windows kernel with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <wdm.h>
    typedef unsigned __int64             lfds600_atom_t;
    #define LFDS600_INLINE                extern __forceinline
    #define LFDS600_ALIGN(alignment)      __declspec( align(alignment) )
    #define LFDS600_ALIGN_SINGLE_POINTER  8
    #define LFDS600_ALIGN_DOUBLE_POINTER  16
  #endif

  #if (!defined _WIN64 && defined _WIN32 && defined _MSC_VER && defined WIN_KERNEL_BUILD)
    // TRD : 32-bit Windows kernel with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <wdm.h>
    typedef unsigned long int            lfds600_atom_t;
    #define LFDS600_INLINE                extern __forceinline
    #define LFDS600_ALIGN(alignment)      __declspec( align(alignment) )
    #define LFDS600_ALIGN_SINGLE_POINTER  4
    #define LFDS600_ALIGN_DOUBLE_POINTER  8

    // TRD : this define is documented but missing in Microsoft Platform SDK v7.0
    #define _InterlockedCompareExchangePointer(destination, exchange, compare) _InterlockedCompareExchange((volatile long *) destination, (long) exchange, (long) compare)
  #endif

  #if (defined __unix__ && defined __x86_64__ && __GNUC__)
    // TRD : any UNIX with GCC on x64
    #define _XOPEN_SOURCE 600
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    typedef unsigned long long int       lfds600_atom_t;
    #define LFDS600_INLINE                inline
    #define LFDS600_ALIGN(alignment)      __attribute__( (aligned(alignment)) )
    #define LFDS600_ALIGN_SINGLE_POINTER  8
    #define LFDS600_ALIGN_DOUBLE_POINTER  16
  #endif

  #if (defined __unix__ && defined __i686__ && __GNUC__)
    // TRD : any UNIX with GCC on x86
    #define _XOPEN_SOURCE 600
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    typedef unsigned long int            lfds600_atom_t;
    #define LFDS600_INLINE                inline
    #define LFDS600_ALIGN(alignment)      __attribute__( (aligned(alignment)) )
    #define LFDS600_ALIGN_SINGLE_POINTER  4
    #define LFDS600_ALIGN_DOUBLE_POINTER  8
  #endif

  #if (defined __unix__ && defined __arm__ && __GNUC__)
    // TRD : any UNIX with GCC on ARM
    #define _XOPEN_SOURCE 600
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    typedef unsigned long int            lfds600_atom_t;
    #define LFDS600_INLINE                inline
    #define LFDS600_ALIGN(alignment)      __attribute__( (aligned(alignment)) )
    #define LFDS600_ALIGN_SINGLE_POINTER  4
    #define LFDS600_ALIGN_DOUBLE_POINTER  8
  #endif

  /***** enums *****/
  enum data_structure_validity
  {
    VALIDITY_VALID,
    VALIDITY_INVALID_LOOP,
    VALIDITY_INVALID_MISSING_ELEMENTS,
    VALIDITY_INVALID_ADDITIONAL_ELEMENTS,
    VALIDITY_INVALID_TEST_DATA
  };

  /***** structs *****/
  struct lfds600_validation_info
  {
    lfds600_atom_t
      min_elements,
      max_elements;
  };

  /***** public prototypes *****/
  void lfds600_abstraction_aligned_free( void *memory );
  void *lfds600_abstraction_aligned_malloc( size_t size, size_t align_in_bytes );
  lfds600_atom_t lfds600_abstraction_cas( volatile lfds600_atom_t *destination, lfds600_atom_t exchange, lfds600_atom_t compare );
  unsigned char lfds600_abstraction_dcas( volatile lfds600_atom_t *destination, lfds600_atom_t *exchange, lfds600_atom_t *compare );
  lfds600_atom_t lfds600_abstraction_increment( lfds600_atom_t *value );





  /***** lfds600_freelist *****/

  /***** enums *****/
  enum lfds600_freelist_query_type
  {
    LFDS600_FREELIST_QUERY_ELEMENT_COUNT,
    LFDS600_FREELIST_QUERY_VALIDATE
  };

  /***** incomplete types *****/
  struct lfds600_freelist_state;
  struct lfds600_freelist_element;

  /***** public prototypes *****/
  int lfds600_freelist_new( struct lfds600_freelist_state **fs, lfds600_atom_t number_elements, int (*user_data_init_function)(void **user_data, void *user_state), void *user_state );
  void lfds600_freelist_delete( struct lfds600_freelist_state *fs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  lfds600_atom_t lfds600_freelist_new_elements( struct lfds600_freelist_state *fs, lfds600_atom_t number_elements );

  struct lfds600_freelist_element *lfds600_freelist_pop( struct lfds600_freelist_state *fs, struct lfds600_freelist_element **fe );
  struct lfds600_freelist_element *lfds600_freelist_guaranteed_pop( struct lfds600_freelist_state *fs, struct lfds600_freelist_element **fe );
  void lfds600_freelist_push( struct lfds600_freelist_state *fs, struct lfds600_freelist_element *fe );

  void *lfds600_freelist_get_user_data_from_element( struct lfds600_freelist_element *fe, void **user_data );
  void lfds600_freelist_set_user_data_in_element( struct lfds600_freelist_element *fe, void *user_data );

  void lfds600_freelist_query( struct lfds600_freelist_state *fs, enum lfds600_freelist_query_type query_type, void *query_input, void *query_output );





  /***** lfds600_queue *****/

  /***** enums *****/
  enum lfds600_queue_query_type
  {
    LFDS600_QUEUE_QUERY_ELEMENT_COUNT,
    LFDS600_QUEUE_QUERY_VALIDATE
  };

  /***** incomplete types *****/
  struct lfds600_queue_state;

  /***** public prototypes *****/
  int lfds600_queue_new( struct lfds600_queue_state **sq, lfds600_atom_t number_elements );
  void lfds600_queue_delete( struct lfds600_queue_state *qs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  int lfds600_queue_enqueue( struct lfds600_queue_state *qs, void *user_data );
  int lfds600_queue_guaranteed_enqueue( struct lfds600_queue_state *qs, void *user_data );
  int lfds600_queue_dequeue( struct lfds600_queue_state *qs, void **user_data );

  void lfds600_queue_query( struct lfds600_queue_state *qs, enum lfds600_queue_query_type query_type, void *query_input, void *query_output );





  /***** lfds600_ringbuffer *****/

  /***** enums *****/
  enum lfds600_ringbuffer_query_type
  {
    LFDS600_RINGBUFFER_QUERY_VALIDATE
  };

  /***** incomplete types *****/
  struct lfds600_ringbuffer_state;

  /***** public prototypes *****/
  int lfds600_ringbuffer_new( struct lfds600_ringbuffer_state **rs, lfds600_atom_t number_elements, int (*user_data_init_function)(void **user_data, void *user_state), void *user_state );
  void lfds600_ringbuffer_delete( struct lfds600_ringbuffer_state *rs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  struct lfds600_freelist_element *lfds600_ringbuffer_get_read_element( struct lfds600_ringbuffer_state *rs, struct lfds600_freelist_element **fe );
  struct lfds600_freelist_element *lfds600_ringbuffer_get_write_element( struct lfds600_ringbuffer_state *rs, struct lfds600_freelist_element **fe, int *overwrite_flag );

  void lfds600_ringbuffer_put_read_element( struct lfds600_ringbuffer_state *rs, struct lfds600_freelist_element *fe );
  void lfds600_ringbuffer_put_write_element( struct lfds600_ringbuffer_state *rs, struct lfds600_freelist_element *fe );

  void lfds600_ringbuffer_query( struct lfds600_ringbuffer_state *rs, enum lfds600_ringbuffer_query_type query_type, void *query_input, void *query_output );





  /***** lfds600_slist *****/

  /***** incomplete types *****/
  struct lfds600_slist_state;
  struct lfds600_slist_element;

  /***** public prototypes *****/
  int lfds600_slist_new( struct lfds600_slist_state **ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );
  void lfds600_slist_delete( struct lfds600_slist_state *ss );

  struct lfds600_slist_element *lfds600_slist_new_head( struct lfds600_slist_state *ss, void *user_data );
  struct lfds600_slist_element *lfds600_slist_new_next( struct lfds600_slist_element *se, void *user_data );

  void lfds600_slist_delete_element( struct lfds600_slist_state *ss, struct lfds600_slist_element *se );
  void lfds600_slist_delete_all_elements( struct lfds600_slist_state *ss );

  int lfds600_slist_get_user_data_from_element( struct lfds600_slist_element *se, void **user_data );
  int lfds600_slist_set_user_data_in_element( struct lfds600_slist_element *se, void *user_data );

  struct lfds600_slist_element *lfds600_slist_get_head( struct lfds600_slist_state *ss, struct lfds600_slist_element **se );
  struct lfds600_slist_element *lfds600_slist_get_next( struct lfds600_slist_element *se, struct lfds600_slist_element **next_se );
  struct lfds600_slist_element *lfds600_slist_get_head_and_then_next( struct lfds600_slist_state *ss, struct lfds600_slist_element **se );





  /***** lfds600_stack *****/

  /***** enums *****/
  enum lfds600_stack_query_type
  {
    LFDS600_STACK_QUERY_ELEMENT_COUNT
  };

  /***** incomplete types *****/
  struct lfds600_stack_state;

  /***** public prototypes *****/
  int lfds600_stack_new( struct lfds600_stack_state **ss, lfds600_atom_t number_elements );
  void lfds600_stack_delete( struct lfds600_stack_state *ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  void lfds600_stack_clear( struct lfds600_stack_state *ss, void (*user_data_clear_function)(void *user_data, void *user_state), void *user_state );

  int lfds600_stack_push( struct lfds600_stack_state *ss, void *user_data );
  int lfds600_stack_guaranteed_push( struct lfds600_stack_state *ss, void *user_data );
  int lfds600_stack_pop( struct lfds600_stack_state *ss, void **user_data );

  void lfds600_stack_query( struct lfds600_stack_state *ss, enum lfds600_stack_query_type query_type, void *query_input, void *query_output );





  #define __LIBLFDS600_H

#endif

