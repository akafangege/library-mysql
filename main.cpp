#include<bits/stdc++.h>
#include <mysql.h>
using namespace std;
#define len_1 sizeof(book)//用len_1表示book所占内存
#define len_2 sizeof(user)

//结构体定义
typedef struct book_Information {
    char number[20];//书籍编号
    char name[30];//书籍名
    char author[20];//作者
    char press[30];//出版社
    int all;//总量
    int now;//现存余量
    struct book_Information* next;
} book;

typedef struct user_Information {
    int id;//用户编号
    char name[20];//用户昵称
    char account[20];//用户账号
    char password[20];//用户密码
    struct user_Information* next;
} user;


//函数定义
void show_Menu_main(void);//展示主菜单
void show_Menu_root(void);//展示管理员菜单
void show_Menu_user(user* head, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql);//展示用户菜单

book* add_New_book(book* head, MYSQL mysql);//添加新的书籍信息
book* add_Book_again(book* head, MYSQL mysql);//连续添加书籍信息
book* load_Book_information(book* head, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql);//从数据库中加载书籍信息
void show_All_book_Information(book* head);//展示所有的书籍信息
book* delete_Book_information(book* head, MYSQL mysql);//删除书籍信息
void search_Book_information(book* head);//查找书籍信息
int change_Book_information(book* head, MYSQL mysql);//修改书籍信息

user* add_New_user(user* head, MYSQL mysql);//添加新的用户信息
void show_All_user_Information(user* head);//展示所有的用户信息
user* load_User_information(user* head, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql);//从数据库中加载用户信息
int change_User_password(user* head, MYSQL mysql);//修改用户密码
user* user_Login(user* head);//用户登录

void display_Book_remaing(MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql);//显示仍有剩余的书籍

int show_book_borrowed(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head);//显示已借阅的书籍
int borrow_book(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head);//借阅图书
int return_book(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head);//归还图书








void show_Menu_main(void) {
    printf("=======================================\n");
    printf("|            1.管理员登录             |\n");
    printf("|            2.读者登录               |\n");
    printf("|            3.读者注册               |\n");
    printf("|            4.修改密码               |\n");
    printf("|            5.退出本系统             |\n");
    printf("=======================================\n");
}

void show_Menu_root(void) {
    printf("========================\n");
    printf("|    1.添加书籍信息    |\n");
    printf("|    2.显示所有书籍    |\n");
    printf("|    3.查找书籍信息    |\n");
    printf("|    4.删除书籍信息    |\n");
    printf("|    5.修改书籍信息    |\n");
    printf("|    6.显示用户数据    |\n");
    printf("|    9.返回至主菜单    |\n");
    printf("|    0.退出管理系统    |\n");
    printf("========================\n");
}

//展示用户信息
void show_Menu_user(user* head, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql) {
    char sql[300]{};
    int number = 0;
    sprintf_s(sql, sizeof(sql), "select * from borrow_return where u_id = %d;", head->id);
    mysql_query(&mysql, sql);
    res = mysql_store_result(&mysql);
    while (row = mysql_fetch_row(res)) {
        number++;
    }
    printf("%s - 欢迎使用图书借阅系统\n\n", head->name);
    printf("======================================\n");
    printf("|账号                |编号  |已借    |\n");
    printf("|%-20s|%5d |%4.4d 本 |\n", head->account, head->id, number);
    printf("--------------------------------------\n");
    printf("|1.查看可借阅书籍   2.查看已借阅书籍 |\n");
    printf("|3.归还借阅的图书   4.借阅图书       |\n");
    printf("|9.返回主菜单       0.退出借阅系统   |\n");
    printf("======================================\n");
}

//添加新的书籍信息
book* add_New_book(book* head, MYSQL mysql) {
    book* p = NULL, * pr = head, * judge = head;
    p = (book*)malloc(len_1);//动态申请内存
    char sql[300]{};//存放sql命令

    //当p为空（即内存不足），提示内存不足并退出程序
    //注：此函数仅在管理员界面下才提供选择使用
    if (p == NULL) {
        printf("Not enough space!\n");
        exit(1);
    }

    //如果链表头为空，则将p的地址给予它
    if (head == NULL) {
        head = p;
    }
    else {
        //如果不为空，则用while循环找到链表尾部
        while (pr->next != NULL) {
            pr = pr->next;
        }
        //寻找到时将p的地址给予尾部的next指针
        pr->next = p;
    }

    //输入书籍信息
    printf("请输入书籍编号：");
    scanf_s("%s", p->number, 20);

    //检测输入的编号是否重复
    while (judge != p) {//当检测指针未到达p时（到达尾部）
        if (strcmp(p->number,judge->number) == 0) {//比较输入编号与已有编号，相同则不会执行if中的内容
            printf("编号重复，请重新输入\n");
            system("pause");
            system("cls");
            printf("请输入书籍编号：");
            scanf_s("%s", p->number, 20);
            continue;
        }
        judge = judge->next;
    }

    printf("请输入书籍名：");
    scanf_s("%s", p->name, 30);
    printf("请输入作者：");
    scanf_s("%s", p->author, 20);
    printf("请输入出版社：");
    scanf_s("%s", p->press, 30);
    printf("请输入书籍总量：");
    scanf_s("%d", &p->all);
    p->now = p->all;

    //将新增的书籍信息存入数据库
    //int sprintf_s( char *buffer, size_t sizeOfBuffer, const char *format [, argument] ... );
    sprintf_s(sql, sizeof(sql), "insert into book_information values ('%s','%s','%s','%s',%d,%d);", p->number, p->name, p->author, p->press, p->all, p->now);
    mysql_query(&mysql, sql); //向 MySQL 发送并执行 SQL 语句。

    //将p指针的尾部赋NULL
    p->next = NULL;

    return head;
}

//连续添加书籍信息
book* add_Book_again(book* head, MYSQL mysql) {
    int decision = 1;
    while (decision) {
        head = add_New_book(head, mysql);
        printf("========================\n");
        printf("|    是否继续添加？    |\n");
        printf("|  0.取消     1.确定   |\n");
        printf("========================\n");
        scanf_s("%d", &decision);
        system("cls");
    }

    return head;
}

//从数据库中加载书籍信息
book* load_Book_information(book* head, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql) {
    book* p = NULL, * pr = head;

    //查询数据
    mysql_query(&mysql, "select * from book_information;");

    //获取结果集
    res = mysql_store_result(&mysql);
    //mysql_store_result()将查询的全部结果读取到客户端，分配1个MYSQL_RES结构，并将结果置于该结构中。

    //给链表赋值，判断ROW是否为空，不为空就赋值
    while (row = mysql_fetch_row(res)) {
        //检索一个结果集合的下一行
        p = NULL, pr = head;
        p = (book*)malloc(len_1);//动态申请内存
        if (p == NULL) {
            printf("Not enough space!\n");
            exit(1);
        }
        if (head == NULL) {
            head = p;
        }
        else {
            while (pr->next != NULL) {
                pr = pr->next;
            }
            pr->next = p;
        }
        // strcpy_s（*a，strlen(b)+1，*b）函数。
        //首先知道函数用法是讲指针b开始指向的内容复制到a指针，中间那个是保证了复制的内容长度。
        strcpy_s(p->number, 20, row[0]);
        strcpy_s(p->name, 30, row[1]);
        strcpy_s(p->author, 20, row[2]);
        strcpy_s(p->press, 30, row[3]);
        // 把字符串转换成整型数的
        p->all = atoi(row[4]);
        p->now = atoi(row[5]);
        p->next = NULL;
    }

    //释放结果集
    mysql_free_result(res);

    return head;
}

//展示所有的书籍信息
void show_All_book_Information(book* head) {
    book* p = head;
    printf("===================================================================================\n");
    printf("|书名                    |作者            |出版社          |总量  |余量  |编号    |\n");
    while (p != NULL) {
        printf("-----------------------------------------------------------------------------------\n");
        printf("|%-24s|%-16s|%-16s|%-6d|%-6d|%-8s|\n",p->name,p->author,p->press,p->all,p->now,p->number);
        p = p->next;
    }
    printf("===================================================================================\n");
}

//删除书籍信息
book* delete_Book_information(book* head, MYSQL mysql) {
    book* p = head, * pr = NULL;
    int id, decision;
    char sql[300]{};//存放sql语句
    search_Book_information(head);//先通过查找发现编号，再通过编号精确查找
    printf("请输入要删除的书籍的编号：");
    scanf_s("%d", &id);
    while (p != NULL) {
        if (id != atoi(p->number) ){
            pr = p;
            p = p->next;
            continue;
        }
        break;
    }
    if (p == NULL) {
        printf("未查找到此id\n");
        return 0;
    }
    else {
        system("cls");
        printf("已搜索到：\n");
        printf("===================================================================================\n");
        printf("|书名                    |作者            |出版社          |总量  |余量  |编号    |\n");
        printf("-----------------------------------------------------------------------------------\n");
        printf("|%-24s|%-16s|%-16s|%-6d|%-6d|%-8s|\n", p->name, p->author, p->press, p->all, p->now, p->number);
        printf("===================================================================================\n");
    }
    //当all大于now时，说明书籍已被借去一部分，此时不可删除信息，否则会导致归还书籍时产生问题，因此直接提示外借中不可删除
    if (p->all > p->now) {
        printf("该书籍正在外借，不可删除库中信息!\n");
    }
        //若非上述情况，则可继续删除
    else
    {
        printf("================\n");
        printf("|是否确定删除？|\n");
        printf("|0.取消  1.确定|\n");
        printf("================\n");
        scanf_s("%d", &decision);
        while (decision) {
            switch (decision) {
                case 0:
                    printf("已取消删除！\n");
                    break;
                case 1: {
                    //删除数据库内信息
                    sprintf_s(sql, sizeof(sql), "delete from book_information where b_id = %d;", atoi(p->number));
                    mysql_query(&mysql, sql);
                    if (p == head) {
                        head = head->next;
                        free(p);
                    }
                    else {
                        pr->next = p->next;
                        free(p);
                    }
                    printf("已删除!\n");
                    decision = 0;
                }
                    break;
                default:
                    printf("选项输入错误！\n");
                    break;
            }
        }
    }

    return head;
}

//查找书籍信息
void search_Book_information(book* head) {
    book* p = head;
    int decision;
    char search[20]{};//定义字符数组，用来存储后续搜索内容
    printf("============================\n");
    printf("|    请输入要查找的项目    |\n");
    printf("|1.书名   2.作者   3.出版社|\n");
    printf("============================\n");
    scanf_s("%d", &decision);
    while (1) {
        printf("============================\n");
        printf("|    请输入要查找的内容    |\n");
        printf("============================\n");
        scanf_s("%s", search, 20);
        printf("===================================================================================\n");
        printf("|书名                    |作者            |出版社          |总量  |余量  |编号    |\n");
        while (p != NULL) {
            switch (decision) {
                case 1:
                    //char *strstr(const char *haystack, const char *needle)
                    // 在字符串 haystack 中查找第一次出现字符串 needle 的位置
                    // 该函数返回在 haystack 中第一次出现 needle 字符串的位置，如果未找到则返回 null
                    if (strstr(p->name, search) != NULL) {
                        printf("-----------------------------------------------------------------------------------\n");
                        printf("|%-24s|%-16s|%-16s|%-6d|%-6d|%-8s|\n", p->name, p->author, p->press, p->all, p->now, p->number);
                    }
                    break;
                case 2:
                    if (strstr(p->author, search) != NULL) {
                        printf("-----------------------------------------------------------------------------------\n");
                        printf("|%-24s|%-16s|%-16s|%-6d|%-6d|%-8s|\n", p->name, p->author, p->press, p->all, p->now, p->number);
                    }
                    break;
                case 3:
                    if (strstr(p->press, search) != NULL) {
                        printf("-----------------------------------------------------------------------------------\n");
                        printf("|%-24s|%-16s|%-16s|%-6d|%-6d|%-8s|\n", p->name, p->author, p->press, p->all, p->now, p->number);
                    }
                    break;
            }
            p = p->next;
        }
        printf("===================================================================================\n");
        break;
    }
}

//修改书籍信息
int change_Book_information(book* head, MYSQL mysql) {
    int id = 0;
    book* p = head;
    char sql[300]{};
    search_Book_information(head);
    printf("请输入要修改的书籍编号:");
    scanf_s("%d", &id);
    while (p != NULL) {
        if (id != atoi(p->number)) {
            p = p->next;
            continue;
        }
        break;
    }
    if (p == NULL) {
        printf("未查找到此id\n");
        return 0;
    }
    else {
        system("cls");
        printf("已搜索到：\n");
        printf("===================================================================================\n");
        printf("|书名                    |作者            |出版社          |总量  |余量  |编号    |\n");
        printf("-----------------------------------------------------------------------------------\n");
        printf("|%-24s|%-16s|%-16s|%-6d|%-6d|%-8s|\n", p->name, p->author, p->press, p->all, p->now, p->number);
        printf("===================================================================================\n");
        printf("|1.书名            2.作者           3.出版社           4.总量          0.退出修改 |\n");
        printf("===================================================================================\n");
    }
    if (p->all > p->now) {
        printf("该书籍正在外借，不可修改其信息!\n");
    }
    else
    {
        printf("请输入要修改的内容：");
        scanf_s("%d", &id);

        while (id != 0) {
            switch (id) {
                case 1:
                    printf("请输入修改后的书名：");
                    scanf_s("%s", p->name, 30);
                    break;
                case 2:
                    printf("请输入修改后的作者姓名：");
                    scanf_s("%s", p->author, 20);
                    break;
                case 3:
                    printf("请输入修改后的出版社：");
                    scanf_s("%s", p->press, 30);
                    break;
                case 4:
                    printf("请输入修改后的总量：");
                    scanf_s("%d", &p->all);
                    p->now = p->all;
                    break;
                case 0:
                    break;
            }
            printf("修改结束\n");
            printf("请输入选项：");
            scanf_s("%d", &id);
        }
        if (id == 0) {
            //选择0说明退出修改，此时将当前该书籍的信息重新写入数据库内
            sprintf_s(sql, sizeof(sql), "update book_information set b_name = '%s', b_author = '%s', b_press = '%s', b_all = %d, b_now = %d where b_id = %d;", p->name, p->author, p->press, p->all, p->now, atoi(p->number));
            mysql_query(&mysql, sql);
        }
    }

    return 0;
}


//添加新的用户信息
user* add_New_user(user* head, MYSQL mysql) {
    user* p = NULL, * pr = head, * judge = head;
    p = (user*)malloc(len_2);//动态申请内存
    char sql[300]{};//存放sql命令
    if (p == NULL) {
        printf("Not enough space!\n");
        exit(1);
    }
    if (head == NULL) {
        head = p;
    }
    else {
        while (pr->next != NULL) {
            pr = pr->next;
        }
        pr->next = p;
    }

    //输入用户
    printf("请输入账号：");
    scanf_s("%s", p->account, 20);

    //检测输入的账号是否重复
    while (judge != p) {
        if (strcmp(p->account, judge->account) == 0) {
            printf("账号重复，请重新输入\n");
            system("pause");
            system("cls");
            printf("请输入账号：");
            scanf_s("%s", p->account, 20);
            continue;
        }
        judge = judge->next;
    }

    p->id = pr->id + 1;
    printf("请输入昵称：");
    scanf_s("%s", p->name, 20);
    printf("请输入密码：");
    scanf_s("%s", p->password, 20);

    sprintf_s(sql, sizeof(sql), "insert into user_information values (%d,'%s','%s','%s');", p->id, p->name, p->account, p->password);
    mysql_query(&mysql, sql);

    p->next = NULL;

    return head;
}

//展示所有的用户信息
void show_All_user_Information(user* head) {
    user* p = head;
    printf("=========================================================\n");
    printf("|用户编号  |昵称          |账号          |密码          |\n");
    while (p != NULL) {
        printf("---------------------------------------------------------\n");
        printf("|%-10d|%-14s|%-14s|%-14s|\n", p->id, p->name, p->account, p->password);
        p = p->next;
    }
    printf("=========================================================\n");
}

//从数据库中加载用户信息
user* load_User_information(user* head, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql) {
    user* p = NULL, * pr = head;

    mysql_query(&mysql, "select * from user_information;");

    res = mysql_store_result(&mysql);

    while (row = mysql_fetch_row(res)) {
        p = NULL, pr = head;
        p = (user*)malloc(len_2);
        if (p == NULL) {
            printf("Not enough space!\n");
            exit(1);
        }
        if (head == NULL) {
            head = p;
        }
        else {
            while (pr->next != NULL) {
                pr = pr->next;
            }
            pr->next = p;
        }
        p->id = atoi(row[0]);
        strcpy_s(p->name, 20, row[1]);
        strcpy_s(p->account, 20, row[2]);
        strcpy_s(p->password, 20, row[3]);
        p->next = NULL;
    }

    mysql_free_result(res);

    return head;
}

//修改用户密码
int change_User_password(user* head, MYSQL mysql) {
    char name[20]{}, password[20]{}, sql[300]{};
    int decision;
    user* p = head;
    printf("请输入用户昵称:");
    scanf_s("%s", name, 20);
    while (p != NULL) {
        if (strcmp(name, p->name)) {
            p = p->next;
            continue;
        }
        break;
    }
    if (p == NULL) {
        printf("未查找到该用户\n");
        system("pause");
        return 0;
    }
    else {
        system("cls");
        printf("已搜索到：\n");
        printf("==================================================\n");
        printf("|昵称                |账号                |编号  |\n");
        printf("--------------------------------------------------\n");
        printf("|%-20s|%-20s|%6d|\n", p->name, p->account, p->id);
        printf("==================================================\n");
    }
    printf("|1.使用管理员密码修改      2.使用原有密码修改    |\n");
    printf("==================================================\n");

    while (1) {
        printf("请选择修改方式：");
        scanf_s("%d", &decision);
        if (decision == 1) {
            printf("请输入管理员密码：");
            scanf_s("%s", password, 20);
            if (strcmp(password, "root") == 0) {
                printf("请输入新的密码：");
                scanf_s("%s", p->password, 20);
                sprintf_s(sql, sizeof(sql), "update user_information set u_password = '%s' where u_id = %d;", p->password, p->id);
                mysql_query(&mysql, sql);
                break;
            }
            else {
                printf("密码错误！请重试！\n");
                continue;
            }
        }
        if (decision == 2) {
            printf("请输入原有密码：");
            scanf_s("%s", password, 20);
            if (strcmp(password, p->password) == 0) {
                printf("请输入新的密码：");
                scanf_s("%s", p->password, 20);
                sprintf_s(sql, sizeof(sql), "update user_information set u_password = '%s' where u_id = %d;", p->password, p->id);
                mysql_query(&mysql, sql);
                break;
            }
            else {
                printf("密码错误！请重试！\n");
                continue;
            }
        }
        else {
            printf("选项输入错误！请重新输入！\n");
            continue;
        }
    }

    return 1;
}

//用户登陆
user* user_Login(user* head) {
    user* p = head;
    char account[20], password[20];
    printf("请输入账号：");
    scanf_s("%s", account, 20);
    while (p != NULL) {
        if (strcmp(account, p->account)) {
            p = p->next;
            continue;
        }
        break;
    }
    if (p == NULL) {
        printf("系统中无该账号！\n");
        return NULL;
    }
    else
    {
        while (1) {
            printf("请输入密码：");
            scanf_s("%s", password, 20);
            if (strcmp(password, p->password) == 0) {
                printf("成功登录！\n");
                system("pause");
                system("cls");
                break;
            }
            else {
                printf("密码错误！请重新输入！\n");
                system("pause");
                system("cls");
                continue;
            }
        }
    }

    return p;
}

//显示仍有剩余的书籍
void display_Book_remaing(MYSQL_RES* res,MYSQL_ROW row,MYSQL mysql) {
    mysql_query(&mysql, "select * from book_information;");
    res = mysql_store_result(&mysql);
    printf("===================================================================================\n");
    printf("|书名                    |作者            |出版社          |总量  |余量  |编号    |\n");
    while (row = mysql_fetch_row(res)) {
        //若余量大于0则输出
        if (atoi(row[5]) > 0) {
            printf("-----------------------------------------------------------------------------------\n");
            printf("|%-24s|%-16s|%-16s|%-6d|%-6d|%-8s|\n", row[1], row[2], row[3], atoi(row[4]), atoi(row[5]), row[0]);
        }
    }
    printf("===================================================================================\n");

}

//显示已借阅的书籍
int show_book_borrowed(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head) {
    char sql[300]{}, name[24]{};
    book* judge = head;
    sprintf_s(sql, sizeof(sql), "select * from borrow_return where u_id = %d order by return_time asc;", id);
    mysql_query(&mysql, sql);
    res = mysql_store_result(&mysql);
    printf("===========================================================\n");
    printf("|书名                    |借书日期    |最晚归还日期|借阅数|\n");
    while (row = mysql_fetch_row(res)) {
        judge = head;
        while (judge != NULL) {
            if (strcmp(judge->number, row[1]) == 0) {
                strcpy_s(name, 24, judge->name);
            }
            judge = judge->next;
        }
        printf("-----------------------------------------------------------\n");
        printf("|%-24s| %10s | %10s |%4d本|\n",name,row[2],row[3],atoi(row[4]));
    }
    printf("===========================================================\n");

    return 0;
}

//借阅图书
int borrow_book(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head) {
    int i = 0;
    book* judge = head;
    char sql[300]{}, number[20];
    printf("请输入要借阅的书籍编号：");
    scanf_s("%s", number, 20);
    printf("请输入要借阅的本数：");
    scanf_s("%d", &i);
    while (judge != NULL) {
        if (strcmp(number, judge->number) == 0) {
            sprintf_s(sql, sizeof(sql), "select * from book_information where b_id = '%s';", number);
            mysql_query(&mysql, sql);
            res = mysql_store_result(&mysql);
            row = mysql_fetch_row(res);
            if (i > atoi(row[5])) {
                printf("库存量不足！\n");
                return 0;
            }
            else
            {
                sprintf_s(sql, sizeof(sql), "insert into borrow_return values (%d,'%s',curdate(),date_add(curdate(),interval 30 day),%d);", id, number, i);
                mysql_query(&mysql, sql);
                sprintf_s(sql, sizeof(sql), "update book_information set b_now = b_now - %d where b_id = '%s';", i, number);
                mysql_query(&mysql, sql);
                judge->now = judge->now - i;
                printf("借阅成功!\n");
                return 0;
            }
        }
        judge = judge->next;
    }
    if (judge == NULL) {
        printf("图书编号输入错误！\n");
    }

    return 0;
}

//归还图书
int return_book(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head) {
    int i = 0, num = 0;
    book* judge = head;
    char sql[300]{}, number[20];
    printf("请输入要归还的书籍编号：");
    scanf_s("%s", number, 20);
    printf("请输入要归还的本数：");
    scanf_s("%d", &i);
    while (judge != NULL) {
        if (strcmp(number, judge->number) == 0) {
            sprintf_s(sql, sizeof(sql), "select * from borrow_return where u_id = %d and b_id = '%s';", id, number);
            mysql_query(&mysql, sql);
            res = mysql_store_result(&mysql);
            while (row = mysql_fetch_row(res)) {
                num = atoi(row[4]);
            }
            if (i < num) {
                sprintf_s(sql, sizeof(sql), "update book_information set b_now = b_now + %d where b_id = '%s';", i, number);
                mysql_query(&mysql, sql);
                sprintf_s(sql, sizeof(sql), "update borrow_return set state = state - %d where u_id = %d b_id = '%s';", id, i, number);
                mysql_query(&mysql, sql);
                judge->now = judge->now + i;
                printf("归还成功!\n");
                return 0;
            }
            if (i = num) {
                sprintf_s(sql, sizeof(sql), "delete from borrow_return where u_id = %d and b_id = '%s';", id, number);
                mysql_query(&mysql, sql);
                sprintf_s(sql, sizeof(sql), "update borrow_return set state = state - %d where u_id = %d b_id = '%s';", id, i, number);
                mysql_query(&mysql, sql);
                sprintf_s(sql, sizeof(sql), "update book_information set b_now = b_now + %d where b_id = '%s';", i, number);
                mysql_query(&mysql, sql);
                judge->now = judge->now + i;
                printf("归还成功!\n");
                return 0;
            }
            if (i > num) {
                printf("书籍归还错误！您并未借出当前数量书籍！");
                return 0;
            }
        }
        judge = judge->next;
    }
    if (judge == NULL) {
        printf("图书编号输入错误！\n");
    }

    return 0;
}
int main() {
    int decision,decision_1{};
    char temporary[20]{};
    user* p = NULL;
    book* book_Head = NULL;//定义书籍头指针
    user* user_Head = NULL;//定义读者头指针
    system("color 3F");//设置窗口显示颜色

    MYSQL mysql;//数据库句柄
    MYSQL_RES* res = nullptr;//查询结果集
    MYSQL_ROW row = nullptr;//记录结构体

    //初始化数据库
    mysql_init(&mysql);

    //设置字符编码
    mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

    //连接数据库
    if (mysql_real_connect(&mysql, "127.0.0.1"/*主机地址*/, "root", "123456"/*数据库的密码*/, "library"/*要使用的数据库*/, 3306, NULL, 0) == NULL) {
        printf("错误原因：%s\n", mysql_error(&mysql));
        printf("连接失败！\n");
        exit(-1);
    }

    book_Head = load_Book_information(book_Head, res, row, mysql);//加载数据库中的书籍信息
    user_Head = load_User_information(user_Head, res, row, mysql);//加载数据库中的用户信息

    while (1) {
        show_Menu_main();
        printf("请输入选项：");
        scanf_s("%d", &decision);
        while (decision == 1) {
            system("cls"); //清屏
            printf("请输入管理员账号：");
            scanf_s("%s", temporary, 20);
            if (strcmp(temporary, "hmf")) {
                printf("账号输入错误！\n");
                continue;
            }
            else {
                printf("请输入管理员密码：");
                scanf_s("%s", temporary, 20);
                if (strcmp(temporary, "root")) {
                    printf("密码输入错误！\n");
                    continue;
                }
                else {
                    decision_1 = 1;//登录成功后将decision_1默认赋值为0以供后续使用
                    printf("登录成功!\n");
                    system("pause");
                    system("cls");
                    show_Menu_root();
                    printf("请输入选项：");
                    scanf_s("%d", &decision);
                    while (1) {
                        system("cls");
                        switch (decision) {
                            case 1:
                                book_Head = add_Book_again(book_Head, mysql);
                                break;
                            case 2:
                                show_All_book_Information(book_Head);
                                break;
                            case 3:
                                search_Book_information(book_Head);
                                break;
                            case 4:
                                book_Head = delete_Book_information(book_Head, mysql);
                                break;
                            case 5:
                                change_Book_information(book_Head, mysql);
                                break;
                            case 6:
                                show_All_user_Information(user_Head);
                                break;
                            case 9:
                                decision_1 = 0;//在此处修改变量decision_1
                                break;
                            case 0:
                                printf("已退出本系统");
                                return 0;
                            default:
                                printf("无效选项，请重新选择");
                                break;
                        }
                        system("pause");
                        system("cls");
                        if (decision_1 == 0) {
                            break;//若在选择时选择7返回主菜单，则会break出当前while回到开始菜单
                        }
                        show_Menu_root();
                        printf("请输入选项：");
                        scanf_s("%d", &decision);
                    }
                }
            }

        }
        while (decision == 2) {
            decision_1 = 1;
            p = user_Login(user_Head);
            if (p == NULL) {//若p等于NULL说明输入账号错误，将重新进行循环
                system("pause");
                system("cls");
                continue;
            }
            while (1) {
                show_Menu_user(p, res, row, mysql);
                printf("请输入选项：");
                scanf_s("%d", &decision);
                while (1) {
                    system("cls");
                    switch (decision) {
                        case 1:
                            display_Book_remaing(res, row, mysql);
                            break;
                        case 2:
                            show_book_borrowed(p->id, res, row, mysql, book_Head);
                            break;
                        case 3:
                            return_book(p->id, res, row, mysql, book_Head);
                            break;
                        case 4:
                            borrow_book(p->id, res, row, mysql, book_Head);
                            break;
                        case 9:
                            decision_1 = 0;
                            break;
                        case 0:
                            printf("已退出本系统");
                            return 0;
                        default:
                            printf("无效选项，请重新选择");
                            break;
                    }
                    system("pause");
                    system("cls");
                    if (decision_1 == 0) {
                        break;
                    }
                    show_Menu_user(p, res, row, mysql);
                    printf("请输入选项：");
                    scanf_s("%d", &decision);
                }
                if (decision_1 == 0) {
                    break;
                }
            }
        }
        while (decision == 3) {
            add_New_user(user_Head, mysql);
            break;
        }
        while (decision == 4) {
            change_User_password(user_Head, mysql);
            break;
        }
        while (decision == 5) {
            printf("已退出本系统");
            system("pause");
            return 0;
        }
        system("cls");
    }


    //关闭数据库
    mysql_close(&mysql);

    return 0;
}
