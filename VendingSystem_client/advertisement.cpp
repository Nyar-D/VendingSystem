#include "advertisement.h"
#include "ui_advertisement.h"

Advertisement::Advertisement(QString advFoldPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Advertisement),
    m_stAdvFoldPath(advFoldPath)
{
    qDebug()<<m_stAdvFoldPath;
    ui->setupUi(this);

    this->ui->advertisement_lb->setScaledContents(true);

    PlayAdv(m_stAdvFoldPath); //加载广告文件
}

Advertisement::~Advertisement()
{
    delete ui;
    delete m_timerAdvPlay;
}

void Advertisement::on_adv_enterShop_bt_clicked()
{
    shop = new Shop();
    this->hide();
    shop->show();
}

QFileInfoList Advertisement::GetFileList(QString st_path)
{
    QFileInfoList stlFileList;
    QFileInfoList stlFolderList;

    QDir dir(st_path);

    if(!dir.exists())
        return stlFileList;

    stlFileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks); //列出目录下所有文件和目录信息，存储到fileList
    stlFolderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i= 0; i != stlFolderList.size(); i++)
    {
        QString st_childFoldPath = stlFolderList.at(i).absoluteFilePath();
        QFileInfoList stl_childFileList = GetFileList(st_childFoldPath);             //folderList递归遍历
        stlFileList.append(stl_childFileList);
    }
    return stlFileList;
}

void Advertisement::PlayAdv(QString path)
{
    QString filter;

    qDebug()<<"show advertistment";

    QFileInfoList stl_fileList = GetFileList(path);  //遍历文件夹获取文件信息
    if(stl_fileList.isEmpty())
    {
        ui->advertisement_lb->setText("广告位招租，联系电话xxx-xxxxxxxx");
        return;
    }

    foreach(QFileInfo advName, stl_fileList)         //使用Qt定义的宏
    {
        filter = advName.suffix();                   //后缀名
        if((filter != "jpg") && (filter != "jpeg") && (filter != "png") && (filter != "gif") && (filter != "mp4"))
            continue;

        qDebug() << advName.absoluteFilePath();
        m_stlAdvList.append(advName.absoluteFilePath());
    }

    m_nCurrentAdvNum = 0;                            //设置当前播放的广告

    m_timerAdvPlay = new QTimer();                   //设置定时器
    connect(m_timerAdvPlay, SIGNAL(timeout()), this, SLOT(play_nextAdv()));
    play_nextAdv();
}

void Advertisement::play_nextAdv()
{
    ui->advertisement_lb->clear();

    QString advPath = m_stlAdvList.at(m_nCurrentAdvNum % m_stlAdvList.size());

    if(advPath.endsWith(".mp4"))
    {
        QProcess *player = new QProcess();
        QStringList args;

        args << "-slave";
        args << "-quiet";
        args << "-wid";
        args << QString::number(ui->advertisement_lb->winId());
        args << advPath;

        m_nCurrentAdvNum = (m_nCurrentAdvNum + 1) % m_stlAdvList.size();
        connect(player, SIGNAL(finished(int)), this, SLOT(play_nextAdv()));
        player->start("/usr/bin/mplayer", args);

        return;
    }
    if(advPath.endsWith(".jpg") || advPath.endsWith(".jpeg") || advPath.endsWith(".png"))
    {
        QImage *img = new QImage(advPath);           //将图像资源载入对象img
        ui->advertisement_lb->setPixmap(QPixmap::fromImage(*img));
    }
    else if(advPath.endsWith(".gif"))
    {
        QMovie *mov = new QMovie(advPath);
        ui->advertisement_lb->setMovie(mov);
        mov->start();
    }

    m_nCurrentAdvNum = (m_nCurrentAdvNum + 1) % m_stlAdvList.size();
    m_timerAdvPlay->singleShot(3000, this, SLOT(play_nextAdv()));
}

void Advertisement::SetAdvFoldPath(QString stAdvFoldPath)
{
    m_stAdvFoldPath = stAdvFoldPath;
}
