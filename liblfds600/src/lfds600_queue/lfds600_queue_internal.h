/***** the library wide include file *****/
#include "lfds600_internal.h"

/***** pragmas *****/

/***** defines *****/
#define LFDS600_QUEUE_STATE_UNKNOWN               -1
#define LFDS600_QUEUE_STATE_EMPTY                  0
#define LFDS600_QUEUE_STATE_ENQUEUE_OUT_OF_PLACE   1
#define LFDS600_QUEUE_STATE_ATTEMPT_DELFDS600_QUEUE        2

#define LFDS600_QUEUE_POINTER 0
#define LFDS600_QUEUE_COUNTER 1
#define LFDS600_QUEUE_PAC_SIZE 2

/***** structures *****/
#pragma pack( push, LFDS600_ALIGN_DOUBLE_POINTER )

struct lfds600_queue_state
{
  struct lfds600_queue_element
    *volatile enqueue[LFDS600_QUEUE_PAC_SIZE],
    *volatile dequeue[LFDS600_QUEUE_PAC_SIZE];

  lfds600_atom_t
    aba_counter;

  struct lfds600_freelist_state
    *fs;
};

struct lfds600_queue_element
{
  // TRD : next in a lfds600_queue requires volatile as it is target of CAS
  struct lfds600_queue_element
    *volatile next[LFDS600_QUEUE_PAC_SIZE];

  struct lfds600_freelist_element
    *fe;

  void
    *user_data;
};

#pragma pack( pop )

/***** externs *****/

/***** private prototypes *****/
int lfds600_queue_internal_freelist_init_function( void **user_data, void *user_state );
void lfds600_queue_internal_freelist_delete_function( void *user_data, void *user_state );

void lfds600_queue_internal_new_element_from_freelist( struct lfds600_queue_state *qs, struct lfds600_queue_element *qe[LFDS600_QUEUE_PAC_SIZE], void *user_data );
void lfds600_queue_internal_guaranteed_new_element_from_freelist( struct lfds600_queue_state *qs, struct lfds600_queue_element * qe[LFDS600_QUEUE_PAC_SIZE], void *user_data );
void lfds600_queue_internal_init_element( struct lfds600_queue_state *qs, struct lfds600_queue_element *qe[LFDS600_QUEUE_PAC_SIZE], struct lfds600_freelist_element *fe, void *user_data );

void lfds600_queue_internal_queue( struct lfds600_queue_state *qs, struct lfds600_queue_element *qe[LFDS600_QUEUE_PAC_SIZE] );

void lfds600_queue_internal_validate( struct lfds600_queue_state *qs, struct lfds600_validation_info *vi, enum data_structure_validity *lfds600_queue_validity, enum data_structure_validity *lfds600_freelist_validity );

