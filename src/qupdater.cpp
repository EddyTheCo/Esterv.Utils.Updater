#include"qupdater.hpp"
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QDomDocument>

namespace qutils{

Updater* Updater::m_instance=nullptr;
Updater* Updater::instance()
{
    if (!m_instance)
    {
        m_instance=new Updater();
    }
    return m_instance;
}
Updater::Updater(QObject *parent):QObject(parent),
    m_state(Null),m_updateDetails(""),m_hasUpdate(false)
{
    QObject::connect(this,&Updater::restarting,QCoreApplication::instance(),&QCoreApplication::exit,Qt::QueuedConnection);
    QObject::connect(&m_check,&QProcess::finished,this,&Updater::checkfinished);
    QObject::connect(&m_update,&QProcess::finished,this,&Updater::finishUpdate);

}
void Updater::update()
{
    if(m_state==Ready&&m_updateDetails.size())
    {
        setState(Installing);
        m_update.start(m_MTPath, {"up","-c"});
    }
}
void Updater::restart()
{
    if(m_state==ReadyToRestart)
    {
        auto args=QCoreApplication::arguments();
        args.removeFirst();
        qint64 pid;
        emit restarting(0);
        auto started=QProcess::startDetached(QCoreApplication::applicationFilePath(), args,QString(),&pid);
        qDebug()<<"started:"<<started<<" pid:"<<pid;
    }
}
void Updater::finishUpdate(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "finishUpdate exitCode=" << exitCode << ", exitStatus=" << exitStatus;
    QByteArray stdOut = m_update.readAllStandardOutput();
    QString stdOutStr = QString::fromLocal8Bit(stdOut);
    qDebug() << "finishUpdate out>" << stdOutStr;

    if(exitCode == 0){
        setState(ReadyToRestart);
        return;
    }
    setState(Ready);
}
void Updater::checkfinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "checkfinished exitCode=" << exitCode << ", exitStatus=" << exitStatus;

    QByteArray stdOut = m_check.readAllStandardOutput();

    qDebug() << "checkfinished>" << stdOut;

    if(exitCode == 0){

        auto xmlindex= stdOut.lastIndexOf("<?xml");
        QByteArray xml = stdOut.sliced(xmlindex);
        QDomDocument xmldoc;
        xmldoc.setContent(xml);

         qDebug()<<"xmlindex:"<<xmlindex;
        qDebug()<<"xml:"<<xml;
         qDebug()<<"xmldoc:"<<xmldoc.toString();
        auto updates=xmldoc.firstChildElement("updates");
        QString updateDetails;
        if(!updates.isNull())
        {
            for(auto n = updates.firstChildElement("update"); !n.isNull(); n = n.nextSiblingElement("update"))
            {
                const auto name=n.attributeNode("name").value();
                const auto version=n.attributeNode("version").value();
                qDebug()<<"name:"<<name<<" version:"<<version;
                updateDetails.append(name+"-"+version+"\n");
            }
        }

        if(updateDetails.length() > 0){
            m_updateDetails=updateDetails;
            m_hasUpdate=true;
            emit updateDetailsChanged();
            emit hasUpdateChanged();
        }
        setState(Ready);
    }
}
void Updater::checkUpdates(void)
{
    setState(Checking);
    m_check.start(m_MTPath, {"ch"});
}

void Updater::hasIFW(QString toolname, bool absolute)
{
    QString toolpath;

    if(absolute)
    {
        toolpath=toolname;
    }
    else
    {
        toolpath=QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(toolname);
    }

    qDebug() <<"Tool path:"<< toolpath;
    QFile file(toolpath);
    qDebug() <<"file.exists():"<< file.exists();
    if(file.exists())
    {
        m_MTPath=toolpath;
        setState(Ready);
    }

}

};
