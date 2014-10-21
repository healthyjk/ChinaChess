#include "configdialog.h"
#include "chessdefine.h"
#include <QtGui>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("ѡ������");
    typeGroup = new QGroupBox(tr("��սģʽѡ��"));
    mmButton = new QRadioButton(this);
    mmLabel = new QLabel(this);
    mmLabel->setText(tr("˫�˶�ս"));
    mmLabel->setAlignment(Qt::AlignLeft);
    cmButton = new QRadioButton(this);
    cmButton->setChecked(true);
    cmLabel = new QLabel(this);
    cmLabel->setText(tr("�˻���ս"));
    cmLabel->setAlignment(Qt::AlignLeft);
    group1 = new QButtonGroup(this);
    group1->addButton(mmButton);
    group1->addButton(cmButton);

    compLevelGroup = new QGroupBox(this);
    compLevelGroup->setTitle(tr("���Եȼ�ѡ��"));
    compLevelLabel = new QLabel(this);
    compLevelLabel->setText(tr("�����Ѷ�"));

    compLevelCombo = new QComboBox(this);
    compLevelCombo->addItem(tr("�򵥣�����ɳ��"));
    compLevelCombo->addItem(tr("�м�������ս"));
    compLevelCombo->addItem(tr("�߼���ս�޲�ʤ"));

    musicGroup = new QGroupBox(this);
    musicGroup->setTitle(tr("����"));
    bgMusicLabel = new QLabel(this);
    bgMusicLabel->setText(tr("��������"));
    bgMusicButton = new QCheckBox(this);
    atMusicLabel = new QLabel(this);
    atMusicLabel->setText(tr("������Ч"));
    atMusicButton = new QCheckBox(this);

    submit = new QPushButton(this);
    submit->setText(tr("ȷ��"));
    connect(submit,SIGNAL(clicked()),this,SLOT(confirmConfig()));
    cancel = new QPushButton(this);
    cancel->setText(tr("ȡ��"));
    connect(cancel,SIGNAL(clicked()),this,SLOT(closeConfig()));

    //����Ϊ���沼��
    QHBoxLayout *typeMMLayout = new QHBoxLayout;
    typeMMLayout->addWidget(mmLabel);
    typeMMLayout->addWidget(mmButton);

    QHBoxLayout *typeCMLayout = new QHBoxLayout;
    typeCMLayout->addWidget(cmLabel);
    typeCMLayout->addWidget(cmButton);

    QVBoxLayout *typeLayout = new QVBoxLayout;
    typeLayout->addLayout(typeMMLayout);
    typeLayout->addLayout(typeCMLayout);
    typeGroup->setLayout(typeLayout);

    QHBoxLayout *compLevelLayout = new QHBoxLayout;
    compLevelLayout->addWidget(compLevelLabel);
    compLevelLayout->addWidget(compLevelCombo);
    QVBoxLayout *compLayout = new QVBoxLayout;
    compLayout->addLayout(compLevelLayout);
    compLevelGroup->setLayout(compLayout);

    QHBoxLayout *bgMusicLayout = new QHBoxLayout;
    bgMusicLayout->addWidget(bgMusicLabel);
    bgMusicLayout->addWidget(bgMusicButton);
    QHBoxLayout *atMusicLayout = new QHBoxLayout;
    atMusicLayout->addWidget(atMusicLabel);
    atMusicLayout->addWidget(atMusicButton);

    QVBoxLayout *musicLayout = new QVBoxLayout;
    musicLayout->addLayout(bgMusicLayout);
    musicLayout->addLayout(atMusicLayout);
    musicGroup->setLayout(musicLayout);

    QHBoxLayout *submitLayout = new QHBoxLayout;
    submitLayout->addWidget(submit);
    submitLayout->addWidget(cancel);

    QVBoxLayout *submitMainLayout = new QVBoxLayout;
    submitMainLayout->addLayout(submitLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(typeGroup);
    mainLayout->addWidget(compLevelGroup);
    mainLayout->addWidget(musicGroup);
    mainLayout->addLayout(submitMainLayout);
    this->setLayout(mainLayout);
}

//�û����ȷ����ť�¼�
void ConfigDialog::confirmConfig()
{
    if(this->mmButton->isChecked())
        chessMode = MAN_MANCHESS;
    else
        chessMode = MAN_PCCHESS;
    if(compLevelCombo->currentIndex()==0)
        compLevel = 4;
    else if(compLevelCombo->currentIndex()==1)
        compLevel = 5;
    else
        compLevel = 6;
    if(bgMusicButton->isChecked())
        bgMusicSwitch = true;
    else
        bgMusicSwitch = false;
    if(atMusicButton->isChecked())
        atMusicSwitch = true;
    else
        atMusicSwitch = false;
    emit getAllConfig(chessMode,compLevel,bgMusicSwitch,atMusicSwitch);
    this->accept();         //�رմ���
}

//�û����ȡ����ť�¼�
void ConfigDialog::closeConfig()
{
    emit getDefaultConfig();    //ʹ��Ĭ������
    this->reject();              //�رմ���
}
