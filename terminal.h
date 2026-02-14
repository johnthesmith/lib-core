#pragma once

#include <ncurses.h>
#include <string>
#include <map>

#include "log.h"



enum Align
{
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    CENTER_VERTICAL,
    CENTER_HORIZONTAL
};



class Terminal
{

private:

    Log* log;

    /* Массив цветовых пар имя:индекс*/
    std::map <std::string, int> colors;

    /* Массив окон */
    std::map<std::string, WINDOW*> windows;

    /* Current window */
    WINDOW* currentWin = stdscr;
    /* Last color */
    int lastColor = 0;

public:

    Terminal( Log* );
    ~Terminal();

    static Terminal* create( Log* );
    void destroy();

    Terminal* disableEcho();
    Terminal* restoreEcho();



    /*
        Пометка обновления элемента
    */
    Terminal* refresh();



    /*
        Вывод буффера
    */
    Terminal* finish();




    Terminal* addWindow
    (
        const std::string& name,
        int = 0,
        int = 0,
        int = 1,
        int = 1
    );



    /*
        Активация окна
    */
    Terminal* window
    (
        const std::string& = ""
    );


    /*
    */
    Terminal* alignWindow
    (
        Align align
    );



    Terminal* clear();


    /*
        Добавление цветовой пары
    */
    Terminal* addColor
    (
        /* Имя цветовой пары */
        const std::string& name,
        int fg,
        int bg
    );



    /*
        Переместить на позицию
    */
    Terminal* moveTo
    (
        int,
        int
    );



    /*
        Выбор цветовой пар
    */
    Terminal* color
    (
        const std::string&
    );



    Terminal* setColorDefault();



    /*
        Вывод текста
    */
    Terminal* text
    (
        const std::string&
    );



    Terminal* cursor
    (
        int
    );


    Log* getLog();




    /***************************************************************************
        Keyboard
    */

    int readKey();



};
