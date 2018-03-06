//===---- test_target_teams_distribute_parallel_for_defaultmap.c - combined consutrct -===//
// 
// OpenMP API Version 4.5 Nov 2015
// 
// testing de defaultmap of different scalar values. We check when it is off and when it is
// on. The first one should not copy values back from the device of scalars. the second one 
// should copy the values back even if they are not mapped explicitely
//
//===----------------------------------------------------------------------------------===//

#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"

#define ITERATIONS 10000

int test_defaultmap_on() {
  OMPVV_INFOMSG("test_defaultmap_on");

  int errors = 0;
  int i;

  // we try with all the scalars
  char scalar_char = 'a';
  short scalar_short = 10;
  int scalar_int = 11;
  float scalar_float = 5.5f;
  double scalar_double = 10.45;
  enum { VAL1 = 1, VAL2, VAL3, VAL4} scalar_enum = VAL1;
  

  // Map the same array to multiple devices. initialize with device number
#pragma omp target teams distribute parallel for defaultmap(tofrom: scalar)
  for (i = 0; i < ITERATIONS; ++i) {
    scalar_char = 'b';
    scalar_short = 20;
    scalar_int = 33;
    scalar_float = 6.5f;
    scalar_double = 20.45;
    scalar_enum = VAL4;
  } // end of omp target 

  OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_char != 'b');
  OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_short != 20);
  OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_int != 33);
  OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_float != 6.5f);
  OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_double != 20.45);
  OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_enum != VAL4);
  
  return errors;
}

int test_defaultmap_off() {
  OMPVV_INFOMSG("test_defaultmap_off");
  
  int errors = 0;
  int i;
  
  // we try with all the scalars
  char scalar_char = 'a';
  short scalar_short = 10;
  int scalar_int = 11;
  float scalar_float = 5.5f;
  double scalar_double = 10.45;
  enum { VAL1 = 1, VAL2, VAL3, VAL4} scalar_enum = VAL1;
  
  int isHost = 0; 

  // Map the same array to multiple devices. initialize with device number
#pragma omp target teams distribute parallel for map(from: isHost)
  for (i = 0; i < ITERATIONS; ++i) {
      isHost = omp_is_initial_device();
      scalar_char = 'b';
      scalar_short = 20;
      scalar_int = 33;
      scalar_float = 6.5f;
      scalar_double = 20.45;
      scalar_enum = VAL4;
  } // end of omp target 
  
  // If it is initial device then we will modify the original memory region
  if (isHost) {
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_char != 'b');
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_short != 20);
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_int != 33);
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_float != 6.5f);
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_double != 20.45);
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_enum != VAL4);
  } else {
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_char != 'a');
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_short != 10);
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_int != 11);
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_float != 5.5f);
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_double != 10.45);
    OMPVV_TEST_AND_SET_VERBOSE(errors, scalar_enum != VAL1);
  }
  
  return errors;
}
int main() {
  OMPVV_TEST_OFFLOADING;
  int errors = 0;
  OMPVV_TEST_AND_SET_VERBOSE(errors, test_defaultmap_on());
  OMPVV_TEST_AND_SET_VERBOSE(errors, test_defaultmap_off());

  OMPVV_REPORT_AND_RETURN(errors);
}
