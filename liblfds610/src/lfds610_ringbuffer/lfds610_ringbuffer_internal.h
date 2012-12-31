/***** the library wide include file *****/
#include "liblfds610_internal.h"

/***** defines *****/

/***** structures *****/
#pragma pack( push, LFDS610_ALIGN_DOUBLE_POINTER )

struct lfds610_ringbuffer_state
{
  struct lfds610_queue_state
    *qs;

  struct lfds610_freelist_state
    *fs;
};

#pragma pack( pop )

/***** externs *****/

/***** private prototypes *****/
void lfds610_ringbuffer_internal_validate( struct lfds610_ringbuffer_state *rs, struct lfds610_validation_info *vi, enum lfds610_data_structure_validity *lfds610_queue_validity, enum lfds610_data_structure_validity *lfds610_freelist_validity );

