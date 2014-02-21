#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct test_struct {
  char *firstName, *lastName, *telNumber;
  struct test_struct *next;
};

struct test_struct *head = NULL;
struct test_struct *curr = NULL;

/**
 * [create_list description]
 * @param  firstName [description]
 * @return     [description]
 */
struct test_struct* create_list(char *firstName, char *lastName, char *telNumber) {
  printf("\nCreating contact list with entry: %s %s -> %s\n\n", firstName, lastName, telNumber);
  struct test_struct *ptr = (struct test_struct*)malloc(sizeof(struct test_struct));

  if (ptr == NULL) {
    printf("\nNode creation failed \n");
    return NULL;
  }

  ptr->firstName = malloc(sizeof(firstName));
  ptr->lastName = malloc(sizeof(lastName));
  ptr->telNumber = malloc(sizeof(telNumber));

  strcpy(ptr->firstName, firstName);
  strcpy(ptr->lastName, lastName);
  strcpy(ptr->telNumber, telNumber);
  ptr->next = NULL;

  head = curr = ptr;
  return ptr;
}

/**
 * [add_to_list description]
 * @param  firstName  [description]
 * @param  add_to_end [description]
 * @return            [description]
 */
struct test_struct* add_to_list(char *firstName, char *lastName, char *telNumber) {
  if (head == NULL) {
    return create_list(firstName, lastName, telNumber);
  }

  printf("\nAdding contact: %s %s -> %s\n\n", firstName, lastName, telNumber);

  struct test_struct *ptr = (struct test_struct*)malloc(sizeof(struct test_struct));

  if (ptr == NULL) {
    printf("\nNode creation failed \n");
    return NULL;
  }

  ptr->firstName = malloc(sizeof(firstName));
  ptr->lastName = malloc(sizeof(lastName));
  ptr->telNumber = malloc(sizeof(telNumber));

  strcpy(ptr->firstName, firstName);
  strcpy(ptr->lastName, lastName);
  strcpy(ptr->telNumber, telNumber);
  ptr->next = NULL;

  curr->next = ptr;
  curr = ptr;
  return ptr;
}

/**
 * [search_in_list description]
 * @param  firstName  [description]
 * @param  prev [description]
 * @return      [description]
 */
struct test_struct* search_in_list(char *firstName, char *lastName, struct test_struct **prev) {
  struct test_struct *ptr = head;
  struct test_struct *tmp = NULL;
  bool found = false;

  printf("\nSearching the list for: %s %s\n", firstName, lastName);

  while (ptr != NULL) {
    if (ptr->firstName == firstName && ptr->lastName == lastName) {
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
 * @param  firstName [description]
 * @return     [description]
 */
int delete_from_list(char *firstName, char *lastName) {
  struct test_struct *prev = NULL;
  struct test_struct *del = NULL;

  printf("\nDeleting: %s %s\n", firstName, lastName);

  del = search_in_list(firstName, lastName, &prev);
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
  int i = 0;

  printf("\n-------Printing List Start-------\n\n");
  while (ptr != NULL) {
    printf("%s %s -> %s\n", ptr->firstName, ptr->lastName, ptr->telNumber);
    ptr = ptr->next;
    i++;
  }
  printf("\nTotal Contacts: %d\n", i);
  printf("\n-------Printing List End-------\n\n");

  return;
}

/**
 * [main description]
 * @return  [description]
 */
int main(void) {
  int i = 0, choice = 0;
  char input[3];
  char name_str[70];
  char num_str[10];

  char *fName = (char *)malloc(sizeof(char *));
  char *lName = (char *)malloc(sizeof(char *));
  char *tel = (char *)malloc(sizeof(char *));
  struct test_struct *ptr = NULL;

  printf("Contact List\n");

  do {
    printf("What would you like to do?\n");
    printf("1. Add entry\n");
    printf("2. Edit entry\n");
    printf("3. Delete entry\n");
    printf("4. Search list\n");
    printf("5. Display List\n");
    printf("0. Exit\n");
    printf("\nEnter 1-5 or 0 to exit: ");
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &choice);

    switch(choice) {
      case 1:
        printf("\nContact Name: ");
        fgets(name_str, sizeof(name_str), stdin);
        sscanf(name_str, "%s %s", fName, lName);

        printf("Telephone #(XXX-XXXX): ");
        fgets(num_str, sizeof(num_str), stdin);
        sscanf(num_str, "%s", tel);
        add_to_list(fName, lName, tel);
        break;
      case 2:
        // TODO: Edit entry
        break;
      case 3:
        // TODO: Delete entry
        break;
      case 4:
        search_in_list(fName, lName, NULL);
        break;
      case 5:
        // TODO: Sorting options?
        print_list();
        break;
      case 0:
        printf("Exiting program\n");
        exit(0);
      default:
        printf("Please enter a valid choice\n");
        break;
    }
  } while (true);

  return 0;
}