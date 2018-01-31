#include "clientsserver.h"



ClientsServer::ClientsServer(QWidget *parent)
    : QWidget(parent), m_nNextBlockSize(0)
{

    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);
   // m_ptxt->setMinimumHeight(this->height() / 4);
    portField = new QSpinBox;
    portField->setMinimum(1);
    portField->setMaximum(65535);
   // portField->setMaximumWidth(this->width() / 3);
    QVBoxLayout* pvbxLayout = new QVBoxLayout;

    QHBoxLayout* hLblLyt = new QHBoxLayout;
    hLblLyt->addWidget(new QLabel("<H1>Prometheus 2018-A</H1>"));


    //QWidget* pixWgt = new QWidget;
    QLabel* pixWgt = new QLabel;
    QPixmap img(":/logo.png");
    QPalette pal;
    pal.setBrush(pixWgt->backgroundRole(), QBrush(img));
    pixWgt->setPalette(pal);
    pixWgt->setAutoFillBackground(true);
    pixWgt->setFixedSize(img.width(), img.height());
    hLblLyt->addWidget(pixWgt);



    pvbxLayout->addLayout(hLblLyt);
 // pvbxLayout->addWidget(new QLabel("<H1>Prometheus 2018-A</H1>"));
    pvbxLayout->addWidget(new QLabel("\"Успех\" телекоммуникационная компания."));

    QHBoxLayout* phLayout = new QHBoxLayout;
    QLabel* portLbl = new QLabel("Listen port: ");


    // Здесь будут 2 поля и кнопка, для установки соответствия

    grpBox = new QGroupBox("Query control");
    grpBoxTCP = new QGroupBox("TCP Server control");
    grpBoxSQL = new QGroupBox("SQL Server control");

    //grpBoxTCP->setMinimumWidth(this->width() / 3);


    // SQL Block
    QVBoxLayout *sqlLyt = new QVBoxLayout;
    sqlName = new QLineEdit;
    sqlName->setPlaceholderText("Name");
    sqlUser = new QLineEdit;
    sqlUser->setPlaceholderText("Username");
    sqlHost = new QLineEdit;
    sqlHost->setPlaceholderText("ip address / host");
    sqlPasswd = new QLineEdit;
    sqlPasswd->setPlaceholderText("Password");
    sqlPort = new QLineEdit;
    sqlPort->setText("3306");
    connectSqlButt = new QPushButton("Connect");
    connectStopButt = new QPushButton("STOP");
    connectStopButt->setMaximumWidth(grpBoxSQL->width() / 3);
    connectSqlButt->setMaximumWidth(grpBoxSQL->width() / 3);
    sqlLyt->addWidget(sqlName);
    sqlLyt->addWidget(sqlUser);
    sqlLyt->addWidget(sqlHost);
    sqlLyt->addWidget(sqlPasswd);
    sqlLyt->addWidget(sqlPort);
    chkbxSQL = new QCheckBox("Remember");
    sqlLyt->addWidget(chkbxSQL);




    sqlLyt->addWidget(connectSqlButt);
    sqlLyt->addWidget(connectStopButt);
    grpBoxSQL->setLayout(sqlLyt);


    // TCP Block
    QVBoxLayout* tcpLyt = new QVBoxLayout;
    stopButt = new QPushButton("STOP");
    tcpLyt->addWidget(portLbl);
    tcpLyt->addWidget(portField);
    chkbxTCP = new QCheckBox("Remember");
    tcpLyt->addWidget(chkbxTCP);
    startButt = new QPushButton("START");
    startButt->setDisabled(true);
    tcpLyt->addWidget(startButt);
    tcpLyt->addWidget(stopButt);
    grpBoxTCP->setLayout(tcpLyt);


    forBoxesLyt = new QHBoxLayout;
    forBoxesLyt->addWidget(grpBoxSQL);
    forBoxesLyt->addWidget(grpBoxTCP);
    forBoxesLyt->setSpacing(50);

    QVBoxLayout* vComboLyt = new QVBoxLayout;
    sqlLineEdit = new QLineEdit;
    tcpLineEdit = new QLineEdit;

    sqlLineEdit->setPlaceholderText("Set your query. Example: SELECT 'data' FROM 'table' WHERE");
    tcpLineEdit->setPlaceholderText("Set appropriate command here: \"get... / set...: \"");
    createButt = new QPushButton("Create");
    createButt->setMaximumWidth(tcpLineEdit->width() / 3);
    removeButt = new QPushButton("Remove");
    removeButt->setMaximumWidth(createButt->width());
    saveButt = new QPushButton("Save all");
    saveButt->setMaximumWidth(createButt->width());
    showRules = new QPushButton("Show all");
    showRules->setMaximumWidth(createButt->width());


    vComboLyt->addWidget(sqlLineEdit);
    vComboLyt->addWidget(tcpLineEdit);

    QHBoxLayout* buttonLyt = new QHBoxLayout;
    buttonLyt->addWidget(removeButt);
    buttonLyt->addWidget(createButt);
    buttonLyt->addWidget(saveButt);
    buttonLyt->addWidget(showRules);

    vComboLyt->addLayout(buttonLyt);


    grpBox->setLayout(vComboLyt);

    pvbxLayout->addWidget(grpBox);
    pvbxLayout->addLayout(forBoxesLyt);


    pvbxLayout->addLayout(phLayout);
    QHBoxLayout* phButtLy = new QHBoxLayout;
    phButtLy->setSpacing(60);

    QPushButton* clearButt = new QPushButton("Clear");
    phButtLy->addWidget(clearButt);
    pvbxLayout->addWidget(new QLabel("<H1>monitoring:</H1>"));
    pvbxLayout->addLayout(phButtLy);
    pvbxLayout->addWidget(m_ptxt);
    setLayout(pvbxLayout);

    m_ptcpServer = new QTcpServer(this);

    connect(m_ptcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    connect(clearButt, SIGNAL(clicked()), m_ptxt, SLOT(clear()));
    connect(portField, SIGNAL(valueChanged(int)), this, SLOT(readyToStart(int)));
    connect(startButt, SIGNAL(clicked()), this, SLOT(slotStartServer()));
    connect(stopButt, SIGNAL(clicked()), this, SLOT(slotStopServer()));
    connect(connectStopButt, SIGNAL(clicked()), this, SLOT(slotSqlStop()));
    connect(connectSqlButt, SIGNAL(clicked()), this, SLOT(connectToSql()));
    connect(createButt, SIGNAL(clicked()), this, SLOT(slotCreateNewRule()));
    connect(saveButt, SIGNAL(clicked()), this, SLOT(slotSaveSettings()));
    connect(removeButt, SIGNAL(clicked()), this, SLOT(slotRemoveRule()));
    connect(showRules, SIGNAL(clicked()), this, SLOT(slotShowPatterns()));


     // Reading settings (created patterns) from HDD to RAM(QMap);
     // Read then like this: rulesMap["key"]);

    keys = settingsPatterns.allKeys();
    QString tempStr;
    foreach (tempStr, keys) {
        rulesMap.insert(tempStr, settingsPatterns.value(tempStr).toString());
    }







    chkbxSQL->setChecked(settings.value("/chbx").toBool());

    chkbxTCP->setChecked(settings.value("/chkbxTCP").toBool());

    portField->setValue(settings.value("/tspPort").toInt());


    sqlHost->setText(settings.value("/sqlHost").toString());
    sqlName->setText(settings.value("/sqlName").toString());
    sqlUser->setText(settings.value("/sqlUser").toString());
    sqlPasswd->setText(settings.value("/sqlPasswd").toString());
    sqlPort->setText(settings.value("/sqlPort").toString());


}

//////////////////////////////////////////////////////////////////////////////FUNCTIONS
///
///


bool ClientsServer::checkValid1(QString str)
{
    const QString validKey1 =
            "Here you should place validation KEY # 1";  // 64
    if(str == validKey1)
        return true;
    else return false;
}

bool ClientsServer::checkValid2(QString str)
{
    const QString validKey2 =
            "Here you should place validation KEY # 2";  // 64
    if(str == validKey2)
        return true;
    else return false;
}

bool ClientsServer::checkValid3(QString str)
{
    const QString validKey3 =
            "Here you should place validation KEY # 3";  // 64
    if(str == validKey3)
        return true;
    else return false;
}




void ClientsServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_10);

    for(;;){
        if(!m_nNextBlockSize){
            if(pClientSocket->bytesAvailable() < sizeof(quint16)){
                break;
            }
            in >> m_nNextBlockSize;
        }
        if(pClientSocket->bytesAvailable() < m_nNextBlockSize){
            break;
        }
        QTime time;
        QString str;

        in >> time >> str;
        QString strMessage = time.toString() + " " + "Client: ";
        m_ptxt->append(strMessage);

        m_nNextBlockSize = 0;


        QString name;
        QString t_name;
        QString pswd;
        QString t_pswd;


        /////////////////////// Getting Name if the key is valid
        ///                                                 ////
        ///                                                 ////
        ///                                                 ////
        if(checkValid1(str.left(64)))                       ////
             t_name = str.mid(64, 11);                      ////
        for(auto &c:t_name)                                 ////
        {                                                   ////
            if(c == "!")                                    ////
                break;                                      ////
            name += c;                                      ////
        }                                                   ////
        ///                                                 ////
        ////////////////////////////////////////////////////////

        m_ptxt->append(name);

        /////////////////////// Getting Pass if the key is valid
        ///                                                 ////
        ///                                                 ////
        ///                                                 ////
        if(checkValid2(str.mid(64 + name.length(), 64)))    ////
            t_pswd = str.mid(128 + name.length(), 12);      ////
        for(auto &c:t_pswd)                                 ////
        {                                                   ////
            if(c == "!")                                    ////
                break;                                      ////
            pswd += c;                                      ////
        }                                                   ////
        ///                                                 ////
        ////////////////////////////////////////////////////////

        //m_ptxt->append(pswd);


       QString passQue = "SELECT AES_DECRYPT(passwd, 'YOUR_KEY')"
                         " as password from users where name= '" + name + "'";
       if(pswd != SqlQuery(passQue))
       {
           m_ptxt->append("The client attempted to connect using wrong password.");
           slotDisconnect();
       }else{

        /////////////////////// Getting the command ////////////
        ///                                                
        ///                                                 
        ///                                                 
        QString t_command;                         
        QString req;                                       
       // t_command = str.right(16);
        t_command = str.mid(128 + pswd.length()
                           + name.length() + 1);



        int i(0);                                         
        for(auto &c:t_command)                              
        {
            req += c;
            if((c == "!" && i > 4) || c == ":")            
                break;                                  ////
                                                            ////
            ++i;                                          ////
        }                                                   ////
                                                            ////
        ///                                                 ////
        ////////////////////////////////////////////////////////




        QString request = rulesMap[req];
        request += " name = '" + name + "'";

        QSqlQuery query(db);


        if(!query.exec(request)){
            m_ptxt->append("Unable to make operation.");
        }

        QSqlRecord rec = query.record();

        QString strID;
        QString strBalance;
        QString strState;
        QString strPay_day;
        QString strPaket;

        while (query.next()) {

              strID = query.value(rec.indexOf("id")).toString();
              strBalance = query.value(rec.indexOf("balance")).toString();
              strState = query.value(rec.indexOf("state")).toString();
              strPay_day = query.value(rec.indexOf("pay_day")).toString();
              strPaket = query.value(rec.indexOf("paket")).toString();

             }


        m_ptxt->append(strID);
        m_ptxt->append(strBalance);
        m_ptxt->append(strState);
        m_ptxt->append(strPay_day);
        m_ptxt->append(strPaket);



        QString strStartData = "id: " + strID + "\n"
                + "Balance: " + strBalance + "\n"
                + "Access: " + strState + "\n"
                + "Pay_day: " + strPay_day + "\n"
                + "Packet: " + strPaket;
        sendToClient(pClientSocket, "Server response: \n" + strStartData);












       }






        //sendToClient(pClientSocket, "Server response: " + strStartData);
    }
}




QString ClientsServer::SqlQuery(QString& str)
{
    QSqlQuery query(db);



    if(!query.exec(str)){
        m_ptxt->append("Unable to make operation.");
    }


        QString answer;

        while (query.next()) {
                 answer += query.value(0).toString();
                  // answer += query.value(rec.indexOf("id")).toString();
             }
        return answer;
}




void ClientsServer::slotCreateNewRule()
{
    rulesMap.insert(tcpLineEdit->text(), sqlLineEdit->text());
    msgBox = new QMessageBox;
    msgBox->setText("OK: The rule for \"" + tcpLineEdit->text() + "\" was created!");
    msgBox->exec();
    tcpLineEdit->clear();
    sqlLineEdit->clear();
}



void ClientsServer::slotSaveSettings()
{
    auto beg = rulesMap.cbegin(), end = rulesMap.cend();
    while(beg != end)
    {
        settingsPatterns.setValue(beg.key(), *beg);
        ++beg;
    }
}



void ClientsServer::slotRemoveRule()
{
    settingsPatterns.remove(tcpLineEdit->text());
    msgRemove = new QMessageBox;
    msgRemove->setText("OK: The pattern \"" + tcpLineEdit->text() + "\" was removed.");
    msgRemove->exec();
    tcpLineEdit->clear();

}
 //

void ClientsServer::slotShowPatterns()
{
    QString str;
    QString tempStr;
    foreach (tempStr, keys) {
        str += tempStr + ": "
        + settingsPatterns.value(tempStr).toString() + "\n";
    }
    msgPatterns = new QMessageBox;
    msgPatterns->setText("Existed patterns: \t\n\t\n" + str);
    msgPatterns->exec();
}


void ClientsServer::slotSaveSettingsSQL()
{

    settings.setValue("/sqlHost", sqlHost->text());
    settings.setValue("/sqlName", sqlName->text());
    settings.setValue("/sqlUser", sqlUser->text());
    settings.setValue("/sqlPasswd", sqlPasswd->text());
    settings.setValue("/sqlPort", sqlPort->text());

    settings.setValue("/chbx", true);


}


void ClientsServer::slotRemoveSettingsSQL()
{
    settings.remove("/sqlHost");
    settings.remove("/sqlName");
    settings.remove("/sqlUser");
    settings.remove("/sqlPasswd");
    settings.remove("/sqlPort");
    settings.setValue("/chbx", false);
}




void ClientsServer::slotSaveSettingsTCP()
{
    settings.setValue("/tspPort", portField->text());
    settings.setValue("/chkbxTCP", true);

}

void ClientsServer::slotRemoveSettingsTCP()
{
    settings.remove("/tspPort");
    settings.setValue("/chkbxTCP", false);
}



void ClientsServer::slotSqlStop()
{
    db.close();
}





void ClientsServer::connectToSql()
{
    db  = QSqlDatabase::addDatabase("QMYSQL");


    if(chkbxSQL->isChecked()){

         slotSaveSettingsSQL();


    }else if(!chkbxSQL->isChecked()){

        slotRemoveSettingsSQL();

    }

    db.setHostName(sqlHost->text());
    db.setDatabaseName(sqlName->text());
    db.setUserName(sqlUser->text());
    db.setPassword(sqlPasswd->text());
    db.setPort(sqlPort->text().toInt());


    db.setConnectOptions("MYSQL_OPT_RECONNECT=TRUE;");




    if(!db.open() ) {
           m_ptxt->append(QTime::currentTime().toString() + db.lastError().text());
           return;
    }else{
              QStringList lst = db.tables();
              foreach (QString str, lst) {
              m_ptxt->append(QTime::currentTime().toString() + ": " + str);
           }
    }



}



void ClientsServer::readyToStart(int port)
{
    startButt->setEnabled(true);
    nPort = port;

}


void ClientsServer::slotStartServer()
{


    if(chkbxTCP->isChecked())
        slotSaveSettingsTCP();
    else
        slotRemoveSettingsTCP();


    m_ptcpServer->close();
    if(!m_ptcpServer->listen(QHostAddress::Any, nPort)){
        m_ptxt->append(QTime::currentTime().toString() + ": " + "Server error: " + m_ptcpServer->errorString());
        m_ptcpServer->close();
        return;
    }else{
        m_ptxt->append(QTime::currentTime().toString() + ": " + "Listening port: " + QString::number(nPort) + "...");
    }

}

void ClientsServer::slotStopServer()
{
    m_ptcpServer->close();
    m_ptxt->append(QTime::currentTime().toString() + ": " + " The server was stopped!" );
}


void ClientsServer::slotNewConnection()
{
    pClientSocket = m_ptcpServer->nextPendingConnection();

    connect(pClientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnect()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));

    m_ptxt->append(QTime::currentTime().toString() + " Connection established.");
    sendToClient(pClientSocket, "Server response: Connection established.");

}

void ClientsServer::slotDisconnect()
{
    m_ptxt->append(QTime::currentTime().toString() + ": " + " Client was disconnected." );
    pClientSocket->deleteLater();
}


void ClientsServer::sendToClient(QTcpSocket *pSocket, const QString &str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    pSocket->write(arrBlock);
}






















