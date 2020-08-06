#pragma once
#include <time.h>

typedef struct
{
	char password[20], username[20];
}Credentials;

typedef struct
{
	char name[50], address[60], citizenship[15];
	int acc_no, age, trans_count;	
	long int phone;
	float balance;
	struct tm date;
}Action;

typedef struct
{
	char trans_type;
	struct tm date;
	long int amount;
	int trans_id;
}Transaction;

void menu(void);
void user_check(Credentials *);
void new_acc(void);
void initialize_accounts(void);
int get_acc_num(void);
int chk_unique(int);
void edit(void);
void transact(void);
void see(void);
void erase(void);
void reset_acc(int);
void print_transaction(int, char*, FILE *);
