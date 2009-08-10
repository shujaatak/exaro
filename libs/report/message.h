#ifndef MESSAGE_H
#define MESSAGE_H
#include <QObject>

class QWidget;
class QTimer;

class Message: public QObject
{
    Q_OBJECT
public:
    Message();
    static Message * instance();

    void show( QString str, int sec = 20);
    void assignMonitor(QWidget * w);

public slots:
    void hide();

private:
    static Message * m_instance;
    QWidget * m_monitor;
    QTimer * timer;
};

#endif // MESSAGE_H
