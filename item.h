#ifndef ITEM_H
#define ITEM_H

class Item
{
public:
    Item(char symbol, int pos_x, int pos_y);
    ~Item();

    char get_symbol() const;
    int get_x() const;
    int get_y() const;
    void set_x(int x);
    void set_y(int y);

private:
    int pos_x;
    int pos_y;

    char symbol;
};

#endif
