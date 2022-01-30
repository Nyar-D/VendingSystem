#ifndef ADVERTISEMENT_H
#define ADVERTISEMENT_H

#include <QMainWindow>
#include <QProcess>
#include <QDir>
#include <QImage>
#include <QDebug>
#include <QMovie>
#include <QElapsedTimer>
#include <QTimer>
#include <QThread>
#include "shop.h"

#define ADVFOLDPATH "../vendingSys_adv"

namespace Ui {
class Advertisement;
}

class Advertisement : public QMainWindow
{
    Q_OBJECT

public:
    explicit Advertisement(QString advFoldPath = ADVFOLDPATH, QWidget *parent = 0);
    ~Advertisement();

    QFileInfoList GetFileList(QString);    //遍历文件夹获取文件列表
    void PlayAdv(QString);                 //加载广告文件
    void SetAdvFoldPath(QString);

private slots:
    void on_adv_enterShop_bt_clicked();
    void play_nextAdv();                //轮播广告槽函数

private:
    Ui::Advertisement *ui;
    Shop *shop;

    QString m_stAdvFoldPath;           //广告文件夹路径
    QStringList m_stlAdvList;          //广告列表
    QTimer *m_timerAdvPlay;            //轮播定时器
    int m_nCurrentAdvNum;              //当前广告索引
};

#endif // ADVERTISEMENT_H
