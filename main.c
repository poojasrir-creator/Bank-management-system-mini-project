// Bank Account Management System using Random Access File

#include <stdio.h>
#include <stdlib.h>

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    // open existing file or create new file
    cfPtr = fopen("credit.dat", "rb+");

    if (cfPtr == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be opened.\n");
            exit(1);
        }
    }

    // menu loop
    while ((choice = enterChoice()) != 5)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        default:
            printf("Invalid Choice!\n");
        }
    }

    fclose(cfPtr);

    printf("\nProgram Ended Successfully.\n");

    return 0;
}

// create formatted text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    int result;

    struct clientData client = {0, "", "", 0.0};

    writePtr = fopen("accounts.txt", "w");

    if (writePtr == NULL)
    {
        printf("File could not be opened.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr,
            "%-6s %-15s %-12s %10s\n",
            "Acct",
            "Last Name",
            "First Name",
            "Balance");

    while ((result = fread(&client,
                           sizeof(struct clientData),
                           1,
                           readPtr)) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr,
                    "%-6u %-15s %-12s %10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("accounts.txt file created successfully.\n");
}

// update existing record
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number to update (1 - 100): ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Number : %u\n", client.acctNum);
        printf("Name           : %s %s\n",
               client.firstName,
               client.lastName);

        printf("Current Balance: %.2f\n",
               client.balance);

        printf("Enter amount (+ deposit / - withdraw): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Balance Updated Successfully.\n");
        printf("New Balance: %.2f\n", client.balance);
    }
}

// add new record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        printf("Enter Last Name: ");
        scanf("%14s", client.lastName);

        printf("Enter First Name: ");
        scanf("%9s", client.firstName);

        printf("Enter Balance: ");
        scanf("%lf", &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (client.acctNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("New Account Added Successfully.\n");
    }
}

// delete record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete (1 - 100): ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account Deleted Successfully.\n");
    }
}

// display menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n========== BANK ACCOUNT MENU ==========\n");
    printf("1 - Create text file\n");
    printf("2 - Update account\n");
    printf("3 - Add new account\n");
    printf("4 - Delete account\n");
    printf("5 - Exit\n");
    printf("Enter your choice: ");

    scanf("%u", &choice);

    return choice;
}