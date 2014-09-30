/* File:     prog2.c
 *
 * Author:   Mark Yan
 *
 * Purpose:  Implement an sorted linked list with ops insert,
 *           print, member, delete, free_list. Items in the list are stored
 *           in alphabetical order.
 * 
 * Input:    Single character lower case letters to indicate operators, 
 *           followed by arguments needed by operators.
 * Output:   Results of operations.
 *
 * Compile:  gcc -g -Wall -o prog2 prog2.c
 * Run:      ./prog2
 *
 * Notes:
 *    1.  Repeated values are not allowed in the list (will print a
 *        message if the input value is already in the list)
 *    2.  Program assumes a char or string will be entered when prompted
 *        for one.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list_node_s {
   char*    data;
   struct list_node_s* pred_p;
   struct list_node_s* next_p;
};

struct list_s {
   struct list_node_s* h_p;
   struct list_node_s* t_p;
};

int  Member(struct list_s* list_p, char* val);
void Insert(struct list_s* list_p, char* val);
void Delete(struct list_s* list_p, char* val);
void Print(struct list_s* list_p);
void Free_list(struct list_s* list_p); 
char Get_command(void);
void Get_value(char* dest);
struct list_node_s* Allocate_node(int size);
void Free_node(struct list_node_s* node_p);

/*-----------------------------------------------------------------*/
int main(void) {
   char command;
   char value[99]; /* stores the input */

   /* start with empty list */
   struct list_s list;
   list.h_p = NULL;
   list.t_p = NULL;

   command = Get_command();
   while (command != 'q' && command != 'Q') {
      switch (command) {
         case 'i': 
         case 'I': 
            Get_value(value);
            Insert(&list, value);
            break;
         case 'p':
         case 'P':
            Print(&list);
            break;
         case 'm': 
         case 'M':
            Get_value(value);
            if (Member(&list, value))
               printf("%s is in the list\n", value);
            else
               printf("%s is not in the list\n", value);
            break;
         case 'd':
         case 'D':
            Get_value(value);
            Delete(&list, value);
            break;
         case 'f':
         case 'F':
            Free_list(&list);
            break;
         default:
            printf("There is no %c command\n", command);
            printf("Please try again\n");
      }
      command = Get_command();
   }

   Free_list(&list);

   return 0;
}  /* main */


/*-----------------------------------------------------------------
 * Function:    Member
 * Purpose:     search list for val
 * Input args:  list_p:  pointer the list
 *              val:     value to search for
 * Return val:  1 if val is in list, 0 otherwise
 */
int Member(struct list_s* list_p, char* val) {
   struct list_node_s* curr_p = list_p->h_p;

   while (curr_p != NULL) {
      if (strcmp(curr_p->data, val) == 0)
         return 1;
      else
         curr_p = curr_p->next_p;
   }
   return 0;
}  /* Member */


/*-----------------------------------------------------------------
 * Function:   Delete
 * Purpose:    Delete val from list
 * Input args: list_p: pointer to the list
 *             val:    value to be deleted
 */
void Delete(struct list_s* list_p, char* val) {
   struct list_node_s* curr_p = list_p->h_p;

   /* Find node containing val */
   while (curr_p != NULL) {
      if (strcmp(curr_p->data, val) == 0) 
         break;
      else /* curr_p->data != val */
         curr_p = curr_p->next_p;
   }

   if (curr_p == NULL)
      printf("%s is not in the list\n", val);
   else { /* Delete the node */
      if (curr_p->pred_p == NULL) /* first in the list */
         list_p->h_p = curr_p->next_p;
      if (curr_p->next_p == NULL) /* last in the list */
         list_p->t_p = curr_p->pred_p;
      if (curr_p->next_p != NULL) /* has successor */
         curr_p->next_p->pred_p = curr_p->pred_p;
      if (curr_p->pred_p != NULL) /* has predecessor */
         curr_p->pred_p->next_p = curr_p->next_p;
      Free_node(curr_p);
   }
}  /* Delete */


/*-----------------------------------------------------------------
 * Function:   Insert
 * Purpose:    Insert val in the correct position in the list
 *             according to alphabetical order, or
 *             Print a message if val is already in the list
 * Input args: list_p: pointer to the list
 *             val:    new value to be inserted
 */
void Insert(struct list_s* list_p, char* val) {
   struct list_node_s* temp_p;

   /* Initialize temp_p */
   temp_p = Allocate_node(strlen(val));
   strcpy(temp_p->data, val);
   
   if (list_p->h_p == NULL){ /* list is empty */
      list_p->h_p = temp_p;
      list_p->t_p = temp_p;
   } else {
      struct list_node_s* curr_p = list_p->h_p;
      while (curr_p != NULL) {
         if (strcmp(curr_p->data, val) == 0){
            printf("%s is already in the list\n", val);
            return;
         }
         else if (strcmp(curr_p->data, val) > 0)
            break;
         curr_p = curr_p->next_p;
      }

      if (curr_p != NULL){
         temp_p->pred_p = curr_p->pred_p;
         temp_p->next_p = curr_p;
         if (curr_p->pred_p == NULL) { /* first in the list */
            list_p->h_p = temp_p;
            curr_p->pred_p = temp_p;
         }
         else {
            curr_p->pred_p->next_p = temp_p;
            curr_p->pred_p = temp_p;
         }
      } else { /* val is the "biggest" word */
         temp_p->pred_p = list_p->t_p; /* attach to the end */
         list_p->t_p->next_p = temp_p;
         list_p->t_p = temp_p;
      }
   }
}   /* Insert */

/*-----------------------------------------------------------------
 * Function:   Print
 * Purpose:    Print list on a single line of stdout
 * Input arg:  list_p : pointer to the list
 */
void Print(struct list_s* list_p) {
   struct list_node_s* curr_p = list_p->h_p;

   printf("list = ");
   while (curr_p != NULL) {
      printf("%s ", curr_p->data);
      curr_p = curr_p->next_p;
   }  
   printf("\n");
}  /* Print */


/*-----------------------------------------------------------------
 * Function:    Free_list
 * Purpose:     free each node in the list
 * Input arg:   list_p:  pointer to the list
 */
void Free_list(struct list_s* list_p) {
   struct list_node_s* curr_p;
   struct list_node_s* temp_p;

   curr_p = list_p->h_p;
   while (curr_p != NULL) {
      temp_p = curr_p;
      curr_p = curr_p->next_p;
      Free_node(temp_p);     
   }

   list_p->h_p = NULL;
   list_p->t_p = NULL;
}  /* Free_list */

/*-----------------------------------------------------------------
 * Function:      Get_command
 * Purpose:       Get a single character command from stdin
 * Return value:  the first non-whitespace character from stdin
 */
char Get_command(void) {
   char c;

   printf("Please enter a command (i, p, m, d, f, q):  ");
   /* Put the space before the %c so scanf will skip white space */
   scanf(" %c", &c);
   return c;
}  /* Get_command */

/*-----------------------------------------------------------------
 * Function:   Get_value
 * Purpose:    Get string from stdin and assign the value to dest
 * Input arg:  dest: pointer to the string to store the input in
 * Note:     Behavior unpredictable if an char/string isn't entered
 */
void Get_value(char* dest) {
   printf("Please enter a value:  ");
   scanf("%s", dest);
}  /* Get_value */

/*-----------------------------------------------------------------
 * Function:      Allocate_node
 * Purpose:       Allocate a node
 * Return value:  Pointer to the allocated node
 */
struct list_node_s* Allocate_node(int size){
   struct list_node_s* temp_p;

   temp_p = malloc(sizeof(struct list_node_s));
   temp_p->data = malloc((size+1)*sizeof(char));
   temp_p->next_p = NULL;
   temp_p->pred_p = NULL;
   return temp_p;
}

/*-----------------------------------------------------------------
 * Function:      Free_node
 * Purpose:       Free a node
 */
void Free_node(struct list_node_s* node_p){
   free(node_p->data);
   free(node_p);
}