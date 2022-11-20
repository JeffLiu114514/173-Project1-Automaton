#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nfa.h"
#include "LinkedList.h"

/**
 * Allocate and return a new NFA containing the given number of states.
 */
NFA* new_NFA(int nstates){
    NFA* new_nfa = (NFA*)malloc(sizeof(NFA));
    LinkedList linkedList = new_LinkedList();
    LinkedList_add_at_front(linkedList, 0);
    new_nfa->curr_states = linkedList;
    new_nfa->num_states = nstates;
    new_nfa->states = (NFA_STATE*)malloc(nstates * sizeof(NFA_STATE));

    for (int i = 0; i < nstates; i++) {
        new_nfa->states[i].accepting = false;
    }
    for (int i = 0; i < nstates; i++)
        for(int j = 0; j < 128; j++)
            new_nfa->states[i].transitions[j] = new_LinkedList(); // -1 for no transition

    return new_nfa;
}

/**
 * Free the given NFA.
 */
void NFA_free(NFA *nfa){
    for (int i = 0; i < nfa->num_states; i++) {
        NFA_STATE *state = &nfa->states[i];
        for (int j = 0; j < 128; j++) {
            LinkedList_free(state->transitions[j], true);
        }
    }
    LinkedList_free(nfa->curr_states, true);
    free(nfa->states);
    free(nfa);
}

/**
 * Return the number of states in the given NFA.
 */
int NFA_get_size(NFA *nfa){
    return nfa->num_states;
}

/**
 * Return the set of next states specified by the given NFA's transition
 * function from the given state on input symbol sym.
 */
LinkedList NFA_get_transitions(NFA *nfa, int state, char sym){
    return nfa->states[state].transitions[(int)state];
}

/**
 * For the given NFA, add the state dst to the set of next states from
 * state src on input symbol sym.
 */
void NFA_add_transition(NFA *nfa, int src, char sym, int dst){
    LinkedList_add_at_front(nfa->states[src].transitions[(int)sym], dst);
}


/**
 * Add a transition for the given NFA for each symbol in the given str.
 */
void NFA_add_transition_str(NFA *nfa, int src, char *str, int dst){
    for(int i = 0; i < strlen(str); i++) {
        LinkedList_add_at_front(nfa->states[src].transitions[(int)str[i]], dst);
    }
}


/**
 * Add a transition for the given NFA for each input symbol.
 */
void NFA_add_transition_all(NFA *nfa, int src, int dst){
    for (int i = 0; i < 128; i++)
        LinkedList_add_at_front(nfa->states[src].transitions[i], dst);
}


/**
 * Set whether the given NFA's state is accepting or not.
 */
void NFA_set_accepting(NFA *nfa, int state, bool value){
    nfa->states[state].accepting = value;
}

/**
 * Return true if the given NFA's state is an accepting state.
 */
bool NFA_get_accepting(NFA *nfa, int state){
    return nfa->states[state].accepting;
}

/**
 * Run the given NFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
bool NFA_execute(NFA *nfa, char *input, LinkedList initial_state){
    LinkedListIterator iterator = LinkedList_iterator(initial_state);
    while (LinkedListIterator_hasNext(iterator)) {
        int temp = LinkedListIterator_next(iterator);
        if (strlen(input) > 0) {//go through input
            //recursively analyze state. If accepting, return
            if(NFA_execute(nfa, (input+1), nfa->states[temp].transitions[(int)input[0]])) {
                free(iterator);
                return true;
            }
        }
        else if (nfa->states[temp].accepting) {//when input is finished and state is accepting, return
            free(iterator);
            return true;
        }
    }
    free(iterator);
    return false;
}


/**
 * Print the given NFA to System.out.
 *
 */
 //Unused
void NFA_print(NFA nfa){}

//Difference from the DFA one: using LinkedList to initialize the starting state.
void NFA_repl(NFA* problem){
    while (true){
        LinkedList linkedList = new_LinkedList();
        LinkedList_add_at_front(linkedList, 0);
        problem->curr_states = linkedList;
        char* input_str = malloc(64*sizeof(char));
        printf("Enter an input (\"quit\" to quit):");
        //fgets(input_str, 128, stdin);
        scanf("%s", input_str);
        if(strcmp(input_str,"quit")==0){
            break;
        }
        bool result = NFA_execute(problem, input_str, problem->curr_states);
        printf("Result for input %s: %d\n", input_str, result);
    }
}

void solve_2a() {
    printf("This is problem 2a: Strings that end in \"gs\"\n");
    NFA *problem = new_NFA(3);
    NFA_add_transition_all(problem, 0, 0);
    NFA_add_transition(problem, 0, 'g', 1);
    NFA_add_transition(problem, 1, 's', 2);
    NFA_set_accepting(problem, 2, true);

    NFA_repl(problem);
    NFA_free(problem);
}

void solve_2b(){
    printf("This is problem 2b: Strings that contain \"mas\"\n");
    NFA *problem = new_NFA(4);
    NFA_add_transition_all(problem, 0, 0);
    NFA_add_transition(problem, 0, 'm', 1);
    NFA_add_transition(problem, 1, 'a', 2);
    NFA_add_transition(problem, 2, 's', 3);
    NFA_add_transition_all(problem, 3, 3);
    NFA_set_accepting(problem, 3, true);

    NFA_repl(problem);
    NFA_free(problem);
 }

void solve_2c(){
    printf("This is problem 2c:Strings with more than one 'a', 'b', 'c', 'd', 'k', or 'o',or more than two r's or three e's \n");
    NFA *problem = new_NFA(20);
    NFA_add_transition(problem, 0, 'a', 1);
    NFA_add_transition(problem, 1, 'a', 2);
    NFA_set_accepting(problem, 2, true);

    NFA_add_transition(problem, 0, 'b', 3);
    NFA_add_transition(problem, 3, 'b', 4);
    NFA_set_accepting(problem, 4, true);

    NFA_add_transition(problem, 0, 'c', 5);
    NFA_add_transition(problem, 5, 'c', 6);
    NFA_set_accepting(problem, 6, true);

    NFA_add_transition(problem, 0, 'd', 7);
    NFA_add_transition(problem, 7, 'd', 8);
    NFA_set_accepting(problem, 8, true);

    NFA_add_transition(problem, 0, 'k', 9);
    NFA_add_transition(problem, 9, 'k', 10);
    NFA_set_accepting(problem, 10, true);

    NFA_add_transition(problem, 0, 'o', 11);
    NFA_add_transition(problem, 11, 'o', 12);
    NFA_set_accepting(problem, 12, true);

    NFA_add_transition(problem, 0, 'r', 13);
    NFA_add_transition(problem, 13, 'r', 14);
    NFA_add_transition(problem, 14, 'r', 15);
    NFA_set_accepting(problem, 15, true);

    NFA_add_transition(problem, 0, 'e', 16);
    NFA_add_transition(problem, 16, 'e', 17);
    NFA_add_transition(problem, 17, 'e', 18);
    NFA_add_transition(problem, 18, 'e', 19);
    NFA_set_accepting(problem, 19, true);

    for (int i = 0; i < 19; i++) {
        NFA_add_transition_all(problem, i, i);
    }

    NFA_repl(problem);
    NFA_free(problem);
 }

