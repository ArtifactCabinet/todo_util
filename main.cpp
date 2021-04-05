#include <iostream>
#include <sqlite3.h>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

sqlite3 *db;

//Globally shared variable name,
//As this is a one thread program

int rc; // return code
sqlite3_stmt *stmt;

void check(int irc,  const std::string& msg){
    if(irc){
        std::cout<<msg<<": "<<sqlite3_errmsg(db)<<std::endl;
    }
}

// [SQL stmts] -----------------------------------------

const char *createDB = R"(
create table if not exists todo
(
addTime TEXT not null,
MESSAGE TEXT not null
);
)";

const char*dropDB =R"(
DROP TABLE IF EXISTS todo;
)";

const char*insert =R"(
    INSERT INTO todo VALUES(datetime('now'), ? );"
)";


// [Subroutines] -----------------------------------------

void insertDB(const char *msg){
    sqlite3_prepare_v2(db,insert,-1,&stmt, nullptr);
    sqlite3_bind_text(stmt,
                      1,
                      msg,
                      -1,
                      nullptr);

    rc = sqlite3_step(stmt);
    sqlite3_reset(stmt);
};

void printDB(){
    rc = sqlite3_prepare_v2(db, "SELECT * from todo", -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        int i;
        int num_cols = sqlite3_column_count(stmt);

        for (i = 0; i < num_cols; i++)
        {
            switch (sqlite3_column_type(stmt, i))
            {
                case (SQLITE3_TEXT):
                    printf("%s, ", sqlite3_column_text(stmt, i));
                    break;
                case (SQLITE_INTEGER):
                    printf("%d, ", sqlite3_column_int(stmt, i));
                    break;
                case (SQLITE_FLOAT):
                    printf("%g, ", sqlite3_column_double(stmt, i));
                    break;
                default:
                    break;
            }
        }
        printf("\n");
    }

    sqlite3_reset(stmt);
}


int main(int argc, char **argv) {

#ifdef DEBUG
    auto dbPath = "todo.db";
#else
    auto dbPath = fs::path(getenv("HOME"))+="/todo.db";
#endif


    std::cout<<dbPath<<std::endl;
    char *errMsg = nullptr;

    rc = sqlite3_open_v2(dbPath, &db,SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, nullptr);

//    rc = sqlite3_exec(db, dropDB, nullptr, nullptr, &errMsg);
//    check(rc,"Cannot delete table");

    rc = sqlite3_exec(db, createDB, nullptr, nullptr, &errMsg);
    check(rc,"Cannot create table");

    insertDB("what");

    printDB();

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::exit(1);
}
