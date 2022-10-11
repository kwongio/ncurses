#include <stdio.h>
#include <ncurses.h>
void mainPage();
void enterPage();
void updownPage();
void printPage(char c);

int main(int args, char const *argv[])
{
    initscr(); // 시작하는 함수

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
        init_pair(2, COLOR_WHITE, COLOR_BLUE);
    }
    refresh(); // 화면을 갱신한다.

    wchar_t key;
    while (true)
    {

        curs_set(FALSE);      // 0: 커서 없애기, 1: 커서 보이기
        keypad(stdscr, TRUE); // 특수키 입력 허용

        noecho(); //입력한 값을 화면에 보이지 함

        mainPage();    // 시작 페이지
        key = getch(); // 키보드 입력값
        if (key == 27) // esc
            break;
        else if (key == KEY_UP || key == KEY_DOWN)
            updownPage(); // 방향키 위 아래 눌렀을 때
        else if (key == 10)
            enterPage(); // 엔터키를 눌렀을 때
        else
            printPage(key);

        clear();   // 화면을 지운다.
        refresh(); //화면을 갱신시킨다.

        mainPage();
    }
    endwin(); // 끝 내는 함수
    return 0;
}

void mainPage()
{
    WINDOW *win1 = newwin(18, 80, 0, 0); // 높이, 넓이 ,x축 이동, y축 이동
    WINDOW *win2 = newwin(6, 80, 18, 0); // 높이, 넓이 ,x축 이동, y축 이동

    wbkgd(win1, COLOR_PAIR(1)); //  init_pair로 정의한 색상번호
    wbkgd(win2, COLOR_PAIR(2));
    mvwprintw(win1, 8, 34, "Press any key"); // 특정 윈도우 특정 위치에 문자열을 출력함

    wrefresh(win1); // window 갱싱
    wrefresh(win2);
}

void enterPage()
{
    WINDOW *win1 = newwin(18, 80, 0, 0); // 높이, 넓이 ,x축 이동, y축 이동
    WINDOW *win2 = newwin(6, 80, 18, 0); // 높이, 넓이 ,x축 이동, y축 이동

    wbkgd(win1, COLOR_PAIR(1)); //  init_pair로 정의한 색상번호
    wbkgd(win2, COLOR_PAIR(2));

    mvwprintw(win1, 10, 37, "Enter");         // 특정 윈도우 특정 위치에 문자열을 출력함
    mvwprintw(win1, 11, 34, "Press any key"); // 특정 윈도우 특정 위치에 문자열을 출력함

    wrefresh(win1); // window 갱싱
    wrefresh(win2);

    getch(); //하나의 문자를 받을 때 까지 기다림
}

void updownPage()
{

    WINDOW *win1 = newwin(18, 80, 0, 0); // 높이, 넓이 ,x축 이동, y축 이동
    WINDOW *win2 = newwin(6, 80, 18, 0); // 높이, 넓이 ,x축 이동, y축 이동
    wbkgd(win1, COLOR_PAIR(1));          //  init_pair로 정의한 색상번호
    wbkgd(win2, COLOR_PAIR(2));

    mvwprintw(win1, 8, 31, "asdfasdfasdfasdfads"); // 특정 윈도우 특정 위치에 문자열을 출력함
    mvwprintw(win1, 9, 34, "Pasdfasdfasdf");       // 특정 윈도우 특정 위치에 문자열을 출력함

    wrefresh(win1); // window 갱싱
    wrefresh(win2);

    getch(); //하나의 문자를 받을 때 까지 기다림
}

void printPage(char c)
{
    curs_set(1); // 0: 커서 없애기, 1: 커서 보이기

    WINDOW *win1 = newwin(18, 80, 0, 0); // 높이, 넓이 ,x축 이동, y축 이동
    WINDOW *win2 = newwin(6, 80, 18, 0); // 높이, 넓이 ,x축 이동, y축 이동

    wbkgd(win1, COLOR_PAIR(1)); //  init_pair로 정의한 색상번호
    wbkgd(win2, COLOR_PAIR(2));

    mvwprintw(win1, 8, 31, "Key Up or Key Down"); // 특정 윈도우 특정 위치에 문자열을 출력함
    mvwprintw(win1, 9, 34, "Press any key");      // 특정 윈도우 특정 위치에 문자열을 출력함

    wrefresh(win1); // window 갱싱
    wrefresh(win2); 


    char key;
    for (int i = 0; i < 10; i++)
    {

        key = getch();            //문자 1개를 입력받음
        wprintw(win2, "%c", key); // 출력
        wrefresh(win2);           // 갱신
    }

    mvwprintw(win1, 9, 29, "End print press any key");
    wrefresh(win1);

    getch();
}