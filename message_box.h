#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H
#include <QMessageBox>


struct messageBox
{
    explicit messageBox(QString const& message)
        : box_{ }
    {
        box_.setText(message);
        box_.exec();
    }

private:
    QMessageBox box_;
};
#endif // MESSAGE_BOX_H
