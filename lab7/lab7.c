#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUTS 1000

typedef struct {
  int line_number;
  int value;
} Input;

typedef struct {
  int line_number;
  int value;
} IntermediateInput;

typedef struct {
  int doubled_value;
  int line_numbers[MAX_INPUTS];
  int line_number_count;
} Output;

void map(Input *input, IntermediateInput *intermediate_input) {
  intermediate_input->line_number = input->line_number;
  intermediate_input->value = input->value * 2;
}

void groupByKey(IntermediateInput input, Output output[], int *result_count) {
  for (int i = 0; i < *result_count; i++) {
    if (output[i].doubled_value == input.value) {
      output[i].line_numbers[output[i].line_number_count] = input.line_number;
      output[i].line_number_count++;
      return;
    }
  }

  output[*result_count].doubled_value = input.value;
  output[*result_count].line_numbers[0] = input.line_number;
  output[*result_count].line_number_count = 1;
  (*result_count)++;
}

void reduce(Output output) {
  printf("(%d, [", output.doubled_value);

  for (int i = 0; i < output.line_number_count; i++) {
    if (i > 0) {
      printf(", ");
    }
    printf("%d", output.line_numbers[i]);
  }

  printf("])\n");
}

int main(void) {
  Input inputs[MAX_INPUTS];
  IntermediateInput intermediate_inputs[MAX_INPUTS];
  Output outputs[MAX_INPUTS];

  int input_count = 0;
  char buffer[128];

  printf("Enter values (one per line). Type 'end' to finish:\n");

  while (input_count < MAX_INPUTS &&
         fgets(buffer, sizeof(buffer), stdin) != NULL) {
    buffer[strcspn(buffer, "\n")] = '\0';

    if (strcmp(buffer, "end") == 0) {
      break;
    }

    inputs[input_count].line_number = input_count + 1;
    inputs[input_count].value = (int)strtol(buffer, NULL, 10);
    input_count++;
  }

  for (int i = 0; i < input_count; i++) {
    map(&inputs[i], &intermediate_inputs[i]);
  }

  int result_count = 0;
  for (int i = 0; i < input_count; i++) {
    groupByKey(intermediate_inputs[i], outputs, &result_count);
  }

  for (int i = 0; i < result_count; i++) {
    reduce(outputs[i]);
  }

  return 0;
}
