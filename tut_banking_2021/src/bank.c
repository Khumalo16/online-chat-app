/* vim settings: se ts=4 tw=72
 * File: bank.c 
 * 
 * An account, Account holders, Deposit and Withdrawel transactions 
 *         
 * Run: . run.sh 
 * 
 * TODO: Currently, only one transaction list is read in and executed by one thread 
 *       1. Parallelise this, so that 
 *       - multiple threads can each read a transaction list (one transaction list per thread) 
 *       - each thread can execute its own list of transactions in parallel with the other threads 
 *       2. free all memory allocated before exiting the program 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loader.h"
#include "parser.h"
#include <omp.h> 

void get_args(int argc, char* argv[], int* thread_count_p, char* fname);
void usage(char* prog_name);
void initialize_locks(int num_lock);
void deposit(int acc_num, double amount);
void withdrawal(int acc_num, double amount);
void transfer(int acc1, int acc2, double amount);
void acc_balance(int acc_num);

int trans_file = 2;
omp_lock_t *lock = NULL;
double *account_balances;

/*------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int thread_count, j, i, key1, key2,k;
	char* fname;
	struct transaction_t* transaction_list;

	i = strtol(argv[1], NULL, 10);

	/** Declaire and initialize the number of locks*/
	initialize_locks(i);

	struct transaction_t all_trans_list[i];
	j = 0;
    if (i <= 0) usage(argv[0]);
	for ( j = 0; j< i; j++) {
		fname =  malloc(sizeof(char)*64);
		get_args(argc, argv, &thread_count, fname);
	
		transaction_list = malloc(sizeof(struct transaction_t));
		if (transaction_list == NULL) { 
			printf("Something bad happened, no list\n");
			exit(0);
		}
		transaction_list = parse_transaction_file(fname);
		all_trans_list[j].type = transaction_list->type;
		all_trans_list[j].src = transaction_list->src;
		all_trans_list[j].dest = transaction_list->dest;
		all_trans_list[j].amount = transaction_list->amount;
		all_trans_list[j].next = transaction_list->next;
	}
	
	account_balances = (double*) calloc(MAX_ACCS, sizeof(double));	
 	if (account_balances == NULL) {
		fprintf(stderr,"Memory could not be allocated, exiting\n");
		exit(0);		
	}

	
	i = 0;
	printf("  ***    Start of transaction batch    ***  \n");
#	pragma omp parallel private(key1, key2, k) num_threads(thread_count)
	{

	#	pragma omp barrier
		for (k = 0; k < thread_count; k++) {
		
	//		k = omp_get_thread_num();
			transaction_list = malloc(sizeof(struct transaction_t));
			transaction_list->type = all_trans_list[k].type;
			transaction_list->src = all_trans_list[k].src;
			transaction_list->dest = all_trans_list[k].dest;
			transaction_list->amount = all_trans_list[k].amount;
			transaction_list->next = all_trans_list[k].next;

		#	pragma omp task firstprivate(transaction_list)
			while (transaction_list != NULL){
				if (transaction_list->type == DP_T){
					key1 = transaction_list->dest;
					deposit(key1, transaction_list->amount);
				} else if (transaction_list->type == WD_T){
					key1 = transaction_list->src;
						withdrawal(key1, transaction_list->amount);
				} else if (transaction_list->type == TR_T){
					key1 = transaction_list->src;
					key2 = transaction_list->dest;
			#	 pragma omp critical
					{
						transfer(key1, key2, transaction_list->amount);
					}
				} else if (transaction_list->type == BL_T){
					acc_balance(transaction_list->src);
				}
	//			printf("sdfsdfsda %f\n", transaction_list->amount);
				transaction_list = transaction_list->next;
				}
		}
   	}

	printf("  ***     End of transaction batch    ***  \n");
	for (i = 0; i < MAX_ACCS; i++){
		printf("Account %d balance after completion of transaction batch: %f\n", \
				i, account_balances[i]);
	}
	return 0;		
}

/**
 * @brief Initialize the number of locks
 *
 * Initialize the number of locks equavilent to the number of account holders
 *
 * @param num_lock Number of locks
 * @param locks Initialize the locks
 */
void initialize_locks(int num_lock) {
	int i;
	lock = malloc(sizeof(omp_lock_t) * num_lock);
	for (i = 0; i <num_lock; i++) {
		omp_init_lock(&(lock[i]));
	}
}

/*------------------------------------------------------------------
 * @brief  get_args
 *            Get command line args
 * In args:   argc, argv
 * Out args:  thread_count_p, m_p, n_p
 */
void get_args(int argc, char* argv[], int* thread_count_p, char* fname)  {
	
   if (argc < 3) usage(argv[0]);
   *thread_count_p = strtol(argv[1], NULL, 10);
   strcpy(fname,argv[trans_file]);
   trans_file++;
   if (*thread_count_p <= 0) usage(argv[0]);

}  /* get_args */

/*------------------------------------------------------------------
 * @brief  usage
 *	print a message showing what the command line should be, and terminate
 * In arg : prog_name
 */
void usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count> <datafile.txt> {<datafile.txt>}\n", prog_name);
   exit(0);
}  /* usage */

/*--------------------------------------------------------------------
 * @brief deposit 
 * 	Add amount to balance 
 * @param acc_num: Account number
 * @param amount:  Amount to deposit 
 * @param balance: Balance of acc_num  
 */
void deposit(int acc_num, double amount)
{
	printf("User %d, Acc %d Balance %f, Deposit %f \n", \
			omp_get_thread_num(), acc_num, account_balances[acc_num], amount);
    
//	omp_set_lock(&(lock[acc_num]));
# pragma omp atomic
	account_balances[acc_num] += amount;
//	omp_unset_lock(&(lock[acc_num]));
} 

/*--------------------------------------------------------------------
 * @brief withdrawal 
 * 	If amount available, subtract amount from balance 
 * @param acc_num: Account Number
 * @param amount:  Amount to withdraw 
 * @param balance: Balance of acc_num  
 */
void withdrawal(int acc_num, double amount)
{
	if (amount <= account_balances[acc_num]) { 
		printf("User %d, Acc %d Balance %f, Withdraw %f \n", \
				omp_get_thread_num(), acc_num, account_balances[acc_num], amount);  
	
//		omp_set_lock(&(lock[acc_num]));
# pragma omp atomic
		account_balances[acc_num] -= amount;
//		omp_unset_lock(&(lock[acc_num]));
	} else {
		printf("\033[22;32mUser %d, Acc %d Balance %f, \
				Withdraw %f --insufficient funds\033[0m\n", \
				omp_get_thread_num(), acc_num, account_balances[acc_num], amount);
	}
} 

/*--------------------------------------------------------------------
 * @brief transfer 
 *           If amount available in acc1, 
 *           subtract amount from acc1 balance and add to acc2 balance
 * @param acc1:    Number of account from which money is transferred
 * @param acc2:    Number of account to which money is transferred 
 * @param amount:  Amount to transfer 
 * @param balance: Balance of acc1  
 */
void transfer(int acc1, int acc2, double amount) {
	if (amount <= account_balances[acc1]) { 
		printf("User %d, Acc %d Balance %f, Transfer %f to Acc %d\n", \
				omp_get_thread_num(), acc1, account_balances[acc1], amount, acc2);

	printf("%d \n", acc1);
		omp_set_lock(&lock[9]);
		omp_set_lock(&lock[2]);
		account_balances[acc1] -= amount;  
		account_balances[acc2] += amount;  
		omp_unset_lock(&lock[2]);
		omp_unset_lock(&lock[9]);
	}
}

/*--------------------------------------------------------------------
 * @brief balance 
 *	Return the current balance of account acc_num 
 * @param acc_num: The number of the account 
 * @param balance: The current balance of account acc_num
 */
void acc_balance(int acc_num)
{
	printf("Balance in %d: %f\n", acc_num, account_balances[acc_num]);
}

