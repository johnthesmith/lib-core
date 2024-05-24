#include "text.h"



/*
    Create new Text object
*/
Text* Text::create()
{
    return new Text();
}



/*
    Destroy Text object
*/
void Text::destroy()
{
    delete this;
}



/*
    Add string to text object
*/
Text* Text::add
(
    string aText,       /* Value */
    string aPrefix,     /* Prefix */
    string aSuffix      /* Suffix */
)
{
    aSuffix = aPrefix != "" && aSuffix == "" ? aPrefix : aSuffix;
    *this << aPrefix << aText << aSuffix;
    return this;
}



/*
    Begin of section with value
*/
Text* Text::begin
(
    string aText /* text */
)
{
    lineBegin();
    add( aText );
    depth ++;
    return this;
}



/*
    End of section with value
*/
Text* Text::end
(
    string aText /* text */
)
{
    depth --;
    lineBegin();
    add( aText );
    return this;
}



/*
    Add new line
*/
Text* Text::lineBegin()
{
    lineEnd();
    if( !compact )
    {
        string tab = "";
        tab.resize( tabSize * depth, tabChar );
        *this << tab;
    }
    return this;
}



/*
    Add line end
*/
Text* Text::lineEnd()
{
    if( !compact )
    {
        *this << lineDelimiter;
    }
    return this;
}



/*
    Return string
*/
string Text::getString()
{
    return this -> str();
}



/*
    Set line delimiter string
*/
Text* Text::setLineDelimiter
(
    string a  /* Delimiter string */
)
{
    lineDelimiter = a;
    return this;
}



/*
    Set tab character
*/
Text* Text::setTabChar
(
    char a /* Tab character */
)
{
    tabChar = a;
    return this;
}



/*
    Set tcompact mode
*/
Text* Text::setCompact
(
    bool a
)
{
    compact = a;
    return this;
}
