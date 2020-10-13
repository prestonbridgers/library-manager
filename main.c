#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql.h>

void printDatabases(MYSQL *db, MYSQL_RES *res)
{
    MYSQL_ROW row;
    int count = 0;
    int num_fields;
    size_t i;

    res = mysql_list_dbs(db, NULL);
    if (res == NULL)
    {
        fprintf(stderr, "Error occured listing databases!\n");
        return;
    }
    
    printf("Printing Databases:\n");
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        num_fields = mysql_num_fields(res);
        printf("%d) ", ++count);
        for (i = 0; i < num_fields; i++)
            printf("%s ", row[i]);
        printf("\n");
    }

    mysql_free_result(res);
}

int main(void)
{
    // Vars
    MYSQL_RES *res = NULL;
    char *database = "C_DataBase";
    char *user = "root";
    char *pass = "adamsandler1";
    
    // Initializing a new mysql struct
    MYSQL *db = mysql_init(NULL);
    if (db == NULL)
    {
        fprintf(stderr, "Error: Couldn't init mysql struct!\n");
        return 1;
    }

    // Connecting to the MySQL server running on localhost
    MYSQL *db_con = mysql_real_connect(db, "localhost", user,
                                       pass, NULL, 0,
                                       NULL, 0);
    if (db_con != db) fprintf(stderr, "Couldn't establish connection!\n");

    // Printing a list of databases on the server
    printDatabases(db_con, res);

    // Selecting a database to use
    char use_teaminfo[255];
    sprintf(use_teaminfo, "USE %s", database);
    mysql_real_query(db_con, use_teaminfo, strlen(use_teaminfo));
    printf("\nUsing %s\n", database);

    // Selecting records from the myTeam table
    char *select = "SELECT * FROM players";
    mysql_real_query(db_con, select, strlen(select));

    // Using the result of the last query to print
    printf("\nPrinting all records from myTeam:\n");
    res = mysql_use_result(db_con);
    MYSQL_ROW row;
    int num_fields;
    for (; (row = mysql_fetch_row(res)) != NULL;)
    {
        num_fields = mysql_num_fields(res);
        for (int i = 0; i < num_fields; i++)
            if (i < num_fields - 1) 
                printf("%s\t| ", row[i]);
            else    
                printf("%s", row[i]);
        printf("\n");
    }
    mysql_free_result(res);

    // Cleanup
    mysql_library_end();
    return 0;
}
