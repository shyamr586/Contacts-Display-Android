#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJniObject>
#include <contactslist.h>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ContactsList>("Contacts",1,0,"Contacts");

    QQmlApplicationEngine engine;

    //QJniObject javaClass = QNativeInterface::QAndroidApplication::context();

    //javaClass.callMethod<void>("addDummyContacts", "()V");



    const QUrl url(u"qrc:/ContactsDisplay/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
