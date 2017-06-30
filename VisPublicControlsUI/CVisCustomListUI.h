/************************************************************************/
/*		         �Զ����б�UI                                           */
/************************************************************************/
#ifndef CVISCUSTOMLISTUI_H
#define CVISCUSTOMLISTUI_H

#include "vispubliccontrolsui_global.h"
#include <QWidget>
#include <QMap>

class QListWidget;
class CVisPageTurningUI;
class QDomNode;
class QListWidgetItem;
class QLabel;

class VISPUBLICCONTROLSUI_EXPORT CVisCustomListUI : public QWidget
{
	Q_OBJECT

public:
	CVisCustomListUI(QWidget *parent = 0, bool bLoadDefault = true);
	~CVisCustomListUI();

	//��ʼ����ʽ�ļ�
	void InitPolicyFile(const QString &sFileName);
	//���������Ϣ
	QListWidgetItem * AppChildItemInfo(const QString &sItemID);
	//��ȡ�����Ӧ��Widget
	QWidget * GetChildItemWidget(QListWidgetItem *pItem);
	//ɾ��ָ��ItemUI
	void DelItemUI(const QString &sItemID);
	//ɾ����ǰItemUI
	void DelCurrentItemUI();
	//����б�����
	void ClearCustomListInfo();
	//����Item������ʾ
	void SetItemCountTip(int nCount);
	//������Ŀҳ����
	void SetItemPageCount(int nCurPage, int nTotal);	


signals:
	void signal_OperBtn_ToDo(const QString &sItemID, const QString &sToDo);
	void signal_PageTurningUI_PageChanged(int nPage);
	void signal_Item_Clicked(const QString &sItemID);
	void signal_Item_DoubleClicked(const QString &sItemID);

protected:
	void paintEvent(QPaintEvent *event);

private:
	void InitLayout();	//��ʼ������
	void InitListSettings();	//��ʼ���������
	void InitOperBtnSignal(QWidget *pItemUI);	//��ʼ��ItemUI������ť�ź�
	void InitPageTurningUI(const QDomNode &node);	//��ʼ����ҳ����
	void InitTopTipUI(const QDomNode &node);		//��ʼ��������ʾ����

	void InitTestInfo();

private slots:
	void slot_ListWidget_ItemClicked(QListWidgetItem *item);		//�б��������
	void slot_ListWidget_ItemDoubleClicked(QListWidgetItem *item);	//�б���˫������
	void slot_ItemUi_OperBtn_Clicked();								//�б���UI��ť���������¼�

	void slot_OperBtn_ToDo(const QString &sItemID, const QString &sToDo);	//���԰�ť

private:
	QListWidget					*m_pListUI;			//�б�	
	QSize						m_itemSize;			//item��С
	QSize						m_GridSize;			//
	int							m_nSpacing;			//��������GridSize�г�ͻ
	int							m_nLayoutMode;		//��ʾ������ʽ
	int							m_nViewMode;		//������ʾ��ʽ
	QString						m_sChildUIFile;		//itemUI�ļ�
	QMap<QString, QWidget*>		m_index2ItemUI;		//ID��itemUI���ձ�

	QMap<QWidget*, QListWidgetItem*>		m_ui2Item;	//itemUi��Item���ձ�

	CVisPageTurningUI			*m_pCustomPageTurningUI;	//��ҳ
	QLabel						*m_pItemInfoCountLbl;		//�������ʾ
	QString						 m_sItemCountText;			//�������ʾ�ַ���

};

#endif // CVISCUSTOMLISTUI_H
