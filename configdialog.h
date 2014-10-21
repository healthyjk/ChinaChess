#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QtGui>

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = 0);

signals:
    void getAllConfig(const int chessMode,const int compLevel,
                      const bool bgMusicSwitch,const bool atMusicSwitch);   //设置确定后发出信号后更新主游戏参数
    void getDefaultConfig();    //设置完成后发出信号使用默认参数
public slots:
    void confirmConfig();       //用户点击确定按钮事件
    void closeConfig();         //用户点击取消按钮事件

private:
    QGroupBox *typeGroup;
    QRadioButton *mmButton; //双人对战
    QLabel *mmLabel;
    QRadioButton *cmButton; //人机对战
    QLabel *cmLabel;
    QButtonGroup *group1;

    QGroupBox *compLevelGroup;//电脑等级选择
    QLabel *compLevelLabel;
    QComboBox *compLevelCombo;

    QGroupBox *musicGroup;
    QLabel *bgMusicLabel;
    QCheckBox *bgMusicButton; //背景音乐开关
    QGroupBox *atMusicGroup;
    QLabel *atMusicLabel;
    QCheckBox *atMusicButton; //下棋动作音效开关

    QPushButton *submit;
    QPushButton *cancel;

    int chessMode;           //游戏模式
    int compLevel;          //电脑等级
    bool bgMusicSwitch;     //背景音乐开关
    bool atMusicSwitch;     //下棋音效开关
};

#endif // CONFIGDIALOG_H
