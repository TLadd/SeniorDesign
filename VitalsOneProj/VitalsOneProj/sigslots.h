#include <QMainWindow>
#include <QObject>

class testSig : public QObject
{
    Q_OBJECT
public:
    explicit testSig(QObject *parent = 0) :
        QObject(parent)
    {
    }

    void getNum();

signals:
    void sig_1(int i );
};

class testSlot : public QObject
{
    Q_OBJECT
public:
    explicit testSlot(QObject *parent = 0) :
        QObject(parent)
    {
    }

public slots:
    void slot1(int i);
};