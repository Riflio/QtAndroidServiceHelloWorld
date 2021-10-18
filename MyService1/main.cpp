#include <QAndroidService>
#include <QDebug>

int main(int argc, char *argv[])
{
    QAndroidService app(argc, argv);

    qWarning()<<"MyService1 started!";

    return app.exec();
}
