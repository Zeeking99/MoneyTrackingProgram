#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "login.h"
#include "bank_functions.h"

void user_check(Credentials *User) // this function will verify the details of the user
{
	FILE *fptr;
	char username[20], password[20], try;
	Credentials user;

	do
	{
		printf("\t\t\nUsername: ");
		GetCredentials(username, 1);
		printf("\t\t\nPassword: ");
		GetCredentials(password, 0);

		if(strcmp(User->username, username)==0 && strcmp(User->password, password)==0)
		{
			printf("\t\t\t\n\nLOGGED IN SUCCESSFULLY");
			printf("\nLoading....\n");
			
			try = 0;
			delay(3); // used to create a delay in seconds
			system("clear");
			return;
		}
		else
		{
			printf("\t\t\t\n\nLOG IN FAILED");
			printf("\t\t\t\nEnter 1 to try again and anykey to EXIT: ");
			
			try = getchar();
			getchar(); // to consume the newline character from buffer
		}
	}while(try == '1');
	
	exit(0);
}
	
void menu(void)
{
	int choice;

    	do
    	{
        	system("clear");
        	printf("\n\n\t\t\tCUSTOMER ACCOUNT BANKING MANAGEMENT SYSTEM");
        	printf("\n\n\n\t\t\t\xB2\xB2\xB2\xB2\xB2\xB2\xB2 WELCOME TO THE MAIN MENU \xB2\xB2\xB2\xB2\xB2\xB2\xB2");
        	printf("\n\n\t\t1.Create new account\n\t\t2.Update information of existing account\n\t\t3.For transactions\n\t\t4.Check the details of existing account\n\t\t5.Removing existing account\n\t\t6.Exit\n\n\n\n\n\t\t Enter your choice: ");
         	scanf("%d",&choice);

         	system("clear");

    		switch(choice)
    		{
        	case 1:new_acc();
        	break;
       		case 2:edit();
        	break;
        	case 3:transact();
        	break;
        	case 4:see();
        	break;
        	case 5:erase();
        	break; 
        	case 6:exit(0);
        	break; 
    		}
	}while(1); // infinite loop exits only when the chooses to exit
}

void new_acc(void) // function to create a new account
{
	FILE *ptr;
	char acc_num[5];
	time_t samay = time(NULL);

	system("clear");
	Action Create; // declaring a structure to store details
	Credentials User; // structure to hold credentials

	printf("\n\n\n\t\t\tACCOUNT CREATION");

	printf("\n\n\t\tCreate a username: ");
	scanf(" %[^\n]", &User.username);
	printf("\n\t\tCreate a password: ");
	scanf(" %[^\n]", &User.password);
	
	printf("\n\n\t\tEnter your name: ");
	scanf(" %[^\n]", &Create.name);
	printf("\n\n\t\tEnter your age: ");
	scanf(" %d", &Create.age);
	printf("\n\n\t\tEnter your address: ");
	scanf(" %[^\n]", &Create.address);
	printf("\n\n\t\tEnter your nationality: ");
	scanf(" %[^\n]", &Create.citizenship);
	printf("\n\n\t\tEnter your phone number: ");
	scanf(" %ld", &Create.phone);

	Create.date = *localtime(&samay); // getting the system time	
	Create.balance = 0;
	Create.trans_count = 0;

	Create.acc_no = get_acc_num(); // generating a random account number
	
	printf("\n\t\tYour Account Number: %d", Create.acc_no);
	printf("\n\t\tThis number is unique to you, Note it.");
	
	sprintf(acc_num, "%d", Create.acc_no);

	ptr = fopen(acc_num, "wb");

	fwrite(&User, sizeof(User), 1, ptr);
	fwrite(&Create, sizeof(Create), 1, ptr);
	
	fclose(ptr);

	printf("\n\n\t\t\tYour Bank Account is created successfully.\n");
	getchar();
	system("clear");
	menu();
} 

int get_acc_num() // will generate a valid random account number
{
	srand(time(NULL));
	int acc_num;

	printf("\n\n\t\t\tGenerating a account number");
	getchar();
	
	do
	{
		acc_num = abs(rand()%10000);

	}while(chk_unique(acc_num) != 1);
	
	return acc_num;
}


int chk_unique(int acc_num) // This function will check if the account number is already in use.
{
	FILE *ptr;
	bool result = true;
	
	ptr = fopen("accounts", "rb+");	

	if(ptr == NULL) // checking if the file already exists
	{
		initialize_accounts();
		ptr = fopen("accounts", "rb");	
	}


	fseek(ptr, sizeof(bool)*acc_num, SEEK_SET);
	fread(&result, sizeof(result), 1, ptr);

	if(result)
	{
		return 0; // will return 0 if it is not unique	
	}
	else
	{
		result = false; // reassigning it to true 
		fseek(ptr, sizeof(bool)*acc_num, SEEK_SET);
		fwrite(&result, sizeof(result), 1, ptr); // writing it as true to denote it has been used
		fclose(ptr);
		return 1; // will return 1 if it is unique
	}
}

void reset_acc(int acc_num) // this function will reset a numbers position when it is deleted
{
	FILE *ptr;
	bool result = false;

	ptr = fopen("accounts", "rb+");

	fseek(ptr, sizeof(bool)*acc_num, SEEK_SET);
	fwrite(&result, sizeof(result), 1, ptr); // resetting it to 0 
	fclose(ptr);
}

void initialize_accounts(void) // This function will create a file which will store account numbers and will is used to prevent the creation of duplicate account numbers
{
	FILE *ptr;
	bool result = false;
	
	ptr = fopen("accounts", "wb+");

	for(int i=0;i<10000;i++)
		fwrite(&result, sizeof(result), 1, ptr);
	
	fclose(ptr);
}

void print_transaction(int count, char *acc_num, FILE *ptr) // function used to print the transactions of the account.
{
	Transaction Show;
	Action See;
	int temp, i=1;

	ptr = fopen(acc_num, "rb+");
	fseek(ptr, sizeof(Credentials), SEEK_SET); // moving the cursor at the beginning of the user's details
	fread(&See, sizeof(See), 1, ptr);
	
	temp = See.trans_count;
	printf("\n\n\t\t\tNumber of transactions: %d", temp);

	printf("\t\t\t\n\nYOUR TRANSACTIONS");

	while(temp>0 && count>0) // count will keep in track how many transactions are there for the account
	{
		printf("\n--------------------------------------");
		fseek(ptr, -(sizeof(Show)*i), SEEK_END); // reading backwards 
		fread(&Show, sizeof(Show), 1, ptr);
		
		printf("\t\t\n\nTransaction Type: %c", Show.trans_type);
		printf("\t\t\n\nTransaction Date: %2d/%2d/%4d", Show.date.tm_mday, Show.date.tm_mon+1, Show.date.tm_year+1900);
		printf("\t\t\n\nTransaction Amount: %ld", Show.amount);
		printf("\t\t\n\nTransaction ID: %d", Show.trans_id);
		
		temp--;
		i++;
	}
}

void edit(void) // to edit the details of the user
{
	system("clear");
	FILE *ptr;
	Action Edit;
	Credentials User;
	char acc_num[5];
	int option;

	printf("\t\t\t\n\nEDIT YOUR ACCOUNT DETAILS");
	
	do // entering the account number to get details
	{
		printf("\t\t\n\nEnter your Account Number: ");
		scanf(" %d", &Edit.acc_no);
		getchar();
	
		sprintf(acc_num, "%d", Edit.acc_no);
		ptr = fopen(acc_num, "rb+");

		if(ptr==NULL)
			printf("\t\t\n\n\tINVALID ACCOUNT NUMBER");
	}while(ptr == NULL);

	fseek(ptr, 0, SEEK_SET);
	fread(&User, sizeof(User), 1, ptr);
	user_check(&User);	// function to check if the user is authorized	

	fread(&Edit, sizeof(Edit), 1, ptr);

	printf("\n\n\t\tWhat do you want to edit: ");
	printf("\n\n\t\t\t1. Phone Number");
	printf("\n\n\t\t\t2. Address");
	printf("\n\n\t\t\t3. Nationality");
	printf("\n\n\t\t\t4. Cancel");
	printf("\n\n\t\t\t\tEnter: ");
	scanf(" %d", &option);

	switch(option)
	{
		case 1:
			printf("\n\n\t\t\tEnter your new Phone Number: ");
			scanf(" %ld", &Edit.phone);
			break;
		case 2:
			printf("\n\n\t\t\tEnter your new Address: ");
			scanf(" %[^\n]", &Edit.address);
			break;
		case 3:
			printf("\n\n\t\t\tEnter your new Nationality: ");
			scanf(" %[^\n]", &Edit.citizenship);
			break;
		default:
			printf("\n\n\t\t\tThe process was cancelled.");
	};

	fseek(ptr, sizeof(User), SEEK_SET);
	fwrite(&Edit, sizeof(Edit), 1, ptr);

	printf("\n\n\t\t\tThe changes are saved successfully.");

	fclose(ptr);
}

void transact(void) // function to let user do transactions
{
	system("clear");
	int option, count, temp;
	float amount;
	FILE *ptr;
	time_t samay = time(NULL);
	char acc_num[5];
	Action Transact;
	Credentials User;
	Transaction Type;
	
	printf("\t\t\t\n\nTRANSACTION PORTAL");

	do // checking the users credentials
	{
		printf("\t\t\n\nEnter your Account Number: ");
		scanf(" %d", &Transact.acc_no);
		getchar();
	
		sprintf(acc_num, "%d", Transact.acc_no);
		ptr = fopen(acc_num, "rb+");

		if(ptr==NULL)
			printf("\t\t\n\n\tINVALID ACCOUNT NUMBER");
	}while(ptr == NULL);

	fseek(ptr, 0, SEEK_SET);
	fread(&User, sizeof(User), 1, ptr);
	user_check(&User);		

	fread(&Transact, sizeof(Transact), 1, ptr);	

	printf("\t\t\n\nSelect the type of transaction");
	printf("\t\t\n\n1. Deposit");
	printf("\t\t\n2. Withdraw");
	printf("\t\t\n3. Check Balance");
	printf("\t\t\n4. Check Transactions");
	printf("\t\t\n\nAny Key to Exit: ");
	scanf(" %d", &option);

	if(option==1)
	{
		printf("\t\t\t\n\nEnter the amount you want to deposit: ");
		scanf(" %f", &amount);
		Transact.balance += amount;
		Transact.trans_count++;
		
		// saving the details of the transaction
		Type.trans_type = 'C';
		Type.date = *localtime(&samay);	
		Type.amount = amount;
		Type.trans_id = Transact.trans_count;

		printf("\t\t\n\nYour balance after deposit is: %.2f\n", Transact.balance);
	}
	else if(option==2)
	{
		printf("\t\t\t\n\nEnter the amount you want to withdraw: ");
		scanf(" %f", &amount);

		temp = Transact.balance;
		if(temp - amount < 0) // ensuring the user has sufficient balance
		{
			printf("\t\t\t\n\nInsufficient Balance to Continue the Transaction");
			getchar();
			return;
		}
	
		Transact.balance -= amount;
		Transact.trans_count++;

		// saving the details of the transaction
		Type.trans_type = 'D';
		Type.date = *localtime(&samay);	
		Type.amount = amount;
		Type.trans_id = Transact.trans_count;
		
		printf("\t\t\n\nYour account balance after withdrawal is: %.2f\n", Transact.balance);
	}
	else if(option==3)
	{
		printf("\t\t\t\n\nYour account balance is = %.2f", Transact.balance);
	}
	else if(option==4)
	{
		printf("\t\t\n\nEnter the number of transactions you want to see: ");
		scanf(" %d", &count);
	
		print_transaction(count, acc_num, ptr);
	}
	else
	{
		printf("\t\t\t\n\nTHE TRANSACTION WAS CANCELLED");
	}	

	getchar(); // to take the newline character in the buffer
	getchar(); // waiting for the user to click something
	
	if(option==1||option==2) // details will be saved only if the transaction is done
	{
		fseek(ptr, sizeof(User), SEEK_SET);
		fwrite(&Transact, sizeof(Transact), 1, ptr);
		fseek(ptr, 0, SEEK_END);
		fwrite(&Type, sizeof(Type), 1, ptr);
	}
	
	fclose(ptr);
}

void see(void) // function to see the user's details
{
	system("clear");
	FILE *ptr;
	char acc_num[5];
	Action See;
	Credentials User;
	
	printf("\t\t\t\n\nTRANSACTION PORTAL");

	do // verifying the user
	{
		printf("\t\t\n\nEnter your Account Number: ");
		scanf(" %d", &See.acc_no);
		getchar();
	
		sprintf(acc_num, "%d", See.acc_no);
		ptr = fopen(acc_num, "rb+");

		if(ptr==NULL)
			printf("\t\t\n\n\tINVALID ACCOUNT NUMBER");
	}while(ptr == NULL);

	fseek(ptr, 0, SEEK_SET);
	fread(&User, sizeof(User), 1, ptr);
	user_check(&User);		

	fread(&See, sizeof(See), 1, ptr);	

	printf("\t\t\t\n\nYour Account Details");
	// Printing the Account Holder's name 
	printf("\t\t\n\nName: %s", See.name);
	printf("\t\t\n\nAge: %d", See.age);
	printf("\t\t\n\nAddress: %s", See.address);
	printf("\t\t\n\nCitizenshp: %s", See.citizenship);
	printf("\t\t\n\nPhone: %ld", See.phone);
	printf("\t\t\n\nBalance: %.2f", See.balance);
	printf("\t\t\n\nDate of Account Creation: %2d/%2d/%4d", See.date.tm_mday, See.date.tm_mon+1, See.date.tm_year+1900);
	putchar('\n');
	getchar();

}

void erase(void) // deleting the user's account
{
	system("clear");
	FILE *ptr;
	char acc_num[5];
	Action Delete;
	Credentials User;
	
	printf("\t\t\t\n\nACCOUNT DELETION  PORTAL");

	do // authenticating the user
	{
		printf("\t\t\n\nEnter your Account Number: ");
		scanf(" %d", &Delete.acc_no);
		getchar();
	
		sprintf(acc_num, "%d", Delete.acc_no);
		ptr = fopen(acc_num, "rb+");

		if(ptr==NULL)
			printf("\t\t\n\n\tINVALID ACCOUNT NUMBER");
	}while(ptr == NULL);

	fseek(ptr, 0, SEEK_SET);
	fread(&User, sizeof(User), 1, ptr);
	user_check(&User);		

	fread(&Delete, sizeof(Delete), 1, ptr);	

	printf("\t\t\t\n\nYour Account Details");
	// Printing the Account Holder's name 
	printf("\t\t\n\nName: %s", Delete.name);
	printf("\t\t\n\nAge: %d", Delete.age);
	printf("\t\t\n\nAddress: %s", Delete.address);
	printf("\t\t\n\nCitizenshp: %s", Delete.citizenship);
	printf("\t\t\n\nPhone: %ld", Delete.phone);
	printf("\t\t\n\nBalance: %.2f", Delete.balance);
	printf("\t\t\n\nDate of Account Creation: %2d/%2d/%4d", Delete.date.tm_mday, Delete.date.tm_mon+1, Delete.date.tm_year+1900);
	putchar('\n');
	remove(acc_num); // function to remove the file
	
	fclose(ptr);
	getchar();
}

