/**
 * @file main.c
 * @author Francois de Villiers (Demi 2011-2012)
 * @description The main function file for the process management application.
 *
 * @mainpage Process Simulation
 *
 * @section intro_sec Introduction
 *
 * The project consists of 3 main files parser, loader and manager. Which 
 * respectively handles the loading, parsing and management of the processes.
 *
 * @section make_sec Compile
 * 
 * $ make
 *
 * @section run_sec Execute
 *
 * $ ./process-management data/process.list
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "loader.h"
#include "manager.h"
#define DEBUG 1
void print_ready_queue ( struct pcb_t *pcb );
void print_mailboxes ( struct mailbox_t *mail );
void print_instruction( struct instruction_t *instr);

int main(int argc, char** argv) {
  char* filename;
  struct pcb_t *ready_queue;
  struct resource_t *resources;
  struct mailbox_t *mailboxes;

  filename = NULL;

  if ( argc < 1 ) {
    return EXIT_FAILURE; 
  }
  
  filename = argv[1];
  parse_process_file(filename);

  ready_queue = get_loaded_processes();
  resources = get_available_resources();
  mailboxes = get_mailboxes();
  if (resources == NULL) {
    printf("yes i'm null");
  }
  while(resources != NULL) {
    printf(" %s \n", resources->name);
    resources  = resources->next;
  }
#ifdef DEBUG
  print_ready_queue(ready_queue);
  print_mailboxes(mailboxes);
  print_ready_queue(ready_queue);

 // process_request(ready_queue->page, ready_queue->next_instruction,)
#endif

  //TODO: Schedule processes

  dealloc_pcb_list(ready_queue);
  dealloc_data_structures();
  
  return EXIT_SUCCESS;
}

#ifdef DEBUG
void print_ready_queue ( struct pcb_t *pcb ) {
  struct pcb_t *current_pcb;
  current_pcb = pcb;
  printf("pcb full list:\n");
  do {
    printf("  %s (%d):\n", current_pcb->page->name, current_pcb->state);
    print_instruction(current_pcb->next_instruction);
    current_pcb = current_pcb->next;
  } while(current_pcb != NULL);
  printf("\n");
}

void print_instruction(struct instruction_t *instr) {
    while (instr != NULL) {
	switch (instr->type) {
	case REQ_V: 
          printf("    (req %s)\n", instr->resource);
	  break;
	case REL_V: 
          printf("    (rel %s)\n", instr->resource);
	  break;
	case SEND_V :
          printf("    (send %s %s)\n", instr->resource, instr->msg);
	  break;
	case RECV_V:
          printf("    (recv %s %s)\n", instr->resource, instr->msg);
	  break;
      }
      instr = instr->next;
    }
}

void print_mailboxes ( struct mailbox_t *mailbox ) {
  struct mailbox_t *current_mailbox = mailbox;

  printf("Mailboxes: ");
  while (current_mailbox != NULL) {
    printf("%s ", current_mailbox->name);
    current_mailbox = current_mailbox->next;
  } 
  printf("\n");
}
#endif
  
