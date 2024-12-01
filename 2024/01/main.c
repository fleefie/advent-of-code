#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


typedef struct _linked_list
{
    int val;
    struct _linked_list * p_next;
} Linked_list;

// Used for quicksort later
int compare_ints(const void *p1, const void *p2)
{
    // I just came out of learning Rust, this stuff makes my eyes bleed now.
    // I've seen the light, just to be basked in darkness after.
    int i1 = *(const int *)p1;
    int i2 = *(const int *)p2;
    return (i1 - i2);
}

// I refuse to import all of math.h just for this.
int abs(int x)
{
    return (x <= 0) ? -x : x;
}

int main ()
{
    // We'll make use of glibc's sorting functions, since I feel like those
    // are out of the scope of this exercise.

    // First, we want to read our input somehow. Here, we will use a file
    // named input.txt
    FILE * input_file = fopen("input.txt", "r");
    if (input_file == NULL)
        exit(1);

    // Initialize file reading variables
    char buffer[256];
    char * line = NULL;
    int line_count = 0; // Will be used later
    Linked_list * lhs_head = malloc(sizeof(Linked_list)); // Our two linked lists, representing our two arrays
    Linked_list * rhs_head = malloc(sizeof(Linked_list)); // This is the right hand side one :)
    if (lhs_head == NULL || rhs_head == NULL)
        exit(2);
    Linked_list * lhs_local_head = lhs_head; // Local heads, because screw making functions for this
    Linked_list * rhs_local_head = rhs_head; // Could've been simpler, to be fair.

    // Main loop
    while ((line = fgets(buffer, sizeof(buffer), input_file)) != NULL)
    {
        // We'll use the strtok function to tokenize our input
        char * token = strtok(line, " ");
        int value = 0;

        // Just puts our line of the format [int] [spaces] [int] into
        // our two linked lists
        while (token != NULL)
        {
            if (sscanf(token, "%i", &value) > 0)
            {
                lhs_local_head->val = value;
                // Remember to tokenize again, otherwise we'll get stuck on the last value!
                token = strtok(NULL, " ");
                break;
            }
            token = strtok(NULL, " ");
        }
        while (token != NULL)
        {
            if (sscanf(token, "%i", &value) > 0)
            {
                rhs_local_head->val = value;
                break;
            }
            token = strtok(NULL, " ");
        }

        // Now we update the state of our reading variables
        line_count++;
        Linked_list * lhs_next = malloc(sizeof(Linked_list));
        Linked_list * rhs_next = malloc(sizeof(Linked_list));
        if (lhs_next == NULL || rhs_next == NULL)
            exit(3);
        lhs_local_head->p_next = lhs_next;
        lhs_local_head = lhs_next;
        rhs_local_head->p_next = rhs_next;
        rhs_local_head = rhs_next;
    }

    // At this point we've successfuly read our inputs. Time to flatten our arrays.
    lhs_local_head = lhs_head;
    rhs_local_head = rhs_head;
    int * lhs_ints = malloc(sizeof(int) * line_count);
    int * rhs_ints = malloc(sizeof(int) * line_count);

    for (int i = 0; i < line_count; i++)
    {
        lhs_ints[i] = lhs_local_head->val;
        lhs_local_head = lhs_local_head->p_next;

        rhs_ints[i] = rhs_local_head->val;
        rhs_local_head = rhs_local_head->p_next;
    }

    // Now we can free our old tokenized inputs
    lhs_local_head = lhs_head;
    rhs_local_head = rhs_head;

    for (int i = 0; i < line_count; i++)
    {
        lhs_local_head = lhs_head;
        lhs_head = lhs_head->p_next;
        free(lhs_local_head);

        rhs_local_head = rhs_head;
        rhs_head = rhs_head->p_next;
        free(rhs_local_head);
    }

    // Now we can use quicksort!
    // Uh, if you've never dealt with function pointers...
    // Sucks to suck, I guess.
    // Fine. Here's how qsort works. These are the args, in order:
    // - the array
    // - number of elements
    // - size of an element
    // - function to compare two elements
    //   This function returns 0 if equal, 1 if greater, -1 if lesser.
    //   You just put the function name as an argument, and qsort will use that function.
    //   This function must have the signature:
    //       int func (const void * p1, const void * p2)
    //   Any decent language would use a lamda / closure to do this, but C is old and crusty.
    qsort(lhs_ints, line_count, sizeof(int), compare_ints);
    qsort(rhs_ints, line_count, sizeof(int), compare_ints);

    // We now have matched our arrays one to one, from smallest to greatest.
    // The n-th smallest number of the LHS is at the same index as the
    // n-th smallest number of the RHS. So now the final comparaison is trivial. Yippee!
    int distance = 0;

    for (int i = 0; i < line_count; i++)
    {
        distance += abs(lhs_ints[i] - rhs_ints[i]);
    }

    // Tada!
    printf("%i\n", distance);

    // Finally, clean up the last arrays
    free(lhs_ints);
    free(rhs_ints);
}