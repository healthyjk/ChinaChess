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
                      const bool bgMusicSwitch,const bool atMusicSwitch);   //����ȷ���󷢳��źź��������Ϸ����
    void getDefaultConfig();    //������ɺ󷢳��ź�ʹ��Ĭ�ϲ���
public slots:
    void confirmConfig();       //�û����ȷ����ť�¼�
    void closeConfig();         //�û����ȡ����ť�¼�

private:
    QGroupBox *typeGroup;
    QRadioButton *mmButton; //˫�˶�ս
    QLabel *mmLabel;
    QRadioButton *cmButton; //�˻���ս
    QLabel *cmLabel;
    QButtonGroup *group1;

    QGroupBox *compLevelGroup;//���Եȼ�ѡ��
    QLabel *compLevelLabel;
    QComboBox *compLevelCombo;

    QGroupBox *musicGroup;
    QLabel *bgMusicLabel;
    QCheckBox *bgMusicButton; //�������ֿ���
    QGroupBox *atMusicGroup;
    QLabel *atMusicLabel;
    QCheckBox *atMusicButton; //���嶯����Ч����

    QPushButton *submit;
    QPushButton *cancel;

    int chessMode;           //��Ϸģʽ
    int compLevel;          //���Եȼ�
    bool bgMusicSwitch;     //�������ֿ���
    bool atMusicSwitch;     //������Ч����
};

#endif // CONFIGDIALOG_H
