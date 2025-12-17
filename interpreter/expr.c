
#include "expr.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct SymbolMap *symbol_table = NULL;

/*
Create one node in an expression tree and return the structure.
*/

struct type *type_create(type_t kind, struct type *sub) {
  struct type *t = malloc(sizeof(*t));
  t->kind = kind;
  t->subtype = sub;
  return t;
}

struct decl *decl_create(struct expr *name, struct type *type,
                         struct expr *value, struct stmt *code,
                         struct decl *next) {
  struct decl *d = malloc(sizeof(*d));
  d->name = name;
  d->type = type;
  d->value = value;
  d->code = code;
  d->next = next;
  return d;
}

struct stmt *stmt_create(stmt_t kind, struct decl *decl, struct expr *init_expr,
                         struct expr *expr, struct expr *next_expr,
                         struct stmt *body, struct stmt *else_body,
                         struct stmt *next) {
  struct stmt *s = malloc(sizeof(*s));

  s->kind = kind;
  s->decl = decl;
  s->init_expr = init_expr;
  s->expr = expr;
  s->next_expr = next_expr;
  s->body = body;
  s->else_body = else_body;
  s->next = next;

  return s;
}

struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right) {
  /* Shortcut: sizeof(*e) means "the size of what e points to" */
  struct expr *e = malloc(sizeof(*e));

  e->kind = kind;
  e->left = left;
  e->right = right;
  e->name = "";
  e->integer_value = 0;
  e->float_value = 0.0;
  e->string_literal = "";
  return e;
}

struct expr *expr_create_name(const char *value) {
  struct expr *e = expr_create(EXPR_NAME, 0, 0);
  char *dest = malloc(strlen(value) + 1);
  strcpy(dest, value); // copy contents of source to dest
  e->name = dest;
  return e;
}

struct expr *expr_create_integer_literal(int value) {
  struct expr *e = malloc(sizeof(*e));
  e->kind = EXPR_INTEGER_LITERAL;
  e->integer_value = value;
  e->left = 0;
  e->right = 0;
  e->name = 0;
  e->string_literal = 0;
  return e;
}

struct expr *expr_create_float_literal(float value) {
  struct expr *e = malloc(sizeof(*e));
  e->kind = EXPR_FLOAT_LITERAL;
  e->float_value = value;
  e->left = 0;
  e->right = 0;
  e->name = 0;
  e->string_literal = 0;
  return e;
}

struct expr *expr_create_string_literal(const char *value) {
  struct expr *e = expr_create(EXPR_STRING_LITERAL, 0, 0);
  if (!value) {
    e->string_literal = strdup("");
    return e;
  }
  size_t n = strlen(value);
  if (n >= 2 && value[0] == '"' && value[n - 1] == '"') {
    char *buf = malloc(n - 1); // n-2 chars + '\0'
    memcpy(buf, value + 1, n - 2);
    buf[n - 2] = '\0';
    e->string_literal = buf;
  } else {
    e->string_literal = strdup(value);
  }
  return e;
}

/*
Recursively delete an expression tree.
*/

void expr_delete(struct expr *e) {
  /* Careful: Stop on null pointer. */
  if (!e) {
    return;
  }
  expr_delete(e->left);
  expr_delete(e->right);
  free(e);
}

void scope_bind(const char *name, struct expr *value) {
  if (symbol_table == NULL) {
    symbol_table = createSymbolMap();
  }
  insertSymbol(symbol_table, name, value);
}

struct expr *scope_lookup(const char *name) {
  return getSymbol(symbol_table, name);
}

/*
Recursively print an expression tree by performing an
in-order traversal of the tree, printing the current node
between the left and right nodes.
*/

void stmt_print(struct stmt *s) {
  if (!s)
    return;

  switch (s->kind) {
    case STMT_DECL:
      decl_print(s->decl);
      printf(";");
      break;

    case STMT_EXPR:
      expr_print(s->expr);
      break;

    case STMT_IF:
      printf("if ");
      expr_print(s->expr);
      printf(" {\n");
      stmt_print(s->body);
      printf("\n}");
      break;

    case STMT_FOR:
      printf("for ");
      decl_print(s->decl);
      printf(" in ");
      expr_print(s->expr);
      printf(" {\n");
      stmt_print(s->body);
      printf("\n}");
      break;

    case STMT_WHILE:
      printf("while ");
      expr_print(s->expr);
      printf(" {\n");
      stmt_print(s->body);
      printf("\n}");
      break;

    case STMT_PRINT:
      if (s->expr && s->expr->kind == EXPR_ARG_LIST) {
        printf("println!(\"");
        expr_print(s->expr->left); // prints "{}"
        printf("\", ");
        expr_print(s->expr->right); // prints variable
        printf(");");
      } else {
        printf("println!(\"");
        expr_print(s->expr);
        printf("\");");
      }
      break;

    case STMT_BLOCK:
      stmt_print(s->body);
      break;

    case STMT_ASSIGN:
      expr_print(s->expr);
      printf(";");
      break;
  }

  if (s->next) {
    printf("\n");
  }
  stmt_print(s->next);
}

void decl_print(struct decl *d) {
  printf("let ");
  if (d->type && d->type->kind == TYPE_MUT) {
    printf("mut ");
  } 
  expr_print(d->name);
  if (d->type && d->type->kind == TYPE_INTEGER) {
    printf(": i32");
  }
  printf(" = ");
  expr_print(d->value);
}

void expr_print(struct expr *e) {
  /* Careful: Stop on null pointer. */
  if (!e) {
    return;
  }

  const char *close = "";
  if (e->kind == EXPR_ARRAY) {
    printf("[");
    close = "]";
  }

  expr_print(e->left);

  switch (e->kind) {
    case EXPR_NAME:
      printf("%s", e->name);
      break;

    case EXPR_ARRAY:
      if (e->right) {
        printf(", ");
      }
      break;

    case EXPR_SUBSCRIPT:
      printf("[");
      close = "]";
      break;

    case EXPR_ADD:
      printf(" + ");
      break;

    case EXPR_SUBTRACT:
      printf(" - ");
      break;

    case EXPR_MULTIPLY:
      printf(" * ");
      break;

    case EXPR_DIVIDE:
      printf(" / ");
      break;

    case EXPR_EQUAL_EQUAL:
      printf(" == ");
      break;

    case EXPR_NOT_EQUAL:
      printf(" != ");
      break;

    case EXPR_GREATER_THAN:
      printf(" > ");
      break;

    case EXPR_GREATER_THAN_OR_EQUAL:
      printf(" >= ");
      break;

    case EXPR_LESS_THAN:
      printf(" < ");
      break;

    case EXPR_ASSIGN:
      printf(" = ");
      break;

    case EXPR_LESS_THAN_OR_EQUAL:
      printf(" <= ");
      break;

    case EXPR_INTEGER_LITERAL:
      printf("%d", e->integer_value);
      break;

    case EXPR_FLOAT_LITERAL:
      printf("%f", e->float_value);
      break;

    case EXPR_STRING_LITERAL:
      printf("%s", e->string_literal);
      break;

    case EXPR_ARG_LIST:
      if (e->right) {
        printf(", ");
        break;
      }

    case EXPR_LEN:
      printf(".len()");
      break;

    case EXPR_ISQRT:
      printf(".isqrt()");
      break;

    case EXPR_RANGE:
      printf("..");
      break;
  }

  expr_print(e->right);

    // Options for closing
  printf("%s", close);
}

  /*
  Recursively evaluate an expression by performing
  the desired operation and returning it up the tree.
  */

  // helper for array printing
static void print_array_elements(struct expr *e) {
  if (!e) return;

  if (e->kind == EXPR_ARG_LIST) {
    print_array_elements(e->left);
    if (e->right) {
      printf(", ");
      print_array_elements(e->right);
    }
    } else if (e->kind == EXPR_INTEGER_LITERAL) {
        printf("%d", e->integer_value);
    } else if (e->kind == EXPR_FLOAT_LITERAL) {
        printf("%.0f", e->float_value);
    } else if (e->kind == EXPR_NAME) {
        printf("%s", e->name);
    } else {
        printf("?");
    }
}

void stmt_evaluate(struct stmt *s) {
  if (!s) {
    return;
  }

  switch (s->kind) {
    case STMT_DECL:
      decl_evaluate(s->decl);
      break;

    case STMT_EXPR:
      expr_evaluate(s->expr);
      break;

    case STMT_IF:
      if (expr_evaluate(s->expr)) {
        stmt_evaluate(s->body);
      }
      break;

    case STMT_WHILE:
      while (expr_evaluate(s->expr)) {
        stmt_evaluate(s->body);
      }
      break;

    case STMT_FOR:
      if (!s->expr || s->expr->kind != EXPR_RANGE) {
        printf("runtime error: invalid range in for loop\n");
        exit(1);
      }
      float start = expr_evaluate(s->expr->left);
      float end = expr_evaluate(s->expr->right);

      for (float i = start; i < end; i++) {
        struct expr *val = expr_create_float_literal(i);
        if (s->decl && s->decl->name && s->decl->name->name) {
          scope_bind(s->decl->name->name, val);
        }

        stmt_evaluate(s->body);
        printf("\n");
        // Always print newline per iteration
        printf("\n");
      }
      break;

    case STMT_ASSIGN:
      expr_evaluate(s->expr);
      break;

    case STMT_PRINT:
      struct expr *args = s->expr;

      /* Handle println!("{:?}", var) for arrays */
      if (args && args->kind == EXPR_ARG_LIST && args->left &&
      args->left->kind == EXPR_STRING_LITERAL &&
      strstr(args->left->string_literal, "{:?}")) {
        const char *fmt = expr_string_evaluate(args->left);
        const char *brace = strstr(fmt, "{:?}");
        if (brace) {
          fwrite(fmt, 1, (size_t)(brace - fmt), stdout);
        }
        if (args->right && args->right->kind == EXPR_NAME) {
          struct expr *arr = scope_lookup(args->right->name);
          if (arr && arr->kind == EXPR_ARRAY) {
            printf("[");
            print_array_elements(arr->left);
            printf("]");
          } else {
            printf("[]");
          }
        } 

        if (brace && brace[4] != '\0') {
          fputs(brace + 4, stdout);
        }

        printf("\n");
        break;
      }

      /* Handle println!("{}", x) and println!("text") */
      if (args && args->kind == EXPR_ARG_LIST && args->left &&
        args->left->kind == EXPR_STRING_LITERAL) {
        const char *fmt = expr_string_evaluate(args->left);
        const char *p = strstr(fmt, "{}");

      if (!p) {
        fputs(fmt, stdout);
      } else {
          fwrite(fmt, 1, (size_t)(p - fmt), stdout);
          float v = expr_evaluate(args->right);
          if ((int)v == v) {
            printf("%.0f", v);
          } else {
              printf("%g", v);
          }
          fputs(p + 2, stdout);
        }
      } else if (args && args->kind == EXPR_STRING_LITERAL) {
          const char *sval = expr_string_evaluate(args);
          fputs(sval, stdout);
      } else {
          float v = expr_evaluate(args);
          if ((int)v == v) {
            printf("%.0f", v);
          } else {
            printf("%g", v);
        }
      }

      if (s->next) {
        printf("\n");
    }
    break;

    case STMT_BLOCK:
      stmt_evaluate(s->body);
      break;
  }
stmt_evaluate(s->next);
}


void decl_evaluate(struct decl *d) {
  if (!d) {
    return;
  }

  if (!d->name || d->name->kind != EXPR_NAME) {
    printf("runtime error: declaration missing valid name\n");
    exit(1);
  }

  float value = 0;
  struct expr *evaluated = NULL;

  if (d->value) {
    if (d->value->kind == EXPR_ARRAY) {
      /* store array directly instead of evaluating */
      evaluated = d->value;
    } else {
        value = expr_evaluate(d->value);
        evaluated = expr_create_float_literal(value);
      }
  } else {
      evaluated = expr_create_float_literal(0);
    }
  /* Bind variable name to evaluated expression */
  scope_bind(d->name->name, evaluated);
}

void decl_subscript_evaluate(struct expr *e, float value) {
  /* Careful: Return zero on null pointer. */
  if (!e) {
    return;
  }

  if (e->kind != EXPR_SUBSCRIPT) {
    printf("runtime error: not a subscript expression\n");
    exit(1);
  }

  if (e->left->kind != EXPR_NAME) {
    printf("runtime error: subscript has no name\n");
    exit(1);
  }

  // Get array expresion
  struct expr *a = scope_lookup(e->left->name);
  float index = expr_evaluate(e->right);

  // Find the right offset
  while (index > 0) {
    a = a->right;
    index--;
  }
  expr_delete(a->left);
  a->left = expr_create_float_literal(value);
}

const char *expr_string_evaluate(struct expr *e) {
  /* Careful: Return zero on null pointer. */
  if (!e) {
    return 0;
  }

  switch (e->kind) {
    case EXPR_NAME:
    case EXPR_ARRAY:
    case EXPR_SUBSCRIPT:
    case EXPR_ADD:
    case EXPR_SUBTRACT:
    case EXPR_MULTIPLY:
    case EXPR_DIVIDE:
    case EXPR_EQUAL_EQUAL:
    case EXPR_NOT_EQUAL:
    case EXPR_GREATER_THAN:
    case EXPR_GREATER_THAN_OR_EQUAL:
    case EXPR_LESS_THAN:
    case EXPR_LESS_THAN_OR_EQUAL:
    case EXPR_INTEGER_LITERAL:
    case EXPR_FLOAT_LITERAL:
      printf("runtime error: not a string expression\n");
      exit(1);
    case EXPR_STRING_LITERAL:
      return e->string_literal;
  }

  return "";
}

struct expr *expr_sub_evaluate(struct expr *e) {
  if (!e) {
    return 0;
  }
  if (e->kind != EXPR_ARRAY) {
    printf("runtime error: not an array literal\n");
    exit(1);
  }
  for (struct expr *node = e; node; node = node->right) {
    float v = expr_evaluate(node->left);
    expr_delete(node->left);
    node->left = expr_create_float_literal(v);
  }
  return e;
}

/*
Recursively evaluate an expression by performing
the desired operation and returning it up the tree.
*/

float expr_subscript_evaluate(struct expr *e) {
  /* Careful: Return zero on null pointer. */
  if (!e) {
    return 0;
  }

  if (e->kind != EXPR_SUBSCRIPT) {
    printf("runtime error: not a subscript expression\n");
    exit(1);
  }

  if (e->left->kind != EXPR_NAME) {
    printf("runtime error: subscript has no name\n");
    exit(1);
  }

  // Get array expresion
  struct expr *a = scope_lookup(e->left->name);
  float index = expr_evaluate(e->right);

  // Find the right offset
  while (index > 0) {
    a = a->right;
    index--;
  }
  return expr_evaluate(a->left);
}

static float expr_len_evaluate(struct expr *e) {
  if (!e) {
    return 0;
  }

  if (e->kind == EXPR_ARRAY) {
    int n = 0;
    for (struct expr *p = e->left; p; p = p->right) {
      n++;
    }
    return (float)n;
  }

  if (e->kind == EXPR_STRING_LITERAL && e->string_literal) {
    return (float)strlen(e->string_literal);
  }

  return 0;
}

float expr_evaluate(struct expr *e) {
  /* Careful: Return zero on null pointer. */
  if (!e) {
    return 0;
  }

  fflush(stderr);

  if (e->kind == EXPR_SUBSCRIPT) {
    return expr_subscript_evaluate(e);
  }

  float l = expr_evaluate(e->left);
  float r = expr_evaluate(e->right);
  switch (e->kind) {
    case EXPR_NAME:
      struct expr *bound = scope_lookup(e->name);
      if (!bound) {
        fprintf(stderr, "runtime error: undefined variable '%s'\n", e->name);
        exit(1);
      }

      /* If variable is an array, return 0 when used in numeric context */
      if (bound->kind == EXPR_ARRAY) {
        return 0;
      }

      return expr_evaluate(bound);

    case EXPR_ARRAY:
      printf("runtime error: array in expression\n");
      fflush(stderr);
      exit(1);

    case EXPR_SUBSCRIPT:
      printf("runtime error: subscript should be processed earlier in expression\n");
      fflush(stderr);
      exit(1);

    case EXPR_ADD:
      return l + r;

    case EXPR_SUBTRACT:
      return l - r;

    case EXPR_MULTIPLY:
      return l * r;

    case EXPR_DIVIDE:
      if (r == 0) {
        printf("runtime error: divide by zero\n");
        fflush(stderr);
        exit(1);
      }
      return l / r;

    case EXPR_EQUAL_EQUAL:
      return l == r;

    case EXPR_NOT_EQUAL:
      return l != r;

    case EXPR_GREATER_THAN:
      return l > r;

    case EXPR_GREATER_THAN_OR_EQUAL:
      return l >= r;

    case EXPR_LESS_THAN:
      return l < r;

    case EXPR_ASSIGN:
      if (e->left->kind == EXPR_NAME) {
        float value = expr_evaluate(e->right);
        struct expr *val_expr = expr_create_float_literal(value);
        scope_bind(e->left->name, val_expr);
        return value;
      } else if (e->left->kind == EXPR_SUBSCRIPT) {
        float value = expr_evaluate(e->right);
        decl_subscript_evaluate(e->left, value);
        return value;
      } else {
        printf("runtime error: invalid assignment target\n");
        fflush(stderr);
        exit(1);
      }

    case EXPR_ISQRT:
      return sqrt(expr_evaluate(e->left));

    case EXPR_LEN:
      return expr_len_evaluate(e->left); // or handle arrays/strings directly

    case EXPR_LESS_THAN_OR_EQUAL:
      return l <= r;

    case EXPR_INTEGER_LITERAL:
      return e->integer_value;

    case EXPR_FLOAT_LITERAL:
      return e->float_value;

    case EXPR_STRING_LITERAL:
      return 0;

      fflush(stderr);
      exit(1);
  }

  return 0;
}

void close_parser() {
  if (symbol_table) {
    destroySymbolMap(symbol_table);
    symbol_table = NULL;
  }
}
