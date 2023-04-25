#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJniObject>
#include <contactsmodel.h>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    QQmlApplicationEngine engine;

    qmlRegisterType<ContactsModel>("ContactsModel",1,0,"ContactsModel");

    //QJniObject javaClass = QNativeInterface::QAndroidApplication::context();

    //javaClass.callMethod<void>("addDummyContacts", "()V");



    const QUrl url(u"qrc:/ContactsDisplay/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
