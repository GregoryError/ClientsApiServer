#include "clientsserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Success");
    a.setApplicationName("Prometheus");
    ClientsServer server;
    server.show();
    return a.exec();
}
