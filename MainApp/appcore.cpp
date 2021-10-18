#include "appcore.h"
#include <QDebug>

#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidIntent>


AppCore::AppCore(QObject *parent) : QObject(parent)
{

}

void AppCore::runMyService1()
{
    qInfo()<<"Go run MyService1";

    QAndroidIntent serviceIntent(QtAndroid::androidActivity().object(),"ru.pavelk.QtServiceHelloWorld.MyService1"); //-- Имя пакета и название класса
    QAndroidJniObject result = QtAndroid::androidActivity().callObjectMethod("startService", "(Landroid/content/Intent;)Landroid/content/ComponentName;", serviceIntent.handle().object());

}
