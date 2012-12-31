/***** the library wide include file *****/
#include "liblfds610_internal.h"

/***** pragmas *****/

/***** defines *****/
#define LFDS610_QUEUE_STATE_UNKNOWN               -1
#define LFDS610_QUEUE_STATE_EMPTY                  0
#define LFDS610_QUEUE_STATE_ENQUEUE_OUT_OF_PLACE   1
#define LFDS610_QUEUE_STATE_ATTEMPT_DELFDS610_QUEUE        2

#define LFDS610_QUEUE_POINTER 0
#define LFDS610_QUEUE_COUNTER 1
#define LFDS610_QUEUE_PAC_SIZE 2

/***** structures *****/
#pragma pack( push, LFDS610_ALIGN_DOUBLE_POINTER )

struct lfds610_queue_state
{
  struct lfds610_queue_element
    *volatile enqueue[LFDS610_QUEUE_PAC_SIZE],
    *volatile dequeue[LFDS610_QUEUE_PAC_SIZE];

  lfds610_atom_t
    aba_counter;

  struct lfds610_freelist_state
    *fs;
};

struct lfds610_queue_element
{
  // TRD : next in a lfds610_queue requires volatile as it is target of CAS
  struct lfds610_queue_element
    *volatile next[LFDS610_QUEUE_PAC_SIZE];

  struct lfds610_freelist_element
    *fe;

  void
    *user_data;
};

#pragma pack( pop )

/***** externs *****/

/***** private prototypes *****/
int lfds610_queue_internal_freelist_init_function( void **user_data, void *user_state );
void lfds610_queue_internal_freelist_delete_function( void *user_data, void *user_state );

void lfds610_queue_internal_new_element_from_freelist( struct lfds610_queue_state *qs, struct lfds610_queue_element *qe[LFDS610_QUEUE_PAC_SIZE], void *user_data );
void lfds610_queue_internal_guaranteed_new_element_from_freelist( struct lfds610_queue_state *qs, struct lfds610_queue_element * qe[LFDS610_QUEUE_PAC_SIZE], void *user_data );
void lfds610_queue_internal_init_element( struct lfds610_queue_state *qs, struct lfds610_queue_element *qe[LFDS610_QUEUE_PAC_SIZE], struct lfds610_freelist_element *fe, void *user_data );

void lfds610_queue_internal_queue( struct lfds610_queue_state *qs, struct lfds610_queue_element *qe[LFDS610_QUEUE_PAC_SIZE] );

void lfds610_queue_internal_validate( struct lfds610_queue_state *qs, struct lfds610_validation_info *vi, enum lfds610_data_structure_validity *lfds610_queue_validity, enum lfds610_data_structure_validity *lfds610_freelist_validity );

