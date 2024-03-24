
#include <QCoreApplication>
#include "qupdater.hpp"

using namespace qutils;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const auto instance=Updater::instance();

    QObject::connect(instance,&Updater::stateChanged,&app,[=](){
        const auto state= instance->state();
        auto details=instance->updateDetails();
        qDebug()<<"Updater State Changed to:"<< state;
        qDebug()<<"Update details:"<< details;

        if(state==Updater::Ready)
        {
            if(instance->hasUpdate())
            {
                instance->update();
            }
            else
            {
                instance->checkUpdates();
            }
        }
        if(state==Updater::ReadyToRestart)
        {
            instance->restart();
        }

    });

    instance->hasIFW();
    return app.exec();
}
