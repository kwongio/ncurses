#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include<stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#define ENTER 10
#define ESC 27
#define PERMISSON 0644
#define INPUT_SIZE 10
void menuPage();
void signPage();
void loginPage();
void mainPage();
void menuKeyEvent();
void signKeyEvent();
void loginKeyEvent();
void mainKeyEvent();
void newuser();
typedef enum page
{
    MENUPAGE,
    LOGINPAGE,
    SIGNUPPAGE,
    MAINPAGE,

} Page;

typedef enum menuCursor
{
    LOGIN,
    SIGNUP
} MenuPageCursor;

typedef struct _User
{
    char name[INPUT_SIZE + 1];
    char id[INPUT_SIZE + 1];
    char pw[INPUT_SIZE + 1];
    char pwEncrypt[INPUT_SIZE + 1];
    int nameCount = 0;
    int idCount = 0;
    int pwCount = 0;
} User;

User *user;
wchar_t key;
int signFormCursor = 1;
int loginFormCursor = 1;
int fd = 0;
int fd2 = 0;
int fd3 = 0;
int userCount = 0;
int messageCount = 4;
int messageCursor = 1;
const char *pathname = "./user.dat";
MenuPageCursor menuPageCursor;
Page page;
ssize_t wSize = 0;
ssize_t rSize = 0;
int main(int args, char const *argv[])
{
    fd = open(pathname, O_CREAT | O_TRUNC | O_RDWR, PERMISSON);
    if (fd == -1)
        exit(-1);

    initscr();                 // 시작하는 함수
    if (has_colors() == FALSE) // 색상을 사용할 수 있는지 판단한다.
    {
        puts("Terminal does not support colors");
        endwin();
        return 1;
    }
    else
    {
        start_color();                         // 색상을 사용한다.
        init_pair(1, COLOR_BLUE, COLOR_WHITE); // 번호, 글자색, 배경색 wbkgd에 사용됨
        init_pair(2, COLOR_WHITE, COLOR_CYAN);
    }
    refresh(); // 화면을 갱신한다.

    while (true)
    {
      
  if((page == LOGINPAGE || page == SIGNUPPAGE) && loginFormCursor != 3 && signFormCursor != 4 ){
            curs_set(2);
        }else{
            curs_set(0);
        }

        keypad(stdscr, true); // 특수키 입력 허용
        noecho();             //입력한 값을 화면에 보이지 함
      
        if (page == MENUPAGE)
            menuPage();

        key = getch();

        switch (page)
        {
        case MENUPAGE:
            menuKeyEvent();
            break;
        case SIGNUPPAGE:
            signKeyEvent();
            break;
        case LOGINPAGE:
            loginKeyEvent();
            break;
        case MAINPAGE:
            mainKeyEvent();
            break;
        }

        clear();
        refresh();
  

        switch (page)
        {
        case MENUPAGE:
            menuPage();
            break;
        case SIGNUPPAGE:
            signPage();
            break;
        case LOGINPAGE:
            loginPage();
            break;
        case MAINPAGE:
            mainPage();
            break;
        }

    }
    endwin(); // 끝 내는 함수
    close(fd);
    free(user);
    return 0;
}

void menuPage()
{
    WINDOW *win1 = newwin(18, 80, 0, 0); // 높이, 넓이 ,x축 이동, y축 이동
    WINDOW *win2 = newwin(6, 80, 18, 0); // 높이, 넓이 ,x축 이동, y축 이동
    wbkgd(win1, COLOR_PAIR(1));          //  init_pair로 정의한 색상번호
    wbkgd(win2, COLOR_PAIR(2));
    mvwprintw(win1, 8, 34, "Login");    // 윈도우, x , y, 문자
    mvwprintw(win1, 10, 34, "Sign up"); // 윈도우, x , y, 문자

    if (menuPageCursor == LOGIN)
        mvwprintw(win1, 8, 32, ">"); // 특정 윈도우 특정 위치에 문자열을 출력함
    else if (menuPageCursor == SIGNUP)
        mvwprintw(win1, 10, 32, ">"); // 특정 윈도우 특정 위치에 문자열을 출력함

    wrefresh(win1); // window 갱싱
    wrefresh(win2);
}

void loginPage()
{
    WINDOW *win1 = newwin(18, 80, 0, 0); // 높이, 넓이 ,x축 이동, y축 이동
    WINDOW *win2 = newwin(6, 80, 18, 0); // 높이, 넓이 ,x축 이동, y축 이동
    WINDOW *win3 = newwin(1, 1, 4, 33);  // 높이, 넓이 ,x축 이동, y축 이동
    wbkgd(win1, COLOR_PAIR(1));          //  init_pair로 정의한 색상번호
    wbkgd(win2, COLOR_PAIR(2));
    mvwprintw(win1, 4, 28, "ID:");    // 윈도우, x , y, 문자
    mvwprintw(win1, 6, 28, "PW:");    // 윈도우, x , y, 문자
    mvwprintw(win1, 10, 33, "Login"); // 윈도우, x , y, 문자

    if (loginFormCursor == 1)
    {
        win3 = newwin(1, 1, 4, 31 + user->idCount); // 높이, 넓이 ,x축 이동, y축 이동
        wbkgd(win3, COLOR_PAIR(2));
    }
    else if (loginFormCursor == 2)
    {
        win3 = newwin(1, 1, 6, 31 + user->pwCount); // 높이, 넓이 ,x축 이동, y축 이동
        wbkgd(win3, COLOR_PAIR(2));
    }
    else if (loginFormCursor == 3)
    {

        win3 = newwin(1, 1, 10, 31); // 높이, 넓이 ,x축 이동, y축 이동
        wbkgd(win3, COLOR_PAIR(1));
        mvwprintw(win3, 0, 0, ">"); // 특정 윈도우 특정 위치에 문자열을 출력함
    }
    mvwprintw(win1, 4, 31, user->id);        // 특정 윈도우 특정 위치에 문자열을 출력함
    mvwprintw(win1, 6, 31, user->pwEncrypt); // 특정 윈도우 특정 위치에 문자열을 출력함
    wrefresh(win1);                          // window 갱싱
    wrefresh(win2);
    wrefresh(win3);
    
  
}

void signPage()
{
    WINDOW *win1 = newwin(18, 80, 0, 0); // 높이, 넓이 ,x축 이동, y축 이동
    WINDOW *win2 = newwin(6, 80, 18, 0); // 높이, 넓이 ,x축 이동, y축 이동
    WINDOW *win3 = newwin(1, 1, 4, 33);  // 높이, 넓이 ,x축 이동, y축 이동
    wbkgd(win1, COLOR_PAIR(1));          //  init_pair로 정의한 색상번호
    wbkgd(win2, COLOR_PAIR(2));

    mvwprintw(win1, 4, 28, "name:");    // 윈도우, x , y, 문자
    mvwprintw(win1, 6, 28, "ID:");      // 윈도우, x , y, 문자
    mvwprintw(win1, 8, 28, "PW:");      // 윈도우, x , y, 문자
    mvwprintw(win1, 14, 30, "Sign up"); // 윈도우, x , y, 문자
    if (user->idCount == 0 || user->nameCount == 0 || user->pwCount == 0)
    {
        mvwprintw(win2, 0, 20, "Please fill out all blanks"); // 윈도우, x , y, 문자
    }

    if (signFormCursor == 1)
    {   
        win3 = newwin(1, 1, 4, 33 + user->nameCount); // 높이, 넓이 ,x축 이동, y축 이동
        wbkgd(win3, COLOR_PAIR(2));
    }
    else if (signFormCursor == 2)
    {   
        win3 = newwin(1, 1, 6, 31 + user->idCount); // 높이, 넓이 ,x축 이동, y축 이동
        wbkgd(win3, COLOR_PAIR(2));
    }
    else if (signFormCursor == 3)
    {   
        win3 = newwin(1, 1, 8, 31 + user->pwCount); // 높이, 넓이 ,x축 이동, y축 이동
        wbkgd(win3, COLOR_PAIR(2));
    }
    else if (signFormCursor == 4)
    {
         win3 = newwin(1, 1, 14, 28); // 높이, 넓이 ,x축 이동, y축 이동
        wbkgd(win3, COLOR_PAIR(1));
        mvwprintw(win3, 0, 0, ">"); // 특정 윈도우 특정 위치에 문자열을 출력함

  
    }

    mvwprintw(win1, 4, 33, user->name);      // 특정 윈도우 특정 위치에 문자열을 출력함
    mvwprintw(win1, 6, 31, user->id);        // 특정 윈도우 특정 위치에 문자열을 출력함
    mvwprintw(win1, 8, 31, user->pwEncrypt); // 특정 윈도우 특정 위치에 문자열을 출력함
    wrefresh(win1);                          // window 갱싱
    wrefresh(win2);
    wrefresh(win3);
}

void mainPage()
{
    WINDOW *win1 = newwin(18, 80, 0, 0); // 높이, 넓이 ,x축 이동, y축 이동
    WINDOW *win2 = newwin(6, 80, 18, 0); // 높이, 넓이 ,x축 이동, y축 이동
    WINDOW *win3;
    if (messageCursor == 5)
    {
        win3 = newwin(1, 1, 18, 72); // 높이, 넓이 ,x축 이동, y축 이동
        wbkgd(win3, COLOR_PAIR(2));
    }
    else
    {
        win3 = newwin(1, 1, messageCursor - 1, 0); // 높이, 넓이 ,x축 이동, y축 이동
        wbkgd(win3, COLOR_PAIR(1));
    }

    wbkgd(win1, COLOR_PAIR(1)); //  init_pair로 정의한 색상번호
    wbkgd(win2, COLOR_PAIR(2));
    mvwprintw(win1, 0, 2, "Message1"); // 윈도우, x , y, 문자
    mvwprintw(win1, 1, 2, "Message2"); // 윈도우, x , y, 문자
    mvwprintw(win1, 2, 2, "Message3"); // 윈도우, x , y, 문자
    mvwprintw(win1, 3, 2, "Message4"); // 윈도우, x , y, 문자
    User *readUser = (User *)malloc(sizeof(User));
    fd2 = open(pathname, O_RDONLY, PERMISSON);
    for (int i = 0; i < userCount; i++)
    {
        rSize = read(fd2, (User *)readUser, sizeof(User));
        if (wSize == -1)
            exit(-1);
        if (strcmp(user->id, readUser->id) == 0)
        {
            memcpy(user->name, readUser->name, sizeof(char) * INPUT_SIZE);
            break;
        }
    }
    // TODO
    mvwprintw(win2, 0, 0, "ID: ");
    mvwprintw(win2, 0, 3, user->id);
    mvwprintw(win2, 1, 0, "Name: ");
    mvwprintw(win2, 1, 5, user->name);
    mvwprintw(win2, 0, 74, "Logout");
    mvwprintw(win3, 0, 0, ">"); // 특정 윈도우 특정 위치에 문자열을 출력함
    wrefresh(win1);             // window 갱싱
    wrefresh(win2);
    wrefresh(win3);
    close(fd2);
}

void mainKeyEvent()
{
    if (key == KEY_UP)
    {
        if (messageCursor != 5)
        {
            if (messageCursor >= 2)
                messageCursor--;
            else
                messageCursor = messageCount;
        }
    }

    else if (key == KEY_DOWN)
    {
        if (messageCursor != 5)
        {
            if (messageCursor <= messageCount - 1)
                messageCursor++;
            else
                messageCursor = 1;
        }
    }
    else if (key == ESC)
    {
        if (messageCursor != 5)
        {
            messageCursor = 5;
        }
        else
        {
            messageCursor = 1;
        }
    }
    else if (key == ENTER)
    {
        if (messageCursor == 5)
        {
            page = MENUPAGE;
        }
    }
}

void menuKeyEvent()
{
    if (key == ENTER)
    {
        if (menuPageCursor == LOGIN)
            page = LOGINPAGE;
        user = (User *)malloc(sizeof(User));
        loginFormCursor = 1;

        if (menuPageCursor == SIGNUP)
        {
            page = SIGNUPPAGE;
            user = (User *)malloc(sizeof(User));
            signFormCursor = 1;
        }
    }
    else if (key == ESC)
    {
        clear();
        refresh();
        exit(0);
    }

    else if (key == KEY_UP)
        if (menuPageCursor == LOGIN)
            menuPageCursor = SIGNUP;
        else
            menuPageCursor = LOGIN;

    else if (key == KEY_DOWN)
        if (menuPageCursor == SIGNUP)
            menuPageCursor = LOGIN;
        else
            menuPageCursor = SIGNUP;
}

void loginKeyEvent()
{

    if (key == KEY_UP)
    {
        
        if (loginFormCursor >= 2)
            loginFormCursor--;
        else
            loginFormCursor = 3;
    }
    else if (key == KEY_DOWN)
    {
        if (loginFormCursor <= 2)
            loginFormCursor++;
        else
            loginFormCursor = 1;
    }
    else if (key == ESC)
    {
        page = MENUPAGE;
    }
    else
    {
        if (loginFormCursor == 1)
        {
            if (user->idCount < INPUT_SIZE)
                user->id[user->idCount++] = key;
        }
        else if (loginFormCursor == 2)
        {
            if (user->pwCount < INPUT_SIZE)
            {
                user->pwEncrypt[user->pwCount] = '*';
                user->pw[user->pwCount] = key;
                user->pwCount++;
            }
        }
        else if (loginFormCursor == 3)
        {
            if (key == ENTER)
            {
                User *readUser;
                bool isExist = false;
                bool isPasswordWrong = false;
                fd2 = open(pathname, O_RDONLY, PERMISSON);
                for (int i = 0; i < userCount; i++)
                {
                    readUser = (User *)malloc(sizeof(User));
                    rSize = read(fd2, (User *)readUser, sizeof(User));
                    if (wSize == -1)
                        exit(-1);
                    if (strcmp(user->id, readUser->id) == 0)
                    {
                        isExist = true;
                        if (strcmp(user->pw, readUser->pw) == 0)
                        {
                            messageCursor = 1;
                            page = MAINPAGE;
                            return;
                        }
                        else
                            isPasswordWrong = true;
                    }
                    free(readUser);
                }
                if (!isExist && page != MAINPAGE)
                {
                    WINDOW *win2 = newwin(6, 80, 18, 0); // 높이, 넓이 ,x축 이동, y축 이동
                    wrefresh(win2);
                    wbkgd(win2, COLOR_PAIR(2));
                    mvwprintw(win2, 0, 30, "ID doesn't exist"); // 윈도우, x , y, 문자
                    free(user);
                    wrefresh(win2);
                    key = getch();
                    newuser();
                    loginFormCursor = 1;
                }
                else if (isPasswordWrong && page != MAINPAGE)
                {
                    WINDOW *win2 = newwin(6, 80, 18, 0); // 높이, 넓이 ,x축 이동, y축 이동
                    wrefresh(win2);
                    wbkgd(win2, COLOR_PAIR(2));
                    mvwprintw(win2, 0, 30, "Incorrect Password"); // 윈도우, x , y, 문자
                    free(user);
                    wrefresh(win2);
                    key = getch();
                    newuser();
                    loginFormCursor = 1;
                }

                close(fd2);
            }
        }
    }
}

void signKeyEvent()
{
    if (key == KEY_UP)
    {
        if (signFormCursor >= 2)
            signFormCursor--;
        else
            signFormCursor = 4;
    }

    else if (key == KEY_DOWN)
    {
        if (signFormCursor <= 3)
            signFormCursor++;
        else
            signFormCursor = 1;
    }
    else if (key == ESC)
    {
        page = MENUPAGE;
    }
    else
    {
        if (signFormCursor == 1)
        {
            if (user->nameCount < INPUT_SIZE)
                user->name[user->nameCount++] = key;
        }
        else if (signFormCursor == 2)
        {
            if (user->idCount < INPUT_SIZE)
                user->id[user->idCount++] = key;
        }
        else if (signFormCursor == 3)
        {
            if (user->pwCount < INPUT_SIZE)
            {
                user->pwEncrypt[user->pwCount] = '*';
                user->pw[user->pwCount] = key;
                user->pwCount++;
            }
        }
        else if (signFormCursor == 4)
        {
            if (key == ENTER)
            {
                User *readUser;
                bool isDuplicate = false;
                fd2 = open(pathname, O_RDONLY, PERMISSON);
                for (int i = 0; i < userCount; i++)
                {
                    readUser = (User *)malloc(sizeof(User));
                    rSize = read(fd2, (User *)readUser, sizeof(User));
                    if (wSize == -1)
                        exit(-1);
                    if (strcmp(user->name, readUser->name) == 0)
                    {
                        isDuplicate = true;
                        WINDOW *win2 = newwin(6, 80, 18, 0); // 높이, 넓이 ,x축 이동, y축 이동
                        wrefresh(win2);
                        wbkgd(win2, COLOR_PAIR(2));
                        mvwprintw(win2, 0, 25, "Already exigts ID"); // 윈도우, x , y, 문자
                        wrefresh(win2);
                        key = getch();
                        signFormCursor = 1;
                        break;
                    }
                    free(readUser);
                }

                if (!isDuplicate)
                {
                    wSize = write(fd, (User *)user, sizeof(User));
                    if (wSize == -1)
                        exit(-2);
                    userCount++;
                    page = LOGINPAGE;
                }
                newuser();
                close(fd2);
            }
        }
    }
}

void newuser()
{
    user = (User *)malloc(sizeof(User));
    memset(user->id, 0, sizeof(user->id));
    memset(user->name, 0, sizeof(user->id));
    memset(user->pw, 0, sizeof(user->id));
    memset(user->pwEncrypt, 0, sizeof(user->id));
    user->idCount = 0;
    user->nameCount = 0;
    user->pwCount = 0;
}