#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


enum
{
  INTERSECT_OPERATOR = '&',
  UNION_OPERATION = '|',
  SYMMDIFF_OPERATION = '^',
  ADDITION_OPERATION = '~'
};


bool
isoperation (char value);


char
decode (uint8_t code);

uint8_t
encode (char value);


struct BinOperation
{
  uint64_t lhs, rhs;
};

typedef struct BinOperation bin_operation_t;


bin_operation_t *
get_empty_expression ();

bool
handle_request (bin_operation_t *expression, char value);

void
push_value (bin_operation_t *expression, char value);

void
evaluate (bin_operation_t *expression, char operation);

void
compute_intersection (bin_operation_t *expression);

void
compute_union (bin_operation_t *expression);

void
compute_symmetric_difference (bin_operation_t *expression);

void
compute_full_addition (bin_operation_t *expression);

void
show_result_bitset (bin_operation_t *expression);


void
execute (bin_operation_t *expr);


int
main ()
{
  bin_operation_t *expr = get_empty_expression();
  execute(expr);
  free(expr);
  return 0;
}


void
execute (bin_operation_t *expr)
{
  char input;
  while (true)
  {
    input = (char) getchar();
    if (!handle_request(expr, input))
    {
      break;
    }
  }
  show_result_bitset(expr);
}


bool
isoperation (char value)
{
  if (value == INTERSECT_OPERATOR
      || value == UNION_OPERATION
      || value == SYMMDIFF_OPERATION
      || value == ADDITION_OPERATION)
  {
    return true;
  }
  else
  {
    return false;
  }
}


char
decode (uint8_t code)
{
  if (code >= 0 && code <= 9)
  {
    return (char) ((uint8_t) '0' + code);
  }
  else if (code >= 10 && code <= 35)
  {
    return (char) ((uint8_t) 'A' - 10 + code);
  }
  else if (code >= 36 && code <= 61)
  {
    return (char) ((uint8_t) 'a' - 36 + code);
  }
  else
  {
    return 0;
  }
}

uint8_t
encode (char value)
{
  if (isalpha(value))
  {
    if (isupper(value))
    {
      return (uint8_t) (value - (uint8_t) 'A' + 10);
    }
    else
    {
      return (uint8_t) (value - (uint8_t) 'a' + 36);
    }
  }
  else if (isdigit(value))
  {
    return (uint8_t) (value - (uint8_t) '0');
  }
  else
  {
    return ~(uint8_t) 0;
  }
}

bin_operation_t *
get_empty_expression ()
{
  bin_operation_t *expression =
      (bin_operation_t *) malloc(sizeof(bin_operation_t));
  expression->rhs = expression->lhs = 0;
  return expression;
}

bool
handle_request (bin_operation_t *expression, char value)
{
  if (isalpha(value) || isdigit(value))
  {
    push_value(expression, value);
  }
  else if (isoperation(value))
  {
    evaluate(expression, value);
  }
  else
  {
    return false;
  }
  return true;
}

void
push_value (bin_operation_t *expression, char value)
{
  expression->rhs |= ((uint64_t) 1 << encode(value));
}

void
evaluate (bin_operation_t *expression, char operation)
{
  bool clear_rhs = true;
  switch (operation)
  {
    case INTERSECT_OPERATOR:
      compute_intersection(expression);
      break;
    case UNION_OPERATION:
      compute_union(expression);
      break;
    case ADDITION_OPERATION:
      compute_full_addition(expression);
      clear_rhs = false;
      break;
    case SYMMDIFF_OPERATION:
      compute_symmetric_difference(expression);
      break;
    default:
      break;
  }
  if (clear_rhs)
  {
    expression->rhs = 0;
  }
}

void
compute_intersection (bin_operation_t *expression)
{
  expression->lhs &= expression->rhs;
}

void
compute_union (bin_operation_t *expression)
{
  expression->lhs |= expression->rhs;
}

void
compute_symmetric_difference (bin_operation_t *expression)
{
  expression->lhs ^= expression->rhs;
}

void
compute_full_addition (bin_operation_t *expression)
{
  expression->lhs = ~expression->lhs;
}

void
show_result_bitset (bin_operation_t *expression)
{
  uint64_t bitset = (expression->lhs == 0)
                    ? expression->rhs
                    : expression->lhs;
  for (uint8_t code = 0; code < 62; code++)
  {
    if ((bitset >> code) & (uint64_t) 1)
    {
      printf("%c", decode(code));
    }
  }
}