#ifndef CLIENTSSERVER_H
#define CLIENTSSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTextEdit>
#include <QTcpSocket>
#include <QtNetwork>
#include <QtGui>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QPixmap>


#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QSettings>
#include <QLineEdit>
#include <QMap>
#include <QMessageBox>
#include <QGroupBox>
#include <QCheckBox>





class QTcpServer;
class QTextEdit;
class QTcpSocket;


class ClientsServer : public QWidget
{
    Q_OBJECT

public:
    ClientsServer(QWidget *parent = 0);

private:
    QTcpServer* m_ptcpServer;
    QTcpSocket* pClientSocket;
    QTextEdit* m_ptxt;
    quint16 m_nNextBlockSize;

    QGroupBox* grpBox;
    QGroupBox* grpBoxTCP;
    QGroupBox* grpBoxSQL;
    QHBoxLayout* forBoxesLyt;

    // SQL settings
    QSqlDatabase db;
    QLineEdit* sqlName;
    QLineEdit* sqlUser;
    QLineEdit* sqlHost;
    QLineEdit* sqlPasswd;
    QLineEdit* sqlPort;
    QPushButton* connectSqlButt;
    QPushButton* connectStopButt;
    QCheckBox* chkbxSQL;




    // TCP settings
    int nPort;
    QSpinBox* portField;
    QPushButton* startButt;
    QPushButton* stopButt;
    QCheckBox* chkbxTCP;

    QLineEdit* sqlLineEdit;
    QLineEdit* tcpLineEdit;
    QPushButton* createButt;
    QPushButton* removeButt;
    QPushButton* saveButt;
    QPushButton* showRules;
    QMessageBox* msgBox;
    QMessageBox* msgRemove;
    QMessageBox* msgPatterns;

    QMap<QString, QString> rulesMap;
    QSettings settings;          // For storing data like connection attributes etc;
    QSettings settingsPatterns;  // For pattern storing (SQL + TCP)
    QStringList keys;


private:

    void sendToClient(QTcpSocket* pSocket, const QString& str);
    bool checkValid1(QString str);
    bool checkValid2(QString str);
    bool checkValid3(QString str);
    QString SqlQuery(QString &str);



public slots:
virtual void slotNewConnection();
        void slotDisconnect();
        void slotReadClient();
        void slotStartServer();
        void slotStopServer();
        void readyToStart(int port);
        void connectToSql();
        void slotSaveSettings();
        void slotCreateNewRule();
        void slotRemoveRule();

        void slotSaveSettingsSQL();  
        void slotRemoveSettingsSQL();
        void slotSaveSettingsTCP();
        void slotRemoveSettingsTCP();

        void slotShowPatterns();

        void slotSqlStop();

};

#endif // CLIENTSSERVER_H

























