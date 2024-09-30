#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

void list_matches(MYSQL *conn) {
    if (mysql_query(conn, "SELECT m.match_id, m.finish_time, m.duration, p.username FROM Match m "
                          "JOIN Player_Match pm ON m.match_id = pm.match_id "
                          "JOIN Player p ON pm.player_id = p.player_id")) {
        fprintf(stderr, "SELECT * failed. Error: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        return;
    }

    printf("Match ID | Finish Time        | Duration | Player\n");
    printf("-----------------------------------------------------\n");

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        printf("%s       | %s | %s       | %s\n",
               row[0], row[1], row[2], row[3]);
    }

    mysql_free_result(res);
}

int main() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return EXIT_FAILURE;
    }

    if (mysql_real_connect(conn, "localhost", "your_username", "your_password", "GameDB", 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    list_matches(conn);

    mysql_close(conn);
    return EXIT_SUCCESS;
}
