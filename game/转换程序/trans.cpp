#include <iostream>
#include <fstream>
#include <string>


    
#define maxLine 80                  //推箱子关卡文件每行最大字符数

//以下需对应修改
#define fileInName "../charFile/test.txt"        //源文件目录   
#define fileOutChName "../sqlFile/test.sql"       //转换后文件目录
#define gameTable "test"                   //新数据表名
#define database "AI_BoxPush"                //mysql数据库名

using namespace std;
int main(){



	ofstream sqlfile(fileOutChName);//实例化写入文件对象 并打开
    ifstream charfile(fileInName);     //～读取文件～
    
    if(!sqlfile) {        //确定写入文件打开 不知道有什么用
        cout<< "no such file" << endl;
        exit(1);
    }
    
    char buf[maxLine];
    string s;    //每次读取一行 存储
    
    int i = 1, j = 1;
    if(charfile.is_open()){   //读取文件打开成功
        while(charfile.good() && !charfile.eof()){   //文件是否打开正确 是否结束
            memset(buf, 0, maxLine);

            //sql文件写入控制语句
            {
                sqlfile<< "use " << database << ";" << endl;
                sqlfile<< "-- 使用本数据库" << endl;
                sqlfile<< "DROP TABLE IF EXISTS `" << gameTable << "`;" << endl;
                sqlfile<< "CREATE TABLE IF NOT EXISTS `" << gameTable << "` (" << endl;
                sqlfile<< "`type` varchar(30) not null," << endl;
                sqlfile<< "`x` int not null," << endl;
                sqlfile<< "`y` int not null," << endl;
                sqlfile<< "primary key (`x`,`y`)" << endl;
                sqlfile<< ");" << endl;

                sqlfile << flush;
            }
              

            int y=0;
            while(charfile.getline(buf, maxLine)){
                s = buf;         //读一行 赋给s
                int len = s.size();

                for(int x=0;x<len;x++){  //分析每一行的每个符号
                    char c;
                    c = s.at(x);


                    string cmd, table, s1,s2,s3;
                    table = gameTable;
                    s1 = "INSERT INTO `" + table + "` (`type`,`x`,`y`) VALUES ('";
                    s3 = "'," + to_string(x) +   "," + to_string(y) + ");";


                    switch(c){
                        case '#' :
                            s2 = '#';
                            cmd = s1 + s2 + s3;
                            // cout << cmd << endl;

                            sqlfile<< cmd << endl;  //写入插入语句
                            break;

                        case '.' :
                            s2 = '.';
                            cmd = s1 + s2 + s3;
                            sqlfile<< cmd << endl;  //写入插入语句
                            break;

                        case '$':
                            s2 = '$';
                            cmd = s1 + s2 + s3;
                            sqlfile<< cmd << endl;  //写入插入语句
                            break;

                        case '@':
                            s2 = '@';
                            cmd = s1 + s2 + s3;
                            sqlfile<< cmd << endl;  //写入插入语句
                            break;

                        case ' ':

                            break;

                        default:
                            cout << "error input ";
                    }

                    sqlfile << flush;   //保存文件

                }

                y++;

            }
            //遍历结束
            sqlfile<< endl <<"insert IGNORE into info (gameTable) values ('"<<gameTable<<"');" << endl;    //新表信息加入info表
            sqlfile << flush;       //保存文件

            sqlfile.close();
            charfile.close();
            cout << "转换结束" << endl;


        }
    } else cout << "charFile 打开失败" << endl;
            
    return 1;
        
}