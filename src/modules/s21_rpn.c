#include "s21_rpn.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stack_dbl* stack_dbl_init() {
  struct stack_dbl* tmp = (struct stack_dbl*)malloc(sizeof(struct stack_dbl));
  tmp->ptr = NULL;
  tmp->count = 0;
  return tmp;
}

void stack_dbl_push(struct stack_dbl* st, double n) {
  if (st != NULL) {
    struct stack_node_dbl* tmp =
        (struct stack_node_dbl*)malloc(sizeof(struct stack_node_dbl));
    tmp->value = n;
    if (st->ptr == NULL) {
      tmp->next = NULL;
    } else {
      tmp->next = st->ptr;
    }
    st->ptr = tmp;
    st->count++;
  }
}

double stack_dbl_pop(struct stack_dbl* st) {
  double res = 0;
  if (st != NULL && st->ptr != NULL) {
    struct stack_node_dbl* tmp = st->ptr;
    st->ptr = tmp->next;
    res = tmp->value;
    free(tmp);
    st->count--;
  }
  return res;
}

void stack_dbl_destroy(struct stack_dbl* st) {
  while (st->ptr != NULL) {
    struct stack_node_dbl* tmp = st->ptr;
    st->ptr = tmp->next;
    free(tmp);
  }
  free(st);
}

struct stack_char* stack_char_init() {
  struct stack_char* tmp =
      (struct stack_char*)malloc(sizeof(struct stack_char));
  tmp->ptr = NULL;
  tmp->count = 0;
  return tmp;
}

void stack_char_push(struct stack_char* st, char c) {
  if (st != NULL) {
    struct node_char* tmp = (struct node_char*)malloc(sizeof(struct node_char));
    tmp->c = c;
    if (st->ptr == NULL) {
      tmp->next = NULL;
    } else {
      tmp->next = st->ptr;
    }
    st->ptr = tmp;
    st->count++;
  }
}

char stack_char_pop(struct stack_char* st) {
  char res = 0;
  if (st != NULL && st->ptr != NULL) {
    struct node_char* tmp = st->ptr;
    st->ptr = tmp->next;
    res = tmp->c;
    free(tmp);
    st->count--;
  }
  return res;
}

void stack_char_destroy(struct stack_char* st) {
  while (st->ptr != NULL) {
    struct node_char* tmp = st->ptr;
    st->ptr = tmp->next;
    free(tmp);
  }
  free(st);
}

struct node_list* list_init(double value) {
  struct node_list* tmp = (struct node_list*)malloc(sizeof(struct node_list));
  tmp->value = value;
  tmp->next = NULL;
  return tmp;
}

void list_add_value(struct node_list* root, double value) {
  struct node_list* end = root;
  while (end->next != NULL) end = end->next;
  struct node_list* tmp = (struct node_list*)malloc(sizeof(struct node_list));
  tmp->value = value;
  tmp->next = NULL;
  end->next = tmp;
  root->value++;
}

void list_destroy(struct node_list* root) {
  if (root != NULL) {
    if (root->next != NULL) list_destroy(root->next);
    free(root);
  }
}

int prepare_string(char* str) {
  int error = 0;
  find_replace(str, "asin", 'i');
  find_replace(str, "sin", 's');
  find_replace(str, "acos", 'o');
  find_replace(str, "cos", 'c');
  find_replace(str, "atan", 'n');
  find_replace(str, "tan", 't');
  find_replace(str, "sqrt", 'q');
  find_replace(str, "ln", 'l');
  find_replace(str, "log", 'g');
  find_replace(str, "mod", '%');
  int len = strlen(str);
  for (int i = 0; i < len && error == 0; i++) {
    if (strchr(SYMBOLS, str[i]) == NULL) error = 1;
    if (str[i] == '-' && (i == 0 || strchr("+-*/%^(", str[i - 1]) != NULL)) {
      str[i] = '~';
    }
    if (str[i] == '+' && (i == 0 || strchr("+-*/%^(", str[i - 1]) != NULL)) {
      str[i] = '?';
    }
    if (strchr("sicotnqlg", str[i]) != NULL && str[i + 1] != '(') error = 1;
    if (str[i] == ')' && strchr("+-*/%^()", str[i + 1]) == NULL) error = 1;
  }
  if (error == 1) str[0] = 0;
  return error;
}

void find_replace(char* str, char* find, char replace) {
  char* finds;
  int flen = strlen(find);
  while ((finds = strstr(str, find)) != NULL) {
    int f = finds - str;
    int i = 1;
    str[f] = replace;
    while (str[f + i + flen - 1] != '\0') {
      str[f + i] = str[f + i + flen - 1];
      i = i + 1;
    }
    str[f + i] = '\0';
  }
}

struct node_list* rpn(const char* str) {
  struct node_list* root = list_init(0);
  struct stack_char* st = stack_char_init();
  char* str_var = (char*)calloc(strlen(str) + 1, sizeof(char));
  strcpy(str_var, str);
  int i = 0;
  int error = prepare_string(str_var);
  while (str_var[i] != '\0') {
    if (strchr("0123456789.", str_var[i]) != NULL) {
      int j = 1;
      while (strchr("0123456789.", str_var[i + j]) != NULL &&
             str_var[i + j] != '\0')
        j++;
      char* tmp_str = (char*)malloc((j + 1) * sizeof(char));
      for (int k = 0; k < j; k++) tmp_str[k] = str_var[i + k];
      tmp_str[j] = '\0';
      list_add_value(root, atof(tmp_str));
      free(tmp_str);
      i = i + j - 1;
    }
    if (str_var[i] == 'x') list_add_value(root, -str_var[i]);
    if (strchr("sicotnqlg(~?", str_var[i]) != NULL)
      stack_char_push(st, str_var[i]);
    if (str_var[i] == ')') {
      while (st->count > 0 && st->ptr->c != '(')
        list_add_value(root, -stack_char_pop(st));
      if (st->count == 0) error = 1;
      if (st->count > 0 && st->ptr->c == '(') stack_char_pop(st);
    }
    if (strchr("+-*/%^", str_var[i]) != NULL) {
      while ((st->count > 0) && (strchr("sicotnqlg", st->ptr->c) != NULL ||
                                 priority(st->ptr->c) >= priority(str_var[i])))
        list_add_value(root, -stack_char_pop(st));
      stack_char_push(st, str_var[i]);
    }
    i++;
  }
  free(str_var);
  while (st->count > 0) {
    char c = stack_char_pop(st);
    if (strchr("+-*/%^sicotnqlg~?", c) != NULL) {
      list_add_value(root, -c);
    } else {
      error = 1;
    }
  }
  if (st->count != 0) error = 1;
  stack_char_destroy(st);
  int error_test = 0;
  rpn_calculate(root, 0, &error_test);
  if (error_test == 2) error = 1;
  if (error == 1) {
    list_destroy(root);
    root = NULL;
  }
  return root;
}

double rpn_calculate(struct node_list* root, double x, int* error) {
  struct node_list* iter;
  if (root != NULL) {
    iter = root->next;
  } else {
    iter = NULL;
  }
  struct stack_dbl* st_dbl = stack_dbl_init();
  double res;
  while (iter != NULL && *error == 0) {
    if (iter->value >= 0) {
      stack_dbl_push(st_dbl, iter->value);
    } else {
      char c = (int)round(-iter->value);
      if (strchr("+-*/%^", c) != NULL && st_dbl->count >= 2) {
        double a = stack_dbl_pop(st_dbl);
        double b = stack_dbl_pop(st_dbl);
        stack_dbl_push(st_dbl, math_calculate(a, b, c));
      } else if (c == 'x') {
        stack_dbl_push(st_dbl, x);
      } else if (strchr("sicotnqlg~?", c) != NULL && st_dbl->count >= 1) {
        double a = stack_dbl_pop(st_dbl);
        stack_dbl_push(st_dbl, func_calculate(a, c));
      } else {
        *error = 2;
      }
    }
    iter = iter->next;
  }
  res = stack_dbl_pop(st_dbl);
  stack_dbl_destroy(st_dbl);
  return res;
}

int priority(char c) {
  int res = -1;
  switch (c) {
    case '(':
      res = 0;
      break;
    case ')':
      res = 0;
      break;
    case '+':
      res = 1;
      break;
    case '-':
      res = 1;
      break;
    case '*':
      res = 2;
      break;
    case '/':
      res = 2;
      break;
    case '%':
      res = 2;
      break;
    case '~':
      res = 3;
      break;
    case '?':
      res = 3;
      break;
    case '^':
      res = 4;
      break;
    default:
      break;
  }
  return res;
}

double func_calculate(double a, char c) {
  double r = 0;
  switch (c) {
    case 's':
      r = sin(a);
      break;
    case 'i':
      r = asin(a);
      break;
    case 'c':
      r = cos(a);
      break;
    case 'o':
      r = acos(a);
      break;
    case 't':
      r = tan(a);
      break;
    case 'n':
      r = atan(a);
      break;
    case 'q':
      r = sqrt(a);
      break;
    case 'l':
      r = log(a);
      break;
    case 'g':
      r = log10(a);
      break;
    case '~':
      r = -a;
      break;
    case '?':
      r = a;
      break;
  }
  return r;
}

double math_calculate(double a, double b, char c) {
  double r = 0;
  switch (c) {
    case '+':
      r = b + a;
      break;
    case '-':
      r = b - a;
      break;
    case '*':
      r = b * a;
      break;
    case '^':
      r = pow(b, a);
      break;
    case '/':
      r = b / a;
      break;
    case '%':
      r = fmod(b, a);
      break;
  }
  return r;
}
