#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "./modules/s21_rpn.h"

#define EPS 1e-8

START_TEST(s21_test_1) {
  int error = 0;
  struct node_list* rpn_list = rpn("sin(x)+cos(2*x)+0.01");
  for (double x = -10; x < 10; x += 0.1) {
    double res = rpn_calculate(rpn_list, x, &error);
    double fres = sin(x) + cos(2 * x) + 0.01;
    ck_assert_double_eq_tol(res, fres, EPS);
    ck_assert_int_eq(error, 0);
  }
  list_destroy(rpn_list);
}

START_TEST(s21_test_2) {
  int error = 0;
  struct node_list* rpn_list = rpn("sin(cos(2*x))+sqrt(x*123.321)");
  for (double x = 0; x < 10; x += 0.1) {
    double res = rpn_calculate(rpn_list, x, &error);
    double fres = sin(cos(2 * x)) + sqrt(x * 123.321);
    ck_assert_double_eq_tol(res, fres, EPS);
    ck_assert_int_eq(error, 0);
  }
  list_destroy(rpn_list);
}

START_TEST(s21_test_3) {
  int error = 0;
  struct node_list* rpn_list = rpn("asin(x)/acos(x)+(x+3)^1.321");
  for (double x = -1; x < 1; x += 0.01) {
    double res = rpn_calculate(rpn_list, x, &error);
    double fres = asin(x) / acos(x) + pow(x + 3, 1.321);
    ck_assert_double_eq_tol(res, fres, EPS);
    ck_assert_int_eq(error, 0);
  }
  list_destroy(rpn_list);
}

START_TEST(s21_test_4) {
  int error = 0;
  struct node_list* rpn_list = rpn("x^2+-x++x/2+xmod4");
  for (double x = 100; x < 200; x++) {
    double res = rpn_calculate(rpn_list, x, &error);
    double fres = x * x - x + x / 2 + fmod(x, 4);
    ck_assert_double_eq_tol(res, fres, EPS);
    ck_assert_int_eq(error, 0);
  }
  list_destroy(rpn_list);
}

START_TEST(s21_test_5) {
  int error = 0;
  struct node_list* rpn_list = rpn("tan(x)+atan(x)-0.5");
  for (double x = 0; x < 1; x = x + 0.01) {
    double res = rpn_calculate(rpn_list, x, &error);
    double fres = tan(x) + atan(x) - 0.5;
    // printf("%.8lf, %.8lf, %.8lf\n", x, res, fres);
    ck_assert_double_eq_tol(res, fres, EPS);
    ck_assert_int_eq(error, 0);
  }
  list_destroy(rpn_list);
}

START_TEST(s21_test_6) {
  int error = 0;
  struct node_list* rpn_list = rpn("1-1/2+3/2+x-2*2/2^2");
  for (double x = 10; x < 20; x++) {
    double res = rpn_calculate(rpn_list, x, &error);
    double fres = 1.0 - 1.0 / 2.0 + 3.0 / 2.0 + x - 2.0 * 2.0 / pow(2, 2);
    // printf("%.8lf, %.8lf, %.8lf\n", x, res, fres);
    ck_assert_double_eq_tol(res, fres, EPS);
    ck_assert_int_eq(error, 0);
  }
  list_destroy(rpn_list);
}

START_TEST(s21_test_7) {
  int error = 0;
  struct node_list* rpn_list = rpn("(log(x*2)+ln(x+2)-2)/2");
  for (double x = 0.01; x < 1; x = x + 0.01) {
    double res = rpn_calculate(rpn_list, x, &error);
    double fres = (log10(x * 2) + log(x + 2) - 2) / 2;
    // printf("%.8lf, %.8lf, %.8lf\n", x, res, fres);
    ck_assert_double_eq_tol(res, fres, EPS);
    ck_assert_int_eq(error, 0);
  }
  list_destroy(rpn_list);
}

START_TEST(s21_test_null) {
  struct node_list* rpn_list = rpn("asin(x)/acos(x)+(x+3)^1.321!");
  ck_assert_ptr_null(rpn_list);
  list_destroy(rpn_list);
  rpn_list = rpn("asin(x)acos(x)");
  ck_assert_ptr_null(rpn_list);
  list_destroy(rpn_list);
  rpn_list = rpn("(((1+2)-1)-1)-1)");
  ck_assert_ptr_null(rpn_list);
  list_destroy(rpn_list);
  rpn_list = rpn("/*/2-----3-3-3-3-3");
  ck_assert_ptr_null(rpn_list);
  list_destroy(rpn_list);
  rpn_list = rpn("sin1");
  ck_assert_ptr_null(rpn_list);
  list_destroy(rpn_list);
  rpn_list = rpn("1.111(x)2.222");
  ck_assert_ptr_null(rpn_list);
  list_destroy(rpn_list);
  int error;
  double res = rpn_calculate(NULL, 1, &error);
  ck_assert_double_eq_tol(res, 0, EPS);
}

Suite* s21_smartcalc_suite(void) {
  Suite* suite = suite_create("s21_smartcalc");
  TCase* tcase_core_fromto = tcase_create("simple");
  tcase_add_test(tcase_core_fromto, s21_test_1);
  tcase_add_test(tcase_core_fromto, s21_test_2);
  tcase_add_test(tcase_core_fromto, s21_test_3);
  tcase_add_test(tcase_core_fromto, s21_test_4);
  tcase_add_test(tcase_core_fromto, s21_test_5);
  tcase_add_test(tcase_core_fromto, s21_test_6);
  tcase_add_test(tcase_core_fromto, s21_test_7);

  tcase_add_test(tcase_core_fromto, s21_test_null);
  suite_add_tcase(suite, tcase_core_fromto);
  return suite;
}

int main(void) {
  int failed_count;
  Suite* suite = s21_smartcalc_suite();
  SRunner* suite_runner = srunner_create(suite);
  srunner_set_fork_status(suite_runner, CK_NOFORK);
  srunner_run_all(suite_runner, CK_NORMAL);
  failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  return (failed_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
