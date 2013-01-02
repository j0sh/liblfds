/***** the library wide include file *****/
#include "lfds601_internal.h"

/***** pragmas *****/

/***** defines *****/
#define LFDS601_QUEUE_STATE_UNKNOWN               -1
#define LFDS601_QUEUE_STATE_EMPTY                  0
#define LFDS601_QUEUE_STATE_ENQUEUE_OUT_OF_PLACE   1
#define LFDS601_QUEUE_STATE_ATTEMPT_DELFDS601_QUEUE        2

#define LFDS601_QUEUE_POINTER 0
#define LFDS601_QUEUE_COUNTER 1
#define LFDS601_QUEUE_PAC_SIZE 2

/***** structures *****/
#pragma pack( push, LFDS601_ALIGN_DOUBLE_POINTER )

struct lfds601_queue_state
{
  struct lfds601_queue_element
    *volatile enqueue[LFDS601_QUEUE_PAC_SIZE],
    *volatile dequeue[LFDS601_QUEUE_PAC_SIZE];

  lfds601_atom_t
    aba_counter;

  struct lfds601_freelist_state
    *fs;
};

struct lfds601_queue_element
{
  // TRD : next in a lfds601_queue requires volatile as it is target of CAS
  struct lfds601_queue_element
    *volatile next[LFDS601_QUEUE_PAC_SIZE];

  struct lfds601_freelist_element
    *fe;

  void
    *user_data;
};

#pragma pack( pop )

/***** externs *****/

/***** private prototypes *****/
int lfds601_queue_internal_freelist_init_function( void **user_data, void *user_state );
void lfds601_queue_internal_freelist_delete_function( void *user_data, void *user_state );

void lfds601_queue_internal_new_element_from_freelist( struct lfds601_queue_state *qs, struct lfds601_queue_element *qe[LFDS601_QUEUE_PAC_SIZE], void *user_data );
void lfds601_queue_internal_guaranteed_new_element_from_freelist( struct lfds601_queue_state *qs, struct lfds601_queue_element * qe[LFDS601_QUEUE_PAC_SIZE], void *user_data );
void lfds601_queue_internal_init_element( struct lfds601_queue_state *qs, struct lfds601_queue_element *qe[LFDS601_QUEUE_PAC_SIZE], struct lfds601_freelist_element *fe, void *user_data );

void lfds601_queue_internal_queue( struct lfds601_queue_state *qs, struct lfds601_queue_element *qe[LFDS601_QUEUE_PAC_SIZE] );

void lfds601_queue_internal_validate( struct lfds601_queue_state *qs, struct lfds601_validation_info *vi, enum lfds601_data_structure_validity *lfds601_queue_validity, enum lfds601_data_structure_validity *lfds601_freelist_validity );

