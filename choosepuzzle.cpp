#include "choosepuzzle.h"
#include "ui_choosepuzzle.h"

#include <QDebug>

choosePuzzle::choosePuzzle(QWidget *parent) : QWidget(parent), ui(new Ui::choosePuzzle)
{
    ui->setupUi(this);



}

choosePuzzle::~choosePuzzle()
{
    delete ui;
}






void choosePuzzle::init_table()
{
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置表格不可编辑（通过按钮编辑数据）
    ui->tableWidget->verticalHeader()->setVisible(false);   //隐藏行号（数据库含id）
    ui->tableWidget->horizontalHeader()->setMinimumHeight(40);   //设置表头高度
    {  //设置表头元素
      QTableWidgetItem  *headerItem;
      QStringList headerText;
      headerText<<"puzzle name"<<"block number"<<"box number"<<"difficulty";
         for (int i=0;i<ui->tableWidget->columnCount();i++)//列编号从0开始
          {
            headerItem =new QTableWidgetItem(headerText.at(i)); //新建一个QTableWidgetItem， headerText.at(i)获取headerText的i行字符串
            QFont font=headerItem->font();//获取原有字体设置
            font.setBold(true);//设置为粗体
            font.setPointSize(18);//字体大小
            headerItem->setTextColor(Qt::black);//字体颜色
            headerItem->setFont(font);//设置字体
            ui->tableWidget->setHorizontalHeaderItem(i,headerItem); //设置表头单元格的Item
        }
    }
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void choosePuzzle::appendRow(QStringList rowData)    //添加一行数据
{
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);          //最下面一行加入空行

    for (int i = 0;i < rowData.size();i++) {
        QTableWidgetItem *item = new QTableWidgetItem;    //本行i列的内容
        item->setText(rowData.at(i));
        ui->tableWidget->setItem(rowCount,i,item);
    }
}

void choosePuzzle::load_data()
{
    this->choose_Createdb = new mysql_conn;     //在这里打开数据库
    this->choose_Createdb->linkMySql();   //进入时连接数据库

    set_currentTable("");
    QList<QStringList> tableData;
    tableData = this->choose_Createdb->selectDataFromDb("Select * From info");   //调用sql类方法

    if(!tableData.isEmpty()){
        {  //清除表格
            int rowCount = ui->tableWidget->rowCount();
            for(int r = rowCount-1; r >= 0; r--) ui->tableWidget->removeRow(r);
        }
        QListIterator<QStringList> itr(tableData);     //迭代器
        while (itr.hasNext()) {   //加载数据
            this->appendRow(itr.next());      //一行一行加载
        }


    }


}

void choosePuzzle::set_currentTable(QString table)
{
        QString currentTable;

        if(table==""){
            currentTable = "game1";
        }
        else {
            currentTable = table;
        }


        QString cmd = "insert into currentGame select * from "+currentTable+ ";";

        this->choose_Createdb->execCommand("truncate table currentGame;");
        bool ok = this->choose_Createdb->execCommand(cmd);

        if(ok) qDebug() << "game data init ok";
        else qDebug() << "game data init error";



}

void choosePuzzle::on_back_clicked()
{
    this->choose_Createdb->closeMysql();    //返回主界面时关闭数据库（否则与其他界面冲突）
    emit signal_back();

}

void choosePuzzle::slot_showing()  //页面即将显示时，做初始化
{
    init_table();
    load_data();
}