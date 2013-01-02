#ifndef __LIBLFDS601_H

  /***** library header *****/
  #define LFDS601_RELEASE_NUMBER_STRING  "6.0.1"




  /***** lfds601_abstraction *****/

  /***** defines *****/
  #if (defined _WIN64 && defined _MSC_VER && !defined WIN_KERNEL_BUILD)
    // TRD : 64-bit Windows user-mode with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <windows.h>
    #include <intrin.h>
    typedef unsigned __int64              lfds601_atom_t;
    #define LFDS601_INLINE                extern __forceinline
    #define LFDS601_ALIGN(alignment)      __declspec( align(alignment) )
    #define LFDS601_ALIGN_SINGLE_POINTER  8
    #define LFDS601_ALIGN_DOUBLE_POINTER  16
  #endif

  #if (!defined _WIN64 && defined _WIN32 && defined _MSC_VER && !defined WIN_KERNEL_BUILD)
    // TRD : 32-bit Windows user-mode with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <windows.h>
    #include <intrin.h>
    typedef unsigned long int             lfds601_atom_t;
    #define LFDS601_INLINE                extern __forceinline
    #define LFDS601_ALIGN(alignment)      __declspec( align(alignment) )
    #define LFDS601_ALIGN_SINGLE_POINTER  4
    #define LFDS601_ALIGN_DOUBLE_POINTER  8

    // TRD : this define is documented but missing in Microsoft Platform SDK v7.0
    #define _InterlockedCompareExchangePointer(destination, exchange, compare) _InterlockedCompareExchange((volatile long *) destination, (long) exchange, (long) compare)
  #endif

  #if (defined _WIN64 && defined _MSC_VER && defined WIN_KERNEL_BUILD)
    // TRD : 64-bit Windows kernel with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <wdm.h>
    typedef unsigned __int64              lfds601_atom_t;
    #define LFDS601_INLINE                extern __forceinline
    #define LFDS601_ALIGN(alignment)      __declspec( align(alignment) )
    #define LFDS601_ALIGN_SINGLE_POINTER  8
    #define LFDS601_ALIGN_DOUBLE_POINTER  16
  #endif

  #if (!defined _WIN64 && defined _WIN32 && defined _MSC_VER && defined WIN_KERNEL_BUILD)
    // TRD : 32-bit Windows kernel with the Microsoft C compiler, any CPU
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <wdm.h>
    typedef unsigned long int             lfds601_atom_t;
    #define LFDS601_INLINE                extern __forceinline
    #define LFDS601_ALIGN(alignment)      __declspec( align(alignment) )
    #define LFDS601_ALIGN_SINGLE_POINTER  4
    #define LFDS601_ALIGN_DOUBLE_POINTER  8

    // TRD : this define is documented but missing in Microsoft Platform SDK v7.0
    #define _InterlockedCompareExchangePointer(destination, exchange, compare) _InterlockedCompareExchange((volatile long *) destination, (long) exchange, (long) compare)
  #endif

  #if (defined __unix__ && defined __x86_64__ && __GNUC__)
    // TRD : any UNIX with GCC on x64
    #define _XOPEN_SOURCE 600
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    typedef unsigned long long int        lfds601_atom_t;
    #define LFDS601_INLINE                inline
    #define LFDS601_ALIGN(alignment)      __attribute__( (aligned(alignment)) )
    #define LFDS601_ALIGN_SINGLE_POINTER  8
    #define LFDS601_ALIGN_DOUBLE_POINTER  16
  #endif

  #if (defined __unix__ && defined __i686__ && __GNUC__)
    // TRD : any UNIX with GCC on x86
    #define _XOPEN_SOURCE 600
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    typedef unsigned long int             lfds601_atom_t;
    #define LFDS601_INLINE                inline
    #define LFDS601_ALIGN(alignment)      __attribute__( (aligned(alignment)) )
    #define LFDS601_ALIGN_SINGLE_POINTER  4
    #define LFDS601_ALIGN_DOUBLE_POINTER  8
  #endif

  #if (defined __unix__ && defined __arm__ && __GNUC__)
    // TRD : any UNIX with GCC on ARM
    #define _XOPEN_SOURCE 600
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    typedef unsigned long int             lfds601_atom_t;
    #define LFDS601_INLINE                inline
    #define LFDS601_ALIGN(alignment)      __attribute__( (aligned(alignment)) )
    #define LFDS601_ALIGN_SINGLE_POINTER  4
    #define LFDS601_ALIGN_DOUBLE_POINTER  8
  #endif

  /***** enums *****/
  enum lfds601_data_structure_validity
  {
    LFDS601_VALIDITY_VALID,
    LFDS601_VALIDITY_INVALID_LOOP,
    LFDS601_VALIDITY_INVALID_MISSING_ELEMENTS,
    LFDS601_VALIDITY_INVALID_ADDITIONAL_ELEMENTS,
    LFDS601_VALIDITY_INVALID_TEST_DATA
  };

  /***** structs *****/
  struct lfds601_validation_info
  {
    lfds601_atom_t
      min_elements,
      max_elements;
  };

  /***** public prototypes *****/
  void lfds601_abstraction_aligned_free( void *memory );
  void *lfds601_abstraction_aligned_malloc( size_t size, size_t align_in_bytes );
  lfds601_atom_t lfds601_abstraction_cas( volatile lfds601_atom_t *destination, lfds601_atom_t exchange, lfds601_atom_t compare );
  unsigned char lfds601_abstraction_dcas( volatile lfds601_atom_t *destination, lfds601_atom_t *exchange, lfds601_atom_t *compare );
  lfds601_atom_t lfds601_abstraction_increment( lfds601_atom_t *value );





  /***** lfds601_freelist *****/

  /***** enums *****/
  enum lfds601_freelist_query_type
  {
    LFDS601_FREELIST_QUERY_ELEMENT_COUNT,
    LFDS601_FREELIST_QUERY_VALIDATE
  };

  /***** incomplete types *****/
  struct lfds601_freelist_state;
  struct lfds601_freelist_element;

  /***** public prototypes *****/
  int lfds601_freelist_new( struct lfds601_freelist_state **fs, lfds601_atom_t number_elements, int (*user_data_init_function)(void **user_data, void *user_state), void *user_state );
  void lfds601_freelist_delete( struct lfds601_freelist_state *fs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  lfds601_atom_t lfds601_freelist_new_elements( struct lfds601_freelist_state *fs, lfds601_atom_t number_elements );

  struct lfds601_freelist_element *lfds601_freelist_pop( struct lfds601_freelist_state *fs, struct lfds601_freelist_element **fe );
  struct lfds601_freelist_element *lfds601_freelist_guaranteed_pop( struct lfds601_freelist_state *fs, struct lfds601_freelist_element **fe );
  void lfds601_freelist_push( struct lfds601_freelist_state *fs, struct lfds601_freelist_element *fe );

  void *lfds601_freelist_get_user_data_from_element( struct lfds601_freelist_element *fe, void **user_data );
  void lfds601_freelist_set_user_data_in_element( struct lfds601_freelist_element *fe, void *user_data );

  void lfds601_freelist_query( struct lfds601_freelist_state *fs, enum lfds601_freelist_query_type query_type, void *query_input, void *query_output );





  /***** lfds601_queue *****/

  /***** enums *****/
  enum lfds601_queue_query_type
  {
    LFDS601_QUEUE_QUERY_ELEMENT_COUNT,
    LFDS601_QUEUE_QUERY_VALIDATE
  };

  /***** incomplete types *****/
  struct lfds601_queue_state;

  /***** public prototypes *****/
  int lfds601_queue_new( struct lfds601_queue_state **sq, lfds601_atom_t number_elements );
  void lfds601_queue_delete( struct lfds601_queue_state *qs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  int lfds601_queue_enqueue( struct lfds601_queue_state *qs, void *user_data );
  int lfds601_queue_guaranteed_enqueue( struct lfds601_queue_state *qs, void *user_data );
  int lfds601_queue_dequeue( struct lfds601_queue_state *qs, void **user_data );

  void lfds601_queue_query( struct lfds601_queue_state *qs, enum lfds601_queue_query_type query_type, void *query_input, void *query_output );





  /***** lfds601_ringbuffer *****/

  /***** enums *****/
  enum lfds601_ringbuffer_query_type
  {
    LFDS601_RINGBUFFER_QUERY_VALIDATE
  };

  /***** incomplete types *****/
  struct lfds601_ringbuffer_state;

  /***** public prototypes *****/
  int lfds601_ringbuffer_new( struct lfds601_ringbuffer_state **rs, lfds601_atom_t number_elements, int (*user_data_init_function)(void **user_data, void *user_state), void *user_state );
  void lfds601_ringbuffer_delete( struct lfds601_ringbuffer_state *rs, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  struct lfds601_freelist_element *lfds601_ringbuffer_get_read_element( struct lfds601_ringbuffer_state *rs, struct lfds601_freelist_element **fe );
  struct lfds601_freelist_element *lfds601_ringbuffer_get_write_element( struct lfds601_ringbuffer_state *rs, struct lfds601_freelist_element **fe, int *overwrite_flag );

  void lfds601_ringbuffer_put_read_element( struct lfds601_ringbuffer_state *rs, struct lfds601_freelist_element *fe );
  void lfds601_ringbuffer_put_write_element( struct lfds601_ringbuffer_state *rs, struct lfds601_freelist_element *fe );

  void lfds601_ringbuffer_query( struct lfds601_ringbuffer_state *rs, enum lfds601_ringbuffer_query_type query_type, void *query_input, void *query_output );





  /***** lfds601_slist *****/

  /***** incomplete types *****/
  struct lfds601_slist_state;
  struct lfds601_slist_element;

  /***** public prototypes *****/
  int lfds601_slist_new( struct lfds601_slist_state **ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );
  void lfds601_slist_delete( struct lfds601_slist_state *ss );

  struct lfds601_slist_element *lfds601_slist_new_head( struct lfds601_slist_state *ss, void *user_data );
  struct lfds601_slist_element *lfds601_slist_new_next( struct lfds601_slist_element *se, void *user_data );

  void lfds601_slist_delete_element( struct lfds601_slist_state *ss, struct lfds601_slist_element *se );
  void lfds601_slist_delete_all_elements( struct lfds601_slist_state *ss );

  int lfds601_slist_get_user_data_from_element( struct lfds601_slist_element *se, void **user_data );
  int lfds601_slist_set_user_data_in_element( struct lfds601_slist_element *se, void *user_data );

  struct lfds601_slist_element *lfds601_slist_get_head( struct lfds601_slist_state *ss, struct lfds601_slist_element **se );
  struct lfds601_slist_element *lfds601_slist_get_next( struct lfds601_slist_element *se, struct lfds601_slist_element **next_se );
  struct lfds601_slist_element *lfds601_slist_get_head_and_then_next( struct lfds601_slist_state *ss, struct lfds601_slist_element **se );





  /***** lfds601_stack *****/

  /***** enums *****/
  enum lfds601_stack_query_type
  {
    LFDS601_STACK_QUERY_ELEMENT_COUNT
  };

  /***** incomplete types *****/
  struct lfds601_stack_state;

  /***** public prototypes *****/
  int lfds601_stack_new( struct lfds601_stack_state **ss, lfds601_atom_t number_elements );
  void lfds601_stack_delete( struct lfds601_stack_state *ss, void (*user_data_delete_function)(void *user_data, void *user_state), void *user_state );

  void lfds601_stack_clear( struct lfds601_stack_state *ss, void (*user_data_clear_function)(void *user_data, void *user_state), void *user_state );

  int lfds601_stack_push( struct lfds601_stack_state *ss, void *user_data );
  int lfds601_stack_guaranteed_push( struct lfds601_stack_state *ss, void *user_data );
  int lfds601_stack_pop( struct lfds601_stack_state *ss, void **user_data );

  void lfds601_stack_query( struct lfds601_stack_state *ss, enum lfds601_stack_query_type query_type, void *query_input, void *query_output );





  #define __LIBLFDS601_H

#endif

