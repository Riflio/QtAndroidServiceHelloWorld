1. Создаём новый проект, в мастере создания выбираете жругой проект -> проект с поддиректориями

2. Создаём подпроект - основное приложение, из которого будет запускаться наш сервис, в моём случае Qml с названием MainApp

3. Создаём подпроект - сервис. В дереве проектов кликаем по проекту, в субменюшке выбираем создать подпроект, и в мастере создания выбираем Библиотека -> Библиотека C++. Название у меня MyService1, на следующих шагах выбираете Модуль Qt:  none и всё далее.


3. Создаём подпроект - сервис. Он у нас будет в отдельной подключаемой библиотеке.
В дереве проектов кликаем по проекту, в субменюшке выбираем создать подпроект, и в мастере создания выбираем другой проект -> пустой проект qmake. Название ставим по вкусу, у меня MyService1

4. Открываем на редактирование .pro файл сервиса и прописываем:

TEMPLATE = lib
CONFIG += dll
QT += core androidextras
TARGET = myservice1 #Заменить на своё усмотрение

5. Создаём точку входа в библиотеку. Через мастер создания добавляем "файл исходных текстов C/C++", название по вкусу, у меня main, открываем на редактирование и прописываем:

#include <QAndroidService>
#include <QDebug>

int main(int argc, char *argv[])
{
    qWarning()<<"Service started!";
    QAndroidService app(argc, argv);
    return app.exec();
}


6. Пока что с сервисом хватит, переходим к основному приложению.

В левой колонке кликаем по "Проекты", из списка сборка и запуск выбираем комплект для Андроида (разумеется, должно быть настроено окружение для него), в моём случае Android Qt 5.12.3 Clang armeabi-v7a, в группе "Сборка, этапы" раскрываем список "Build Anroid APK", выбираем версию SDK и нажимаем Create Templates что бы Qt Creator создал все необходимые файлы в папке с приложением.

7. Через QtCreator открываем androidManifest.xml, в графе "Имя пакета" пишем название пакета приложения (грубо говоря пространство имён для Java), у меня ru.pavelk.QtServiceHelloWorld, сохраняем.


8. Идём в файловом менеджере в папку проекта, в подпапку android и создаём папку src, в ней создаём подпапки согласно имени пакета. 
Вот так: android/src/ru/pavelk/QtServiceHelloWorld/ и в ней файл с активити, через который будет запускаться наш сервис, название по вкусу, у меня MyService1.java. Открываем на редактирование и прописываем:

package ru.pavelk.QtServiceHelloWorld; //-- Название пакета должно соответствовать пути
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import org.qtproject.qt5.android.bindings.QtService;

public class MyService1 extends QtService //-- Название класса должно соответствовать названию файла
{
    private static final String TAG = "MyService1";

    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(TAG, "Creating Service");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "Destroying Service");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        int ret = super.onStartCommand(intent, flags, startId);

        // Do some work

        return ret;
    }  
   
}


8. Прописываем в манифесте инфу о сервисе. Открываем androidManifest.xml (если открываете в QtCreator, то в верху перейдите на "Исходник XML"), перед </appplication> добавляем:

 	<!-- Название сервиса как название пакета и через точку класс активности, название процесса сервиса по вкусу -->
	<service android:name="ru.pavelk.QtServiceHelloWorld.MyService1" android:process=":MyService1Process">
   
   	<!-- Имя библиотеки сервиса, т.е. значение TARGET из pro файла сервиса -->
	<meta-data android:name="android.app.lib_name" android:value="myservice1"/> 
	<!-- Что бы мог запускаться в фоне -->
    <meta-data android:name="android.app.background_running" android:value="true"/> 
    
	<!-- Библиотеки Qt, можно скопировать из основной activity, которая выше в файле -->
	<meta-data android:name="android.app.qt_sources_resource_id" android:resource="@array/qt_sources"/>
	<meta-data android:name="android.app.repository" android:value="default"/>
	<meta-data android:name="android.app.qt_libs_resource_id" android:resource="@array/qt_libs"/>
	<meta-data android:name="android.app.bundled_libs_resource_id" android:resource="@array/bundled_libs"/>
	<!-- Deploy Qt libs as part of package -->
	<meta-data android:name="android.app.bundle_local_qt_libs" android:value="-- %%BUNDLE_LOCAL_QT_LIBS%% --"/>
	<meta-data android:name="android.app.bundled_in_lib_resource_id" android:resource="@array/bundled_in_lib"/>
	<meta-data android:name="android.app.bundled_in_assets_resource_id" android:resource="@array/bundled_in_assets"/>
	<!-- Run with local libs -->
	<meta-data android:name="android.app.use_local_qt_libs" android:value="-- %%USE_LOCAL_QT_LIBS%% --"/>
	<meta-data android:name="android.app.libs_prefix" android:value="/data/local/tmp/qt/"/>
	<meta-data android:name="android.app.load_local_libs" android:value="-- %%INSERT_LOCAL_LIBS%% --"/>
	<meta-data android:name="android.app.load_local_jars" android:value="-- %%INSERT_LOCAL_JARS%% --"/>
	<meta-data android:name="android.app.static_init_classes" android:value="-- %%INSERT_INIT_CLASSES%% --"/>  
</service>


9. Ну и осталось только запустить сервис из основного приложения. 

В *.pro файл дописываем QT += androidextras

Код запуска:

#include <QtAndroidExtras/QtAndroid>

QAndroidIntent serviceIntent(QtAndroid::androidActivity().object(),"ru.pavelk.QtServiceHelloWorld.MyService1"); //-- Имя пакета и название класса
    QAndroidJniObject result = QtAndroid::androidActivity().callObjectMethod("startService", "(Landroid/content/Intent;)Landroid/content/ComponentName;", serviceIntent.handle().object());



вызываем когда нужно запустить сервис.
у меня он размещён в AppCore, смотрите исходники на GitHub.



10. Вывод отладочных сообщений в QtCreator не отображается, поэтому подключаемся через adb и смотрим лог: в консоле adb logcat


Вот как-то так. Исходники на GitHub: 