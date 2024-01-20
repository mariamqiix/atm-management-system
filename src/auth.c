#include <termios.h>
#include "header.h"
#include "syslog.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    FILE * usersFile = fopen("./data/users.txt", "r");

    while (getUsers(usersFile,&userChecker))
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}


void Registration(  struct User MyUser) {
    int id;
    char name[50];
    char password[50];

invalid:
    printf("Enter your id: \n");
    scanf("%d", &id);

    printf("Enter your Name: \n");
    scanf("%s", name);

    FILE* usersFile = fopen("./data/users.txt", "r");
    struct User newUser;

    while (getUsers(usersFile, &newUser)) {
        if (strcmp(newUser.name, name) == 0 && id == newUser.id) {
            printf("This user already exists\n");
            fclose(usersFile);
            goto invalid;
        }
    }

    fclose(usersFile);

    printf("Enter your password: \n");
    scanf("%s", password);

    MyUser.id = id;
    strcpy(MyUser.name, name);
    strcpy(MyUser.password, password);

    FILE* UserFile = fopen("./data/users.txt", "a");
    saveUsersToFile(UserFile, MyUser);
    fclose(UserFile);

    return MyUser;
}