#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include "EncodingConverter.h"

class MyConverter : public QObject, public EncodingConverter{
    Q_OBJECT
signals:
    void sendMessage(LogLevel level, const QString& message);

    // EncodingConverter interface
protected:
    void logMessage(LogLevel level, const std::string &message);
};

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void addMessage2DragArea(EncodingConverter::LogLevel level, const QString& message);

    void on_Convert_PushButton_clicked();

  void on_Clear_PushButton_clicked();

private:
    Ui::Widget *ui;

    bool mousePressed = false;
    QPoint mousePressPos;
    MyConverter *converter = nullptr;

};
#endif // WIDGET_H
