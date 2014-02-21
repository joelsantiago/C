#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct test_struct {
  int val;
  struct test_struct *next;
};

struct test_struct *head = NULL;
struct test_struct *curr = NULL;

/**
 * [create_list description]
 * @param  val [description]
 * @return     [description]
 */
struct test_struct* create_list(int val) {
  printf("\ncreating list with headnode as [%d]\n", val);
  struct test_struct *ptr = (struct test_struct*)malloc(sizeof(struct test_struct));

  if (ptr == NULL) {
    printf("\nNode creation failed \n");
    return NULL;
  }

  ptr->val = val;
  ptr->next = NULL;

  head = curr = ptr;
  return ptr;
}

/**
 * [add_to_list description]
 * @param  val        [description]
 * @param  add_to_end [description]
 * @return            [description]
 */
struct test_struct* add_to_list(int val, bool add_to_end) {
  if (head == NULL) {
    return create_list(val);
  }

  if (add_to_end)
    printf("\nAdding node to end of list with value [%d]\n", val);
  else
    printf("\nAdding node to beginning of list with value [%d]\n", val);

  struct test_struct *ptr = (struct test_struct*)malloc(sizeof(struct test_struct));

  if (ptr == NULL) {
    printf("\nNode creation failed \n");
    return NULL;
  }

  ptr->val = val;
  ptr->next = NULL;

  if (add_to_end) {
    curr->next = ptr;
    curr = ptr;
  }
  else {
    ptr->next = head;
    head = ptr;
  }
  return ptr;
}

/**
 * [search_in_list description]
 * @param  val  [description]
 * @param  prev [description]
 * @return      [description]
 */
struct test_struct* search_in_list(int val, struct test_struct **prev) {
  struct test_struct *ptr = head;
  struct test_struct *tmp = NULL;
  bool found = false;

  printf("\nSearching the list for value [%d]\n", val);

  while (ptr != NULL) {
    if (ptr->val == val) {
      found = true;
      break;
    }
    else {
      tmp = ptr;
      ptr = ptr->next;
    }
  }

  if (found == true) {
    if (prev) {
      *prev = tmp;
    }
    return ptr;
  }
  else {
    return NULL;
  }
}

/**
 * [delete_from_list description]
 * @param  val [description]
 * @return     [description]
 */
int delete_from_list(int val) {
  struct test_struct *prev = NULL;
  struct test_struct *del = NULL;

  printf("\nDeleting value [%d] from list\n", val);

  del = search_in_list(val, &prev);
  if (del == NULL) {
    return -1;
  }
  else {
    if (prev != NULL) {
      prev->next = del->next;
    }

    if (del == curr) {
      curr = prev;
    }
    else if (del == head) {
      head = del->next;
    }
  }

  free(del);
  del = NULL;

  return 0;
}

/**
 * [print_list description]
 */
void print_list(void) {
  struct test_struct *ptr = head;

  printf("\n-------Printing List Start------- \n");
  while (ptr != NULL) {
    printf("\n [%d] \n", ptr->val);
    ptr = ptr->next;
  }
  printf("\n-------Printing List End------- \n");

  return;
}

/**
 * [main description]
 * @return  [description]
 */
int main(void) {
  int i = 0, ret = 0;
  struct test_struct *ptr = NULL;

  print_list();

  for (i = 5; i < 10; i++) {
    add_to_list(i, true);
  }

  print_list();

  for (i = 4; i > 0; i--) {
    add_to_list(i, false);
  }

  print_list();

  for (i = 1; i < 10; i+=4) {
    ptr = search_in_list(i, NULL);
    if (NULL == ptr) {
      printf("\nSearch [val = %d] failed, no such element found\n", i);
    }
    else {
      printf("\nSearch passed [val = %d]\n", ptr->val);
    }

    print_list();

    ret = delete_from_list(i);
    if (ret != 0) {
      printf("\ndelete [val = %d] failed, no such element found \n", i);
    }
    else {
      printf("\ndelete [val = %d] passed \n", i);
    }

    print_list();
  }

  return 0;
}