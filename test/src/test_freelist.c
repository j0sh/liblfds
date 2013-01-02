#include "internal.h"





/****************************************************************************/
void test_lfds601_freelist( void )
{
  printf( "\n"
          "Freelist Tests\n"
          "==============\n" );

  freelist_test_internal_popping();
  freelist_test_internal_pushing();
  freelist_test_internal_popping_and_pushing();
  freelist_test_internal_rapid_popping_and_pushing();

  return;
}





/****************************************************************************/
void freelist_test_internal_popping( void )
{
  unsigned int
    loop,
    cpu_count,
    count;

  thread_state_t
    *thread_handles;

  enum lfds601_data_structure_validity
    dvs = LFDS601_VALIDITY_VALID;

  struct lfds601_freelist_state
    *fs;

  struct lfds601_freelist_element
    *fe;

  struct freelist_test_popping_state
    *ftps;

  unsigned int
    *found_count;

  /* TRD : we create a freelist with 1,000,000 elements

           the creation function runs in a single thread and creates
           and pushes those elements onto the freelist

           each element contains a void pointer which is its element number

           we then run one thread per CPU
           where each thread loops, popping as quickly as possible
           each popped element is pushed onto a thread-local freelist

           the threads run till the source freelist is empty

           we then check the thread-local freelists
           we should find we have every element

           then tidy up
  */

  internal_display_test_name( "Popping" );

  cpu_count = abstraction_cpu_count();

  lfds601_freelist_new( &fs, 1000000, freelist_test_internal_popping_init, NULL );
  ftps = malloc( sizeof(struct freelist_test_popping_state) * cpu_count );
  for( loop = 0 ; loop < cpu_count ; loop++ )
  {
    (ftps+loop)->fs = fs;
    lfds601_freelist_new( &(ftps+loop)->fs_thread_local, 0, NULL, NULL );
  }

  thread_handles = malloc( sizeof(thread_state_t) * cpu_count );

  for( loop = 0 ; loop < cpu_count ; loop++ )
    abstraction_thread_start( &thread_handles[loop], loop, freelist_test_internal_thread_popping, ftps+loop );

  for( loop = 0 ; loop < cpu_count ; loop++ )
    abstraction_thread_wait( thread_handles[loop] );

  free( thread_handles );

  // TRD : now we check the thread-local freelists
  found_count = malloc( sizeof(unsigned int) * 1000000 );
  for( loop = 0 ; loop < 1000000 ; loop++ )
    *(found_count+loop) = 0;

  for( loop = 0 ; loop < cpu_count ; loop++ )
  {
    while( lfds601_freelist_pop((ftps+loop)->fs_thread_local, &fe) )
    {
      lfds601_freelist_get_user_data_from_element( fe, (void **) &count );
      (*(found_count+count))++;
      lfds601_freelist_push( fs, fe );
    }
  }

  for( loop = 0 ; loop < 1000000 and dvs == LFDS601_VALIDITY_VALID ; loop++ )
  {
    if( *(found_count+loop) == 0 )
      dvs = LFDS601_VALIDITY_INVALID_MISSING_ELEMENTS;

    if( *(found_count+loop) > 1 )
      dvs = LFDS601_VALIDITY_INVALID_ADDITIONAL_ELEMENTS;
  }

  // TRD : cleanup
  free( found_count );
  for( loop = 0 ; loop < cpu_count ; loop++ )
    lfds601_freelist_delete( (ftps+loop)->fs_thread_local, NULL, NULL );
  lfds601_freelist_delete( fs, NULL, NULL );

  // TRD : print the test result
  internal_display_test_result( 1, "freelist", dvs );

  return;
}





/****************************************************************************/
#pragma warning( disable : 4100 )

int freelist_test_internal_popping_init( void **user_data, void *user_state )
{
  static lfds601_atom_t
    count = 0;

  assert( user_data != NULL );
  assert( user_state == NULL );

  *(lfds601_atom_t *) user_data = count++;

  return( 1 );
}

#pragma warning( default : 4100 )





/****************************************************************************/
thread_return_t CALLING_CONVENTION freelist_test_internal_thread_popping( void *freelist_test_popping_state )
{
  struct freelist_test_popping_state
    *ftps;

  struct lfds601_freelist_element
    *fe;

  assert( freelist_test_popping_state != NULL );

  ftps = (struct freelist_test_popping_state *) freelist_test_popping_state;

  while( lfds601_freelist_pop(ftps->fs, &fe) )
    lfds601_freelist_push( ftps->fs_thread_local, fe );

  return( (thread_return_t) EXIT_SUCCESS );
}





/****************************************************************************/
void freelist_test_internal_pushing( void )
{
  unsigned int
    loop,
    cpu_count;

  thread_state_t
    *thread_handles;

  enum lfds601_data_structure_validity
    dvs;

  struct freelist_test_pushing_state
    *ftps;

  struct lfds601_freelist_element
    *fe;

  struct lfds601_freelist_state
    *fs,
    *cleanup_fs;

  struct freelist_test_counter_and_thread_number
    *cnt,
    *counter_and_number_trackers;

  struct lfds601_validation_info
    vi = { 1000000, 1000000 };

  /* TRD : we create an empty freelist, which we will push to

           we then create one freelist per CPU, where this freelist
           contains 1,000,000/cpu_count number of elements and
           each element is an incrementing counter and unique ID
           (from 0 to number of CPUs)

           we then start one thread per CPU, where each thread is
           given one of the populated freelists and pops from that
           to push to the empty freelist

           the reason for this is to achieve memory pre-allocation
           which allows the pushing threads to run at maximum speed

           the threads end when their freelists are empty

           we then fully pop the now populated main freelist (onto
           a second freelist, so we can cleanly free all memory),
           checking that the counts increment on a per unique ID basis
           and that the number of elements we pop equals 1,000,000
           (since each element has an incrementing counter which is
            unique on a per unique ID basis, we can know we didn't lose
            any elements)
  */

  internal_display_test_name( "Pushing" );

  cpu_count = abstraction_cpu_count();

  ftps = malloc( sizeof(struct freelist_test_pushing_state) * cpu_count );

  lfds601_freelist_new( &fs, 0, NULL, NULL );

  for( loop = 0 ; loop < cpu_count ; loop++ )
  {
    (ftps+loop)->thread_number = (lfds601_atom_t) loop;
    lfds601_freelist_new( &(ftps+loop)->source_fs, 1000000 / cpu_count, freelist_test_internal_pushing_init, (void *) (lfds601_atom_t) loop );
    (ftps+loop)->fs = fs;
  }

  thread_handles = malloc( sizeof(thread_state_t) * cpu_count );

  for( loop = 0 ; loop < cpu_count ; loop++ )
    abstraction_thread_start( &thread_handles[loop], loop, freelist_test_internal_thread_pushing, ftps+loop );

  for( loop = 0 ; loop < cpu_count ; loop++ )
    abstraction_thread_wait( thread_handles[loop] );

  free( thread_handles );

  // TRD : now fully pop and verify the main freelist
  lfds601_freelist_new( &cleanup_fs, 0, NULL, NULL );

  counter_and_number_trackers = malloc( sizeof(struct freelist_test_counter_and_thread_number) * cpu_count );

  for( loop = 0 ; loop < cpu_count ; loop++ )
  {
    (counter_and_number_trackers+loop)->counter = (1000000 / cpu_count) * loop;
    (counter_and_number_trackers+loop)->thread_number = (lfds601_atom_t) loop;
  }

  lfds601_freelist_query( fs, LFDS601_FREELIST_QUERY_VALIDATE, &vi, (void *) &dvs );

  while( dvs == LFDS601_VALIDITY_VALID and lfds601_freelist_pop(fs, &fe) )
  {
    static int count = 0;

    lfds601_freelist_get_user_data_from_element( fe, (void **) &cnt );

    if( cnt->counter != (counter_and_number_trackers+cnt->thread_number)->counter++ )
      dvs = LFDS601_VALIDITY_INVALID_MISSING_ELEMENTS;

    lfds601_freelist_push( cleanup_fs, fe );

    count++;
  }

  // TRD : clean up
  free( counter_and_number_trackers );

  for( loop = 0 ; loop < cpu_count ; loop++ )
    lfds601_freelist_delete( (ftps+loop)->source_fs, NULL, NULL );

  free( ftps );

  lfds601_freelist_delete( cleanup_fs, freelist_test_internal_pushing_delete, NULL );
  lfds601_freelist_delete( fs, NULL, NULL );

  // TRD : print the test result
  internal_display_test_result( 1, "freelist", dvs );

  return;
}





/****************************************************************************/
int freelist_test_internal_pushing_init( void **user_data, void *user_state )
{
  struct freelist_test_counter_and_thread_number
    *ftcatn;

  static lfds601_atom_t
    counter = 0;

  assert( user_data != NULL );
  // TRD : user_state is being used as an integer type

  *user_data = malloc( sizeof(struct freelist_test_counter_and_thread_number) );

  ftcatn = (struct freelist_test_counter_and_thread_number *) *user_data;

  ftcatn->counter = counter++;
  ftcatn->thread_number = (lfds601_atom_t) user_state;

  return( 1 );
}





/****************************************************************************/
#pragma warning( disable : 4100 )

void freelist_test_internal_pushing_delete( void *user_data, void *user_state )
{
  assert( user_data != NULL );
  assert( user_state == NULL );

  free( user_data );

  return;
}

#pragma warning( default : 4100 )





/****************************************************************************/
thread_return_t CALLING_CONVENTION freelist_test_internal_thread_pushing( void *freelist_test_pushing_state )
{
  struct freelist_test_pushing_state
    *ftps;

  struct lfds601_freelist_element
    *fe;

  assert( freelist_test_pushing_state != NULL );

  ftps = (struct freelist_test_pushing_state *) freelist_test_pushing_state;

  while( lfds601_freelist_pop(ftps->source_fs, &fe) )
    lfds601_freelist_push( ftps->fs, fe );

  return( (thread_return_t) EXIT_SUCCESS );
}





/****************************************************************************/
void freelist_test_internal_popping_and_pushing( void )
{
  unsigned int
    loop,
    cpu_count;

  thread_state_t
    *thread_handles;

  enum lfds601_data_structure_validity
    dvs;

  struct lfds601_freelist_state
    *fs;

  struct freelist_test_popping_and_pushing_state
    *pps;

  struct lfds601_validation_info
    vi;

  /* TRD : we have two threads per CPU
           the threads loop for ten seconds
           the first thread pushes 100000 elements then pops 100000 elements
           the second thread pops 100000 elements then pushes 100000 elements
           all pushes and pops go onto the single main freelist

           after time is up, all threads push what they have remaining onto
           the main freelist

           we then validate the main freelist
  */

  internal_display_test_name( "Popping and pushing (10 seconds)" );

  cpu_count = abstraction_cpu_count();

  lfds601_freelist_new( &fs, 100000 * cpu_count, NULL, NULL );

  pps = malloc( sizeof(struct freelist_test_popping_and_pushing_state) * cpu_count * 2 );

  for( loop = 0 ; loop < cpu_count ; loop++ )
  {
    (pps+loop)->fs = fs;
    lfds601_freelist_new( &(pps+loop)->local_fs, 0, NULL, NULL );

    (pps+loop+cpu_count)->fs = fs;
    lfds601_freelist_new( &(pps+loop+cpu_count)->local_fs, 100000, NULL, NULL );
  }

  thread_handles = malloc( sizeof(thread_state_t) * cpu_count * 2 );

  for( loop = 0 ; loop < cpu_count ; loop++ )
  {
    abstraction_thread_start( &thread_handles[loop], loop, freelist_test_internal_thread_popping_and_pushing_start_popping, pps+loop );
    abstraction_thread_start( &thread_handles[loop+cpu_count], loop, freelist_test_internal_thread_popping_and_pushing_start_pushing, pps+loop+cpu_count );
  }

  for( loop = 0 ; loop < cpu_count * 2 ; loop++ )
    abstraction_thread_wait( thread_handles[loop] );

  free( thread_handles );

  for( loop = 0 ; loop < cpu_count * 2 ; loop++ )
    lfds601_freelist_delete( (pps+loop)->local_fs, NULL, NULL );

  free( pps );

  vi.min_elements = vi.max_elements = 100000 * cpu_count * 2;

  lfds601_freelist_query( fs, LFDS601_FREELIST_QUERY_VALIDATE, (void *) &vi, (void *) &dvs );

  lfds601_freelist_delete( fs, NULL, NULL );

  // TRD : print the test result
  internal_display_test_result( 1, "freelist", dvs );

  return;
}






/****************************************************************************/
thread_return_t CALLING_CONVENTION freelist_test_internal_thread_popping_and_pushing_start_popping( void *freelist_test_popping_and_pushing_state )
{
  struct freelist_test_popping_and_pushing_state
    *pps;

  struct lfds601_freelist_element
    *fe;

  time_t
    start_time;

  unsigned int
    count;

  assert( freelist_test_popping_and_pushing_state != NULL );

  pps = (struct freelist_test_popping_and_pushing_state *) freelist_test_popping_and_pushing_state;

  time( &start_time );

  while( time(NULL) < start_time + 10 )
  {
    count = 0;

    while( count < 100000 )
    {
      lfds601_freelist_pop( pps->fs, &fe );

      if( fe != NULL )
      {
        lfds601_freelist_push( pps->local_fs, fe );
        count++;
      }
    }

    while( lfds601_freelist_pop(pps->local_fs, &fe) )
      lfds601_freelist_push( pps->fs, fe );
  }

  return( (thread_return_t) EXIT_SUCCESS );
}





/****************************************************************************/
thread_return_t CALLING_CONVENTION freelist_test_internal_thread_popping_and_pushing_start_pushing( void *freelist_test_popping_and_pushing_state )
{
  struct freelist_test_popping_and_pushing_state
    *pps;

  struct lfds601_freelist_element
    *fe;

  time_t
    start_time;

  unsigned int
    count;

  assert( freelist_test_popping_and_pushing_state != NULL );

  pps = (struct freelist_test_popping_and_pushing_state *) freelist_test_popping_and_pushing_state;

  time( &start_time );

  while( time(NULL) < start_time + 10 )
  {
    while( lfds601_freelist_pop(pps->local_fs, &fe) )
      lfds601_freelist_push( pps->fs, fe );

    count = 0;

    while( count < 1000 )
    {
      lfds601_freelist_pop( pps->fs, &fe );

      if( fe != NULL )
      {
        lfds601_freelist_push( pps->local_fs, fe );
        count++;
      }
    }
  }

  // TRD : now push whatever we have in our local freelist
  while( lfds601_freelist_pop(pps->local_fs, &fe) )
    lfds601_freelist_push( pps->fs, fe );

  return( (thread_return_t) EXIT_SUCCESS );
}





/****************************************************************************/
void freelist_test_internal_rapid_popping_and_pushing( void )
{
  unsigned int
    loop,
    cpu_count;

  thread_state_t
    *thread_handles;

  struct lfds601_freelist_state
    *fs;

  struct lfds601_validation_info
    vi;

  enum lfds601_data_structure_validity
    dvs;

  /* TRD : in these tests there is a fundamental antagonism between
           how much checking/memory clean up that we do and the
           likelyhood of collisions between threads in their lock-free
           operations

           the lock-free operations are very quick; if we do anything
           much at all between operations, we greatly reduce the chance
           of threads colliding

           so we have some tests which do enough checking/clean up that
           they can tell the freelist is valid and don't leak memory
           and here, this test now is one of those which does minimal
           checking - in fact, the nature of the test is that you can't
           do any real checking - but goes very quickly

           what we do is create a small freelist and then run one thread
           per CPU, where each thread simply pops and then immediately
           pushes

           the test runs for ten seconds

           after the test is done, the only check we do is to traverse
           the freelist, checking for loops and ensuring the number of
           elements is correct
  */

  internal_display_test_name( "Rapid popping and pushing (10 seconds)" );

  cpu_count = abstraction_cpu_count();

  lfds601_freelist_new( &fs, cpu_count, NULL, NULL );

  thread_handles = malloc( sizeof(thread_state_t) * cpu_count );

  for( loop = 0 ; loop < cpu_count ; loop++ )
    abstraction_thread_start( &thread_handles[loop], loop, freelist_test_internal_thread_rapid_popping_and_pushing, fs );

  for( loop = 0 ; loop < cpu_count ; loop++ )
    abstraction_thread_wait( thread_handles[loop] );

  free( thread_handles );

  vi.min_elements = cpu_count;
  vi.max_elements = cpu_count;

  lfds601_freelist_query( fs, LFDS601_FREELIST_QUERY_VALIDATE, (void *) &vi, (void *) &dvs );

  lfds601_freelist_delete( fs, NULL, NULL );

  // TRD : print the test result
  internal_display_test_result( 1, "freelist", dvs );

  return;
}





/****************************************************************************/
thread_return_t CALLING_CONVENTION freelist_test_internal_thread_rapid_popping_and_pushing( void *lfds601_freelist_state )
{
  struct lfds601_freelist_state
    *fs;

  struct lfds601_freelist_element
    *fe;

  time_t
    start_time;

  assert( lfds601_freelist_state != NULL );

  fs = (struct lfds601_freelist_state *) lfds601_freelist_state;

  time( &start_time );

  while( time(NULL) < start_time + 10 )
  {
    lfds601_freelist_pop( fs, &fe );
    lfds601_freelist_push( fs, fe );
  }

  return( (thread_return_t) EXIT_SUCCESS );
}

