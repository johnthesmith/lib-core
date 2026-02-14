#include "terminal.h"
#include <unistd.h>

/* Для неблокирующего ввода */
#include <sys/select.h>
#include <termios.h>



/*
    Constructor
*/
Terminal::Terminal( Log* aLog )
{
    log = aLog;

    initscr();            // Инициализация ncurses
    cbreak();             // Посимвольный ввод (без буферизации строк)
    noecho();             // Отключаем эхо клавиш
    keypad(stdscr, TRUE); // Включаем F-клавиши, стрелки
    start_color();        // Включаем поддержку цветов
    refresh();            // Первая отрисовка
}



/*
    Destructor
*/
Terminal::~Terminal()
{
    endwin(); // Восстанавливает терминал
}



/*
    Creator
*/
Terminal* Terminal::create
(
    Log* aLog
)
{
    return new Terminal( aLog );
}



/*
    Destroy
*/
void Terminal::destroy()
{
    delete this;
}



/*
    Пометка элемента как обновленного
*/
Terminal* Terminal::refresh()
{
    if(currentWin)
    {
        wnoutrefresh(currentWin);
    }
    else
    {
        wnoutrefresh( stdscr );
    }
    return this;
}



Terminal* Terminal::finish()
{
    doupdate();
    return this;
}



Terminal* Terminal::addWindow
(
    const std::string& name,
    int x,
    int y,
    int w,
    int h
)
{
    WINDOW* win = newwin( h, w, y, x );
    windows[ name ] = win;
    return this;
}



/*
    Активация окна
*/
Terminal* Terminal::window
(
    const std::string& name
)
{
    auto it = windows.find( name );
    if( it != windows.end())
    {
        currentWin = it -> second;
    }
    else
    {
        currentWin = stdscr;
    }
    return this;
}




Terminal* Terminal::alignWindow
(
    Align align
)
{
    if( currentWin )
    {
        int max_y, max_x;
        getmaxyx( stdscr, max_y, max_x );

        int win_y, win_x;
//        getbegyx( currentWin, win_y, win_x );

        switch(align)
        {
            case TOP:
                win_x = 0;
                win_y = 0;
            break;
            case BOTTOM:
                win_y = max_y - getmaxy( currentWin );
                win_x = 0;
            break;
            case LEFT:
                win_x = 0;
            break;
            case RIGHT:
                win_x = max_x - getmaxx( currentWin );
            break;
            case CENTER_VERTICAL:
                win_y = (max_y - getmaxy( currentWin )) / 2;
            break;
            case CENTER_HORIZONTAL:
                win_x = (max_x - getmaxx(currentWin)) / 2;
            break;
        }
        mvwin( currentWin, win_y, win_x );
        wresize( currentWin, getmaxy( currentWin ), max_x);
    }
    return this;
}



/*
    Очистка окна или буффера
*/
Terminal* Terminal::clear()
{
    if( currentWin )
    {
        wclear( currentWin );
    }
    else
    {
        clear();
    }
    return this;
}



/*
    Добавление цветовой пары
*/
Terminal* Terminal::addColor
(
    const std::string& name,
    int fg,
    int bg
)
{
    int id = colors.size() + 1;
    init_pair(id, fg, bg);
    colors[ name ] = id;
    return this;
}




Terminal* Terminal::moveTo(int y, int x)
{
    if(currentWin)
    {
        wmove(currentWin, y, x);
    }
    else
    {
        move(y, x);
    }
    return this;
}



Terminal* Terminal::color
(
    const std::string& name
)
{
    auto it = colors.find( name );
    if( it != colors.end() )
    {
        lastColor = it -> second;
        if( currentWin )
        {
            wattron( currentWin, COLOR_PAIR( lastColor ));
        }
        else
        {
            attron( COLOR_PAIR( lastColor ));
        }
    }
    return this;
}



Terminal* Terminal::setColorDefault()
{
    if( currentWin )
    {
        wbkgd( currentWin, COLOR_PAIR( lastColor ));
    }
    else
    {
        bkgd( COLOR_PAIR( lastColor ));
    }
    return this;
}



Terminal* Terminal::text
(
    const std::string& text
)
{
    if( currentWin )
    {
        waddstr(currentWin, text.c_str());
    }
    else
    {
        addstr(text.c_str());
    }
    return this;
}



Log* Terminal::getLog()
{
    return log;
}



Terminal* Terminal::cursor
(
    int aCursor
)
{
    initscr();
    curs_set( aCursor );
    return this;
}

/******************************************************************************
    Keyboard
*/

int Terminal::readKey()
{
    if(!currentWin) return 0;

    // Включаем неблокирующий режим
    nodelay(currentWin, TRUE);

    wint_t wch;
    int ret = wget_wch(currentWin, &wch);

    // Возвращаем блокирующий режим
    nodelay(currentWin, FALSE);

    if( ret == OK )
    {
        // Обычный символ (включая UTF-8)
        return static_cast<int>(wch);
    } else if(ret == KEY_CODE_YES) {
        // Специальная клавиша
        return wch;
    }

    return 0;
}

