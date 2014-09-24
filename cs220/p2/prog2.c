/* File:     linked_list.c
 *
 * Purpose:  Implement an unsorted linked list with ops insert (at head),
 *           print, member, delete, free_list.
 * 
 * Input:    Single character lower case letters to indicate operators, 
 *           followed by arguments needed by operators.
 * Output:   Results of operations.
 *
 * Compile:  gcc -g -Wall -o linked_list linked_list.c
 * Run:      ./linked_list
 *
 * Notes:
 *    1.  Repeated values are allowed in the list
 *    2.  delete only deletes the first occurrence of a value
 *    3.  Program assumes an int will be entered when prompted
 *        for one.
 */
#include <stdio.h>
#include <stdlib.h>

struct list_node_s {
   int    data;
   struct list_node_s* pred_p;
   struct list_node_s* next_p;
};

struct list_s {
   struct list_node_s* h_p;
   struct list_node_s* t_p;
};

int  Member(struct list_s* list_p, int val);
void Insert(struct list_s* list_p, int val);
void Delete(struct list_s* list_p, int val);
void Print(struct list_s* list_p);
void Free_list(struct list_s* list_p); 
char Get_command(void);
int  Get_value(void);

/*-----------------------------------------------------------------*/
int main(void) {
   char command;
   int  value;
   struct list_s list;
   list.h_p = NULL;
   list.t_p = NULL; 
      /* start with empty list */

   command = Get_command();
   while (command != 'q' && command != 'Q') {
      switch (command) {
         case 'i': 
         case 'I': 
            value = Get_value();
            Insert(&list, value);
            break;
         case 'p':
         case 'P':
            Print(&list);
            break;
         case 'm': 
         case 'M':
            value = Get_value();
            if (Member(&list, value))
               printf("%d is in the list\n", value);
            else
               printf("%d is not in the list\n", value);
            break;
         case 'd':
         case 'D':
            value = Get_value();
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
 * Input args:  head_p:  pointer to head of list
 *              val:  value to search for
 * Return val:  1 if val is in list, 0 otherwise
 */
int Member(struct list_s* list_p, int val) {
   struct list_node_s* curr_p = list_p->h_p;

   while (curr_p != NULL) {
      if (curr_p->data == val)
         return 1;
      else
         curr_p = curr_p->next_p;
   }
   return 0;
}  /* Member */


/*-----------------------------------------------------------------
 * Function:   Delete
 * Purpose:    Delete the first occurrence of val from list
 * Input args: head_p: pointer to the head of the list
 *             val:    value to be deleted
 * Return val: Possibly updated pointer to head of list
 */
void Delete(struct list_s* list_p, int val) {
   struct list_node_s* curr_p = list_p->h_p;

   /* Find node containing val */
   while (curr_p != NULL)
      if (curr_p->data == val) 
         break;
      else { // curr_p->data != val 
         curr_p = curr_p->next_p;
      }

   if (curr_p == NULL)
      printf("%d is not in the list\n", val);

   else {
      if (curr_p->pred_p == NULL) /* first in the list */
         list_p->h_p = curr_p->next_p;
      if (curr_p->next_p == NULL) /* last in the list */
         list_p->t_p = curr_p->pred_p;
      if (curr_p->next_p != NULL) /* has successor */
         curr_p->next_p->pred_p = curr_p->pred_p;
      if (curr_p->pred_p != NULL) /* has predecessor */
         curr_p->pred_p->next_p = curr_p->next_p;
      free(curr_p);
   }

   // return head_p;
}  /* Delete */


/*-----------------------------------------------------------------
 * Function:   Insert
 * Purpose:    Insert val at head of list
 * Input args: head_p: pointer to head of list
 *             val:  new value to be inserted
 * Return val: Pointer to head of list
 */
void Insert(struct list_s* list_p, int val) {
   struct list_node_s* temp_p;

   temp_p = malloc(sizeof(struct list_node_s));
   temp_p->data = val;

   /* initialize values of next_p and pred_p members */
   temp_p->next_p = NULL;
   temp_p->pred_p = NULL;
   
   if (list_p->h_p == NULL){
      /* list is empty */
      list_p->h_p = temp_p;
      list_p->t_p = temp_p;
   } else {
      temp_p->pred_p = list_p->t_p; /* !!! attach to the end */
      list_p->t_p->next_p = temp_p;
      list_p->t_p = temp_p;
   }
}   /* Insert */

/*-----------------------------------------------------------------
 * Function:   Print
 * Purpose:    Print list on a single line of stdout
 * Input arg:  head_p
 */
void Print(struct list_s* list_p) {
   struct list_node_s* curr_p = list_p->h_p;

   printf("list = ");
   while (curr_p != NULL) {
      printf("%d ", curr_p->data);
      curr_p = curr_p->next_p;
   }  
   printf("\n");
}  /* Print */


/*-----------------------------------------------------------------
 * Function:    Free_list
 * Purpose:     free each node in the list
 * Input arg:   head_p:  pointer to head of list
 * Return val:  NULL pointer
 * Note:        head_p is set to NULL on completion, indicating
 *              list is empty.
 */
void Free_list(struct list_s* list_p) {
   struct list_node_s* curr_p;
   struct list_node_s* temp_p;

   curr_p = list_p->h_p;
   while (curr_p != NULL) {
      temp_p = curr_p;
      curr_p = curr_p->next_p;
      free(temp_p);     
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
 * Purpose:    Get an int from stdin
 * Return value:  the next int in stdin
 * Note:       Behavior unpredictable if an int isn't entered
 */
int  Get_value(void) {
   int val;

   printf("Please enter a value:  ");
   scanf("%d", &val);
   return val;
}  /* Get_value */
