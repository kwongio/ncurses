
#include <login.h>
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