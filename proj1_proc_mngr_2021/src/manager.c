/**
 * @file manager.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "manager.h"

#define QUANTUM 1
#define TRUE 1
#define FALSE 0


/* Complete function definitions */
/* 
 * void process_release( 
 * void process_request(
*/
void process_send_message(struct pcb_t *pcb, 
    struct instruction_t *instruct, struct mailbox_t *mail);
void process_receive_message(struct pcb_t *pcb, 
    struct instruction_t *instruct, struct mailbox_t *mail);
/* 
 * int acquire_resource(
 * int release_resource(
 * void add_resource_to_process(
 * void release_resource_from_process(
 * void set_process_ready(
 * void set_process_waiting(
 * void set_process_terminated(
 * int processes_finished(
 * int processes_deadlocked(
*/

/**
 * @brief Schedules each instruction of each process in a round-robin fashion.
 * The number of instruction to execute for each process is governed by the
 * QUANTUM variable.
 *
 * @param pcb The process control block which contains the current process as
 * well as a pointer to the next process control block.
 * @param resource The list of resources available to the system.
 * @param mail The list of mailboxes available to the system.
 */
void schedule_processes(struct pcb_t *pcb, 
    struct resource_t *resource, struct mailbox_t *mail){
/*
TODO: 
Implement function 
*/
}

/**
 * @brief Handles the request resource instruction.
 *
 * Executes the request instruction for the process. The function loops
 * through the list of resources and acquires the resource if it is available.
 * If the resource is not available the process sits in the waiting queue and
 * tries to acquire the resource on the next cycle.
 *
 * @param current The current process for which the resource must be acquired.
 * @param instruct The instruction which requests the resource.
 * @param resource The list of resources.
 */

void process_request(struct pcb_t *current, struct instruction_t *instruct,
    struct resource_t *resource) {
    
  struct pcb_t *pcb;
  pcb->state = RUNNING;
  int available = acquire_resource(instruct->resource, resource, pcb);
  if (available = TRUE) {
      pcb->state = READY;
      add_resource_to_process(current,resource);
  } else {
    pcb->state = WAITING;
  }
}


/**
 * @brief Handles the release resource instruction.
 *
 * Executes the release instruction for the process. If the resource can be
 * released the process is ready for next execution. If the resource can not
 * be released the process sits in the waiting queue.
 *
 * @param current The process which releases the resource.
 * @param instruct The instruction to release the resource.
 * @param resource The list of resources.
 */

/*
Processes P1 P2
Resources R1 R1 R2 R3 R3 R3 
Mailboxes M1 
Process P1 
  req R1
  rel R1
  send (M1, "sync")
  req R2
  rel R2

Process P2 

  req R1
  req R2
  recv (M1, "")
  rel R1
  rel R2


P1 req R1: acquired; Available: R1 R2 R3 R3 R3
P2 req R1: acquired; Available: R2 R3 R3 R3
P1 rel R1: released; Available: R1 R2 R3 R3 R3
P2 req R2: acquired; Available: R1 R3 R3 R3
P1 sending message "sync" to mailbox M1
P2 received message "sync" from mailbox M1
P1 req R2: waiting;
P2 rel R1: released; Available R1 R1 R3 R3 R3
P2 rel R2: released; Available R1 R1 R2 R3 R3 R3
P2 terminated
P1 req R2: acquired; Available R1 R1 R3 R3 R3
P1 rel R2: released; Available R1 R1 R2 R3 R3 R3
P1 rel R2: Error: Nothing to release
P1 terminated
TODO: 
Implement function 
void process_release(...)
*/

/**
 * @brief Sends the message the prescribed mailbox.
 *
 * Sends the message specified in the instruction of the current process, to
 * the mailbox specified in the instruction. The function gets the pointer to 
 * the first mailbox and loops through all the mailboxes to find the one to
 * which the message must be sent.
 *
 * @param pcb The current process which instruct us to send a message.
 * @param instruct The current send instruction which contains the message.
 * @param mail The list of available mailboxes.
 */
void process_send_message(struct pcb_t *pcb, 
    struct instruction_t *instruct, struct mailbox_t *mail) {
  
  struct mailbox_t *current_mailbox;
  
  pcb->state = RUNNING;

  current_mailbox = mail;
  do {
    if ( strcmp(current_mailbox->name, instruct->resource) == 0 ) {
      /* We found the mailbox in which a message should be left */
      break;
    }
    current_mailbox = current_mailbox->next;
  } while ( current_mailbox != NULL );
 
  printf("%s sending message to mailbox %s which says \033[22;31m %s \033[0m\n",
      pcb->page->name, current_mailbox->name, instruct->msg);

  current_mailbox->msg = instruct->msg;
  pcb->next_instruction = pcb->next_instruction->next;
  dealloc_instruction(instruct);
}

/**
 * @brief Retrieves the message from the mailbox specified in the instruction
 * and stores it in the instruction message field.
 *
 * The function loops through the available mailboxes and finds the mailbox
 * from which the message must be retrieved. The retrieved message is stored
 * in the message field of the instruction of the process.
 *
 * @param pcb The current process which requests a message retrieval.
 * @param instruct The instruction to retrieve a message from a specific
 * mailbox.
 * @param mail The list of mailboxes.
 */
void process_receive_message(struct pcb_t *pcb, 
    struct instruction_t *instruct, struct mailbox_t *mail) {
  
  struct mailbox_t *current_mailbox;

  pcb->state = RUNNING;
  
  current_mailbox = mail;
  do {
    if ( strcmp(current_mailbox->name, instruct->resource) == 0 ) {
      /* We found the mailbox from which a message must be read. */
      break;
    }
    current_mailbox = current_mailbox->next;
  } while ( current_mailbox != NULL );

  printf("%s retrieved message from mailbox %s which says \033[22;32m %s \033[0m\n",
      pcb->page->name, current_mailbox->name, current_mailbox->msg);

  instruct->msg = current_mailbox->msg;
  current_mailbox->msg = NULL;
  pcb->next_instruction = pcb->next_instruction->next;
  dealloc_instruction(instruct);
}

/**
 * @brief Acquires the resource specified by resourceName.
 *
 * The function iterates over the list of resources trying to acquire the
 * resource specified by resourceName. If the resources is available, the
 * process acquires the resource. The resource is indicated as not available
 * in the resourceList and 1 is returned indicating that the resource has been
 * acquired successfully.
 *
 * @param resourceName The name of the resource to acquire.
 * @param resources The list of resources.
 * @param p The process which acquires the resource.
 *
 * @return 1 for TRUE if the resource is available. 0 for FALSE if the resource
 * is not available.
 */

int acquire_resource(char *resourceName, struct resource_t *resources, struct pcb_t *p) {

  struct resource_t *available;

  available = resources;
  p->state = RUNNING; 
  while (available != NULL) {
    if (strcmp(resourceName, available->name) == 0) {
        if (available->available == TRUE) {
          return TRUE;
        } 
    }
    available = available->next;
  }
  return FALSE;
}


/**
 * @brief Releases the resource specified by resourceName
 *
 * Iterates over the resourceList finding the resource which must be set to 
 * available again. The resource is then released from the process. 
 *
 * @param resourceName The name of the resource to release.
 * @param resources The list of available resources.
 * @param p The current process.
 *
 * @return 1 (TRUE) if the resource was released succesfully else 0 (FALSE).
 */

/*
TODO: 
Implement function 
int release_resource(...)
*/

/**
 * @brief Adds the specified resource to the process acquired resource list.
 *
 * After the resource has succesfully been required by the process. This
 * function is called and adds the resource to the list of resources currently
 * held by this process.
 *
 * @param current The process to which the resource must be added.
 * @param resource The resource to add to the process.
 */

void add_resource_to_process(struct pcb_t *current, struct resource_t *resource) {

  struct pcb_t *p;
  p = current;
   while (p->resources != NULL) {
     p->resources = p->resources->next;
   }
   resource->available = FALSE;
   p->resources = resource;
   current = p;
}


/**
 * @brief Release the specified resource from the process acquired list.
 *
 * The function releases the specified resource from the current process 
 * acquired list.
 *
 * @param current The current process from which the resource must be released.
 * @param resource The resource to release.
 */
/*
TODO: 
Implement function 
void release_resource_from_process(...)
*/

/**
 * @brief Add process (with id proc) to readyQueue 
 *
 * @param proc The process which must be set to ready.
 */
/*
TODO: 
Implement function 
void process_to_readyq(...) 
*/

/**
 * @brief Add process (with id proc) to the waitingQueue 
 *
 * @param proc The process which must be set to waiting.
 */

/*
void process_to_waitingq(struct pcb_t *pcb) {

} 
*/
/**
 * @brief Add process (with id proc) to the terminatedQueue 
 *
 * @param proc The process which must be set to waiting.
 */

/*
TODO: 
Implement function 
void process_to_terminateq(...) 
*/


/**
 * @brief Iterates over each of the loaded processes and checks if it has been
 * terminated.
 *
 * Iterates over the processes to determine if they have terminated.
 *
 * @param firstPCB A pointer to start of all the processes.
 *
 * @return 1 (TRUE) if all the processes are terminated else 0 (FALSE).
 */


int processes_finished(struct pcb_t *pcb) {

  struct pcb_t *p;
  p = pcb;

  while ( p != NULL)  {
    if (p->state != TERMINATED) {
      return  FALSE;
    }
    p = p->next;
  }
  return TRUE;
}


/**
 * @brief Detects deadlock. 
 *
 * This function implements a deadlock detection algorithm.
 *
 * @param firstPCB A pointer to the start of all the processes.
 *
 * @return 1 (TRUE) if all the processes are in the waiting state else
 * 0 (FALSE).
 */

/*
TODO: 
Implement function 
int processes_deadlocked(...)
*/

