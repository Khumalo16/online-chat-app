/**
 * @file loader.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "syntax.h"
#include "loader.h"

/*
 * Creates a deposit transaction and adds 
 * it to a linked list of transactions 
 */
struct transaction_t* load_deposit(int type, int dest, double amount, struct transaction_t* transaction_list){
	struct transaction_t *trans;
	struct transaction_t *temp;
	trans = malloc(sizeof(struct transaction_t));
	
	trans->type = type;
	trans->src = 0;
	trans->dest = dest;
	trans->amount = amount;
	trans->next = NULL;

	if (transaction_list == NULL){
		transaction_list = trans;
	} else {
		temp = transaction_list;
		while (temp->next != NULL){
			temp = temp->next;
		}
		temp->next = trans;
	}

#	ifdef DEBUG
        printf("Loaded deposit transaction\n"); 
#	endif
	
	return transaction_list;
}

/*
 * Creates a withdrawal transaction and adds 
 * it to a linked list of transactions 
 */
struct transaction_t* load_withdrawal(int type, int src, double amount, struct transaction_t *transaction_list){
	struct transaction_t *trans;
	struct transaction_t *temp;
	trans = malloc(sizeof(struct transaction_t));

	trans->type = type;
	trans->src = src;
	trans->dest = 0;
	trans->amount = amount;
	trans->next = NULL;

	if (transaction_list == NULL) {
		transaction_list = trans;
	} else {
		temp = transaction_list;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = trans;
	}

#	ifdef DEBUG
		printf("Loaded withdrawal transaction\n");
#	endif

	return transaction_list;
} 

/*
 * Creates a transfer transaction and adds 
 * it to a linked list of transactions 
 */
struct transaction_t * load_transfer(int type, int src, int dest, double amount, struct transaction_t* transaction_list){
	struct transaction_t *trans;
	struct transaction_t *temp;
	trans = malloc(sizeof(struct transaction_t));

	trans->type = type;
	trans->src = src;
	trans->dest = dest;
	trans->amount = amount;
	trans->next = NULL;

	if (transaction_list == NULL) {
		transaction_list = trans;
	} else {
		temp = transaction_list;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = trans;
	}

#	ifdef DEBUG
		printf("Loaded transfer transaction\n");
#	endif

	return transaction_list;
}

/*
 * Creates a balance transaction and adds 
 * it to a linked list of transactions 
 */
struct transaction_t * load_balance(int type, int src, struct transaction_t* transaction_list ){
	struct transaction_t *trans;
	struct transaction_t *temp;
	trans = malloc(sizeof(struct transaction_t));

	trans->type = type;
	trans->src = src;
	trans->next = NULL;

	if (transaction_list == NULL) {
		transaction_list = trans;
	} else {
		temp = transaction_list;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = trans;
	}

#	ifdef DEBUG
		printf("Loaded balance transaction\n");
#	endif

	return transaction_list;
} 
