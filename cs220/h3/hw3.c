/* File:     hw3.c
 *
 * Purpose:  search input text for the occurrence(s) of a word.  
 *           This version gets the word from the command line and
 *           the input text from stdin.
 *
 * Usage:    ./hw3 <word to be searched for>
 * Input:    text
 * Output:   Message if search word not found
 *           Number(s) of the word(s) of input that matched if
 *              found
 *           Number of occurrences of the word in the input
 *              if found
 * Notes:
 * 1.  Word searched for should contain no white space
 * 2.  Words in input text consist of strings separated by white space
 * 3.  No error check for strings exceeding MAX_STRING_LEN
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* For exit */

const int MAX_STRING_LEN = 100;

void Usage(char prog_name[]);

/* Node for linked list to store locations of occurrences */
struct node {
   int val;
   struct node *next_p;
};    

int main(int argc, char* argv[]) {
   char search_for[MAX_STRING_LEN];
   char current_word[MAX_STRING_LEN];
   int  scanf_rv;
   int  word_count = 0;  
   
   struct node *root_found_p;
   struct node *current_found_p;
   struct node *tail_found_p;
   int num_appearances = 0;
   
   /* Initialize pointers */
   root_found_p = 0;
   current_found_p = 0;
   tail_found_p = 0;

   /* Check that the user-specified word is on the command line */
   if (argc != 2) Usage(argv[0]);
   strcpy(search_for, argv[1]);

   printf("Enter the text to be searched\n");
   scanf_rv = scanf("%s", current_word);
   
   while ( scanf_rv != EOF ){
      /* If there is a match, add the location to the linked list */
      if (strcmp(current_word, search_for) == 0){
         current_found_p = (struct node *)malloc(sizeof(struct node));
         current_found_p->val = word_count;
         if (num_appearances == 0){
            root_found_p = current_found_p;
         } else {
            tail_found_p->next_p = current_found_p;
         }
         tail_found_p = current_found_p;
         
         num_appearances++;
      }
      
      word_count++;
      scanf_rv = scanf("%s", current_word);
   }
   
   current_found_p = root_found_p;
   
   /* Print the results */
   if (num_appearances == 0){
      printf("\n%s was not found in the %d words of input\n",
         search_for, word_count);
   }
   else {
      printf("%s was word number(s)", search_for);
      int i;
      for (i = 0; i < num_appearances; i++){
         if (i != 0)
            printf(",");
         if (i == (num_appearances - 1))
            printf(" and");
         printf(" %d", current_found_p->val);
         current_found_p = current_found_p->next_p;
      }
      printf("\n%s was found %d times.\n", search_for, num_appearances);
   }

   return 0;
}  /* main */

/* If user-specified word isn't on the command line, 
 * print a message and quit
 */
void Usage(char prog_name[]) {
   fprintf(stderr, "usage: %s <string to search for>\n",
      prog_name);
   exit(0);
}  /* Usage */

