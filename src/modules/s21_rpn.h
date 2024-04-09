#ifndef S21_RPN_H
#define S21_RPN_H

#define SYMBOLS "1234567890xsicotnqlg+-*/^%?()."

struct stack_node_dbl {
  double value;
  struct stack_node_dbl* next;
};

struct stack_dbl {
  struct stack_node_dbl* ptr;
  int count;
};

struct node_char {
  char c;
  struct node_char* next;
};

struct stack_char {
  struct node_char* ptr;
  int count;
};

struct node_list {
  double value;
  struct node_list* next;
};

struct stack_dbl* stack_dbl_init();
void stack_dbl_push(struct stack_dbl* st, double value);
double stack_dbl_pop(struct stack_dbl* st);
void stack_dbl_destroy(struct stack_dbl* st);

struct stack_char* stack_char_init();
void stack_char_push(struct stack_char* st, char c);
char stack_char_pop(struct stack_char* st);
void stack_char_destroy(struct stack_char* st);

struct node_list* list_init(double value);
void list_add_value(struct node_list* root, double value);
void list_destroy(struct node_list* root);

int prepare_string(char* str);
void find_replace(char* str, char* find, char replace);

struct node_list* rpn(const char* str);
double rpn_calculate(struct node_list* root, double x, int* error);
int priority(char c);
double func_calculate(double a, char c);
double math_calculate(double a, double b, char c);

#endif  // S21_RPN_H
