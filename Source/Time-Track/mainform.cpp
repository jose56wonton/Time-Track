#include "mainform.h"
#include "ui_mainform.h"
#include <QFileDialog>
#include <QMessageBox>
MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    this->hide();
    connectionForm = new ConnectionForm(this);

    QObject::connect(connectionForm,SIGNAL(finished()),this,SLOT(start()));

    connectionForm->auto_connect();

}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::start(){
    ip = connectionForm->getIp();

    //Connect("aacidatabase","3306","user","aaci1234","192.168.41.187");
    if(Connect(connectionForm->getDatabase(),connectionForm->getPort(),connectionForm->getUsername(),connectionForm->getPassword(),connectionForm->getIp())){
        connectionForm->hide();
        if (ip.split(".")[3] =="010")
            this->showFullScreen();
        else
            this->showMaximized();

        qDebug()<<"from start";

        clockoutForm = new ClockoutForm(this);
        clockoutForm->hide();
        shifteditform = new ShiftEditForm(this);
        shifteditform->hide();
        //connectionForm = new ConnectionForm(this);
        exportForm = new ExportForm(this);



        loginInitialize();
        isConnected();
        setIcons();
    }
    else
    {
        this->hide();
//        connectionForm->hide();
//        connectionForm = new ConnectionForm(this);
//        connectionForm->show();
        QObject::connect(connectionForm,SIGNAL(finished()),this,SLOT(start()));
        connectionForm->setError("Invalid Connection");
    }
}

/* These are the database connections that this database uses.
 * The first is called setup. It's purpose is to store the
 * location of data which is the database that can be placed
 * anywhere on your computer or server and holds the actual
 * information about employees and such
*/


bool MainForm::Connect(QString database,QString port,QString username,QString password,QString ip){
    qDebug()<<database<<port<<username<<password<<ip;
    data = QSqlDatabase::addDatabase("QMYSQL");
    data.setPort(port.toInt());
    data.setDatabaseName(database);
    data.setPassword(password);
    data.setUserName(username);
    data.setHostName(ip);
    data.open();
    ui->ConnectionLabel->setText(data.lastError().text());
    if(data.isOpen())
        return true;
     else
    {
        QDir dir(QStandardPaths::locate(QStandardPaths::DocumentsLocation,QString(),QStandardPaths::LocateDirectory)+"Time-Track/");
        dir.removeRecursively();
        return false;
    }
}

bool MainForm::isValidConnection(QString ip){


    QSqlDatabase temp;
    temp = QSqlDatabase::addDatabase("QMYSQL");
    temp.setPort(3306);
    temp.setPassword("aaci1234");
    temp.setDatabaseName("aacidatabase");
    temp.setUserName("client");
    temp.setHostName(ip);
    temp.open();
    //qDebug()<<temp.lastError();
    if(temp.isOpen())
        return true;
     else
        return false;
}



/* These classes connect the different forms through
 * signals and slots
*/
void MainForm::establishConnections(){
    QObject::connect(clockoutForm,SIGNAL(finished()),this,SLOT(reenter()));
    QObject::connect(shifteditform,SIGNAL(finished()),this,SLOT(refreshShiftTab()));
    QObject::connect(shifteditform,SIGNAL(finished()),this,SLOT(displayShiftSuccess()));
    QObject::connect(exportForm,SIGNAL(excel()),this,SLOT(exportToExcel()));

    QObject::connect(ui->ProjectView->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this, SLOT(refreshProjectStuff()));
    QObject::connect(ui->ItemView->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this, SLOT(refreshItemStuff()));
    QObject::connect(ui->ProjectItemView->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(refreshItemStuff()));
    QObject::connect(ui->EmployeeView->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this, SLOT(refreshEmployeeStuff()));

}
void MainForm::reenter(){

    showtheThings();
    basicInitialize();
}

/* Initialization of the different pages of mainform*/
void MainForm::setIcons(){
    QPixmap pixmap("../Icons/checked.png");
    QIcon ButtonIcon(pixmap);
    ui->basicPageClockIn->setIcon(ButtonIcon);
    ui->basicPageClockIn->setIconSize(QSize(256,256));

    pixmap = * new QPixmap("../Icons/unchecked.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->basicPageClockOut->setIcon(ButtonIcon);
    ui->basicPageClockOut->setIconSize(QSize(256,256));

    pixmap = * new QPixmap("../Icons/bars.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->basicPageAdvanced->setIcon(ButtonIcon);
    ui->basicPageAdvanced->setIconSize(QSize(256,256));


    pixmap = * new QPixmap("../Icons/connected.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->basicPageConnect->setIcon(ButtonIcon);
    ui->basicPageConnect->setIconSize(QSize(256,256));

//    pixmap = * new QPixmap("../Icons/connect.png");
//    ButtonIcon =  * new QIcon(pixmap);
//    ui->DataBaseConnect->setIcon(ButtonIcon);
//    ui->DataBaseConnect->setIconSize(QSize(50,50));
//    pixmap = * new QPixmap("../Icons/disconnect.png");
//    ButtonIcon =  * new QIcon(pixmap);
//    ui->DataBaseDisconnect->setIcon(ButtonIcon);
//    ui->DataBaseDisconnect->setIconSize(QSize(50,50));


    pixmap = * new QPixmap("../Icons/EmployeeAdd.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->EmployeeAdd->setIcon(ButtonIcon);
    ui->EmployeeAdd->setIconSize(QSize(34,50));

    pixmap = * new QPixmap("../Icons/EmployeeArchive.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->EmployeeArchive->setIcon(ButtonIcon);
    ui->EmployeeArchive->setIconSize(QSize(34,50));


    pixmap = * new QPixmap("../Icons/EmployeeDelete.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->EmployeeDelete->setIcon(ButtonIcon);
    ui->EmployeeDelete->setIconSize(QSize(34,50));




    pixmap = * new QPixmap("../Icons/ProjectAdd.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->ProjectAdd->setIcon(ButtonIcon);
    ui->ProjectAdd->setIconSize(QSize(34,50));

    pixmap = * new QPixmap("../Icons/ProjectArchive.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->ProjectArchive->setIcon(ButtonIcon);
    ui->ProjectArchive->setIconSize(QSize(34,50));

    pixmap = * new QPixmap("../Icons/ProjectDelete.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->ProjectDelete->setIcon(ButtonIcon);
    ui->ProjectDelete->setIconSize(QSize(34,50));



    pixmap = * new QPixmap("../Icons/SubProjectAdd.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->ItemAdd->setIcon(ButtonIcon);
    ui->ItemAdd->setIconSize(QSize(34,50));
    ui->ProjectItemAdd->setIcon(ButtonIcon);
    ui->ProjectItemAdd->setIconSize(QSize(34,50));
    pixmap = * new QPixmap("../Icons/SubProjectDelete.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->ItemDelete->setIcon(ButtonIcon);
    ui->ItemDelete->setIconSize(QSize(34,50));
    ui->ProjectItemRemove->setIcon(ButtonIcon);
    ui->ProjectItemRemove->setIconSize(QSize(34,50));




    pixmap = * new QPixmap("../Icons/ShiftAdd.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->ShiftAdd->setIcon(ButtonIcon);
    ui->ShiftAdd->setIconSize(QSize(34,50));

    pixmap = * new QPixmap("../Icons/ShiftEdit.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->ShiftEdit->setIcon(ButtonIcon);
    ui->ShiftEdit->setIconSize(QSize(34,50));

    pixmap = * new QPixmap("../Icons/ShiftDelete.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->ShiftDelete->setIcon(ButtonIcon);
    ui->ShiftDelete->setIconSize(QSize(34,50));



    pixmap = * new QPixmap("../Icons/min.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->SettingsMax->setIcon(ButtonIcon);
    ui->SettingsMax->setIconSize(QSize(50,50));

    pixmap = * new QPixmap("../Icons/max.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->SettingsFull->setIcon(ButtonIcon);
    ui->SettingsFull->setIconSize(QSize(50,50));

    pixmap = * new QPixmap("../Icons/export.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->SettingsExport->setIcon(ButtonIcon);
    ui->SettingsExport->setIconSize(QSize(50,50));

    pixmap = * new QPixmap("../Icons/connected2.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->SettingsConnections->setIcon(ButtonIcon);
    ui->SettingsConnections->setIconSize(QSize(50,50));

    pixmap = * new QPixmap("../Icons/print.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->SettingsPrint->setIcon(ButtonIcon);
    ui->SettingsPrint->setIconSize(QSize(50,50));

    pixmap = * new QPixmap("../Icons/Settings.png");
    ButtonIcon =  * new QIcon(pixmap);
    ui->SettingsAll->setIcon(ButtonIcon);
    ui->SettingsAll->setIconSize(QSize(50,50));




}
void MainForm::loginInitialize(){

    ui->mainStack->setCurrentIndex(0);
    admin=false;
    ui->passEdit->setAlignment(Qt::AlignCenter);
    ui->passLabel->setAlignment(Qt::AlignCenter);

    isConnected();
    ui->mainFinish->hide();
    ui->basicPageConnect->hide();
    ui->connectionlabel->hide();
    ui->ConnectionLabel->hide();

}
void MainForm::isConnected(){
    ui->passEdit->setText("");
    ui->passLabel->setText("");
    //qDebug()<<data.isOpen();
    if(data.isOpen())
    {
        ui->passEdit->show();
        ui->passEdit->show();
        ui->loginNumPad->show();
        ui->basicPageConnect->hide();
        ui->ConnectionLabel->hide();
    }
    else
    {
        ui->passEdit->hide();
        ui->passEdit->hide();
        ui->loginNumPad->hide();
        ui->basicPageConnect->show();
        ui->ConnectionLabel->show();
    }
}
void MainForm::basicInitialize()
{
    ui->mainStack->setCurrentIndex(1);
    if(admin==true)
        ui->basicPageAdvanced->show();
    else
        ui->basicPageAdvanced->hide();

    QSqlQuery qry(data);
    QString active;

    qry.prepare("SELECT active FROM employeelist WHERE id = '"+id+"'");
    if(qry.exec())
    {
        while(qry.next())
        {
            active = qry.value(0).toString();
        }
    }

    if(active == "1")
    {
       ui->basicPageClockIn->hide();
       ui->basicPageClockOut->show();
    }
    else if(active== "0")
    {
        ui->basicPageClockIn->show();
        ui->basicPageClockOut->hide();
    }
    ui->mainFinish->show();
}
void MainForm::advInitialize(){
    ui->mainStack->setCurrentIndex(2);
    ui->MainTabs->tabBar()->hide();

    establishConnections();
}

void MainForm::on_Login0_clicked()
{


    ui->passEdit->setText(ui->passEdit->text()+"0");

}
void MainForm::on_Login1_clicked()
{
    ui->passEdit->setText(ui->passEdit->text()+"1");
}
void MainForm::on_Login2_clicked()
{
    ui->passEdit->setText(ui->passEdit->text()+"2");
}
void MainForm::on_Login3_clicked()
{
    ui->passEdit->setText(ui->passEdit->text()+"3");
}
void MainForm::on_Login4_clicked()
{
    ui->passEdit->setText(ui->passEdit->text()+"4");
}
void MainForm::on_Login5_clicked()
{
    ui->passEdit->setText(ui->passEdit->text()+"5");
}
void MainForm::on_Login6_clicked()
{
    ui->passEdit->setText(ui->passEdit->text()+"6");
}
void MainForm::on_Login7_clicked()
{
    ui->passEdit->setText(ui->passEdit->text()+"7");
}
void MainForm::on_Login8_clicked()
{
    ui->passEdit->setText(ui->passEdit->text()+"8");
}
void MainForm::on_Login9_clicked()
{
    ui->passEdit->setText(ui->passEdit->text()+"9");
}
void MainForm::on_LoginBack_clicked()
{
    ui->passEdit->setText("");
}
void MainForm::on_LoginGo_clicked()
{
    on_passEdit_returnPressed();
}

/* Deal with the signals and slots of the mainForm that
 * change what menu you are at
*/
void MainForm::hidetheThings(){
    ui->basicPageAdvanced->hide();
    ui->basicPageClockIn->hide();
    ui->basicPageClockOut->hide();
    ui->mainFinish->hide();

}
void MainForm::showtheThings(){

    ui->basicPageAdvanced->show();
    ui->basicPageClockIn->show();
    ui->basicPageClockOut->show();
    ui->mainFinish->show();
}
void MainForm::on_HeaderTabs_currentChanged(int index)
{
    if(index ==0){
        ui->MainTabs->setCurrentIndex(0);
        refreshEmployeeTab();
    }
    else if(index ==1){
        refreshProjectItemCombo();
        refreshProjectTab();
        ui->MainTabs->setCurrentIndex(1);
    }
    else if(index ==2){
        refreshItemTab();
        ui->MainTabs->setCurrentIndex(2);
    }
    else if(index ==3){
        refreshShiftTab();
        ui->MainTabs->setCurrentIndex(3);
    }
}
void MainForm::on_basicPageClockIn_clicked()
{
    QSqlQuery qry1(data),qry2(data),qry3(data),qry4(data);
    QString employeename, employeeid, timein,datein;
    QDateTime x = QDateTime::currentDateTime();
    QDateTime z = format_datetimes(x);

    employeeid = id;
    timein = z.toString("HH:mm:ss");
    datein = z.toString("yyyy-MM-dd");
    qry1.prepare("SELECT shiftcount,name FROM employeelist WHERE id = '"+id+"'");


    if(qry1.exec())
    {
        while(qry1.next())
        {
            employeename = qry1.value(1).toString();
        }
    }

    QString shiftid;
    qry1.prepare("select MAX(shiftid) As maxshiftid from shiftlist");
    if(qry1.exec()){
        while(qry1.next()){
            shiftid=qry1.value(0).toString();}}
    //qDebug()<<"The max shiftid is shown to be" +shiftid;
    int id1 = shiftid.toInt();
    id1++;
    shiftid = QString::number(id1);


    // Inserting clockin time and shiftnumber into pin.
    qry4.prepare("insert into shiftlist (employeeid,employeename,timein,datein,shiftid) values('"+employeeid+"','"+employeename+"','"+timein+"','"+datein+"','"+shiftid+"')");
    if (qry4.exec())
    {
        // Setting active.
        qry2.prepare("update employeelist set active=1 where id = '"+id+"'");
        qry2.exec();
        qry3.prepare("update employeelist set shiftcount = '"+shiftid+"' where id = '"+id+"'");
        qry3.exec();
    }

    ui->basicPageClockIn->hide();
    ui->basicPageClockOut->show();


}
void MainForm::on_basicPageClockOut_clicked()
{

    clockoutForm = new ClockoutForm(this);
    establishConnections();
    if (ip.split(".")[3] =="010")
        clockoutForm->showFullScreen();
    else
        clockoutForm->showMaximized();

    clockoutForm->ClockoutInitialize(id);

}
void MainForm::on_basicPageConnect_clicked()
{
    data.close();
    connectionForm = new ConnectionForm(this);
    QObject::connect(connectionForm,SIGNAL(finished()),this,SLOT(loginInitialize()));
    connectionForm->show();
}
void MainForm::on_basicPageAdvanced_clicked()
{
    advInitialize();
    EmployeeTab();
    ShiftTab();
    ProjectTab();
    ItemTab();
    SettingsTab();
    ui->MainTabs->setCurrentIndex(0);
    ui->HeaderTabs->setCurrentIndex(0);
}
void MainForm::on_mainFinish_clicked()
{
    loginInitialize();
}

/* Formatting of qdatetimes in order to only have times
 * that are in quarters
*/
QDateTime MainForm::format_datetimes(QDateTime z)
{
    int minTime = z.time().minute();
    int hourTime =z.time().hour();
    int dayOfMonth = z.date().daysInMonth();
    int dayOfYear = z.date().dayOfYear();
    if(minTime<7&&minTime>=0)
    {
        QTime x(z.time().hour(),0,0);
        QDateTime n(z.date(),x);
        z=n;
    }
    else if(minTime>=7&&minTime<23)
    {
        QTime x(z.time().hour(),15,0);
        QDate y(z.date());
        QDateTime n(y,x);
        z=n;
    }
    else if(minTime>=23&&minTime<37)
    {
        QTime x(z.time().hour(),30,0);
        QDateTime n(z.date(),x);
        z=n;
    }
    else if(minTime>=37&&minTime<53)
    {
        QTime x(z.time().hour(),45,0);
        QDateTime n(z.date(),x);
        z=n;
    }
    else if(minTime>=53 && hourTime==23 && z.date().day()==dayOfMonth&& z.date().daysInYear()==dayOfYear)
    {
        z.date().addYears(1);
        z.date().addDays(1);
        z.date().addMonths(1);
        QTime x(z.time().hour()+1,0,0);
        QDateTime n(z.date(),x);
        z=n;
    }
    else if(minTime>=53 && hourTime==23 && z.date().day()==dayOfMonth)
    {
        z.date().addDays(1);
        z.date().addMonths(1);
        QTime x(z.time().hour()+1,0,0);
        QDateTime n(z.date(),x);
        z=n;
    }
    else if(minTime>=53 && hourTime==23)
    {
        z.date().addDays(1);
        QTime x(z.time().hour()+1,0,0);
        QDateTime n(z.date(),x);
        z=n;
    }
    else if(minTime>=53)
    {
        QTime x(z.time().hour()+1,0,0);
        QDateTime n(z.date(),x);
        z=n;
    }
    return z;

}

//Login Section!

void MainForm::on_passEdit_returnPressed()
{
    pin= ui->passEdit->text();
    QSqlQuery qry1(data),qry2(data);
    qry1.prepare("SELECT * FROM employeelist where pin = '"+pin+"'");
    if (qry1.exec())
    {
        int count=0;
        while(qry1.next())
        {
           count++;
        }
        if(count == 1)
        {
            qry2.prepare("SELECT adminstatus,id FROM employeelist WHERE pin = '"+pin+"'");
            if(qry2.exec()){
                while(qry2.next()){
                    QString x= qry2.value(0).toString();
                    if(x=="1")
                        admin=true;
                    else
                        admin=false;
                    id = qry2.value(1).toString();
                }
            }

            basicInitialize();
        }
        if(count < 1){
            if(ui->passEdit->text()!="")
                ui->passLabel->setText("Invalid");

        }
        else
            ui->passLabel->setText("");
    }
    else
    {
        qDebug()<<qry1.lastError();
    }
}


// Employee Section!

/* Initialization and refreshing of the 'employeetab' in
 * 'sections'  along with the model for 'employeeView'.
*/
void MainForm::EmployeeTab()
{
    employeemodel=EmployeeModel();
    employeefiltermodel = EmployeeFilterModel();
    ui->EmployeeView->setModel(employeefiltermodel);
     ui->EmployeeView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    establishConnections();
    ui->EmployeeView->setSortingEnabled(true);
    ui->EmployeeId->setChecked(false);
    ui->EmployeeView->hideColumn(0);
    ui->EmployeeName->setChecked(true);
    ui->EmployeeView->showColumn(1);
    ui->EmployeePin->setChecked(true);
    ui->EmployeeView->showColumn(2);
    ui->EmployeeAdminStatus->setChecked(true);
    ui->EmployeeView->showColumn(3);
    ui->EmployeeShiftCount->setChecked(false);
    ui->EmployeeView->hideColumn(4);
    ui->EmployeeActive->setChecked(false);
    ui->EmployeeView->hideColumn(5);
    ui->EmployeeCurrent->setChecked(false);
    ui->EmployeeView->hideColumn(6);
    ui->AllRadio->setChecked(true);
    refreshEmployeeTab();

}
void MainForm::refreshEmployeeStuff(){

    shifteditform->EmployeeInitialize();
    refreshShiftEmployee();

}
void MainForm::refreshEmployeeTab(){
    employeemodel=EmployeeModel();
    employeefiltermodel = EmployeeFilterModel();
    ui->EmployeeView->setModel(employeefiltermodel);
    establishConnections();

    if(ui->EmployeeId->isChecked())
        ui->EmployeeView->showColumn(0);
    else
        ui->EmployeeView->hideColumn(0);
    if(ui->EmployeeName->isChecked())
        ui->EmployeeView->showColumn(1);
    else
        ui->EmployeeView->hideColumn(1);
    if(ui->EmployeePin->isChecked())
        ui->EmployeeView->showColumn(2);
    else
        ui->EmployeeView->hideColumn(2);

    if(ui->EmployeeAdminStatus->isChecked())
        ui->EmployeeView->showColumn(3);
    else
        ui->EmployeeView->hideColumn(3);

    if(ui->EmployeeShiftCount->isChecked())
        ui->EmployeeView->showColumn(4);
    else
        ui->EmployeeView->hideColumn(4);

    if(ui->EmployeeActive->isChecked())
        ui->EmployeeView->showColumn(5);
    else
        ui->EmployeeView->hideColumn(5);

    if(ui->EmployeeCurrent->isChecked())
        ui->EmployeeView->showColumn(6);
    else
        ui->EmployeeView->hideColumn(6);
    for(int i=1; i< employeefiltermodel->rowCount(); i++)
    {
        ui->EmployeeView->showRow(i);
    }
    if(ui->CurrentRadio->isChecked())
        on_CurrentRadio_toggled(true);
    if(ui->PastRadio->isChecked())
        on_PastRadio_toggled(true);


}
QSqlQueryModel * MainForm::EmployeeModel(){
    QSqlTableModel * model = new QSqlTableModel(0,data);
    model->setTable("employeelist");
    model->select();
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setHeaderData(0,Qt::Horizontal,tr("Id"));
    model->setHeaderData(1,Qt::Horizontal,tr("Name"));
    model->setHeaderData(2,Qt::Horizontal,tr("Pin"));
    model->setHeaderData(3,Qt::Horizontal,tr("Adminstatus"));
    model->setHeaderData(4,Qt::Horizontal,tr("Shiftcount"));
    model->setHeaderData(5,Qt::Horizontal,tr("Active"));
    model->setHeaderData(6,Qt::Horizontal,tr("Current"));
    return model;

}
QSortFilterProxyModel * MainForm::EmployeeFilterModel(){
    QSortFilterProxyModel * m = new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(employeemodel);
    return m;
}

/* Option menu 1 for EmployeeTab*/
void MainForm::on_EmployeeAdd_clicked()
{
    QSqlQuery * qry = new QSqlQuery(data);
    qry->prepare("insert into employeelist(name,pin,adminstatus,shiftcount,active,current)  values('~','"+QString::number(generateRandom())+"','0','1','0','1')");
    qry->exec();
    refreshEmployeeTab();
    refreshShiftEmployee();
    shifteditform->updateShiftEdit();
    ui->MainTabs->setCurrentIndex(0);
}
int MainForm::generateRandom(){
    int x = rand()%9000+1000;
    QSqlQuery * qry = new QSqlQuery(data);
    qry->prepare("select pin from employeelist");
    if(qry->exec())
    {
        while(qry->next())
        {
            if(qry->value(0).toInt()==x)
                return generateRandom();
        }
    }
    return x;
}
void MainForm::on_EmployeeArchive_clicked()
{
    QSqlQuery * qry = new QSqlQuery(data);
    QModelIndexList list = ui->EmployeeView->selectionModel()->selection().indexes();
    if(list != QModelIndexList())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Time-Track",   "Are you sure you want to archive "+
                                      employeemodel->record(employeefiltermodel->mapToSource(list.at(0)).row()).value(1).toString()
                                      + "'s records?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            for(int i=0; i< list.count(); i++)
            {
                QString id = employeemodel->record(employeefiltermodel->mapToSource(list.at(i)).row()).value(0).toString();
                qry->clear();
                if(employeemodel->record(employeefiltermodel->mapToSource(list.at(i)).row()).value(6).toInt()==1)
                    qry->prepare("update employeelist set current=0 where id = '"+id+"'");
                else if (employeemodel->record(employeefiltermodel->mapToSource(list.at(i)).row()).value(6).toInt()==0)
                    qry->prepare("update employeelist set current=1 where id = '"+id+"'");
                qry->exec();
            }
            refreshEmployeeTab();
            shifteditform->updateShiftEdit();
        }
    }
}
void MainForm::on_EmployeeDelete_clicked()
{
    QSqlQuery * qry = new QSqlQuery(data);
    QModelIndexList list = ui->EmployeeView->selectionModel()->selection().indexes();
    if(list != QModelIndexList())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Time-Track",   "Are you sure you want to perminantly delete "+
                                      employeemodel->record(employeefiltermodel->mapToSource(list.at(0)).row()).value(1).toString()
                                      + "'s records?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {

            for(int i=0; i< list.count(); i++)
            {
                QString id = employeemodel->record(employeefiltermodel->mapToSource(list.at(i)).row()).value(0).toString();
                qry->clear();
                qry->prepare("DELETE from employeelist where id='"+id+"'");
                qry->exec();
            }
            refreshEmployeeTab();
            refreshShiftEmployee();
            ui->MainTabs->setCurrentIndex(0);
            shifteditform->updateShiftEdit();
        }
    }
}
void MainForm::on_EmployeeRefresh_clicked()
{
    ui->EmployeeView->model()->submit();
}
/* Option menu 2 for EmployeeTab*/
void MainForm::on_EmployeeName_clicked()
{
    refreshEmployeeTab();
}
void MainForm::on_EmployeePin_clicked()
{
    refreshEmployeeTab();
}
void MainForm::on_EmployeeAdminStatus_clicked()
{
    refreshEmployeeTab();
}
void MainForm::on_EmployeeShiftCount_clicked()
{
    refreshEmployeeTab();
}
void MainForm::on_EmployeeActive_clicked()
{
    refreshEmployeeTab();
}
void MainForm::on_EmployeeId_clicked()
{
    refreshEmployeeTab();
}
void MainForm::on_EmployeeCurrent_clicked()
{
    refreshEmployeeTab();
}
/* Option menu 3 for EmployeeTab*/
void MainForm::on_AllRadio_toggled(bool checked)
{
    if(checked)
    {
        QSqlQueryModel * x = EmployeeModel();
        for(int i=0; i< x->rowCount(); i++)
        {
            ui->EmployeeView->showRow(i);
        }
    }

}
void MainForm::on_CurrentRadio_toggled(bool checked)
{
    if(checked)
    {
        QSqlQueryModel * x = EmployeeModel();
        for(int i=0; i< x->rowCount(); i++)
        {
            int current = x->record(i).value(6).toInt();
            if(current == 1)
               ui->EmployeeView->showRow(i);
            else
                ui->EmployeeView->hideRow(i);
        }
    }
}
void MainForm::on_PastRadio_toggled(bool checked)
{
    if(checked)
    {
        QSqlQueryModel * x = EmployeeModel();
        for(int i=0; i< x->rowCount(); i++)
        {
            int current = x->record(i).value(6).toInt();
            if(current == 1)
               ui->EmployeeView->hideRow(i);
            else
                ui->EmployeeView->showRow(i);
        }
    }
}

// Project Section!

/* Initialization and refreshing of the projecttab in
 * 'sections' along wiht the themodel. This section is
 * particular in the way it has two tables. That is the
 * for the three models for the 2nd table. 'ProjectItemModelFirst'
 * is for just after initialization the the others are for
 * general use after that special case.
*/
void MainForm::ProjectTab(){
    projectfiltermodel = ProjectFilterModel();
    projectmodel =  ProjectModel();
    ui->ProjectView->setModel(projectfiltermodel);

    establishConnections();
    QSqlQueryModel * y = ProjectItemModelFirst();
    ui->ProjectItemView->setModel(y);

    ui->ProjectView->setSortingEnabled(true);
    ui->ProjectItemView->setSortingEnabled(true);
    ui->ProjectView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->ProjectItemView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->ProjectView->hideColumn(0);
    ui->ProjectView->hideColumn(2);
    ui->ProjectName->setChecked(true);
    ui->ProjectAllRadio->setChecked(true);
    ui->ProjectDate->setChecked(true);

    ui->ProjectItemName->setChecked(true);
    ui->ProjectItemId->setChecked(false);
    ui->ProjectItemView->hideColumn(0);
    refreshProjectItemCombo();
    QSqlQueryModel * a = new QSqlQueryModel();
    QSqlQuery * A = new QSqlQuery(data);
    A->prepare("Select name from dimensionlist ORDER BY name ASC");
    A->exec();
    a->setQuery(*A);
    ui->ProjectItemDimension->setModel(a);
    ui->ShiftEmployeeCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

}
void MainForm::refreshProjectItemCombo(){
    QSqlQueryModel * a = new QSqlQueryModel();
    QSqlQuery * A = new QSqlQuery(data);
    A->prepare("Select name from itemlist where id>0 ORDER BY name ASC");
    A->exec();
    a->setQuery(*A);
    ui->ProjectItemCombo->setModel(a);
    ui->ProjectItemCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QCompleter * comp = new QCompleter(this);
    comp->setWidget(ui->ProjectItemCombo);
    comp->setCompletionMode(QCompleter::PopupCompletion);
    comp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->ProjectItemCombo->setCurrentText("");


}
void MainForm::on_ProjectView_clicked(const QModelIndex &index)
{

    QSqlQueryModel * x = ProjectItemModel();
    ui->ProjectItemView->setModel(x);
    refreshProjectItemTab();
    refreshProjectItemCombo();
}
void MainForm::refreshProjectStuff(){
    refreshShiftProject();
    shifteditform->ProjectInitialize();
    clockoutForm->ProjectInitialize();
}
void MainForm::refreshProjectTab(){
    projectmodel = ProjectModel();
    projectfiltermodel = ProjectFilterModel();
    ui->ProjectView->setModel(projectfiltermodel);
    establishConnections();
    ui->ProjectItemView->setModel(ProjectItemModelFirst());

    if(ui->ProjectId->isChecked())
        ui->ProjectView->showColumn(0);
    else
        ui->ProjectView->hideColumn(0);

    if(ui->ProjectName->isChecked())
        ui->ProjectView->showColumn(1);
    else
        ui->ProjectView->hideColumn(1);

    if(ui->ProjectCurrent->isChecked())
        ui->ProjectView->showColumn(2);
    else
        ui->ProjectView->hideColumn(2);
    if(ui->ProjectDate->isChecked())
        ui->ProjectView->showColumn(3);
    else
        ui->ProjectView->hideColumn(3);
    for(int i=1; i< projectfiltermodel->rowCount(); i++)
    {
        ui->ProjectView->showRow(i);
    }
    if(ui->ProjectCurrentRadio->isChecked())
        on_ProjectCurrentRadio_toggled(true);
    if(ui->ProjectPastRadio->isChecked())
        on_ProjectPastRadio_toggled(true);
}
void MainForm::refreshProjectItemTab(){
    ui->ProjectItemView->setModel(ProjectItemModelRefresh());
    ui->ProjectItemView->hideColumn(0);
    if(ui->ProjectItemName->isChecked())
        ui->ProjectItemView->showColumn(2);
    else
        ui->ProjectItemView->hideColumn(2);
    if(ui->ProjectItemId->isChecked())
        ui->ProjectItemView->showColumn(1);
    else
        ui->ProjectItemView->hideColumn(1);
}
QSqlQueryModel * MainForm::ProjectModel(){
    QSqlTableModel * model = new QSqlTableModel(0,data);
    model->setTable("projectlist");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();


    model->setHeaderData(0,Qt::Horizontal,tr("Id"));
    model->setHeaderData(1,Qt::Horizontal,tr("Name"));
    model->setHeaderData(2,Qt::Horizontal,tr("Current"));


    return model;

}
QSortFilterProxyModel * MainForm::ProjectFilterModel(){
    QSortFilterProxyModel * m = new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(ProjectModel());
    return m;
}
QSqlQueryModel * MainForm::ProjectItemModelFirst(){
    QSqlTableModel * model = new QSqlTableModel(0,data);
    QString id = projectfiltermodel->data(projectfiltermodel->index(0,1),Qt::DisplayRole).toString();
    model->setTable("Project"+id);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(0,Qt::Horizontal,tr("Id"));
    model->setHeaderData(1,Qt::Horizontal,tr("Name"));
    model->setHeaderData(2,Qt::Horizontal,tr("Quantity"));
    model->setHeaderData(3,Qt::Horizontal,tr("Dimension"));
    return model;
}
QSqlQueryModel * MainForm::ProjectItemModel(){
    QSqlTableModel * model = new QSqlTableModel(0,data);
    QModelIndexList  list =  ui->ProjectView->selectionModel()->selection().indexes();
    QModelIndex index = list.at(0);
    QString id = projectmodel->record(projectfiltermodel->mapToSource(index).row()).value(0).toString();
    currentProject = projectmodel->record(projectfiltermodel->mapToSource(index).row()).value(1).toString();
    model->setTable("Project"+id);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(1,Qt::Horizontal,tr("Id"));
    model->setHeaderData(2,Qt::Horizontal,tr("Name"));
    model->setHeaderData(3,Qt::Horizontal,tr("Quantity"));
    model->setHeaderData(4,Qt::Horizontal,tr("Dimension"));    
    return model;
}
QSqlQueryModel * MainForm::ProjectItemModelRefresh(){
    QSqlTableModel * model = (QSqlTableModel*)ui->ProjectItemView->model();
    model->select();
    return model;
}

/* Option menu 1 for ProjectTab*/
void MainForm::on_ProjectAdd_clicked()
{
    QSqlQuery * qry = new QSqlQuery(data);
    qry->prepare("insert into projectlist(name,current,date)  values('~','1','"+QString::number(QDate::currentDate().month())+"/1/"+QString::number(QDate::currentDate().year())+"')");
    qry->exec();
    qry->clear();
    QString id;
    qry->prepare("select id from projectlist where id=(select max(id) from projectlist)");
    if(qry->exec())
    {
        while(qry->next())
        {
            id = qry->value(0).toString();
        }
    }
    qry->clear();
    qry->prepare("CREATE TABLE Project"+id+" (id int PRIMARY KEY AUTO_INCREMENT, itemid int, name varchar(45),quantity varchar(45), dimension varchar(45))");
    qry->exec();
    refreshProjectTab();
    refreshShiftProject();
    ui->MainTabs->setCurrentIndex(1);
    shifteditform->updateShiftEdit();

}
void MainForm::on_ProjectDelete_clicked()
{
    QSqlQuery * qry = new QSqlQuery(data);
    QModelIndexList list = ui->ProjectView->selectionModel()->selection().indexes();
    if(list != QModelIndexList())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Time-Track", "Are you sure you want to delete the " +
                                      projectmodel->record(projectfiltermodel->mapToSource(list.at(0)).row()).value(1).toString()
                                      + "'s records?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            for(int i=0; i< list.count(); i++)
            {
                QModelIndex index =list.at(i);
                int idInt = projectmodel->record(projectfiltermodel->mapToSource(index).row()).value(0).toInt();
                QString id = "Project"+QString::number(idInt);
                qDebug()<<id;
                qry->clear();
                qry->prepare("DROP TABLE "+id+"");
                qry->exec();
                qry->clear();
                qry->prepare("DELETE from projectlist where id='"+QString::number(idInt)+"'");
                qry->exec();
            }
        }
        refreshProjectTab();
        refreshShiftProject();
        ui->MainTabs->setCurrentIndex(1);
        shifteditform->updateShiftEdit();
    }
}
void MainForm::on_ProjectArchive_clicked()
{
    QSqlQuery * qry = new QSqlQuery(data);
    QModelIndexList list = ui->ProjectView->selectionModel()->selection().indexes();
    if(list != QModelIndexList())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Time-Track",   "Are you sure you want to archive the " +
                                      projectmodel->record(projectfiltermodel->mapToSource(list.at(0)).row()).value(1).toString()
                                      + "'s records?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {

            for(int i=0; i< list.count(); i++)
            {
                QString id = projectmodel->record(projectfiltermodel->mapToSource(list.at(i)).row()).value(0).toString();
                qry->clear();
                if (projectmodel->record(projectfiltermodel->mapToSource(list.at(i)).row()).value(2).toInt() == 1)
                    qry->prepare("update projectlist set current=0 where id = '"+id+"'");
                else if (projectmodel->record(projectfiltermodel->mapToSource(list.at(i)).row()).value(2).toInt() == 0)
                    qry->prepare("update projectlist set current=1 where id = '"+id+"'");
                qry->exec();
            }
            refreshProjectTab();
            shifteditform->updateShiftEdit();
        }
    }
}
/* Option menu 2 for ProjectTab*/
void MainForm::on_ProjectName_clicked()
{
    refreshProjectTab();
}
void MainForm::on_ProjectId_clicked()
{
    refreshProjectTab();
}
void MainForm::on_ProjectCurrent_clicked()
{
    refreshProjectTab();
}
void MainForm::on_ProjectDate_clicked()
{
    refreshProjectTab();
}
/* Option menu 3 for ProjectTab*/
void MainForm::on_ProjectAllRadio_toggled(bool checked)
{
    if(checked){
        QSqlQueryModel * x = ProjectModel();
        for(int i=0; i< x->rowCount(); i++)
        {
            ui->ProjectView->showRow(i);
        }
    }
}
void MainForm::on_ProjectCurrentRadio_toggled(bool checked)
{
    if(checked){
        QSqlQueryModel * x = ProjectModel();
        for(int i=0; i< x->rowCount(); i++)
        {
            int current = x->record(i).value(2).toInt();
            if(current == 1)
               ui->ProjectView->showRow(i);
            else
                ui->ProjectView->hideRow(i);
        }
    }
}
void MainForm::on_ProjectPastRadio_toggled(bool checked)
{
    if(checked){
        QSqlQueryModel * x = ProjectModel();
        for(int i=0; i< x->rowCount(); i++)
        {
            int current = x->record(i).value(2).toInt();
            if(current == 1)
               ui->ProjectView->hideRow(i);
            else
                ui->ProjectView->showRow(i);
        }
    }
}
/* Option menu 4 for ProjectTab*/
void MainForm::on_ProjectItemName_clicked()
{
    refreshProjectItemTab();
}
void MainForm::on_ProjectItemId_clicked()
{
    refreshProjectItemTab();
}
/* Option menu 5 for ProjectTab*/
void MainForm::on_ProjectItemAdd_clicked()
{
    QString itemName = ui->ProjectItemCombo->currentText();
    QString itemQuantity = ui->ProjectItemQuantity->text();
    QString itemDimension = ui->ProjectItemDimension->currentText();
    if( itemName != "")
    {
        QSqlQuery * qry = new QSqlQuery(data);
        qry->prepare("select id from itemlist where name='"+itemName+"'");
        QString itemId;
        if(qry->exec()){
            while(qry->next())
                itemId = qry->value(0).toString();
        }
        qry->clear();
        QSqlTableModel * model = (QSqlTableModel*)ui->ProjectItemView->model();
        QString table = model->tableName();

        qry->prepare("insert into "+table+"(itemid, name, quantity, dimension) "
                          "values('"+itemId+"','"+itemName+"','"+itemQuantity+"','"+itemDimension+"') "
                                 "ON DUPLICATE KEY UPDATE id = '"+itemId+"'");
        qry->exec();
        model->select();
        ui->ProjectItemView->setModel(model);
        shifteditform->updateShiftEdit();
    }
}
void MainForm::on_ProjectItemRemove_clicked()
{
    QSqlQuery * qry = new QSqlQuery(data);
    QModelIndexList list = ui->ProjectItemView->selectionModel()->selection().indexes();
    if(list != QModelIndexList())
    {
        QSqlTableModel * tablemodel = (QSqlTableModel*)ui->ProjectItemView->model();
        QString table = tablemodel->tableName();

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Time-Track",   "Are you sure you want to remove the "+ tablemodel->record(list.at(0).row()).value(2).toString()+" item from this project?",
                                        QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            for(int i=0; i< list.count(); i++)
            {
                QModelIndex index =list.at(i);
                QString id = tablemodel->record(index.row()).value(0).toString();

                qry->clear();
                qry->prepare("DELETE from "+table+" where id='"+id+"'");
                qry->exec();
            }
            tablemodel->select();
            ui->ProjectItemView->setModel(tablemodel);
            shifteditform->updateShiftEdit();
        }
    }
}

// Item Section!

/* Initialization,model,and refreshing for the
 * 'itemtab' in 'sections'
 */
void MainForm::ItemTab(){
    itemmodel= ItemModel();
    itemfiltermodel = ItemFilterModel();
    ui->ItemView->setModel(itemfiltermodel);

    establishConnections();
    ui->ItemView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->ItemView->hideColumn(1);
    ui->ItemView->setSortingEnabled(true);
    ui->ItemName->setChecked(true);
    ui->ItemId->setChecked(false);
    ui->ItemCategory->setChecked(true);
    ui->ItemSub->setChecked(true);
    ui->ItemDimension->setChecked(true);
}
QSqlQueryModel * MainForm::ItemModel(){
    QSqlTableModel * model = new QSqlTableModel(0,data);
    model->setTable("itemlist");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(0,Qt::Horizontal,tr("Id"));
    model->setHeaderData(1,Qt::Horizontal,tr("Name"));
    model->setHeaderData(2,Qt::Horizontal,tr("Category"));
    model->setHeaderData(3,Qt::Horizontal,tr("Sub-Catergory"));
    model->setHeaderData(4,Qt::Horizontal,tr("Dimension"));
    return model;
}
QSortFilterProxyModel * MainForm::ItemFilterModel(){
    QSortFilterProxyModel * m = new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(itemmodel);
    return m;
}
void MainForm::refreshItemTab(){
    itemmodel= ItemModel();
    itemfiltermodel = ItemFilterModel();
    ui->ItemView->setModel(itemfiltermodel);
    establishConnections();
    if(ui->ItemCategory->isChecked())
        ui->ItemView->showColumn(2);
    else
        ui->ItemView->hideColumn(2);
    if(ui->ItemSub->isChecked())
        ui->ItemView->showColumn(3);
    else
        ui->ItemView->hideColumn(3);
    if(ui->ItemDimension->isChecked())
        ui->ItemView->showColumn(4);
    else
        ui->ItemView->hideColumn(4);
    if(ui->ItemId->isChecked())
        ui->ItemView->showColumn(0);
    else
        ui->ItemView->hideColumn(0);
    if(ui->ItemName->isChecked())
        ui->ItemView->showColumn(1);
    else
        ui->ItemView->hideColumn(1);
}
void MainForm::refreshItemStuff(){
    refreshShiftItem();
    shifteditform->ItemInitialize();
    clockoutForm->ItemInitialize();
}

/* Option menu 1 for ItemTab*/
void MainForm::on_ItemAdd_clicked()
{
    QSqlQuery * qry = new QSqlQuery(data);
    QSqlQueryModel * x = ItemModel();
    qry->prepare("insert into itemlist(name,category,subcategory,dimension)  values('~','~','~','~')");
    qry->exec();
    refreshItemTab();
    refreshProjectItemCombo();
    refreshShiftItem();
    ui->MainTabs->setCurrentIndex(2);
}
void MainForm::on_ItemDelete_clicked()
{
    QSqlQuery * qry = new QSqlQuery(data);
    QModelIndexList list = ui->ItemView->selectionModel()->selection().indexes();
    if(list != QModelIndexList())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Time-Track",   "Are you sure you want to permanantly delete the " +
                                      itemmodel->record(itemfiltermodel->mapToSource(list.at(0)).row()).value(1).toString()
                                      + " item from this project?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            for(int i=0; i< list.count(); i++)
            {
                QString id = itemmodel->record(itemfiltermodel->mapToSource(list.at(i)).row()).value(0).toString();
                qry->clear();
                qry->prepare("DELETE from itemlist where id='"+id+"'");
                qry->exec();
            }
            refreshItemTab();
            refreshProjectItemCombo();
            refreshShiftItem();
            ui->MainTabs->setCurrentIndex(2);
        }
    }
}
/* Option menu 2 for ItemTab*/
void MainForm::on_ItemName_clicked()
{
    refreshItemTab();
}
void MainForm::on_ItemId_clicked()
{
    refreshItemTab();
}
void MainForm::on_ItemCategory_clicked()
{
    refreshItemTab();
}
void MainForm::on_ItemSub_clicked()
{
    refreshItemTab();
}
void MainForm::on_ItemDimension_clicked()
{
    refreshItemTab();
}

// Shift Section!

/* Initialization,three combobox refreshers,model,
 * and main refresher for the 'shiftTab' within 'sections'
*/
void MainForm::ShiftTab(){
    shiftmodel = ShiftModel();
    shiftfiltermodel=ShiftFilterModel();
    ui->ShiftView->setModel(shiftfiltermodel);

    int day = QDate::currentDate().dayOfWeek();
    ui->ShiftDate1->setDate(QDate::currentDate().addDays(-day));
    ui->ShiftDate2->setDate(QDate::currentDate().addDays(6-day));

    refreshShiftProject();
    refreshShiftItem();
    refreshShiftEmployee();



    ui->ShiftView->hideColumn(0);
    ui->ShiftView->hideColumn(1);
    ui->ShiftView->hideColumn(2);
    ui->ShiftView->hideColumn(3);
    ui->ShiftView->hideColumn(13);

    ui->ShiftView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->ShiftView->horizontalHeader()->setStretchLastSection(true);
    ui->ShiftEmployeeCombo->setEnabled(false);
    ui->ShiftProjectCombo->setEnabled(false);
    ui->ShiftItemCombo->setEnabled(false);
    ui->ShiftView->setSortingEnabled(true);

}
void MainForm::refreshShiftEmployee(){
    QSqlQueryModel * a = new QSqlQueryModel();
    QSqlQuery * A = new QSqlQuery(data);
    A->prepare("Select name from employeelist ORDER BY name ASC");
    A->exec();
    a->setQuery(*A);
    ui->ShiftEmployeeCombo->setModel(a);
    ui->ShiftEmployeeCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QCompleter * comp = new QCompleter(this);
    comp->setWidget(ui->ShiftEmployeeCombo);
    comp->setCompletionMode(QCompleter::PopupCompletion);
    comp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->ShiftEmployeeCombo->setCurrentText("");
}
void MainForm::refreshShiftProject(){
    QSqlQueryModel * b = new QSqlQueryModel();
    QSqlQuery * B = new QSqlQuery(data);
    B->prepare("Select name from projectlist ORDER BY name ASC");
    B->exec();
    b->setQuery(*B);
    ui->ShiftProjectCombo->setModel(b);
    ui->ShiftProjectCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QCompleter * comp = new QCompleter(this);
    comp->setWidget(ui->ShiftProjectCombo);
    comp->setCompletionMode(QCompleter::PopupCompletion);
    comp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->ShiftProjectCombo->setCurrentText("");
}
void MainForm::refreshShiftItem(){
    QSqlQueryModel * c = new QSqlQueryModel();
    QSqlQuery * C = new QSqlQuery(data);
    C->prepare("Select name from itemlist ORDER BY name ASC");
    C->exec();
    c->setQuery(*C);
    ui->ShiftItemCombo->setModel(c);
    ui->ShiftItemCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QCompleter * comp = new QCompleter(this);
    comp->setWidget(ui->ShiftItemCombo);
    comp->setCompletionMode(QCompleter::PopupCompletion);
    comp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->ShiftItemCombo->setCurrentText("");
}
QSqlQueryModel * MainForm::ShiftModel(){
    QSqlTableModel * model = new QSqlTableModel(0,data);
    model->setTable("shiftlist");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(0,Qt::Horizontal,tr("Id"));
    model->setHeaderData(1,Qt::Horizontal,tr("Employee Id"));
    model->setHeaderData(2,Qt::Horizontal,tr("Project Id"));
    model->setHeaderData(3,Qt::Horizontal,tr("Item Id"));
    model->setHeaderData(4,Qt::Horizontal,tr("Employee Name"));
    model->setHeaderData(5,Qt::Horizontal,tr("Project Name"));
    model->setHeaderData(6,Qt::Horizontal,tr("Item Name"));
    model->setHeaderData(7,Qt::Horizontal,tr("Time In"));
    model->setHeaderData(8,Qt::Horizontal,tr("Date In"));
    model->setHeaderData(9,Qt::Horizontal,tr("Time Out"));
    model->setHeaderData(10,Qt::Horizontal,tr("Date out"));
    model->setHeaderData(11,Qt::Horizontal,tr("Lunch"));
    model->setHeaderData(12,Qt::Horizontal,tr("Time"));
    model->setHeaderData(14,Qt::Horizontal,tr("Description"));
    return model;
}
QSortFilterProxyModel * MainForm::ShiftFilterModel(){
    QSortFilterProxyModel * m = new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(shiftmodel);
    return m;
}
void MainForm::refreshShiftTab(){
    shiftmodel = ShiftModel();
    shiftfiltermodel = ShiftFilterModel();
    ui->ShiftView->setModel(shiftfiltermodel);

    for(int i = 0; i < shiftfiltermodel->rowCount();  i++)
    {
        ui->ShiftView->showRow(i);
    }
    if(ui->ShiftEmployeeBox->isChecked())
    {
        for(int i = 0; i< shiftfiltermodel->rowCount(); i++)
        {
            QString name = shiftfiltermodel->data(shiftfiltermodel->index(i,4),Qt::DisplayRole).toString();
            if(name != ui->ShiftEmployeeCombo->currentText()){

               ui->ShiftView->hideRow(i);
            }

        }
    }
    if(ui->ShiftProjectBox->isChecked())
    {
        for(int i=0; i< shiftfiltermodel->rowCount(); i++)
        {
            QString project = shiftfiltermodel->data(shiftfiltermodel->index(i,5),Qt::DisplayRole).toString();
            if(project != ui->ShiftProjectCombo->currentText()){
                ui->ShiftView->hideRow(i);
            }
        }
    }
    if(ui->ShiftItemBox->isChecked())
    {
        for(int i=0; i< shiftfiltermodel->rowCount(); i++)
        {
            QString item = shiftfiltermodel->data(shiftfiltermodel->index(i,6),Qt::DisplayRole).toString();
            if(item != ui->ShiftItemCombo->currentText())
            {
                ui->ShiftView->hideRow(i);
            }
        }
    }
    for(int i=0; i< shiftfiltermodel->rowCount(); i++)
    {
        QString date = shiftfiltermodel->data(shiftfiltermodel->index(i,9),Qt::DisplayRole).toString();
        QDate in= QDate(date.split("-")[0].toInt(),date.split("-")[1].toInt(),date.split("-")[2].toInt());
        if(in < ui->ShiftDate1->date() || in > ui->ShiftDate2->date())
            ui->ShiftView->hideRow(i);
    }
    int h = 0,m = 0;
    for(int i=0; i< shiftfiltermodel->rowCount(); i++)
    {
        QString time = shiftfiltermodel->data(shiftfiltermodel->index(i,12),Qt::DisplayRole).toString();
        if(!ui->ShiftView->isRowHidden(i)&&time!="")
        {
            QString hours = time.split(":")[0];
            QString minutes = time.split(":")[1];
            h += hours.toInt();
            m += minutes.toInt();
        }
    }
    h += m/60;
    m = m%60;

    QString hours = QString::number(h);
    QString minutes = QString::number(m);
    if (minutes == "0")
        minutes ="00";
    ui->ShiftTotalTime->setText(hours+":"+minutes);
}
void MainForm::displayShiftSuccess(){
    QMessageBox::StandardButton reply;
    if(!shifteditform->getSuccess())
        reply = QMessageBox::information(this, "Time-Track","Shift edit failed!",QMessageBox::Ok);
}

/* Option menu 1 for shiftTab*/
void MainForm::on_ShiftDate1_dateChanged(const QDate &date)
{
    refreshShiftTab();
}
void MainForm::on_ShiftDate2_dateChanged(const QDate &date)
{
    refreshShiftTab();
}
/* Option menu 2 for shiftTab*/
void MainForm::on_ShiftEmployeeBox_clicked()
{
    refreshShiftTab();
    if(ui->ShiftEmployeeBox->isChecked())
        ui->ShiftEmployeeCombo->setEnabled(true);
    else
        ui->ShiftEmployeeCombo->setEnabled(false);
}
void MainForm::on_ShiftProjectBox_clicked()
{
    refreshShiftTab();
    if(ui->ShiftProjectBox->isChecked())
        ui->ShiftProjectCombo->setEnabled(true);
    else
        ui->ShiftProjectCombo->setEnabled(false);
}
void MainForm::on_ShiftItemBox_clicked()
{
    refreshShiftTab();
    if(ui->ShiftItemBox->isChecked())
        ui->ShiftItemCombo->setEnabled(true);
    else
        ui->ShiftItemCombo->setEnabled(false);
}
void MainForm::on_ShiftEmployeeCombo_currentTextChanged(const QString &arg1)
{
    refreshShiftTab();
}
void MainForm::on_ShiftProjectCombo_currentTextChanged(const QString &arg1)
{
    refreshShiftTab();
}
void MainForm::on_ShiftItemCombo_currentTextChanged(const QString &arg1)
{
    refreshShiftTab();
}
/* Option menu 3 for shiftTab*/
void MainForm::on_ShiftAdd_clicked()
{
    shifteditform = new ShiftEditForm(this);
    establishConnections();
    shifteditform->AddShift();
    refreshShiftTab();
}
void MainForm::on_ShiftEdit_clicked()
{
    shifteditform = new ShiftEditForm(this);
    establishConnections();
    QModelIndexList list = ui->ShiftView->selectionModel()->selection().indexes();
    if(list!= QModelIndexList())
    {
        QModelIndex index =list.at(0);
        QSqlRecord rec = shiftmodel->record(shiftfiltermodel->mapToSource(index).row());
        if(rec.value(2).toString()=="0")
            shifteditform->EditWorkingShift(rec.value(13).toString(),rec.value(0).toString());
        else
            shifteditform->EditFinishedShift(rec.value(13).toString());
    }
    refreshShiftTab();
}
void MainForm::on_ShiftDelete_clicked()
{
    QSqlQuery * qry = new QSqlQuery(data);
    QModelIndexList list = ui->ShiftView->selectionModel()->selection().indexes();
    if(list != QModelIndexList())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Time-Track",   "Are you sure you want to permanantly delete " +
                                      shiftmodel->record(shiftfiltermodel->mapToSource(list.at(0)).row()).value(4).toString()
                                      + "'s shift?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {

            for(int i=0; i< list.count(); i++)
            {
                QString id = shiftmodel->record(shiftfiltermodel->mapToSource(list.at(i)).row()).value(13).toString();
                qry->clear();
                qry->prepare("DELETE from shiftlist where shiftid='"+id+"'");
                qry->exec();
                if(shiftmodel->record(shiftfiltermodel->mapToSource(list.at(i)).row()).value(2).toString()=="0")
                {
                    qry->clear();
                    qry->prepare("update employeelist set active='0' where name='"+shiftmodel->record(shiftfiltermodel->mapToSource(list.at(i)).row()).value(4).toString()+"'");
                    qry->exec();
                }
            }
        }
        refreshShiftTab();
    }
}

//Settings Section!

/* Quickly made this section so that I could maximize
 * and fullscreen the program easily plan on making
 * this cleaner later.*/
void MainForm::SettingsTab(){

    QSqlQueryModel * x = ProjectModel();
    currentProject = x->record(1).value(1).toString();
    ui->groupBox_18->hide();
    ui->groupBox_19->hide();
    ui->groupBox_20->hide();

}
void MainForm::on_SettingsMax_clicked()
{
    this->setWindowState(Qt::WindowMaximized);
}
void MainForm::on_SettingsFull_clicked()
{

    this->setWindowState(Qt::WindowFullScreen);
}
void MainForm::on_SettingsExport_clicked()
{
    exportForm =  new ExportForm(this);
    establishConnections();
    exportForm->show();

}
void MainForm::exportToExcel(){

    QString FileToExport = exportForm->Location;
    QString TabletoExport = exportForm->Table;
    QXlsx::Document doc;

    if(TabletoExport == "Employees")
    {
        QSqlQueryModel * model = (QSqlQueryModel *)ui->EmployeeView->model();
        QXlsx::Format title;
        title.setFontBold(true);
        title.setFontSize(20);
        doc.setRowFormat(1,1,title);

        doc.write(1,1,"AACI");
        if(ui->AllRadio->isChecked())
            doc.write(2,1,"Employee(s): All");
        else if(ui->CurrentRadio->isChecked())
            doc.write(2,1,"Employee(s): Current");
        else if(ui->PastRadio->isChecked())
            doc.write(2,1,"Employee(s): Past");

        QXlsx::Format label;
        label.setFontBold(true);
        label.setFontSize(15);
        doc.setRowFormat(4,4,label);

        qDebug()<<"here";
        doc.write(4,1,"Name");
        doc.write(4,2,"Pin");
        doc.write(4,3,"Admin Status");

        for(int i = 0; i < model->rowCount(); i++){
            for(int j = 0; j < model->columnCount(); j++){
                if(!ui->EmployeeView->isRowHidden(i))
                {
                    if(j == 1 || j == 2 || j == 3)
                        doc.write(i+5,j, model->record(i).value(j).toString());
                }
            }
        }
    }
    if(TabletoExport == "Projects")
    {
        QSqlQueryModel * model = (QSqlQueryModel *)ui->ProjectView->model();
        QXlsx::Format title;
        title.setFontBold(true);
        title.setFontSize(20);

        doc.setRowFormat(1,1,title);

        doc.write(1,1,"AACI");
        if(ui->ProjectAllRadio->isChecked())
            doc.write(2,1,"Project(s): All");
        else if(ui->ProjectCurrentRadio->isChecked())
            doc.write(2,1,"Project(s): Current");
        else if(ui->ProjectPastRadio->isChecked())
            doc.write(2,1,"Project(s): Past");


        QXlsx::Format label;
        label.setFontBold(true);
        label.setFontSize(15);
        doc.setRowFormat(4,4,label);


        doc.write(4,1,"Name");
        doc.write(4,2,"Date");

        for(int i = 0; i < model->rowCount(); i++){
            for(int j = 0; j < model->columnCount(); j++){
                if(!ui->ProjectView->isRowHidden(i))
                {
                    if(j == 1)
                        doc.write(i+5,j, model->record(i).value(j).toString());
                    else if(j == 3)
                        doc.write(i+5,j-1, model->record(i).value(j).toString());
                }
            }
        }
    }
    if(TabletoExport == "Items")
    {
        QSqlQueryModel * model = (QSqlQueryModel *)ui->ItemView->model();


        QXlsx::Format title;
        title.setFontBold(true);
        title.setFontSize(20);

        doc.setRowFormat(1,1,title);

        doc.write(1,1,"AACI");

        doc.write(2,1,"Items(s): ALL");


        QXlsx::Format label;
        label.setFontBold(true);
        label.setFontSize(15);
        doc.setRowFormat(4,4,label);


        doc.write(4,1,"Name");
        doc.write(4,2,"Category");
        doc.write(4,3,"Sub-Catergory");
        doc.write(4,4,"Dimension");

        for(int i = 0; i < model->rowCount(); i++){
            for(int j = 0; j < model->columnCount(); j++){
                if(!ui->ShiftView->isRowHidden(i))
                {
                    if(j == 1 || j == 2 || j == 3 || j == 4)
                        doc.write(i+5,j, model->record(i).value(j).toString());
                }
            }
        }
    }
    if(TabletoExport == "Project's Items")
    {
        QSqlQuery * qry = new QSqlQuery(data);
        QSqlTableModel * model = new QSqlTableModel(0,data);
        qry->prepare("select id from projectlist where name='"+currentProject+"'");
        QString currentId;
        if(qry->exec())
        {
            while(qry->next())
            {
                currentId = qry->value(0).toString();
            }
        }

        doc.write(1,1,"AACI");
        doc.write(2,1,"Project: " + currentProject);
        doc.write(4,1,"Items");

        QXlsx::Format title;
        title.setFontBold(true);
        title.setFontSize(20);
        doc.setRowFormat(1,1,title);

        QXlsx::Format label;
        label.setFontBold(true);
        label.setFontSize(15);
        doc.setRowFormat(4,4,label);

        qry->clear();
        QString table = "project"+currentId;
        qry->prepare("select name from "+table+"");
        if(qry->exec())
        {
            int i =0;
            while(qry->next())
            {
                doc.write(i+5,1, qry->value(0).toString());
                i++;
            }
        }else
        {
            doc.write(5,1,"No project selected in the project tab. Please do so, and try again.");
        }


    }
    if(TabletoExport == "Shifts"){
        QSqlQueryModel * model = (QSqlQueryModel *)ui->ShiftView->model();

        QXlsx::Format title;
        title.setFontBold(true);
        title.setFontSize(20);
        doc.setRowFormat(1,1,title);

        doc.write(1,1,"AACI");
        if(ui->ShiftEmployeeCombo->isEnabled())
            doc.write(2,1,"Employee(s): "+ui->ShiftEmployeeCombo->currentText());
        else
            doc.write(2,1,"Employee(s): All");
        if(ui->ShiftProjectCombo->isEnabled())
            doc.write(3,1,"Project(s): "+ui->ShiftProjectCombo->currentText());
        else
            doc.write(3,1,"Project(s): All");
        if(ui->ShiftItemCombo->isEnabled())
            doc.write(4,1,"Item(s): "+ui->ShiftItemCombo->currentText());
        else
            doc.write(4,1,"Item(s): All");

        QXlsx::Format label;
        label.setFontBold(true);
        label.setFontSize(15);
        doc.setRowFormat(6,6,label);


        doc.write(6,1,"Name");
        doc.write(6,2,"Project");
        doc.write(6,3,"Item");
        doc.write(6,4,"Clock In");
        doc.write(6,5,"Clock out");
        doc.write(6,6,"Lunch");
        doc.write(6,7,"Time");
        doc.write(6,8,"Notes");
        for(int i = 0; i < model->rowCount(); i++){
            for(int j = 0; j < model->columnCount(); j++){

                if(!ui->ShiftView->isRowHidden(i))
                {

                    if(j == 4 || j == 5 || j == 6)
                        doc.write(i+7,j-3, model->record(i).value(j).toString());
                    else if( j == 7 || j == 8)
                        doc.write(i+7,j-3, (model->record(i).value(j).toString() + " " + model->record(i).value(j+2).toString()));
                    else if( j == 11 || j == 12)
                        doc.write(i+7,j-5, model->record(i).value(j).toString());
                    else if(j == 14)
                        doc.write(i+7,j-6, model->record(i).value(j).toString());

                }
                //doc.write(i+1,j+1, model->record(i).value(j).toString());

            }
        }

    }
    doc.saveAs(FileToExport);


}
void MainForm::on_SettingsPrint_clicked()
{

}
void MainForm::on_SettingsAll_clicked()
{

}
void MainForm::on_SettingsConnections_clicked()
{
    this->hide();
    connectionForm = new ConnectionForm(this);
    connectionForm->show();
    QObject::connect(connectionForm,SIGNAL(finished()),this,SLOT(start()));

}
/* This is the method used for sending the data
 * database to sub-form.
 */
QSqlDatabase MainForm::getData() const
{
    return data;
}







