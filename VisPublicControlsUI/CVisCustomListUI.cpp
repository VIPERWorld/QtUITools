#include "CVisCustomListUI.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QDomNode>
#include <QScrollBar>

#include "CVisPageTurningUI.h"
#include "VisQtUiTool.h"


CVisCustomListUI::CVisCustomListUI(QWidget *parent, bool bLoadDefault)
	: QWidget(parent)
{
	m_pListUI = NULL;
	m_pCustomPageTurningUI = NULL;
	m_pItemInfoCountLbl = NULL;

	if(bLoadDefault){
		InitPolicyFile("/Resources/TestUI/custom_list_ui.xml");
	}
	
	//InitTestInfo();
}

CVisCustomListUI::~CVisCustomListUI()
{

}

void CVisCustomListUI::InitPolicyFile(const QString &sFileName)
{
	/*QDomNode childNode;
	QDomElement domEle;
	if(CVisVAnalysisUIXML::InitFristDomNode(domEle, childNode, sFileName)){
		CVisVAnalysisUIXML::InitBaseLayoutInfo(domEle, QSize(), QString(), this);
		QString sTmpSize = domEle.attributeNode("ItemSize").value();
		CVisVAnalysisUIXML::ConvertToPointFormString(m_itemSize, sTmpSize);
		sTmpSize = domEle.attributeNode("GridSize").value();
		CVisVAnalysisUIXML::ConvertToPointFormString(m_GridSize, sTmpSize);
		m_nSpacing = domEle.attributeNode("Spacing").value().toInt();
		m_nLayoutMode = domEle.attributeNode("LayoutMode").value().toInt();
		m_nViewMode = domEle.attributeNode("ViewMode").value().toInt();

		m_sChildUIFile = domEle.attributeNode("ChildItemFile").value();	

		QString styleSheet = domEle.attributeNode("Style").value();
		if(!styleSheet.isEmpty()){
			this->setStyleSheet(styleSheet);
		}

		while(!childNode.isNull()){
			if(childNode.isElement()){
				QString sTagName = childNode.toElement().tagName();
				if(sTagName == QString("PageTurningUI")){
					InitPageTurningUI(childNode);
				}
				else if(sTagName == QString("TopTipUI")){
					InitTopTipUI(childNode);
				}
			}

			childNode = childNode.nextSibling();
		}
	}

	InitLayout();
	InitListSettings();*/
}

//���������Ϣ
QListWidgetItem *  CVisCustomListUI::AppChildItemInfo(const QString &sItemID)
{
	//������Item��С,����item��SizeHint��UI��SizeҪ��һ��,
	//�������������ʾ��ȫ����
	QListWidgetItem *pItem = new QListWidgetItem(m_pListUI);	
	pItem->setSizeHint(m_GridSize);
	pItem->setData(Qt::UserRole+1, sItemID);
	m_pListUI->addItem(pItem);

	QWidget *pChidItemUI = VisQtUiTool::CreateUI(m_sChildUIFile);
	if(pChidItemUI == NULL) return NULL;
	pChidItemUI->setFixedSize(m_itemSize);

	InitOperBtnSignal(pChidItemUI);

	QLabel *pInfoLbl = pChidItemUI->findChild<QLabel*>("label");
	if(pInfoLbl != NULL){
		pInfoLbl->setText(sItemID);
	}

	m_pListUI->setItemWidget(pItem, pChidItemUI);	

	//���ں��ڶ�λ��
	m_ui2Item[pChidItemUI] = pItem;
	m_index2ItemUI[sItemID] = pChidItemUI;

	pChidItemUI->show();
	return pItem;
}

QWidget * CVisCustomListUI::GetChildItemWidget(QListWidgetItem *pItem)
{
	if(m_pListUI == NULL || pItem == NULL) return NULL;

	QWidget *pItemUI = NULL;
	pItemUI = m_pListUI->itemWidget(pItem);

	return pItemUI;
}

//ɾ��ָ��ItemUI
void CVisCustomListUI::DelItemUI(const QString &sItemID)
{
	if(m_pListUI == NULL) return;

	QWidget *pItemUI = m_index2ItemUI.value(sItemID);
	if(pItemUI == NULL) return;
	m_index2ItemUI.remove(sItemID);

	QListWidgetItem *pItem = m_ui2Item.value(pItemUI);
	if(pItem == NULL) return;
	m_ui2Item.remove(pItemUI);

	m_pListUI->setCurrentItem(pItem);
	m_pListUI->removeItemWidget(pItem);
	
	if(pItem != NULL){
		delete pItem;
		pItem = NULL;
	}

	if(pItemUI != NULL){
		delete pItemUI;
		pItemUI = NULL;
	}
}

//ɾ����ǰItemUI
void CVisCustomListUI::DelCurrentItemUI()
{
	if(m_pListUI == NULL) return;

	QListWidgetItem *pItem = m_pListUI->currentItem();
	QString sItemID = pItem->data(Qt::UserRole +1).toString();
	
	QWidget*pItemUI = m_index2ItemUI.value(sItemID);
	if(pItemUI == NULL) return;
	m_index2ItemUI.remove(sItemID);
	m_ui2Item.remove(pItemUI);
	m_pListUI->removeItemWidget(pItem);

	if(pItem != NULL){
		delete pItem;
		pItem = NULL;
	}

	if(pItemUI != NULL){
		delete pItemUI;
		pItemUI = NULL;
	}
}

//����б�����
void CVisCustomListUI::ClearCustomListInfo()
{
	if(m_pListUI == NULL) return;

	QMap<QString, QWidget*>::const_iterator itor = m_index2ItemUI.constBegin();
	while (itor != m_index2ItemUI.constEnd()) {
		QWidget *pItemUI = itor.value();
		QListWidgetItem *pItem = m_ui2Item.value(pItemUI);
		if(pItem != NULL){
			m_pListUI->removeItemWidget(pItem);
		}

		if(pItem != NULL){
			delete pItem;
			pItem = NULL;
		}

		if(pItemUI != NULL){
			delete pItemUI;
			pItemUI = NULL;
		}

		++itor;
	}

	m_index2ItemUI.clear();
	m_ui2Item.clear();
}

//����Item������ʾ
void CVisCustomListUI::SetItemCountTip(int nCount)
{
	if(m_pItemInfoCountLbl == nullptr) return;

	m_pItemInfoCountLbl->setText(m_sItemCountText.arg(nCount));
}

//��ʼ������
void CVisCustomListUI::InitLayout()
{
	m_pListUI = new QListWidget(this);

	//������ʾ����
	QHBoxLayout *pTopTipLyt = NULL;
	if(m_pItemInfoCountLbl != NULL){
		pTopTipLyt = new QHBoxLayout;
		pTopTipLyt->setSpacing(0);
		pTopTipLyt->setMargin(0);
		pTopTipLyt->addWidget(m_pItemInfoCountLbl);
	}
	
	QVBoxLayout *pMainLyt = new QVBoxLayout;

	pMainLyt->setMargin(0);
	pMainLyt->setSpacing(0);

	//������ʾ���� 
	if(pTopTipLyt != NULL){
		pMainLyt->addLayout(pTopTipLyt);
	}
	//�м��б�����
	pMainLyt->addWidget(m_pListUI);
	//��ҳ����
	if(m_pCustomPageTurningUI != NULL){
		pMainLyt->addWidget(m_pCustomPageTurningUI);
	}
	this->setLayout(pMainLyt);
}

//��ʼ���������
void CVisCustomListUI::InitListSettings()
{
	if(m_pListUI == NULL) return;

	//���ڽ�������ק��ʾ��������
	m_pListUI->setMovement(QListView::Static);
	//������ʾ��ʽ
	//m_pListUI->setFlow(QListView::LeftToRight);
	//m_pListUI->setProperty("isWrapping", QVariant(true));
	//�������������Զ���Ӧ��������
	m_pListUI->setResizeMode(QListView::Adjust);
	//�������������Ƿ���չ������Ŀ��, Batched:������Ϊ��׼
	m_pListUI->setLayoutMode((QListView::LayoutMode)m_nLayoutMode);
	//����������,��GridSize�г�ͻ
	m_pListUI->setSpacing(m_nSpacing);	
	//m_pListUI->setWordWrap(true);
	m_pListUI->setWrapping(true);
	//�����б���ʾ��ʽ
	m_pListUI->setViewMode((QListView::ViewMode)m_nViewMode);
	//���ι������Ҽ��˵�
	m_pListUI->horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
	m_pListUI->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
	//�������������
	m_pListUI->viewport()->setFocusPolicy(Qt::NoFocus);

	connect(m_pListUI, SIGNAL(itemClicked(QListWidgetItem*)),
		this, SLOT(slot_ListWidget_ItemClicked(QListWidgetItem*)));
	connect(m_pListUI, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
		this, SLOT(slot_ListWidget_ItemDoubleClicked(QListWidgetItem*)));
}

//��ʼ��ItemUI������ť�ź�
void CVisCustomListUI::InitOperBtnSignal(QWidget *pItemUI)
{
	if(pItemUI == NULL) return;

	QList<QPushButton *> lOperBtn = pItemUI->findChildren<QPushButton *>();

	for(int i = 0; i < lOperBtn.size(); i++){
		if(lOperBtn.at(i) == NULL) continue;
		connect(lOperBtn.at(i), SIGNAL(clicked()),
			this, SLOT(slot_ItemUi_OperBtn_Clicked()));
	}
}

//��ʼ����ҳ����
void CVisCustomListUI::InitPageTurningUI(const QDomNode &node)
{
	QString sUIFile = node.toElement().attributeNode("UIFile").value();
	bool bLoadDefault = sUIFile.isEmpty() ? true: false;
	m_pCustomPageTurningUI = new CVisPageTurningUI(this, bLoadDefault);
	if(!bLoadDefault){
		m_pCustomPageTurningUI->InitUIFile(sUIFile);
	}

	if(m_pCustomPageTurningUI != NULL){
		connect(m_pCustomPageTurningUI, SIGNAL(signal_PageNumber(int)),
			this, SIGNAL(signal_PageTurningUI_PageChanged(int)));
	}
}

//��ʼ��������ʾ����
void CVisCustomListUI::InitTopTipUI(const QDomNode &node)
{
	QDomNode childNode = node.firstChild();

	while(!childNode.isNull()){
		if(childNode.isElement()){
			QString sTagName = childNode.toElement().tagName();
			if(sTagName == QString("ItemCountLbl")){
				m_pItemInfoCountLbl = new QLabel(this);
				CVisVAnalysisUIXML::InitBaseLayoutInfo(childNode,
					QSize(), QString(), m_pItemInfoCountLbl);

				int nHeight = childNode.toElement().attributeNode("Height").value().toInt();
				if(nHeight >0){
					m_pItemInfoCountLbl->setFixedHeight(nHeight);
				}
				m_sItemCountText = childNode.toElement().attributeNode("ShowText").value();				
				SetItemCountTip(0);
			}
		}

		childNode = childNode.nextSibling();
	}
}

//��ʼ����������
void CVisCustomListUI::InitTestInfo()
{
	if(m_pListUI == NULL) return;
	if(m_pCustomPageTurningUI != NULL){
		m_pCustomPageTurningUI->UpdatePageInfo(11, 11);
	}

	QStringList lInfo;
	lInfo << QString("Linux") << QString("Qt") << QString("Qt Linux");

	for(int i = 0; i <= 20; i++){
		AppChildItemInfo(QString::number(i));
	}

	connect(this, SIGNAL(signal_OperBtn_ToDo(const QString&, const QString&)),
		this, SLOT(slot_OperBtn_ToDo(const QString&, const QString &)));
}

void CVisCustomListUI::slot_ListWidget_ItemClicked(QListWidgetItem *pItem)
{
	if(pItem == NULL) return ;

	QString sItemID = pItem->data(Qt::UserRole +1).toString();

	emit signal_Item_Clicked(sItemID);
}

void CVisCustomListUI::slot_ListWidget_ItemDoubleClicked(QListWidgetItem *pItem)
{
	if(pItem == NULL) return ;

	QString sItemID = pItem->data(Qt::UserRole +1).toString();
	emit signal_Item_DoubleClicked(sItemID);
}

//�б���UI��ť���������¼�
void CVisCustomListUI::slot_ItemUi_OperBtn_Clicked()
{
	if (QPushButton* pBtn = dynamic_cast<QPushButton*>(sender())){
		//�л���ǰ��
		QWidget *pItemUI = pBtn->parentWidget();
		if(pItemUI == NULL) return;
		QListWidgetItem *pItem = m_ui2Item.value(pItemUI);
		if(pItem == NULL) return;
		m_pListUI->setCurrentItem(pItem);
		
		QString sItemID = pItem->data(Qt::UserRole + 1).toString();
		QString sToDo = pBtn->property("ToDo").toString();
		emit signal_OperBtn_ToDo(sItemID, sToDo);
	}
}

void CVisCustomListUI::slot_OperBtn_ToDo(const QString &sItemID, const QString &sToDo)
{
	if(sToDo == QString("EditUI")){
		DelItemUI(sItemID);
	}
	else if(sToDo == QString("ExitUI")){
		DelCurrentItemUI();
	}	
}

//������Ŀҳ����
void CVisCustomListUI::SetItemPageCount(int nCurPage, int nTotal)
{
	if(m_pCustomPageTurningUI == nullptr) return;
	m_pCustomPageTurningUI->UpdatePageInfo(nCurPage, nTotal);
}

void CVisCustomListUI::paintEvent(QPaintEvent *event)
{
	QStyleOption opt; 
	opt.init(this); 
	QPainter p(this); 
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

	return QWidget::paintEvent(event);
}
