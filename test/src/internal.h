/***** ANSI includes *****/
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/***** internal includes *****/
#include "abstraction.h"

/***** external includes *****/
#include "liblfds600.h"

/***** defines *****/
#define and &&
#define or  ||

#define RAISED   1
#define LOWERED  0

#define NO_FLAGS 0x0

/***** enums *****/
enum lfds600_test_operation
{
  UNKNOWN,
  HELP,
  TEST,
  BENCHMARK
};

/***** structs *****/
#include "structures.h"

/***** prototypes *****/
int main( int argc, char **argv );

void internal_display_test_name( char *test_name );
void internal_display_test_result( unsigned int number_name_dvs_pairs, ... );
void internal_display_data_structure_validity( enum data_structure_validity dvs );

void benchmark_lfds600_freelist( void );
  thread_return_t CALLING_CONVENTION benchmark_lfds600_freelist_thread_pop_and_push( void *lfds600_freelist_benchmark );

void benchmark_lfds600_queue( void );
  thread_return_t CALLING_CONVENTION benchmark_lfds600_queue_thread_delfds600_queue_and_enqueue( void *lfds600_queue_benchmark );

void benchmark_lfds600_ringbuffer( void );
  thread_return_t CALLING_CONVENTION benchmark_lfds600_ringbuffer_thread_write_and_read( void *lfds600_ringbuffer_benchmark );

void benchmark_lfds600_stack( void );
  thread_return_t CALLING_CONVENTION benchmark_lfds600_stack_thread_push_and_pop( void *lfds600_stack_benchmark );

void test_lfds600_abstraction( void );
  void abstraction_test_increment( void );
    thread_return_t CALLING_CONVENTION abstraction_test_internal_thread_increment( void *shared_counter );
    thread_return_t CALLING_CONVENTION abstraction_test_internal_thread_atomic_increment( void *shared_counter );
  void abstraction_test_dcas( void );
    thread_return_t CALLING_CONVENTION abstraction_test_internal_thread_dcas( void *abstraction_test_dcas_state );

void test_lfds600_freelist( void );
  void freelist_test_internal_popping( void );
    int freelist_test_internal_popping_init( void **user_data, void *user_state );
    thread_return_t CALLING_CONVENTION freelist_test_internal_thread_popping( void *freelist_test_popping_state );
  void freelist_test_internal_pushing( void );
    int freelist_test_internal_pushing_init( void **user_data, void *user_state );
    void freelist_test_internal_pushing_delete( void *user_data, void *user_state );
    thread_return_t CALLING_CONVENTION freelist_test_internal_thread_pushing( void *freelist_test_pushing_state );
  void freelist_test_internal_popping_and_pushing( void );
    thread_return_t CALLING_CONVENTION freelist_test_internal_thread_popping_and_pushing_start_popping( void *freelist_test_popping_and_pushing_state );
    thread_return_t CALLING_CONVENTION freelist_test_internal_thread_popping_and_pushing_start_pushing( void *freelist_test_popping_and_pushing_state );
  void freelist_test_internal_rapid_popping_and_pushing( void );
    thread_return_t CALLING_CONVENTION freelist_test_internal_thread_rapid_popping_and_pushing( void *lfds600_freelist_state );

void test_lfds600_queue( void );
  void queue_test_enqueuing( void );
    thread_return_t CALLING_CONVENTION queue_test_internal_thread_simple_enqueuer( void *queue_test_enqueuing_state );
  void queue_test_dequeuing( void );
    thread_return_t CALLING_CONVENTION queue_test_internal_thread_simple_dequeuer( void *queue_test_dequeuing_state );
  void queue_test_enqueuing_and_dequeuing( void );
    thread_return_t CALLING_CONVENTION queue_test_internal_thread_enqueuer_and_dequeuer( void *queue_test_rapid_enqueuing_and_dequeuing_state );
  void queue_test_rapid_enqueuing_and_dequeuing( void );
    thread_return_t CALLING_CONVENTION queue_test_internal_thread_rapid_enqueuer_and_dequeuer( void *queue_test_rapid_enqueuing_and_dequeuing_state );

void test_lfds600_ringbuffer( void );
  void ringbuffer_test_reading( void );
    thread_return_t CALLING_CONVENTION ringbuffer_test_thread_simple_reader( void *ringbuffer_test_reading_state );
  void ringbuffer_test_writing( void );
    thread_return_t CALLING_CONVENTION ringbuffer_test_thread_simple_writer( void *ringbuffer_test_writing_state );
  void ringbuffer_test_reading_and_writing( void );
    thread_return_t CALLING_CONVENTION ringbuffer_test_thread_reader_writer( void *ringbuffer_test_reading_and_writing_state );

void test_lfds600_slist( void );
  thread_return_t CALLING_CONVENTION lfds600_slist_internal_thread_head_writer( void *lfds600_slist_thread_start_state );
  thread_return_t CALLING_CONVENTION lfds600_slist_internal_thread_after_writer( void *lfds600_slist_thread_start_state );
  thread_return_t CALLING_CONVENTION lfds600_slist_internal_thread_traverser( void *lfds600_slist_thread_start_state );
  thread_return_t CALLING_CONVENTION lfds600_slist_internal_thread_deleter_traverser( void *lfds600_slist_thread_start_state );

void test_lfds600_stack( void );
  thread_return_t CALLING_CONVENTION lfds600_stack_internal_thread_reader( void *lfds600_stack_state );
  thread_return_t CALLING_CONVENTION lfds600_stack_internal_thread_writer( void *lfds600_stack_state );

