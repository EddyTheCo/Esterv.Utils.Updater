#pragma once

#include<QObject>
#include <QProcess>

#if defined(USE_QML)
#include<QtQml>
#endif

#include <QtCore/QtGlobal>
  #ifdef WINDOWS_EXPORT
    #define UPDAT_EXPORT Q_DECL_EXPORT
  #else
    #define UPDAT_EXPORT Q_DECL_IMPORT
  #endif

namespace Esterv::Utils{

class UPDAT_EXPORT Updater: public QObject
{

    Q_OBJECT
#ifdef USE_QML
    Q_PROPERTY(State  state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool  hasUpdate READ hasUpdate NOTIFY hasUpdateChanged)
    Q_PROPERTY(QString  updateDetails READ updateDetails NOTIFY updateDetailsChanged)
    QML_ELEMENT
    QML_SINGLETON
#endif
    Updater(QObject *parent=nullptr);

public:
    static Updater* instance();

    Q_INVOKABLE void update();
    Q_INVOKABLE void restart();
    Q_INVOKABLE void checkUpdates(void);
    Q_INVOKABLE void hasIFW(QString toolname=
#if defined(Q_OS_WIN)
                            "../maintenancetool.exe"
#elif defined(Q_OS_MAC)
                            "../../../../maintenancetool.app/Contents/MacOS/maintenancetool"
#else
                            "../maintenancetool"
#endif
                            , bool absolute=false);
    enum State {
        Null=0,
        Ready,
        Checking,
        Installing,
        ReadyToRestart,
    };
    Q_ENUM(State)
    State state()const{return m_state;}
    QString updateDetails()const{return m_updateDetails;}
    bool hasUpdate()const{return m_hasUpdate;}
#if defined(USE_QML)
    static Updater *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        auto inst=instance();
        inst->hasIFW();
        if(inst->state()==Ready)
        {
            inst->checkUpdates();
        }
        return inst;
    }
#endif

signals:
    void stateChanged();
    void updateDetailsChanged();
    void restarting(int retcode);
    void hasUpdateChanged();

private:
    void setState(State sta){if(sta!=m_state){m_state=sta;emit stateChanged();}}


    void checkfinished(int exitCode, QProcess::ExitStatus exitStatus);
    void finishUpdate(int exitCode, QProcess::ExitStatus exitStatus);
    State m_state;
    QString m_MTPath,m_updateDetails;
    QProcess m_check,m_update;
    static Updater * m_instance;
    bool m_hasUpdate;
};
}
