#include "employeeeditform.h"
#include "ui_employeeeditform.h"
#include "mainform.h"

EmployeeEditForm::EmployeeEditForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeEditForm)
{
    ui->setupUi(this);
    data = ((MainForm*)parentWidget())->getData();
    ui->admin->setChecked(false);


}
// Initializer for when a employee is being created
void EmployeeEditForm::AddEmployee(){
    this->showNormal();
    task="add";
    ui->name->setText("");
    ui->pin->setText("");
    ui->admin->setChecked(false);
    ui->active->setChecked(false);
    ui->current->setChecked(true);
}

// Initializer for when a employee is being edited
void EmployeeEditForm::EditEmployee(QString x){
    this->showNormal();
    task = "edit";
    id=x;
    QSqlQuery * qry = new QSqlQuery(data);
    QString name,pin,admin,current,active;
    qry->prepare("SELECT name,pin,adminstatus,current,active FROM employeelist WHERE id ='"+id+"'");
    if(qry->exec()){
        while(qry->next()){
            name = qry->value(0).toString();
            pin = qry->value(1).toString();
            admin = qry->value(2).toString();
            current = qry->value(3).toString();
            active = qry->value(4).toString();
        }
    }
    ui->name->setText(name);
    ui->pin->setText(pin);
    if(admin =="1")
        ui->admin->setChecked(true);
    else
        ui->admin->setChecked(false);
    if(current =="1")
        ui->current->setChecked(true);
    else
        ui->current->setChecked(false);

    if(active =="1")
        ui->active->setChecked(true);
    else
        ui->active->setChecked(false);

}

EmployeeEditForm::~EmployeeEditForm()
{
    delete ui;
}
// Generates random unique pin
int EmployeeEditForm::generateRandom(){
    int x = (qrand()%999+100) *1000;
    int y = qrand()%999+100;
    x=x+y;
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_real_distribution<> distribution(100000, 999999);
//    int x = (int)distribution(gen);
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
void EmployeeEditForm::on_GenerateButton_clicked()
{
    ui->pin->setText(QString::number(generateRandom()));
}

// Getters for the values of the validation methods
QString EmployeeEditForm::getSuccessMsg() const
{
    return successMsg;
}
bool EmployeeEditForm::getSuccess() const
{
    return success;
}
// Validation for when a employeee is created
QString EmployeeEditForm::AddValid(){
    QString error;
    QRegExp name_regrex("^[a-zA-Z ]+$");
    QRegExp pin_regrex("^[0-9]+$");

    if(ui->name->text().length()<5)
        error = "Invalid Name: Minimum length 5";
    else if (!name_regrex.exactMatch(ui->name->text()))
       error = "Invalid Name: Must be Letters only";
    else if(ui->pin->text().length() < 4)
       error = "Invalid Pin: Minimum length 4";
    else if(!pin_regrex.exactMatch(ui->pin->text()))
           error = "Invalid Pin: Must be Numbers only";
    return error;
}
// Validation for when a employee is edited
QString EmployeeEditForm::EditValid(){
    QString error = "";
    QRegExp name_regrex("^[a-zA-Z ]+$");
    QRegExp pin_regrex("^[0-9]+$");
    if(ui->name->text().length()<5)
        error = "Invalid Name: Minimum length 5";
    else if (!name_regrex.exactMatch(ui->name->text()))
       error = "Invalid Name: Must be Letters only";
    else if(ui->pin->text().length() < 4)
       error = "Invalid Pin: Minimum length 4";
    else if(!pin_regrex.exactMatch(ui->pin->text()))
           error = "Invalid Pin: Must be Numbers only";
    else{
        QSqlQuery * qry = new QSqlQuery(data);
        QString pin = ui->pin->text();
        QString currentPin;
        qry->prepare("SELECT pin FROM employeelist WHERE pin = "+pin+"");
        int pinCount =0;
        if(qry->exec()){
            while(qry->next()){
                pinCount++;
            }
        }
        qry->clear();
        qry->prepare("SELECT pin FROM employeelist WHERE id = "+id+"");
        if(qry->exec()){
            while(qry->next()){
                currentPin = qry->value(0).toString();
            }
        }
        if(pin == currentPin && pinCount > 1){
            error = "Invalid pin: Must be unique";
        }else if(pin !=currentPin && pinCount>0){
            error = "Invalid pin: Must be unique";
        }

       // qDebug()<<qry->lastError().text();
    }
    return error;
}
// Self evident
void EmployeeEditForm::on_FinishButton_clicked()
{
    if(AddValid() == "" && task=="add")
    {
        QSqlQuery * qry = new QSqlQuery(data);
        QString admin = QString::number(int(ui->admin->isChecked()));
        QString active = QString::number(int(ui->active->isChecked()));        
        QString current = QString::number(int(ui->current->isChecked()));
        qry->prepare("insert into employeelist(name,pin,adminstatus,shiftcount,active,current) "
                     " values('"+ui->name->text()+"','"+ui->pin->text()+"','"+admin+"','1','"+active+"','"+current+"')");
        if (qry->exec())
            success = true;
        else{
            success = false;
            successMsg = "Error Creating Employee";
        }

        this->hide();
        emit finished();
    }
    if(EditValid() == "" && task=="edit")
    {
        QSqlQuery * qry = new QSqlQuery(data);
        QString admin = QString::number(int(ui->admin->isChecked()));
        QString active = QString::number(int(ui->active->isChecked()));
        QString current = QString::number(int(ui->current->isChecked()));
        qry->prepare("update employeelist set name='"+ui->name->text()+"', pin='"+ui->pin->text()+"', adminstatus='"+admin+"', current='"+current+"', active='"+active+"'  where id = '"+id+"'");

        if (qry->exec())
            success = true;
        else{
            success = false;
            successMsg = "Error Editing Employee";
        }

        qry->clear();
        qry->prepare("update shiftlist set employeename='"+ui->name->text()+"'  where employeeid = '"+id+"'");
        if (qry->exec())
            success = true;
        else{
            success = false;
            successMsg = "Error Editing Employee";
        }
        this->hide();
        emit finished();
    }
    else if(task=="edit"){
        ui->error->setText(EditValid());
    }
    else{
        ui->error->setText(AddValid());
    }

}
void EmployeeEditForm::on_CancelButton_clicked()
{
    success=true;
    this->hide();
    emit finished();

}



void EmployeeEditForm::on_pin_returnPressed()
{
    on_FinishButton_clicked();
}