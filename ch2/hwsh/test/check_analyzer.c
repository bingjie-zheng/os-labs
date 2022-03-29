#include <stdlib.h>
#include <check.h>
#include "analyzer.h"
#include "exec.h"

#define ANALYZE_TEST_SIZE 4

size_t test_argc[ANALYZE_TEST_SIZE] = {3,2,4,1};
char *test_argv[ANALYZE_TEST_SIZE][100] = {
    {
      "ls",
      "-a",
      "-b"
    }, {
      "cat",
      "foo/bar",
    }, {
      "gcc",
      "-c",
      "-o",
      "test.c"
    }, {
      "bin/hwsh"
    }
};
char *test_cmd[ANALYZE_TEST_SIZE] = {
    "ls -a -b\n",
    "cat   foo/bar\n",
    "  gcc -c -o test.c\n",
    "bin/hwsh  \n",
};


void setup(void) {
  ;
}

void teardown(void) {
  ;
}

START_TEST(analyzer_test) {
  size_t argc;
  char *argv[100];
  size_t len = 0;
  int ret = scanner(test_cmd[_i], &argc, argv);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(argc, test_argc[_i]);
  for(int i = 0; i < argc; i++){
    ck_assert_pstr_eq(argv[i], test_argv[_i][i]);
  }
  Task *t;
  t = create_task(argv, argc);
  ck_assert_int_eq(t->argc, test_argc[_i]);
  for(int i = 0; i < t->argc; i++){
    ck_assert_pstr_eq(t->argv[i], test_argv[_i][i]);
  }
}
END_TEST

Suite *analyzer_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Analyzer");
  tc_core = tcase_create("Core");

  tcase_add_loop_test(tc_core, analyzer_test, 0, ANALYZE_TEST_SIZE);
  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int number_failed;
  Suite *s;
  SRunner *sr;
  s = analyzer_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
