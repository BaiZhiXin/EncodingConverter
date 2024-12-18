#include "widget.h"
#include <QMouseEvent>
#include <QColor>
#include "./ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget),
    converter(new MyConverter)
{
    ui->setupUi(this);

    // 使用 QPalette 设置背景色
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor::fromRgb(249,249,249)); // 可以改为你想要的颜色
    setPalette(pal);
    setAutoFillBackground(true);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    QStringList comboBoxItems = {"utf-8", "gbk", "windows-1252", "iso-8859-1" , "gb18030", "ascii", "big5",
                                 "utf-16le", "utf-16be"};
    ui->TargetEncoding_ComboBox->addItems(comboBoxItems);
    ui->TargetEncoding_ComboBox->setCurrentIndex(0);

    connect(ui->Close_PushButton, &QPushButton::clicked, this, &QWidget::close);
    connect(converter, &MyConverter::sendMessage, this, &Widget::addMessage2DragArea);
}

Widget::~Widget()
{
    delete ui;
    delete converter;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mousePressed = true;
        // auto placeholder = event->globalPos();
        auto placeholder = event->globalPosition();
        mousePressPos = placeholder.toPoint() - this->pos();
        event->accept();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (mousePressed && (event->buttons() & Qt::LeftButton)) {
        // move(event->globalPos() - mousePressPos);
        move(event->globalPosition().toPoint() - mousePressPos);
        event->accept();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mousePressed = false;
        event->accept();
    }
}

void Widget::addMessage2DragArea(EncodingConverter::LogLevel level, const QString &message)
{

    QString prefix;
    switch (level) {
    case EncodingConverter::LogLevel::INFO:
        prefix = "[INFO] ";
        break;
    case EncodingConverter::LogLevel::WARN:
        prefix = "[WARN] ";
        break;
    case EncodingConverter::LogLevel::ERROR:
        prefix = "[ERROR] ";
        break;
    }

    ui->DropArea->appendLog(prefix + message);
}

void Widget::on_Convert_PushButton_clicked()
{
    ui->DropArea->setDisplayMode(DragArea::DisplayMode::LogOutputMode);

    QStringList strList = ui->DropArea->getFilePaths();

    for(const auto &it : strList){
        converter->convert(it.toStdString(), ui->TargetEncoding_ComboBox->currentText().toStdString(),
                                   ui->SourceEncodingFilter_LineEdit->text().toStdString(),
                                   ui->SourceFileFilter_LineEdit->text().toStdString());
    }

}

void Widget::on_Clear_PushButton_clicked()
{
    ui->DropArea->clearDisplay();
    ui->DropArea->setDisplayMode(DragArea::DisplayMode::FileSelectionMode);
}

void MyConverter::logMessage(LogLevel level, const std::string &message)
{
    emit sendMessage(level, QString::fromStdString(message));
}
