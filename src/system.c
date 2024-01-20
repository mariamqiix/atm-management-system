#include "header.h"

#include <stdbool.h>

const char * RECORDS = "./data/records.txt";

int getAccountFromFile(FILE * ptr, char name[50], struct Record * r) {
  return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s", &
    r -> id, &
    r -> userId,
    name, &
    r -> accountNbr, &
    r -> deposit.month, &
    r -> deposit.day, &
    r -> deposit.year,
    r -> country, &
    r -> phone, &
    r -> amount,
    r -> accountType) != EOF;
}

void saveAccountToFile(FILE * ptr, struct User u, struct Record r) {
  fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
    r.id,
    u.id,
    u.name,
    r.accountNbr,
    r.deposit.month,
    r.deposit.day,
    r.deposit.year,
    r.country,
    r.phone,
    r.amount,
    r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u) {
  int option;
  if (notGood == 0) {
    system("clear");
    printf("\n✖ Record not found!!\n");
    invalid:
      printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
    scanf("%d", & option);
    if (option == 0)
      f(u);
    else if (option == 1)
      mainMenu(u);
    else if (option == 2)
      exit(0);
    else {
      printf("Insert a valid operation!\n");
      goto invalid;
    }
  } else {
    printf("\nEnter 1 to go to the main menu and 0 to exit:");
    scanf("%d", & option);
  }
  if (option == 1) {
    system("clear");
    mainMenu(u);
  } else {
    system("clear");
    exit(1);
  }
}

void success(struct User u) {
  int option;
  printf("\n✔ Success!\n\n");
  invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
  scanf("%d", & option);
  system("clear");
  if (option == 1) {
    mainMenu(u);
  } else if (option == 0) {
    exit(1);
  } else {
    printf("Insert a valid operation!\n");
    goto invalid;
  }
}

void createNewAcc(struct User u) {
  struct Record r;
  struct Record cr;
  char userName[50];
  FILE * pf = fopen(RECORDS, "a+");

  noAccount:
    system("clear");
  printf("\t\t\t===== New record =====\n");

  printf("\nEnter today's date(mm/dd/yyyy):");
  scanf("%d/%d/%d", & r.deposit.month, & r.deposit.day, & r.deposit.year);
  printf("\nEnter the account number:");
  scanf("%d", & r.accountNbr);

  while (getAccountFromFile(pf, userName, & cr)) {
    if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr) {
      printf("✖ This Account already exists for this user\n\n");
      goto noAccount;
    }
  }
  printf("\nEnter the country:");
  scanf("%s", r.country);
  printf("\nEnter the phone number:");
  scanf("%d", & r.phone);
  printf("\nEnter amount to deposit: $");
  scanf("%lf", & r.amount);
  printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
  scanf("%s", r.accountType);

  saveAccountToFile(pf, u, r);

  fclose(pf);
  success(u);
}

void checkAllAccounts(struct User u) {
  char userName[100];
  struct Record r;

  FILE * pf = fopen(RECORDS, "r");

  system("clear");
  printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
  while (getAccountFromFile(pf, userName, & r)) {
    if (strcmp(userName, u.name) == 0) {
      printf("_____________________\n");
      printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
        r.accountNbr,
        r.deposit.day,
        r.deposit.month,
        r.deposit.year,
        r.country,
        r.phone,
        r.amount,
        r.accountType);
    }
  }
  fclose(pf);
  success(u);
}

void UpdateAccountInformation(struct User u) {
  int accountNbr;
  struct Record cr;
  char userName[50];

  printf("What is the account number you want to change:");
  scanf("%d", & accountNbr);

  FILE * originalFile = fopen(RECORDS, "r");
  FILE * tempFile = fopen("temp.txt", "w");

  bool recordUpdated = false;

  while (getAccountFromFile(originalFile, userName, & cr)) {
    struct User oldUser;
    if (strcmp(userName, u.name) == 0 && cr.accountNbr == accountNbr) {
      printf("Which information do you want to update?\n1-> Phone number\n2-> Country\n");
      int information;
      scanf("%d", & information);

      switch (information) {
      case 1:
        int phoneNumber;
        printf("Enter the new Phone Number:");
        scanf("%d", & phoneNumber);
        cr.phone = phoneNumber;
        break;
      case 2:
        char country[100];
        printf("Enter the new country:");
        scanf("%s", country);
        strcpy(cr.country, country);
        break;
      }

      recordUpdated = true;
    }
    fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
      cr.id,
      cr.userId,
      userName,
      cr.accountNbr,
      cr.deposit.month,
      cr.deposit.day,
      cr.deposit.year,
      cr.country,
      cr.phone,
      cr.amount,
      cr.accountType);
  }
  fclose(originalFile);
  fclose(tempFile);

  if (!recordUpdated) {
    remove("temp.txt"); // Delete the temporary file
    printf("Account number not found.\n");
    mainMenu(u);
  }

  remove(RECORDS); // Delete the original file
  rename("temp.txt", RECORDS); // Rename the temporary file to the original filename
  success(u);
}

void CheckTheDetailsOfExistingAccounts(struct User u) {
  int accountNbr;
  struct Record cr;
  char userName[50];

  printf("What is the account number :");
  scanf("%d", & accountNbr);

  FILE * originalFile = fopen(RECORDS, "r");

  bool recordUpdated = false;
  while (getAccountFromFile(originalFile, userName, & cr)) {
    if (strcmp(userName, u.name) == 0 && cr.accountNbr == accountNbr) {
      printf("Account number: %d\nDeposit Date: %d/%d/%d\nCountry: %s\nPhone number: %d\nAmount deposited: $%.2lf\nType of Account: %s\n",
        cr.accountNbr,
        cr.deposit.day,
        cr.deposit.month,
        cr.deposit.year,
        cr.country,
        cr.phone,
        cr.amount,
        cr.accountType);
      recordUpdated = true;
      double interest = 0.0;
      char current[10] = "current\0";
      char saving[10] = "saving\0";
      char fixed01[10] = "fixed01\0";
      char fixed02[10] = "fixed02\0";
      char fixed03[10] = "fixed03\0";

      if (strcmp(cr.accountType, current) == 0) {
        printf("You will not get interest because the account is of type 'current'.\n");
      } else if (strcmp(cr.accountType, saving) == 0) {
        printf("You will get $%.2lf interest on day %d of every month.\n", (cr.amount * 0.07), cr.deposit.day);
      } else if (strcmp(cr.accountType, fixed01) == 0) {
        printf("You will get $%.2lf interest on day %d of every month.\n", (cr.amount * 0.04), cr.deposit.day);
      } else if (strcmp(cr.accountType, fixed02) == 0) {
        printf("You will get $%.2lf interest on day %d of every month.\n", (cr.amount * 0.05), cr.deposit.day);
      } else if (strcmp(cr.accountType, fixed03) == 0) {
        printf("You will get $%.2lf interest on day %d of every month.\n", (cr.amount * 0.08), cr.deposit.day);
      }

    }
  }

  fclose(originalFile);

  if (!recordUpdated) {
    printf("Account number not found.\n");
    mainMenu(u);
  }

  success(u);

}

void MakeTransaction(struct User u) {
  int accountNbr;
  struct Record cr;
  char userName[50];

  printf("What is the account number of the customer:");
  scanf("%d", & accountNbr);

  FILE * originalFile = fopen(RECORDS, "r");
  FILE * tempFile = fopen("temp.txt", "w");

  bool recordUpdated = false;

  while (getAccountFromFile(originalFile, userName, & cr)) {
    struct User oldUser;
    if (strcmp(userName, u.name) == 0 && cr.accountNbr == accountNbr) {
      printf("Do you want to?\n1-> Withdraw\n2-> Deposite\n\nEnter your choice:\n");
      int information;
      scanf("%d", & information);

      switch (information) {
      case 1:
        int Withdraw;
        printf("Enter the amount you want to Withdraw: $");
        scanf("%d", & Withdraw);
        if (Withdraw > cr.amount) {
          printf("The amount you chose to Withdraw is superior to your  available balance!");
          mainMenu(u);
        } else {
          cr.amount = cr.amount - Withdraw;
          printf("your current amount is : $%f", cr.amount);
        }
        break;
      case 2:
        int deposite;
        printf("Enter the amount you want to deposite: $");
        scanf("%d", & deposite);
        cr.amount = cr.amount + deposite;
        break;
      }

      recordUpdated = true;
    }
    fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
      cr.id,
      cr.userId,
      userName,
      cr.accountNbr,
      cr.deposit.month,
      cr.deposit.day,
      cr.deposit.year,
      cr.country,
      cr.phone,
      cr.amount,
      cr.accountType);

  }

  fclose(originalFile);
  fclose(tempFile);

  if (!recordUpdated) {
    remove("temp.txt"); // Delete the temporary file
    printf("Account number not found.\n");
    mainMenu(u);
  }

  remove(RECORDS); // Delete the original file
  rename("temp.txt", RECORDS); // Rename the temporary file to the original filename

  success(u);
}

void RemoveExistingAccount(struct User u) {
  int accountNbr;
  struct Record cr;
  char userName[50];
  printf("What is the account number you want to delete:");
  scanf("%d", & accountNbr);
  FILE * originalFile = fopen(RECORDS, "r");
  FILE * tempFile = fopen("temp.txt", "w");
  bool recordUpdated = false;

  while (getAccountFromFile(originalFile, userName, & cr)) {
    if (strcmp(userName, u.name) == 0 && cr.accountNbr == accountNbr) {
      recordUpdated = true;
    } else {
      fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
        cr.id,
        cr.userId,
        userName,
        cr.accountNbr,
        cr.deposit.month,
        cr.deposit.day,
        cr.deposit.year,
        cr.country,
        cr.phone,
        cr.amount,
        cr.accountType);
    }
  }

  fclose(originalFile);
  fclose(tempFile);

  if (!recordUpdated) {
    remove("temp.txt"); // Delete the temporary file
    printf("Account number not found.\n");
    mainMenu(u);
  }

  remove(RECORDS); // Delete the original file
  rename("temp.txt", RECORDS); // Rename the temporary file to the original filename
  success(u);
}

int getUsers(FILE * ptr, struct User * u) {

  return fscanf(ptr, "%d %49s %49s", &
    u -> id,
    u -> name,
    u -> password) != EOF;

}

void TransferOwner(struct User u) {
  int accountNbr;
  struct Record cr;
  char userName[50];

  printf("Enter the account number for which you want to transfer ownership: ");
  scanf("%d", & accountNbr);

  FILE * originalFile = fopen(RECORDS, "r");
  FILE * tempFile = fopen("temp.txt", "w");
  bool recordUpdated = false;

  while (getAccountFromFile(originalFile, userName, & cr)) {
    if (strcmp(userName, u.name) == 0 && cr.accountNbr == accountNbr) {
      printf("Account number: %d\nDeposit Date: %d/%d/%d\nCountry: %s\nPhone number: %d\nAmount deposited: $%.2lf\nType of Account: %s\n",
        cr.accountNbr,
        cr.deposit.day,
        cr.deposit.month,
        cr.deposit.year,
        cr.country,
        cr.phone,
        cr.amount,
        cr.accountType);

      char newOwner[50];

      invalid:
        printf("Enter the username of the new owner: ");
      scanf("%s", newOwner);

      FILE * usersFile = fopen("./data/users.txt", "r");
      bool userFound = false;
      struct User newUser;
      if (strcmp(newOwner, u.name) != 0) {
        while (getUsers(usersFile, & newUser)) {
          if (strcmp(newOwner, newUser.name) == 0) {
            userFound = true;
            break;
          }
        }
        fclose(usersFile);
      }
      if (!userFound) {
        printf("User does not exist.\n");
        goto invalid;
      } else {
        cr.id = newUser.id;
        strcpy(cr.name, newOwner);
      }
      saveAccountToFile(tempFile, newUser, cr);

      recordUpdated = true;
    } else {
      fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
        cr.id,
        cr.userId,
        userName,
        cr.accountNbr,
        cr.deposit.month,
        cr.deposit.day,
        cr.deposit.year,
        cr.country,
        cr.phone,
        cr.amount,
        cr.accountType);
    }

  }

  fclose(originalFile);
  fclose(tempFile);

  if (!recordUpdated) {
    remove("temp.txt"); // Delete the temporary file
    printf("Account number not found.\n");
    mainMenu(u);
  }

  remove(RECORDS); // Delete the original file
  rename("temp.txt", RECORDS); // Rename the temporary file to the original filename

  success(u);
}

void saveUsersToFile(FILE * ptr, struct User u) {
  fprintf(ptr, "%d %s %s\n", u.id, u.name, u.password);
}