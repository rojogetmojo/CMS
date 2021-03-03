#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// All the fields are stored as string with no larger than 50 characters.
#define MAX_STR_LEN 51

// The maximum number of contacts the system can manage is 500.
#define MAX_CON_LEN 500

struct contact
{
    int key;
    char firstName[MAX_STR_LEN];
    char lastName[MAX_STR_LEN];
    char phoneNo[MAX_STR_LEN];
    char email[MAX_STR_LEN];
};

typedef struct contact contact_t;
typedef struct contact contacts_t[MAX_CON_LEN];

/* Function prototypes for functions */
int add_contact(contacts_t contacts);
int delete_contact(contacts_t contacts, int key);
int update_contact(contacts_t contacts, int key);
void search_contact(contacts_t contacts, char s[MAX_STR_LEN]);
int import_from_file(contacts_t contacts, char *fileName);
int export_to_file(contacts_t contacts, char *fileName);
void view_all_contacts(contacts_t contacts);
void exit_system(contacts_t contacts);

/* Function prototypes for helper functions */
static int valid_name(const char name[MAX_STR_LEN]);
static int valid_phoneNo(const char phoneNo[MAX_STR_LEN]);
static int valid_email(const char email[MAX_STR_LEN]);
static contact_t get_info(void);
static int get_length(contacts_t contacts);
static int is_full(contacts_t contacts);
static int is_empty(contacts_t contacts);
static void print_contact(contact_t contact);
static int cmpfunc(const void *a, const void *b);

int main(void)
{
    // Create contacts list
    contacts_t contacts;
    for (int i = 0; i < MAX_CON_LEN; i++)
    {
        contacts[i].key = -1;
    }

    import_from_file(contacts, "contact.txt");
    printf("%d contacts loaded from contact.txt\n", get_length(contacts));

    printf("==========Menu==========\n");
    printf("1 - Add a contact\n");
    printf("2 - Delete a contact\n");
    printf("3 - Update a contact\n");
    printf("4 - Search contacts\n");
    printf("5 - Import from file\n");
    printf("6 - Export to file\n");
    printf("7 - View all contacts\n");
    printf("0 - Exit\n");

    int choice = -1;
    int key = -1;
    char s[MAX_STR_LEN];
    while (choice != 0)
    {
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                add_contact(contacts);
                break;
            case 2:
                printf("Please input a contact key to delete.\n");
                scanf("%d", &key);
                delete_contact(contacts, key);
                break;
            case 3:
                printf("Please input a contact key to update.\n");
                scanf("%d", &key);
                update_contact(contacts, key);
                break;
            case 4:
                printf("Please input a string you want to search.\n");
                scanf("%s", &s);
                search_contact(contacts, s);
                break;
            case 5:
                printf("Please input a file name to import.\n");
                scanf("%s", &s);
                import_from_file(contacts, s);
                break;
            case 6:
                printf("Please input a file name to export.\n");
                scanf("%s", &s);
                export_to_file(contacts, s);
                break;
            case 7:
                view_all_contacts(contacts);

            default:
                break;
        }
    }
    exit_system(contacts);

    return 0;
}

int add_contact(contacts_t contacts)
{
    if (is_full(contacts))
    {
        printf("Fail to add. Contact list is full.\n");
        return 0;
    }

    contact_t con = get_info();
    for (int i = 0; i < MAX_CON_LEN; i++)
    {
        if (contacts[i].key == -1)
        {
            contacts[i] = con;
            contacts[i].key = i;
            break;
        }
    }

    printf("Success to add.\n");
    return 1;
}

int delete_contact(contacts_t contacts, int key)
{
    for (int i = 0; i < MAX_CON_LEN; i++)
    {
        if (contacts[i].key != -1 && i == key)
        {
            contacts[i].key = -1;
            printf("Success to delete.\n");
            return 1;
        }
    }

    printf("No such contact.\n");
    return 0;
}

int update_contact(contacts_t contacts, int key)
{
    for (int i = 0; i < MAX_CON_LEN; i++)
    {
        if (contacts[i].key != -1 && i == key)
        {
            printf("1-firstName, 2-lastName, 3-phoneNo, 4-email\n");
            int field = -1;
            scanf("%d", &field);

            int valid_flag = 0;
            switch (field)
            {
                case 1:
                    scanf("%s", &contacts[i].firstName);
                    while (valid_flag == 0)
                    {
                        if (!valid_name(contacts[i].firstName))
                        {
                            printf("<first name> is invalid beacuse first name must be letters.\n");
                            printf("Please input <first name> again.\n");
                            scanf("%s", &contacts[i].firstName);
                            continue;
                        }
                        valid_flag = 1;
                    }
                    break;

                case 2:
                    scanf("%s", &contacts[i].lastName);
                    while (valid_flag == 0)
                    {
                        if (!valid_name(contacts[i].lastName))
                        {
                            printf("<last name> is invalid beacuse last name must be letters.\n");
                            printf("Please input <last name> again.\n");
                            scanf("%s", &contacts[i].lastName);
                            continue;
                        }
                        valid_flag = 1;
                    }
                    break;

                case 3:
                    scanf("%s", &contacts[i].phoneNo);
                    while (valid_flag == 0)
                    {
                        if (!valid_phoneNo(contacts[i].phoneNo))
                        {
                            printf("<phone number> is invalid beacuse phone number must are 8 digits.\n");
                            printf("Please input <phone number> again.\n");
                            scanf("%s", &contacts[i].phoneNo);
                            continue;
                        }
                        valid_flag = 1;
                    }
                    break;

                case 4:
                    scanf("%s", &contacts[i].email);
                    while (valid_flag == 0)
                    {
                        if (!valid_email(contacts[i].email))
                        {
                            printf("<email> is invalid beacuse email must contain @.\n");
                            printf("Please input <email> again.\n");
                            scanf("%s", &contacts[i].email);
                            continue;
                        }
                        valid_flag = 1;
                    }
                    break;
                default:
                    break;
            }

            printf("Success to update.\n");
            return 1;
        }
    }

    printf("No such contact.\n");
    return 0;
}

void search_contact(contacts_t contacts, char s[MAX_STR_LEN])
{
    printf("contact key\tfirst name\tlast name\tphone number\temail\n");
    for (int i = 0; i < MAX_CON_LEN; i++)
    {
        int info_match = (strcmp(contacts[i].firstName, s) == 0) ||
                         (strcmp(contacts[i].lastName, s) == 0) ||
                         (strcmp(contacts[i].phoneNo, s) == 0) ||
                         (strcmp(contacts[i].email, s) == 0);
        if (contacts[i].key != -1 && info_match)
        {
            print_contact(contacts[i]);
        }
    }
    printf("That's all.\n");
}

int import_from_file(contacts_t contacts, char *fileName)
{
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        return 0;
    }

    int n;
    fscanf(fp, "%d", &n);
    for (int i = 0; i < n; i++)
    {
        contacts[i].key = i;
        fscanf(fp, "%s %s %s %s\n", contacts[i].firstName, contacts[i].lastName,
               contacts[i].phoneNo, contacts[i].email);
    }

    fclose(fp);
    return 1;
}

int export_to_file(contacts_t contacts, char *fileName)
{
    FILE *fp = fopen(fileName, "w");
    if (fp == NULL)
    {
        printf("Error to export\n");
        return 0;
    }

    int n = get_length(contacts);
    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%s %s %s %s\n", contacts[i].firstName, contacts[i].lastName,
                contacts[i].phoneNo, contacts[i].email);
    }

    fclose(fp);
    return 1;
}

void view_all_contacts(contacts_t contacts)
{
    int len = get_length(contacts);
    contact_t tmp[len];
    for (int i = 0, j = 0; i < MAX_CON_LEN; i++)
    {
        if (contacts[i].key != -1)
        {
            tmp[j].key = j;
            tmp[j] = contacts[i];
            j++;
        }
    }

    qsort(tmp, len, sizeof(contact_t), cmpfunc);

    printf("contact key\tfirst name\tlast name\tphone number\temail\n");
    for (int i = 0; i < len; i++)
    {
        if (tmp[i].key != -1)
        {
            print_contact(tmp[i]);
        }
    }
    printf("That's all.\n");
}

void exit_system(contacts_t contacts)
{
    export_to_file(contacts, "latest.txt");
}

/* Helper Functions */

static int valid_name(const char name[MAX_STR_LEN])
{
    for (int i = 0; name[i] != '\0'; i++)
    {
        if (!isalpha(name[i]))
        {
            return 0;
        }
    }
    return 1;
}

static int valid_phoneNo(const char phoneNo[MAX_STR_LEN])
{
    int i;
    for (i = 0; phoneNo[i] != '\0'; i++)
    {
        if (!isdigit(phoneNo[i]))
        {
            return 0;
        }
    }
    if (i != 8)
    {
        return 0;
    }
    return 1;
}

static int valid_email(const char email[MAX_STR_LEN])
{
    if (strstr(email, "@") == NULL)
    {
        return 0;
    }
    return 1;
}

static contact_t get_info(void)
{
    contact_t con;
    printf("Please input first name, last name, phone number and email separated by spaces.\n");
    scanf("%s %s %s %s", &con.firstName, &con.lastName, &con.phoneNo, &con.email);
    int valid_flag = 0;
    while (valid_flag == 0)
    {
        if (!valid_name(con.firstName))
        {
            printf("<first name> is invalid beacuse first name must be letters.\n");
            printf("Please input <first name> again.\n");
            scanf("%s", &con.firstName);
            continue;
        }
        if (!valid_name(con.lastName))
        {
            printf("<last name> is invalid beacuse last name must be letters.\n");
            printf("Please input <last name> again.\n");
            scanf("%s", &con.lastName);
            continue;
        }
        if (!valid_phoneNo(con.phoneNo))
        {
            printf("<phone number> is invalid beacuse phone number must are 8 digits.\n");
            printf("Please input <phone number> again.\n");
            scanf("%s", &con.phoneNo);
            continue;
        }
        if (!valid_email(con.email))
        {
            printf("<email> is invalid beacuse email must contain @.\n");
            printf("Please input <email> again.\n");
            scanf("%s", &con.email);
            continue;
        }
        valid_flag = 1;
    }
    return con;
}

static int get_length(contacts_t contacts)
{
    // len: 0 ~ MAX_CON_LEN
    int len = 0;
    for (int i = 0; i < MAX_CON_LEN; i++)
    {
        if (contacts[i].key != -1)
        {
            len++;
        }
    }
    return len;
}

static int is_full(contacts_t contacts)
{
    return get_length(contacts) >= MAX_CON_LEN;
}

static int is_empty(contacts_t contacts)
{
    return get_length(contacts) <= 0;
}

static void print_contact(contact_t contact)
{
    printf("%11d\t%10s\t%9s\t%12s\t%5s\n", contact.key, contact.firstName, contact.lastName,
           contact.phoneNo, contact.email);
}

static int cmpfunc(const void *a, const void *b)
{
    char *name1 = ((contact_t *)a)->firstName;
    char *name2 = ((contact_t *)b)->firstName;
    return strcmp(name1, name2);
}
