#include "TaoFrameLessView.h"

#include "AppInfo.h"
#include "ComponentsManager.h"
#include "TaoFramework.h"
#include "Trans.h"
#include "logger.h"
#include <QDir>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>
static void prepareApp()
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("JaredTao");
    QCoreApplication::setOrganizationDomain("https://JaredTao.gitee.io");
    QCoreApplication::setApplicationName("TaoQuickShow");
}

int main(int argc, char** argv)
{
    prepareApp();
    QGuiApplication app(argc, argv);
#ifdef TAODEBUG
    qSetMessagePattern("[%{time h:mm:ss.zzz} %{function}] %{message}");
//    qSetMessagePattern("[%{time h:mm:ss.zzz} %{file} row(%{line}) %{function}] %{message}");
#else
    Logger::initLog();
#endif
    const auto appPath = QDir::cleanPath(app.applicationDirPath());
    qWarning() << "appPath" << appPath;

    TaoFrameLessView view;

    TaoFramework::instance()->setMainView(&view);
    TaoFramework::instance()->createObject<Trans>();
    TaoFramework::instance()->createObject<AppInfo>();
    TaoFramework::instance()->createObject<ComponentsMgr>();

    TaoFramework::instance()->init();
    TaoFramework::instance()->beforeUiReady(view.rootContext());

    view.engine()->addImportPath(qmlPath);
#ifdef TaoQuickImportPath
    view.engine()->addImportPath(TaoQuickImportPath);
    qWarning() << "TaoQuickImportPath " << TaoQuickImportPath;
#endif

#ifdef TaoQuickImagePath
    view.rootContext()->setContextProperty("taoQuickImagePath", TaoQuickImagePath);
#endif

#ifdef TAODEBUG
    view.rootContext()->setContextProperty("isDebug", true);
#else
    view.rootContext()->setContextProperty("isDebug", false);
#endif

    view.rootContext()->setContextProperty("qmlPath", qmlPath);
    view.rootContext()->setContextProperty("imgPath", imgPath);
    view.rootContext()->setContextProperty("contentsPath", contentsPath);
    view.rootContext()->setContextProperty("appPath", appPath);
    view.rootContext()->setContextProperty("view", &view);

    const QUrl url(qmlPath + QStringLiteral("main.qml"));
    QObject::connect(&view, &QQuickView::statusChanged, [=](QQuickView::Status status) {
        if (status == QQuickView::Status::Ready) {
            TaoFramework::instance()->afterUiReady();
        }
    });
    QObject::connect(view.engine(), &QQmlEngine::quit, qApp, &QCoreApplication::quit);
    view.setSource(url);
    view.moveToScreenCenter();
    view.show();

    return app.exec();
}
