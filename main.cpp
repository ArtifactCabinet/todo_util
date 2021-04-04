#include <iostream>
#include <sqlite3.h>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

sqlite3 *db;

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

void insertDB(const std::string& msg){

};


void check(int rc,  const std::string& msg){
    if( rc ){
        std::cout<<msg<<": "<<sqlite3_errmsg(db)<<std::endl;
        sqlite3_close(db);
        std::exit(1);
    }
}


int main(int argc, char **argv) {

#ifdef DEBUG
    auto dbPath = "todo.db";
#else
    auto dbPath = fs::path(getenv("HOME"))+="/todo.db";
#endif

    std::cout<<dbPath<<std::endl;
    char *errMsg = nullptr;
    int rc;

    rc = sqlite3_open_v2(dbPath, &db,SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, nullptr);

//    rc = sqlite3_exec(db, dropDB, nullptr, nullptr, &errMsg);
//    check(rc,"Cannot delete table");

    rc = sqlite3_exec(db, createDB, nullptr, nullptr, &errMsg);
    check(rc,"Cannot create table");
    std::exit(1);
}
