#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"

/**
 * Allocate and return a new DFA containing the given number of states.
 */
DFA* new_DFA(int nstates){
    DFA* new_dfa = (DFA*)malloc(sizeof(DFA));
    new_dfa->curr_state = 0; //0 for initial state
    new_dfa->num_states = nstates;
    new_dfa->states = (DFA_STATE*)malloc(nstates * sizeof(DFA_STATE));

    for (int i = 0; i < nstates; i++) {
        new_dfa->states[i].accepting = false;
    }
    for (int i = 0; i < nstates; i++)
        for(int j = 0; j < 128; j++)
            new_dfa->states[i].transitions[j] = -1; // -1 for no transition

    return new_dfa;
}

/**
 * Free the given DFA.
 */
void DFA_free(DFA *dfa){
    free(dfa->states);
    free(dfa);
}

/**
 * Return the number of states in the given DFA.
 */
int DFA_get_size(DFA *dfa){
    return dfa->num_states;
}

/**
 * Return the state specified by the given DFA's transition function from
 * state src on input symbol sym.
 */
int DFA_get_transition(DFA *dfa, int src, char sym){
    return dfa->states[src].transitions[(int)sym];
}

/**
 * For the given DFA, set the transition from state src on input symbol
 * sym to be the state dst.
 */
void DFA_set_transition(DFA *dfa, int src, char sym, int dst){
    dfa->states[src].transitions[(int)sym] = dst;
}

/**
 * Set the transitions of the given DFA for each symbol in the given str.
 * This is a nice shortcut when you have multiple labels on an edge between
 * two states.
 */
void DFA_set_transition_str(DFA *dfa, int src, char *str, int dst){
    for(int i = 1; i <= strlen(str); i++)
        dfa->states[src].transitions[(int)str[i]] = dst;
}

/**
 * Set the transitions of the given DFA for all input symbols.
 * Another shortcut method.
 */
void DFA_set_transition_all(DFA *dfa, int src, int dst){
    for (int i = 0; i < 128; i++)
        dfa->states[src].transitions[i] = dst;
}

/**
 * Set whether the given DFA's state is accepting or not.
 */
void DFA_set_accepting(DFA *dfa, int state, bool value){
    dfa->states[state].accepting = value;
}

/**
 * Return true if the given DFA's state is an accepting state.
 */
bool DFA_get_accepting(DFA *dfa, int state){
    return dfa->states[state].accepting;

}

/**
 * Run the given DFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
bool DFA_execute(DFA *dfa, char *input){
    for (int i = 0; i < strlen(input); i++) {
        DFA_STATE current = dfa->states[dfa->curr_state];
        int dst = current.transitions[(int)input[i]];
        dfa->curr_state = dst;
        if (dfa->curr_state == -1) {return false;}
    }
    int final_state = dfa->curr_state;
    return DFA_get_accepting(dfa, final_state);
}

/**
 * Print the given DFA to System.out.
 */
void DFA_print(DFA *dfa){

}

void DFA_repl(DFA* problem){
    while (true) {
        problem->curr_state = 0;
        char* input_str = malloc(64*sizeof(char));
        printf("Enter an input (\"quit\" to quit):\n");
        //fgets(input_str, 128, stdin);
        scanf("%s", input_str);
        if (strncmp(input_str, "quit", 4) == 0){
            break;
        }
        bool result = DFA_execute(problem, input_str);
        printf("Result for input %s: %d\n", input_str, result);
    }
}

void solve_1a() {
    printf("This is problem 1a: exactly the word \"ullman\"\n");
    DFA* problem = new_DFA(7);
    DFA_set_transition(problem, 0, 'u', 1);
    DFA_set_transition(problem, 1, 'l', 2);
    DFA_set_transition(problem, 2, 'l', 3);
    DFA_set_transition(problem, 3, 'm', 4);
    DFA_set_transition(problem,4, 'a', 5);
    DFA_set_transition(problem, 5, 'n', 6);
    DFA_set_accepting(problem, 6, true);
    DFA_repl(problem);
    DFA_free(problem);

}

void solve_1b() {
    printf("This is problem 1b: starting with \"com\"\n");
    DFA* problem = new_DFA(4);
    DFA_set_transition(problem, 0, 'c', 1);
    DFA_set_transition(problem, 1, 'o', 2);
    DFA_set_transition(problem, 2, 'm', 3);
    DFA_set_transition_all(problem, 3, 3);
    DFA_set_accepting(problem, 3, true);
    DFA_repl(problem);
    DFA_free(problem);
}

void solve_1c() {
    printf("This is problem 1c: containing exactly three 3's\n");
    DFA* problem = new_DFA(5);
    DFA_set_transition_all(problem, 0, 0);
    DFA_set_transition(problem, 0, '3', 1); //every char except 3 will not change state
    DFA_set_transition_all(problem, 1, 1);
    DFA_set_transition(problem, 1, '3', 2);
    DFA_set_transition_all(problem, 2, 2);
    DFA_set_transition(problem, 2, '3', 3);
    DFA_set_transition_all(problem, 3, 3);
    DFA_set_transition(problem, 3, '3', 4);//not accepting string with more than three 3's
    DFA_set_transition_all(problem, 4, 4);
    DFA_set_accepting(problem, 3, true);
    DFA_repl(problem);
    DFA_free(problem);
}

void solve_1d() {
    printf("This is problem 1d: Binary input with an even number of 0's and and odd number of 1'\n");
    DFA* problem = new_DFA(4);
    DFA_set_transition(problem, 0, '1', 1);
    DFA_set_transition(problem, 1, '1', 0);
    DFA_set_transition(problem, 0, '0', 2);
    DFA_set_transition(problem, 2, '0', 0);
    DFA_set_transition(problem, 1, '0', 3);
    DFA_set_transition(problem, 3, '0', 1);
    DFA_set_transition(problem, 2, '1', 3);
    DFA_set_transition(problem, 3, '1', 2);
    DFA_set_accepting(problem, 1, true);
    DFA_repl(problem);
    DFA_free(problem);
}