/************************************************************************/
/*                        ��ҳ������                                    */
/************************************************************************/
#ifndef CVISPAGETURNINGUI_H
#define CVISPAGETURNINGUI_H

#include <QWidget>
#include "vispubliccontrolsui_global.h"

class QPushButton;

class VISPUBLICCONTROLSUI_EXPORT CVisPageTurningUI : public QWidget
{
	Q_OBJECT

public:
	CVisPageTurningUI(QWidget *parent = 0, bool bLoadDefault = true);
	~CVisPageTurningUI();

	//��ʼ��UI�ļ�
	void InitUIFile(const QString &sFile);
	void UpdatePageInfo(int nCurPage, int nTotalPage);	//����ҳ����Ϣ

signals:
	void signal_PageNumber(int nCurPage);

private:
	void InitOperBtnSignal(QWidget *pItemUI);	//��ʼ��ItemUI������ť�ź�
	void PageTurningUI_OperBtn_ToDo(const QString &sToDo);	//��ť����
	void UpatePageInfoLbl(int nCurPage, int nTotal);		//����ҳǩ��Ϣ

	void SetGoOperUIVisible(bool bVisible);	//����ת��ҳ���ܿ���
	void SetNumPageBtnVisible(int nTotalPage); //��������ҳ��ť��ʾ
	void GetNumberPageBtn(QList<QPushButton*> &lNumberBtn);	//��ȡ���ְ�ť
	void SetCurrentPageBtn(int nCurPage);	//���õ�ǰҳ�밴ť
	bool CheckNumberPageBtn(QPushButton *pNumPageBtn, int nCampare);	//���ҳ�밴ť

private slots:
	void slot_OperBtn_Clicked();

private:
	QWidget				*m_pUI;			//����UI

	QString				m_sCurPageLblText;	//��¼��ǰҳǩ��ʾ����
	int					m_nCurPageIndex;	//��ǰҳ
	int					m_nTotalPage;		//��ҳ��	
};

#endif // CVISPAGETURNINGUI_H
