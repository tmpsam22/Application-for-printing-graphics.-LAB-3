/// @file message_box.h представлен класс для вывода оконного сообщения
#pragma once

#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H
#include <QMessageBox>

/// @brief класс для вывода оконного сообщения
struct messageBox
{
    /// @brief конструктор
    /// выводит сообщение
    /// @message вывод сообщения
    /// @title название окна
    explicit messageBox(QString const& message, QString const& title="Info")
        : box_{ }
    {
        box_.setWindowTitle(title);
        box_.setText(message);
        box_.exec();
    }
    /// @brief деструктор
    ~messageBox() = default;

private:
    QMessageBox box_;
};

#endif // MESSAGE_BOX_H
