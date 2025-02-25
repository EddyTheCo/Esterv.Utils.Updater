
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  engine.addImportPath("qrc:/esterVtech.com/imports");

  const QUrl url =
      QUrl("qrc:/esterVtech.com/imports/EquickApp/qml/quickApp.qml");

  engine.load(url);
  return app.exec();
}
