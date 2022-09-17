#include<bits/stdc++.h>
#include <mysql.h>
using namespace std;
#define len_1 sizeof(book)//��len_1��ʾbook��ռ�ڴ�
#define len_2 sizeof(user)

//�ṹ�嶨��
typedef struct book_Information {
    char number[20];//�鼮���
    char name[30];//�鼮��
    char author[20];//����
    char press[30];//������
    int all;//����
    int now;//�ִ�����
    struct book_Information* next;
} book;

typedef struct user_Information {
    int id;//�û����
    char name[20];//�û��ǳ�
    char account[20];//�û��˺�
    char password[20];//�û�����
    struct user_Information* next;
} user;


//��������
void show_Menu_main(void);//չʾ���˵�
void show_Menu_root(void);//չʾ����Ա�˵�
void show_Menu_user(user* head, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql);//չʾ�û��˵�

book* add_New_book(book* head, MYSQL mysql);//����µ��鼮��Ϣ
book* add_Book_again(book* head, MYSQL mysql);//��������鼮��Ϣ
book* load_Book_information(book* head, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql);//�����ݿ��м����鼮��Ϣ
void show_All_book_Information(book* head);//չʾ���е��鼮��Ϣ
book* delete_Book_information(book* head, MYSQL mysql);//ɾ���鼮��Ϣ
void search_Book_information(book* head);//�����鼮��Ϣ
int change_Book_information(book* head, MYSQL mysql);//�޸��鼮��Ϣ

user* add_New_user(user* head, MYSQL mysql);//����µ��û���Ϣ
void show_All_user_Information(user* head);//չʾ���е��û���Ϣ
user* load_User_information(user* head, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql);//�����ݿ��м����û���Ϣ
int change_User_password(user* head, MYSQL mysql);//�޸��û�����
user* user_Login(user* head);//�û���¼

void display_Book_remaing(MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql);//��ʾ����ʣ����鼮

int show_book_borrowed(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head);//��ʾ�ѽ��ĵ��鼮
int borrow_book(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head);//����ͼ��
int return_book(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head);//�黹ͼ��








void show_Menu_main(void) {
    printf("=======================================\n");
    printf("|            1.����Ա��¼             |\n");
    printf("|            2.���ߵ�¼               |\n");
    printf("|            3.����ע��               |\n");
    printf("|            4.�޸�����               |\n");
    printf("|            5.�˳���ϵͳ             |\n");
    printf("=======================================\n");
}

void show_Menu_root(void) {
    printf("========================\n");
    printf("|    1.����鼮��Ϣ    |\n");
    printf("|    2.��ʾ�����鼮    |\n");
    printf("|    3.�����鼮��Ϣ    |\n");
    printf("|    4.ɾ���鼮��Ϣ    |\n");
    printf("|    5.�޸��鼮��Ϣ    |\n");
    printf("|    6.��ʾ�û�����    |\n");
    printf("|    9.���������˵�    |\n");
    printf("|    0.�˳�����ϵͳ    |\n");
    printf("========================\n");
}

//չʾ�û���Ϣ
void show_Menu_user(user* head, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql) {
    char sql[300]{};
    int number = 0;
    sprintf_s(sql, sizeof(sql), "select * from borrow_return where u_id = %d;", head->id);
    mysql_query(&mysql, sql);
    res = mysql_store_result(&mysql);
    while (row = mysql_fetch_row(res)) {
        number++;
    }
    printf("%s - ��ӭʹ��ͼ�����ϵͳ\n\n", head->name);
    printf("======================================\n");
    printf("|�˺�                |���  |�ѽ�    |\n");
    printf("|%-20s|%5d |%4.4d �� |\n", head->account, head->id, number);
    printf("--------------------------------------\n");
    printf("|1.�鿴�ɽ����鼮   2.�鿴�ѽ����鼮 |\n");
    printf("|3.�黹���ĵ�ͼ��   4.����ͼ��       |\n");
    printf("|9.�������˵�       0.�˳�����ϵͳ   |\n");
    printf("======================================\n");
}

//����µ��鼮��Ϣ
book* add_New_book(book* head, MYSQL mysql) {
    book* p = NULL, * pr = head, * judge = head;
    p = (book*)malloc(len_1);//��̬�����ڴ�
    char sql[300]{};//���sql����

    //��pΪ�գ����ڴ治�㣩����ʾ�ڴ治�㲢�˳�����
    //ע���˺������ڹ���Ա�����²��ṩѡ��ʹ��
    if (p == NULL) {
        printf("Not enough space!\n");
        exit(1);
    }

    //�������ͷΪ�գ���p�ĵ�ַ������
    if (head == NULL) {
        head = p;
    }
    else {
        //�����Ϊ�գ�����whileѭ���ҵ�����β��
        while (pr->next != NULL) {
            pr = pr->next;
        }
        //Ѱ�ҵ�ʱ��p�ĵ�ַ����β����nextָ��
        pr->next = p;
    }

    //�����鼮��Ϣ
    printf("�������鼮��ţ�");
    scanf_s("%s", p->number, 20);

    //�������ı���Ƿ��ظ�
    while (judge != p) {//�����ָ��δ����pʱ������β����
        if (strcmp(p->number,judge->number) == 0) {//�Ƚ������������б�ţ���ͬ�򲻻�ִ��if�е�����
            printf("����ظ�������������\n");
            system("pause");
            system("cls");
            printf("�������鼮��ţ�");
            scanf_s("%s", p->number, 20);
            continue;
        }
        judge = judge->next;
    }

    printf("�������鼮����");
    scanf_s("%s", p->name, 30);
    printf("���������ߣ�");
    scanf_s("%s", p->author, 20);
    printf("����������磺");
    scanf_s("%s", p->press, 30);
    printf("�������鼮������");
    scanf_s("%d", &p->all);
    p->now = p->all;

    //���������鼮��Ϣ�������ݿ�
    //int sprintf_s( char *buffer, size_t sizeOfBuffer, const char *format [, argument] ... );
    sprintf_s(sql, sizeof(sql), "insert into book_information values ('%s','%s','%s','%s',%d,%d);", p->number, p->name, p->author, p->press, p->all, p->now);
    mysql_query(&mysql, sql); //�� MySQL ���Ͳ�ִ�� SQL ��䡣

    //��pָ���β����NULL
    p->next = NULL;

    return head;
}

//��������鼮��Ϣ
book* add_Book_again(book* head, MYSQL mysql) {
    int decision = 1;
    while (decision) {
        head = add_New_book(head, mysql);
        printf("========================\n");
        printf("|    �Ƿ������ӣ�    |\n");
        printf("|  0.ȡ��     1.ȷ��   |\n");
        printf("========================\n");
        scanf_s("%d", &decision);
        system("cls");
    }

    return head;
}

//�����ݿ��м����鼮��Ϣ
book* load_Book_information(book* head, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql) {
    book* p = NULL, * pr = head;

    //��ѯ����
    mysql_query(&mysql, "select * from book_information;");

    //��ȡ�����
    res = mysql_store_result(&mysql);
    //mysql_store_result()����ѯ��ȫ�������ȡ���ͻ��ˣ�����1��MYSQL_RES�ṹ������������ڸýṹ�С�

    //������ֵ���ж�ROW�Ƿ�Ϊ�գ���Ϊ�վ͸�ֵ
    while (row = mysql_fetch_row(res)) {
        //����һ��������ϵ���һ��
        p = NULL, pr = head;
        p = (book*)malloc(len_1);//��̬�����ڴ�
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
        // strcpy_s��*a��strlen(b)+1��*b��������
        //����֪�������÷��ǽ�ָ��b��ʼָ������ݸ��Ƶ�aָ�룬�м��Ǹ��Ǳ�֤�˸��Ƶ����ݳ��ȡ�
        strcpy_s(p->number, 20, row[0]);
        strcpy_s(p->name, 30, row[1]);
        strcpy_s(p->author, 20, row[2]);
        strcpy_s(p->press, 30, row[3]);
        // ���ַ���ת������������
        p->all = atoi(row[4]);
        p->now = atoi(row[5]);
        p->next = NULL;
    }

    //�ͷŽ����
    mysql_free_result(res);

    return head;
}

//չʾ���е��鼮��Ϣ
void show_All_book_Information(book* head) {
    book* p = head;
    printf("===================================================================================\n");
    printf("|����                    |����            |������          |����  |����  |���    |\n");
    while (p != NULL) {
        printf("-----------------------------------------------------------------------------------\n");
        printf("|%-24s|%-16s|%-16s|%-6d|%-6d|%-8s|\n",p->name,p->author,p->press,p->all,p->now,p->number);
        p = p->next;
    }
    printf("===================================================================================\n");
}

//ɾ���鼮��Ϣ
book* delete_Book_information(book* head, MYSQL mysql) {
    book* p = head, * pr = NULL;
    int id, decision;
    char sql[300]{};//���sql���
    search_Book_information(head);//��ͨ�����ҷ��ֱ�ţ���ͨ����ž�ȷ����
    printf("������Ҫɾ�����鼮�ı�ţ�");
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
        printf("δ���ҵ���id\n");
        return 0;
    }
    else {
        system("cls");
        printf("����������\n");
        printf("===================================================================================\n");
        printf("|����                    |����            |������          |����  |����  |���    |\n");
        printf("-----------------------------------------------------------------------------------\n");
        printf("|%-24s|%-16s|%-16s|%-6d|%-6d|%-8s|\n", p->name, p->author, p->press, p->all, p->now, p->number);
        printf("===================================================================================\n");
    }
    //��all����nowʱ��˵���鼮�ѱ���ȥһ���֣���ʱ����ɾ����Ϣ������ᵼ�¹黹�鼮ʱ�������⣬���ֱ����ʾ����в���ɾ��
    if (p->all > p->now) {
        printf("���鼮������裬����ɾ��������Ϣ!\n");
    }
        //���������������ɼ���ɾ��
    else
    {
        printf("================\n");
        printf("|�Ƿ�ȷ��ɾ����|\n");
        printf("|0.ȡ��  1.ȷ��|\n");
        printf("================\n");
        scanf_s("%d", &decision);
        while (decision) {
            switch (decision) {
                case 0:
                    printf("��ȡ��ɾ����\n");
                    break;
                case 1: {
                    //ɾ�����ݿ�����Ϣ
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
                    printf("��ɾ��!\n");
                    decision = 0;
                }
                    break;
                default:
                    printf("ѡ���������\n");
                    break;
            }
        }
    }

    return head;
}

//�����鼮��Ϣ
void search_Book_information(book* head) {
    book* p = head;
    int decision;
    char search[20]{};//�����ַ����飬�����洢������������
    printf("============================\n");
    printf("|    ������Ҫ���ҵ���Ŀ    |\n");
    printf("|1.����   2.����   3.������|\n");
    printf("============================\n");
    scanf_s("%d", &decision);
    while (1) {
        printf("============================\n");
        printf("|    ������Ҫ���ҵ�����    |\n");
        printf("============================\n");
        scanf_s("%s", search, 20);
        printf("===================================================================================\n");
        printf("|����                    |����            |������          |����  |����  |���    |\n");
        while (p != NULL) {
            switch (decision) {
                case 1:
                    //char *strstr(const char *haystack, const char *needle)
                    // ���ַ��� haystack �в��ҵ�һ�γ����ַ��� needle ��λ��
                    // �ú��������� haystack �е�һ�γ��� needle �ַ�����λ�ã����δ�ҵ��򷵻� null
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

//�޸��鼮��Ϣ
int change_Book_information(book* head, MYSQL mysql) {
    int id = 0;
    book* p = head;
    char sql[300]{};
    search_Book_information(head);
    printf("������Ҫ�޸ĵ��鼮���:");
    scanf_s("%d", &id);
    while (p != NULL) {
        if (id != atoi(p->number)) {
            p = p->next;
            continue;
        }
        break;
    }
    if (p == NULL) {
        printf("δ���ҵ���id\n");
        return 0;
    }
    else {
        system("cls");
        printf("����������\n");
        printf("===================================================================================\n");
        printf("|����                    |����            |������          |����  |����  |���    |\n");
        printf("-----------------------------------------------------------------------------------\n");
        printf("|%-24s|%-16s|%-16s|%-6d|%-6d|%-8s|\n", p->name, p->author, p->press, p->all, p->now, p->number);
        printf("===================================================================================\n");
        printf("|1.����            2.����           3.������           4.����          0.�˳��޸� |\n");
        printf("===================================================================================\n");
    }
    if (p->all > p->now) {
        printf("���鼮������裬�����޸�����Ϣ!\n");
    }
    else
    {
        printf("������Ҫ�޸ĵ����ݣ�");
        scanf_s("%d", &id);

        while (id != 0) {
            switch (id) {
                case 1:
                    printf("�������޸ĺ��������");
                    scanf_s("%s", p->name, 30);
                    break;
                case 2:
                    printf("�������޸ĺ������������");
                    scanf_s("%s", p->author, 20);
                    break;
                case 3:
                    printf("�������޸ĺ�ĳ����磺");
                    scanf_s("%s", p->press, 30);
                    break;
                case 4:
                    printf("�������޸ĺ��������");
                    scanf_s("%d", &p->all);
                    p->now = p->all;
                    break;
                case 0:
                    break;
            }
            printf("�޸Ľ���\n");
            printf("������ѡ�");
            scanf_s("%d", &id);
        }
        if (id == 0) {
            //ѡ��0˵���˳��޸ģ���ʱ����ǰ���鼮����Ϣ����д�����ݿ���
            sprintf_s(sql, sizeof(sql), "update book_information set b_name = '%s', b_author = '%s', b_press = '%s', b_all = %d, b_now = %d where b_id = %d;", p->name, p->author, p->press, p->all, p->now, atoi(p->number));
            mysql_query(&mysql, sql);
        }
    }

    return 0;
}


//����µ��û���Ϣ
user* add_New_user(user* head, MYSQL mysql) {
    user* p = NULL, * pr = head, * judge = head;
    p = (user*)malloc(len_2);//��̬�����ڴ�
    char sql[300]{};//���sql����
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

    //�����û�
    printf("�������˺ţ�");
    scanf_s("%s", p->account, 20);

    //���������˺��Ƿ��ظ�
    while (judge != p) {
        if (strcmp(p->account, judge->account) == 0) {
            printf("�˺��ظ�������������\n");
            system("pause");
            system("cls");
            printf("�������˺ţ�");
            scanf_s("%s", p->account, 20);
            continue;
        }
        judge = judge->next;
    }

    p->id = pr->id + 1;
    printf("�������ǳƣ�");
    scanf_s("%s", p->name, 20);
    printf("���������룺");
    scanf_s("%s", p->password, 20);

    sprintf_s(sql, sizeof(sql), "insert into user_information values (%d,'%s','%s','%s');", p->id, p->name, p->account, p->password);
    mysql_query(&mysql, sql);

    p->next = NULL;

    return head;
}

//չʾ���е��û���Ϣ
void show_All_user_Information(user* head) {
    user* p = head;
    printf("=========================================================\n");
    printf("|�û����  |�ǳ�          |�˺�          |����          |\n");
    while (p != NULL) {
        printf("---------------------------------------------------------\n");
        printf("|%-10d|%-14s|%-14s|%-14s|\n", p->id, p->name, p->account, p->password);
        p = p->next;
    }
    printf("=========================================================\n");
}

//�����ݿ��м����û���Ϣ
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

//�޸��û�����
int change_User_password(user* head, MYSQL mysql) {
    char name[20]{}, password[20]{}, sql[300]{};
    int decision;
    user* p = head;
    printf("�������û��ǳ�:");
    scanf_s("%s", name, 20);
    while (p != NULL) {
        if (strcmp(name, p->name)) {
            p = p->next;
            continue;
        }
        break;
    }
    if (p == NULL) {
        printf("δ���ҵ����û�\n");
        system("pause");
        return 0;
    }
    else {
        system("cls");
        printf("����������\n");
        printf("==================================================\n");
        printf("|�ǳ�                |�˺�                |���  |\n");
        printf("--------------------------------------------------\n");
        printf("|%-20s|%-20s|%6d|\n", p->name, p->account, p->id);
        printf("==================================================\n");
    }
    printf("|1.ʹ�ù���Ա�����޸�      2.ʹ��ԭ�������޸�    |\n");
    printf("==================================================\n");

    while (1) {
        printf("��ѡ���޸ķ�ʽ��");
        scanf_s("%d", &decision);
        if (decision == 1) {
            printf("���������Ա���룺");
            scanf_s("%s", password, 20);
            if (strcmp(password, "root") == 0) {
                printf("�������µ����룺");
                scanf_s("%s", p->password, 20);
                sprintf_s(sql, sizeof(sql), "update user_information set u_password = '%s' where u_id = %d;", p->password, p->id);
                mysql_query(&mysql, sql);
                break;
            }
            else {
                printf("������������ԣ�\n");
                continue;
            }
        }
        if (decision == 2) {
            printf("������ԭ�����룺");
            scanf_s("%s", password, 20);
            if (strcmp(password, p->password) == 0) {
                printf("�������µ����룺");
                scanf_s("%s", p->password, 20);
                sprintf_s(sql, sizeof(sql), "update user_information set u_password = '%s' where u_id = %d;", p->password, p->id);
                mysql_query(&mysql, sql);
                break;
            }
            else {
                printf("������������ԣ�\n");
                continue;
            }
        }
        else {
            printf("ѡ������������������룡\n");
            continue;
        }
    }

    return 1;
}

//�û���½
user* user_Login(user* head) {
    user* p = head;
    char account[20], password[20];
    printf("�������˺ţ�");
    scanf_s("%s", account, 20);
    while (p != NULL) {
        if (strcmp(account, p->account)) {
            p = p->next;
            continue;
        }
        break;
    }
    if (p == NULL) {
        printf("ϵͳ���޸��˺ţ�\n");
        return NULL;
    }
    else
    {
        while (1) {
            printf("���������룺");
            scanf_s("%s", password, 20);
            if (strcmp(password, p->password) == 0) {
                printf("�ɹ���¼��\n");
                system("pause");
                system("cls");
                break;
            }
            else {
                printf("����������������룡\n");
                system("pause");
                system("cls");
                continue;
            }
        }
    }

    return p;
}

//��ʾ����ʣ����鼮
void display_Book_remaing(MYSQL_RES* res,MYSQL_ROW row,MYSQL mysql) {
    mysql_query(&mysql, "select * from book_information;");
    res = mysql_store_result(&mysql);
    printf("===================================================================================\n");
    printf("|����                    |����            |������          |����  |����  |���    |\n");
    while (row = mysql_fetch_row(res)) {
        //����������0�����
        if (atoi(row[5]) > 0) {
            printf("-----------------------------------------------------------------------------------\n");
            printf("|%-24s|%-16s|%-16s|%-6d|%-6d|%-8s|\n", row[1], row[2], row[3], atoi(row[4]), atoi(row[5]), row[0]);
        }
    }
    printf("===================================================================================\n");

}

//��ʾ�ѽ��ĵ��鼮
int show_book_borrowed(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head) {
    char sql[300]{}, name[24]{};
    book* judge = head;
    sprintf_s(sql, sizeof(sql), "select * from borrow_return where u_id = %d order by return_time asc;", id);
    mysql_query(&mysql, sql);
    res = mysql_store_result(&mysql);
    printf("===========================================================\n");
    printf("|����                    |��������    |����黹����|������|\n");
    while (row = mysql_fetch_row(res)) {
        judge = head;
        while (judge != NULL) {
            if (strcmp(judge->number, row[1]) == 0) {
                strcpy_s(name, 24, judge->name);
            }
            judge = judge->next;
        }
        printf("-----------------------------------------------------------\n");
        printf("|%-24s| %10s | %10s |%4d��|\n",name,row[2],row[3],atoi(row[4]));
    }
    printf("===========================================================\n");

    return 0;
}

//����ͼ��
int borrow_book(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head) {
    int i = 0;
    book* judge = head;
    char sql[300]{}, number[20];
    printf("������Ҫ���ĵ��鼮��ţ�");
    scanf_s("%s", number, 20);
    printf("������Ҫ���ĵı�����");
    scanf_s("%d", &i);
    while (judge != NULL) {
        if (strcmp(number, judge->number) == 0) {
            sprintf_s(sql, sizeof(sql), "select * from book_information where b_id = '%s';", number);
            mysql_query(&mysql, sql);
            res = mysql_store_result(&mysql);
            row = mysql_fetch_row(res);
            if (i > atoi(row[5])) {
                printf("��������㣡\n");
                return 0;
            }
            else
            {
                sprintf_s(sql, sizeof(sql), "insert into borrow_return values (%d,'%s',curdate(),date_add(curdate(),interval 30 day),%d);", id, number, i);
                mysql_query(&mysql, sql);
                sprintf_s(sql, sizeof(sql), "update book_information set b_now = b_now - %d where b_id = '%s';", i, number);
                mysql_query(&mysql, sql);
                judge->now = judge->now - i;
                printf("���ĳɹ�!\n");
                return 0;
            }
        }
        judge = judge->next;
    }
    if (judge == NULL) {
        printf("ͼ�����������\n");
    }

    return 0;
}

//�黹ͼ��
int return_book(int id, MYSQL_RES* res, MYSQL_ROW row, MYSQL mysql, book* head) {
    int i = 0, num = 0;
    book* judge = head;
    char sql[300]{}, number[20];
    printf("������Ҫ�黹���鼮��ţ�");
    scanf_s("%s", number, 20);
    printf("������Ҫ�黹�ı�����");
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
                printf("�黹�ɹ�!\n");
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
                printf("�黹�ɹ�!\n");
                return 0;
            }
            if (i > num) {
                printf("�鼮�黹��������δ�����ǰ�����鼮��");
                return 0;
            }
        }
        judge = judge->next;
    }
    if (judge == NULL) {
        printf("ͼ�����������\n");
    }

    return 0;
}
int main() {
    int decision,decision_1{};
    char temporary[20]{};
    user* p = NULL;
    book* book_Head = NULL;//�����鼮ͷָ��
    user* user_Head = NULL;//�������ͷָ��
    system("color 3F");//���ô�����ʾ��ɫ

    MYSQL mysql;//���ݿ���
    MYSQL_RES* res = nullptr;//��ѯ�����
    MYSQL_ROW row = nullptr;//��¼�ṹ��

    //��ʼ�����ݿ�
    mysql_init(&mysql);

    //�����ַ�����
    mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

    //�������ݿ�
    if (mysql_real_connect(&mysql, "127.0.0.1"/*������ַ*/, "root", "123456"/*���ݿ������*/, "library"/*Ҫʹ�õ����ݿ�*/, 3306, NULL, 0) == NULL) {
        printf("����ԭ��%s\n", mysql_error(&mysql));
        printf("����ʧ�ܣ�\n");
        exit(-1);
    }

    book_Head = load_Book_information(book_Head, res, row, mysql);//�������ݿ��е��鼮��Ϣ
    user_Head = load_User_information(user_Head, res, row, mysql);//�������ݿ��е��û���Ϣ

    while (1) {
        show_Menu_main();
        printf("������ѡ�");
        scanf_s("%d", &decision);
        while (decision == 1) {
            system("cls"); //����
            printf("���������Ա�˺ţ�");
            scanf_s("%s", temporary, 20);
            if (strcmp(temporary, "hmf")) {
                printf("�˺��������\n");
                continue;
            }
            else {
                printf("���������Ա���룺");
                scanf_s("%s", temporary, 20);
                if (strcmp(temporary, "root")) {
                    printf("�����������\n");
                    continue;
                }
                else {
                    decision_1 = 1;//��¼�ɹ���decision_1Ĭ�ϸ�ֵΪ0�Թ�����ʹ��
                    printf("��¼�ɹ�!\n");
                    system("pause");
                    system("cls");
                    show_Menu_root();
                    printf("������ѡ�");
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
                                decision_1 = 0;//�ڴ˴��޸ı���decision_1
                                break;
                            case 0:
                                printf("���˳���ϵͳ");
                                return 0;
                            default:
                                printf("��Чѡ�������ѡ��");
                                break;
                        }
                        system("pause");
                        system("cls");
                        if (decision_1 == 0) {
                            break;//����ѡ��ʱѡ��7�������˵������break����ǰwhile�ص���ʼ�˵�
                        }
                        show_Menu_root();
                        printf("������ѡ�");
                        scanf_s("%d", &decision);
                    }
                }
            }

        }
        while (decision == 2) {
            decision_1 = 1;
            p = user_Login(user_Head);
            if (p == NULL) {//��p����NULL˵�������˺Ŵ��󣬽����½���ѭ��
                system("pause");
                system("cls");
                continue;
            }
            while (1) {
                show_Menu_user(p, res, row, mysql);
                printf("������ѡ�");
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
                            printf("���˳���ϵͳ");
                            return 0;
                        default:
                            printf("��Чѡ�������ѡ��");
                            break;
                    }
                    system("pause");
                    system("cls");
                    if (decision_1 == 0) {
                        break;
                    }
                    show_Menu_user(p, res, row, mysql);
                    printf("������ѡ�");
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
            printf("���˳���ϵͳ");
            system("pause");
            return 0;
        }
        system("cls");
    }


    //�ر����ݿ�
    mysql_close(&mysql);

    return 0;
}
